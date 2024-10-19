package main

import (
	"bufio"
	"encoding/binary"
	"flag"
	"fmt"
	"io"
	"os"
	"os/exec"
)

// Binary Viz Format
//
// Directed Acyclic Graph (DAG) Export ** Simple Binary Format **
//   u64: File ID ' DAGViz '
//   u32: Num Nodes
//   [u32: Node-ID, u32: Color, u32: Shape, u32: Label ID]
//   u32: Num Edges
//   [u32: From(Node ID), u32: To(Node ID), u32: Color, u32: Label ID]
//   Labels
//   [u32: Label ID, u8[64]: Label]
// End
//

type VizNode struct {
	ID      uint32
	Color   uint32
	Shape   uint32
	LabelID uint32

	GraphNodeID int
}

type VizEdge struct {
	From    uint32
	To      uint32
	Color   uint32
	LabelID uint32
}

type VizLabel struct {
	ID    uint32
	Label string
}

type Viz struct {
	FileID   uint64
	NumNodes uint32
	NumEdges uint32
	Nodes    map[uint32]*VizNode
	Edges    []*VizEdge
	Labels   map[uint32]VizLabel
}

func main() {

	// --input is a flag that specifies the input file
	input_filename := flag.String("input", "", "Input file")
	// --output is a flag that specifies the output file
	output_filename := flag.String("output", "", "Output file")

	// Read binary viz file
	input_file, err := os.Open(*input_filename)
	if err != nil {
		panic(err)
	}
	defer input_file.Close()

	// Parse binary viz file
	viz := &Viz{}
	reader := bufio.NewReader(input_file)
	binary.Read(reader, binary.LittleEndian, &viz.FileID)
	binary.Read(reader, binary.LittleEndian, &viz.NumNodes)
	for i := uint32(0); i < viz.NumNodes; i++ {
		node := &VizNode{}
		binary.Read(reader, binary.LittleEndian, &node.ID)
		binary.Read(reader, binary.LittleEndian, &node.Color)
		binary.Read(reader, binary.LittleEndian, &node.Shape)
		binary.Read(reader, binary.LittleEndian, &node.LabelID)
		viz.Nodes[node.ID] = node
	}
	binary.Read(reader, binary.LittleEndian, &viz.NumEdges)
	for i := uint32(0); i < viz.NumEdges; i++ {
		edge := &VizEdge{}
		binary.Read(reader, binary.LittleEndian, &edge.From)
		binary.Read(reader, binary.LittleEndian, &edge.To)
		binary.Read(reader, binary.LittleEndian, &edge.Color)
		binary.Read(reader, binary.LittleEndian, &edge.LabelID)
		viz.Edges = append(viz.Edges, edge)
	}
	for {
		label := VizLabel{}
		err := binary.Read(reader, binary.LittleEndian, &label.ID)
		if err != nil {
			break
		}
		var strID uint32
		var strLen uint32
		var strFormat uint8
		binary.Read(reader, binary.LittleEndian, &strID)
		binary.Read(reader, binary.LittleEndian, &strLen)
		binary.Read(reader, binary.LittleEndian, &strFormat)
		// Read string which is made up of 64 bytes
		str := make([]byte, 64)
		binary.Read(reader, binary.LittleEndian, &str)
		label.Label = string(str)
		viz.Labels[label.ID] = label
	}

	// Create a new graph
	g := &Graph{}

	// Add nodes to the graph
	for _, node := range viz.Nodes {
		node.GraphNodeID = g.AddNode(viz.Labels[node.LabelID].Label)
	}

	// Add edges to the graph
	for _, edge := range viz.Edges {
		from := viz.Nodes[edge.From].GraphNodeID
		to := viz.Nodes[edge.To].GraphNodeID
		g.AddEdge(from, to, viz.Labels[edge.LabelID].Label)
	}

	g.setTitle("Directed Acyclic Render Graph")

	output_file, err := os.Open(*output_filename)
	if err != nil {
		panic(err)
	}
	defer output_file.Close()
	output_writer := bufio.NewWriter(output_file)
	g.GenerateDOT(output_writer)
}

// Common attribute names
const (
	Shape     = "shape"
	Style     = "style"
	Color     = "color"
	FillColor = "fillcolor"
	FontName  = "fontname"
	FontSize  = "fontsize"
	FontColor = "fontcolor"
	NodeSep   = "nodesep"
)

// Common values for shape attribute
const (
	ShapeBox       = "box"
	ShapePolygon   = "polygon"
	ShapeEllipse   = "ellipse"
	ShapeOval      = "oval"
	ShapeCircle    = "circle"
	ShapePoint     = "point"
	ShapeEgg       = "egg"
	ShapeTriangle  = "triangle"
	ShapeNone      = "none"
	ShapeDiamond   = "diamond"
	ShapeRectangle = "rectangle"
	ShapeSquare    = "square"
)

// Common values for style attribute
const (
	StyleSolid   = "solid"
	StyleDashed  = "dashed"
	StyleDotted  = "dotted"
	StyleBold    = "bold"
	StyleRounded = "rounded"
	StyleFilled  = "filled"
	StyleStriped = "striped"
)

// Graph represents a set of nodes, edges and attributes that can be
// translated to DOT language.
type Graph struct {
	nodes           map[int]*node
	edges           map[int]*edge
	n, e            int
	graphAttributes attributes
	nodeAttributes  attributes
	edgeAttributes  attributes
	drawMultiEdges  bool
	directed        bool
	sameRank        [][]*node
}

// SetTitle sets a title for the graph.
func (g *Graph) setTitle(title string) {
	g.GraphAttribute("label", title)
}

// AddNode adds a new node with the given label and returns its id.
func (g *Graph) AddNode(label string) int {
	nod := node{id: g.n, label: label}
	g.n++
	if g.nodes == nil {
		g.nodes = make(map[int]*node)
	}
	g.nodes[nod.id] = &nod
	return nod.id
}

// MakeSameRank causes the specified nodes to be drawn on the same rank.
// Only effective when using the dot tool.
func (g *Graph) MakeSameRank(node1, node2 int, others ...int) {
	r := make([]*node, 2+len(others))
	r[0] = g.nodes[node1]
	r[1] = g.nodes[node2]
	for i := range others {
		r[2+i] = g.nodes[others[i]]
	}
	g.sameRank = append(g.sameRank, r)
}

// AddEdge adds a new edge between the given nodes with the specified
// label and returns an id for the new edge.
func (g *Graph) AddEdge(from, to int, label string) int {
	fromNode := g.nodes[from]
	toNode := g.nodes[to]
	// TODO: Check for errors (non-existing nodes)
	edg := edge{from: fromNode, to: toNode, label: label}
	id := g.e
	g.e++
	if g.edges == nil {
		g.edges = make(map[int]*edge)
	}
	g.edges[id] = &edg
	return id
}

// MakeDirected makes the graph a directed graph. By default, a new
// graph is undirected
func (g *Graph) MakeDirected() {
	g.directed = true
}

// DrawMultipleEdges causes multiple edges between same pair of nodes
// to be drawn separately. By default, for a given pair of nodes, only
// the edge that was last added to the graph is drawn.
func (g *Graph) DrawMultipleEdges() {
	g.drawMultiEdges = true
}

// NodeAttribute sets an attribute for the specified node.
func (g *Graph) NodeAttribute(id int, name, value string) {
	// TODO: check for errors (id out of range)
	g.nodes[id].attributes.set(name, value)
}

// EdgeAttribute sets an attribute for the specified edge.
func (g *Graph) EdgeAttribute(id int, name, value string) {
	// TODO: check for errors (id out of range)
	g.edges[id].attributes.set(name, value)
}

// DefaultNodeAttribute sets an attribute for all nodes by default.
func (g *Graph) DefaultNodeAttribute(name, value string) {
	g.nodeAttributes.set(name, value)
}

// DefaultEdgeAttribute sets an attribute for all edges by default.
func (g *Graph) DefaultEdgeAttribute(name, value string) {
	g.edgeAttributes.set(name, value)
}

// GraphAttribute sets an attribute for the graph
func (g *Graph) GraphAttribute(name, value string) {
	g.graphAttributes.set(name, value)
}

// GenerateDOT generates the graph description in DOT language
func (g Graph) GenerateDOT(w io.Writer) {
	if !g.drawMultiEdges {
		fmt.Fprint(w, "strict ")
	}
	if g.directed {
		fmt.Fprint(w, "digraph ")
	} else {
		fmt.Fprint(w, "graph ")
	}
	fmt.Fprintln(w, "{")
	for graphAttribs := g.graphAttributes.iterate(); graphAttribs.hasMore(); {
		name, value := graphAttribs.next()
		fmt.Fprintf(w, "  %v = %#v;\n", name, value)
	}
	for nodeAttribs := g.nodeAttributes.iterate(); nodeAttribs.hasMore(); {
		name, value := nodeAttribs.next()
		fmt.Fprintf(w, "  node [ %v = %#v ]\n", name, value)
	}
	for edgeAttribs := g.edgeAttributes.iterate(); edgeAttribs.hasMore(); {
		name, value := edgeAttribs.next()
		fmt.Fprintf(w, "  edge [ %v = %#v ]\n", name, value)
	}
	for i := 0; i < g.n; i++ {
		fmt.Fprint(w, "  ")
		g.nodes[i].generateDOT(w)
		fmt.Fprintln(w)
	}
	for _, r := range g.sameRank {
		fmt.Fprint(w, "  {rank=same; ")
		for _, x := range r {
			fmt.Fprintf(w, "%v; ", x.name())
		}
		fmt.Fprintln(w, "}")
	}
	for i := 0; i < g.e; i++ {
		fmt.Fprint(w, "  ")
		g.edges[i].generateDOT(w, g.directed)
		fmt.Fprintln(w)
	}
	fmt.Fprintln(w, "}")
}

// GenerateImage runs a Graphviz tool such as dot or neato to generate
// an image of the graph. filetype can be any file type supported by
// the tool, e.g. png or svg
func (g Graph) GenerateImage(tool, filename, filetype string) error {
	var args []string
	if filetype != "" {
		args = append(args, "-T"+filetype)
	}
	if filename != "" {
		args = append(args, "-o", filename)
	}
	cmd := exec.Command(tool, args...)
	stdin, err := cmd.StdinPipe()
	if err != nil {
		return err
	}
	go func() {
		defer stdin.Close()
		g.GenerateDOT(stdin)
	}()
	return cmd.Run()
}

type attributes struct {
	attributeMap map[string]string
	namesOrdered []string
}

func (a *attributes) set(name, value string) {
	if a.attributeMap == nil {
		a.attributeMap = make(map[string]string)
	}
	if _, exists := a.attributeMap[name]; !exists {
		a.namesOrdered = append(a.namesOrdered, name)
	}
	a.attributeMap[name] = value
}

func (a *attributes) iterate() attributeIterator {
	return attributeIterator{a, 0}
}

type attributeIterator struct {
	attributes *attributes
	index      int
}

func (ai *attributeIterator) hasMore() bool {
	return ai.index < len(ai.attributes.namesOrdered)
}

func (ai *attributeIterator) next() (name, value string) {
	name = ai.attributes.namesOrdered[ai.index]
	value = ai.attributes.attributeMap[name]
	ai.index++
	return name, value
}

type node struct {
	id         int
	label      string
	attributes attributes
}

func (n node) name() string {
	return fmt.Sprintf("n%v", n.id)
}

func (n node) generateDOT(w io.Writer) {
	fmt.Fprintf(w, "%v [label=%#v", n.name(), n.label)
	for attribs := n.attributes.iterate(); attribs.hasMore(); {
		name, value := attribs.next()
		fmt.Fprintf(w, ", %v=%#v", name, value)
	}
	fmt.Fprint(w, "]")
}

type edge struct {
	from       *node
	to         *node
	label      string
	attributes attributes
}

func (e edge) generateDOT(w io.Writer, directed bool) {
	edgeOp := "--"
	if directed {
		edgeOp = "->"
	}
	fmt.Fprintf(w, "%v %v %v [label=%#v", e.from.name(), edgeOp, e.to.name(), e.label)
	for attribs := e.attributes.iterate(); attribs.hasMore(); {
		name, value := attribs.next()
		fmt.Fprintf(w, ", %v=%#v", name, value)
	}
	fmt.Fprint(w, "]")
}
