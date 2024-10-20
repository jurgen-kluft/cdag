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

    void SetLabel(char* dst, s32 dstMaxLen, char const* label)
    {
        s32 len = 0;
        while (len < dstMaxLen)
        {
            dst[len++] = *label;
            label += (*label == '\0') ? 0 : 1;
        }
        dst[len] = '\0';
    }

    void SetLabel(DAGNodeViz* viz, char const* label) { SetLabel(&viz->m_Label[0], sizeof(viz->m_Label) - 1, label); }
    void SetLabel(DAGEdgeViz* viz, char const* label) { SetLabel(&viz->m_Label[0], sizeof(viz->m_Label) - 1, label); }

    bool Export(DirectedAcyclicGraph const& dag, alloc_t* stack, alloc_t* heap, byte*& outData, u32& outSize)
    {
        outData = nullptr;
        outSize = 0;

        // Directed Acyclic Graph (DAG) Export ** Simple Binary Format **
        //   u64: File ID ' DAGViz '
        //   u32: Num Nodes
        //   [u64: Node-ID, u32: Color, u32: Shape, u32: Label ID]
        //   u32: Num Edges
        //   [u64: From(Node ID), u64: To(Node ID), u32: Color, u32: Label ID]
        //   Labels
        //   [u32: Label ID, u32: Length, u8: Format, u8[64]: Label]
        // End

        DAGNode** nodes    = nullptr;
        u32       numNodes = 0;
        dag.GetAllNodes(stack, nodes, numNodes);

        DAGEdge** edges    = nullptr;
        u32       numEdges = 0;
        dag.GetAllEdges(stack, edges, numEdges);

        // File ID, Num Nodes, Num Edges
        u64 data_size = sizeof(u64) + sizeof(u32) + sizeof(u32);
        data_size += numNodes * (sizeof(u64) + sizeof(u32) + sizeof(u32) + sizeof(u32));
        data_size += numEdges * (sizeof(u64) + sizeof(u64) + sizeof(u32) + sizeof(u32));

        s32 numLabels = 0;
        for (s32 i = 0; i < numNodes; ++i)
        {
            numLabels += (dag.HasAttachment<DAGNodeViz>(nodes[i])) ? 1 : 0;
        }
        for (s32 i = 0; i < numEdges; ++i)
        {
            numLabels += (dag.HasAttachment<DAGEdgeViz>(edges[i])) ? 1 : 0;
        }
        data_size += numLabels * (sizeof(u32) + sizeof(u32) + sizeof(u8) + 64);

        outData = (u8*)heap->allocate(data_size + 64);
        binary_writer_t writer(outData, outData + data_size);

        u64 const file_id = 0x2044414756497A20; // ' DAGViz '
        writer.write(file_id);

        // Global label ID
        u32 label_id = 0;

        // Write the number of nodes
        writer.write(numNodes);
        // Write the nodes
        for (s32 i = 0; i < numNodes; ++i)
        {
            DAGNode const* node = nodes[i];
            writer.write(node);

            if (dag.HasAttachment<DAGNodeViz>(node))
            {
                DAGNodeViz const* viz = dag.GetAttachment<DAGNodeViz>(node);

                writer.write((u32)viz->m_Color);
                writer.write((u32)viz->m_Shape);
                writer.write((u32)label_id++); // Label ID
            }
            else // Default
            {
                writer.write((u32)EGraphColor::DarkOliveGreen);
                writer.write((u32)EGraphShape::ShapeRectangle);
                writer.write((u32)0xFFFFFFFF); // Label ID None
            }
        }

        // Write the number of edges
        writer.write(numEdges);
        // Write the edges
        for (s32 i = 0; i < numEdges; ++i)
        {
            DAGEdge const* edge = edges[i];

            writer.write(edge->m_from.m_node);
            writer.write(edge->m_to.m_node);

            if (dag.HasAttachment<DAGEdgeViz>(edges[i]))
            {
                DAGEdgeViz const* viz = dag.GetAttachment<DAGEdgeViz>(edges[i]);
                writer.write((u32)viz->m_EdgeColor);
                writer.write((u32)label_id++); // Label ID
            }
            else
            {
                writer.write((u32)EGraphColor::DarkOliveGreen);
                writer.write((u32)0xFFFFFFFF); // Label ID None
            }
        }

        // Write out all the label content
        label_id = 0;
        for (s32 i = 0; i < numNodes; ++i)
        {
            DAGNode const* node = nodes[i];
            if (dag.HasAttachment<DAGNodeViz>(node))
            {
                writer.write(label_id++); // Label ID
                DAGNodeViz const* viz = dag.GetAttachment<DAGNodeViz>(node);
                writer.write_string(&viz->m_Label[0], &viz->m_Label[0] + sizeof(viz->m_Label), 0); // [u32:length, u8: format, u8[length]: string]
            }
        }
        for (s32 i = 0; i < numEdges; ++i)
        {
            DAGEdge const* edge = edges[i];
            if (dag.HasAttachment<DAGEdgeViz>(edge))
            {
                writer.write(label_id++); // Label ID
                DAGEdgeViz const* viz = dag.GetAttachment<DAGEdgeViz>(edge);
                writer.write_string(&viz->m_Label[0], &viz->m_Label[0] + sizeof(viz->m_Label), 0); // [u32:length, u8: format, u8[length]: string]
            }
        }

        stack->deallocate(nodes);
        stack->deallocate(edges);

        outSize = writer.size();

        return true;
    }

} // namespace ncore
