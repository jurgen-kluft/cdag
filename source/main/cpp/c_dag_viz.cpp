#include "ccore/c_target.h"
#include "cbase/c_buffer.h"

#include "cdag/c_dag_viz.h"

namespace ncore
{
    namespace EGraphColor
    {
        const char* ToString(rgb_t value) { return ""; }

    } // namespace EGraphColor

    namespace EGraphShape
    {
        const char* ToString(shape_t value) { return ""; }

    } // namespace EGraphShape

    bool Export(DirectedAcyclicGraph const& dag, alloc_t* stack, alloc_t* heap, void*& outData, u32& outSize)
    {
        outData = nullptr;
        outSize = 0;

        // Directed Acyclic Graph (DAG) Export ** Simple Binary Format **
        //   u32: Num Nodes
        //   [u32: Node-ID, u32: Color, u32: Shape, u32: Label ID]
        //   u32: Num Edges
        //   [u32: From(Node ID), u32: To(Node ID), u32: Color, u32: Label ID]
        //   Labels
        //   [u32: Label ID, u8[64]: Label]
        // End

        DAGNodeID* nodes    = nullptr;
        u32        numNodes = 0;
        dag.GetAllNodes(stack, nodes, numNodes);

        DAGEdgeID* edges    = nullptr;
        u32        numEdges = 0;
        dag.GetAllEdges(stack, edges, numEdges);

        u64 data_size = 0;
        data_size += numNodes * (sizeof(u32) + sizeof(u32) + sizeof(u32) + sizeof(u32));
        data_size += numEdges * (sizeof(u32) + sizeof(u32) + sizeof(u32) + sizeof(u32));

        data_size += numNodes * (sizeof(u32) + 64);
        data_size += numEdges * (sizeof(u32) + 64);

        u8*             data = (u8*)heap->allocate(data_size);
        binary_writer_t writer(data, data + data_size);

        u64 const file_id = 0x2044414756497A20; // ' DAGViz '
        writer.write(file_id);

        // Write the number of nodes
        writer.write(numNodes);
        for (s32 i = 0; i < numNodes; ++i)
        {
            DAGNode const* node = dag.GetNode(nodes[i]);
            writer.write(nodes[i].index);

            if (dag.HasAttachment(nodes[i], EDagComponents::NodeVizComponent))
            {
                DAGNodeViz const* viz = dag.GetAttachment<DAGNodeViz>(nodes[i], (u16)EDagComponents::NodeVizComponent);

                writer.write((u32)viz->m_NodeColor);
                writer.write((u32)viz->m_EdgeColor);
                writer.write((u32)viz->m_Shape);
                writer.write(i); // Label ID
            }
            else // Default
            {
                writer.write(EGraphColor::DarkOliveGreen);
                writer.write(EGraphColor::DarkOliveGreen);
                writer.write(EGraphShape::Rectangle);
                writer.write((u32)0xFFFFFFFF); // Label ID None
            }
        }

        // Write the number of edges
        writer.write(numEdges);
        for (s32 i = 0; i < numEdges; ++i)
        {
            DAGEdge const* edge = dag.GetEdge(edges[i]);

            writer.write(edge->m_from.m_node.index);
            writer.write(edge->m_to.m_node.index);

            if (dag.HasAttachment(edges[i], EDagComponents::EdgeVizComponent))
            {
                DAGEdgeViz const* viz = dag.GetAttachment<DAGEdgeViz>(edges[i], (u16)EDagComponents::EdgeVizComponent);
                writer.write((u32)viz->m_EdgeColor);
                writer.write(numNodes + i); // Label ID
            }
            else
            {
                writer.write(EGraphColor::DarkOliveGreen);
                writer.write((u32)0xFFFFFFFF); // Label ID None
            }
        }

        // Write out all the label content
        for (s32 i = 0; i < numNodes; ++i)
        {
            DAGNode const* node = dag.GetNode(nodes[i]);
            if (dag.HasAttachment(nodes[i], EDagComponents::NodeVizComponent))
            {
                writer.write(i); // Label ID
                DAGNodeViz const* viz = dag.GetAttachment<DAGNodeViz>(nodes[i], (u16)EDagComponents::NodeVizComponent);
                writer.write_string(viz->m_Label, viz->m_Label + sizeof(viz->m_Label), 0); // [u32:length, u8: format, u8[length]: string]
            }
        }
        for (s32 i = 0; i < numEdges; ++i)
        {
            DAGEdge const* edge = dag.GetEdge(edges[i]);
            if (dag.HasAttachment(edges[i], EDagComponents::EdgeVizComponent))
            {
                writer.write(numNodes + i); // Label ID
                DAGEdgeViz const* viz = dag.GetAttachment<DAGEdgeViz>(nodes[i], (u16)EDagComponents::EdgeVizComponent);
                writer.write_string(viz->m_Label, viz->m_Label + sizeof(viz->m_Label), 0); // [u32:length, u8: format, u8[length]: string]
            }
        }

        return false;
    }

} // namespace ncore
