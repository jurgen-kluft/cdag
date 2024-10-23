#include "ccore/c_target.h"

#include "cdag/c_dag.h"

namespace ncore
{
    void DirectedAcyclicGraph::Setup(alloc_t* allocator, u32 max_nodes, u32 max_edges, u32 max_node_attachments, u32 max_edge_attachments)
    {
        m_allocator = allocator;
        m_pool.setup(allocator, 128);

        m_pool.register_object<DAGNode>(max_nodes, max_node_attachments, 32);
        m_pool.register_object<DAGEdge>(max_edges, max_edge_attachments, 32);
    }

    void DirectedAcyclicGraph::Teardown() { m_pool.teardown(); }

    DAGEdge* DirectedAcyclicGraph::FindEdge(DAGNode const* from, DAGNode const* to) const
    {
        // Iterate over the list of edges that are going out from the node with the id 'from'.
        // See if there are any edges that are connected to the node with the id 'to'.
        DAGEdge* edge = from->m_Outgoing;
        while (edge != nullptr)
        {
            if (edge->m_to.m_node == to)
            {
                return edge;
            }
            edge = edge->m_from.m_next;
        }

        return nullptr;
    }

    bool DirectedAcyclicGraph::IsEdgeValid(DAGEdge const* edge) const { return !IsNodeCulled(edge->m_from.m_node) && !IsNodeCulled(edge->m_to.m_node); }

    DAGNode* DirectedAcyclicGraph::CreateNode()
    {
        DAGNode* node    = m_pool.create_object<DAGNode>();
        node->m_Incoming = nullptr;
        node->m_Outgoing = nullptr;
        return node;
    }

    DAGEdge* DirectedAcyclicGraph::CreateEdge(DAGNode* from, DAGNode* to)
    {
        DAGEdge* edge = m_pool.create_object<DAGEdge>();

        edge->m_from.m_next = from->m_Outgoing;
        edge->m_from.m_prev = nullptr;
        ;
        edge->m_from.m_node = from;
        if (from->m_Outgoing != nullptr)
        {
            from->m_Outgoing->m_from.m_prev = edge;
        }
        from->m_Outgoing = edge;

        edge->m_to.m_next = to->m_Incoming;
        edge->m_to.m_prev = nullptr;
        edge->m_to.m_node = to;
        if (to->m_Incoming != nullptr)
        {
            to->m_Incoming->m_to.m_prev = edge;
        }
        to->m_Incoming = edge;

        return edge;
    }

    void DirectedAcyclicGraph::ClearConnectivity()
    {
        // todo: implement
    }

    void DirectedAcyclicGraph::LockNode(DAGNode const* node) { m_pool.add_tag<DAGNode>(node, EDagTags::Locked); }
    bool DirectedAcyclicGraph::IsNodeLocked(DAGNode const* node) const { return m_pool.has_tag<DAGNode>(node, EDagTags::Locked); }

    void DirectedAcyclicGraph::Cull(alloc_t* allocator)
    {
        // TODO Cull the nodes that end up with a reference count of 0
    }

    void DirectedAcyclicGraph::CullNode(DAGNode const* _node) { m_pool.add_tag<DAGNode>(_node, EDagTags::Culled); }
    bool DirectedAcyclicGraph::IsNodeCulled(DAGNode const* _node) const { return m_pool.has_tag<DAGNode>(_node, EDagTags::Culled); }

    void DirectedAcyclicGraph::GetAllNodes(alloc_t* allocator, DAGNode**& outNodes, u32& outNumNodes) const
    {
        outNumNodes = m_pool.get_number_of_instances<DAGNode>();
        outNodes    = (DAGNode**)allocator->allocate(outNumNodes * sizeof(DAGNode*));

        u32      i    = 0;
        DAGNode* iter = m_pool.begin<DAGNode>();
        while (iter != nullptr)
        {
            outNodes[i++] = iter;
            iter          = m_pool.next(iter);
        }
    }

    void DirectedAcyclicGraph::GetAllEdges(alloc_t* allocator, DAGEdge**& outEdges, u32& outNumEdges) const
    {
        outNumEdges = m_pool.get_number_of_instances<DAGEdge>();
        outEdges    = (DAGEdge**)allocator->allocate(outNumEdges * sizeof(DAGEdge*));

        u32      i    = 0;
        DAGEdge* iter = m_pool.begin<DAGEdge>();
        while (iter != nullptr)
        {
            outEdges[i++] = iter;
            iter          = m_pool.next(iter);
        }
    }

    void DirectedAcyclicGraph::GetActiveNodes(alloc_t* allocator, DAGNode**& outNodes, u32& outNumNodes) const
    {
        outNumNodes = 0;
        outNodes    = (DAGNode**)allocator->allocate(m_pool.get_number_of_instances<DAGNode>() * sizeof(DAGNode*));

        DAGNode* iter = m_pool.begin<DAGNode>();
        while (iter != nullptr)
        {
            if (!IsNodeCulled(iter))
            {
                outNodes[outNumNodes++] = iter;
            }
            iter = m_pool.next(iter);
        }
    }

    void DirectedAcyclicGraph::GetIncomingEdges(DAGNode const* _node, alloc_t* allocator, DAGEdge**& outEdges, u32& outNumEdges) const
    {
        // Count the number of incoming edges on this node
        u32      numEdges = 0;
        DAGEdge* edge     = _node->m_Incoming;
        while (edge != nullptr)
        {
            numEdges++;
            edge = edge->m_to.m_next;
        }

        // Allocate memory for the outgoing edges
        outEdges    = (DAGEdge**)allocator->allocate(numEdges * sizeof(DAGEdge*));
        outNumEdges = numEdges;

        // Copy the incoming edges to the output array
        u32 i = 0;
        edge  = _node->m_Incoming;
        while (edge != nullptr)
        {
            outEdges[i++] = edge;
            edge          = edge->m_to.m_next;
        }
    }

    void DirectedAcyclicGraph::GetOutgoingEdges(DAGNode const* _node, alloc_t* allocator, DAGEdge**& outEdges, u32& outNumEdges) const
    {
        // Count the number of outgoing edges on this node
        u32      numEdges = 0;
        DAGEdge* edge     = _node->m_Outgoing;
        while (edge != nullptr)
        {
            numEdges++;
            edge = edge->m_from.m_next;
        }

        // Allocate memory for the outgoing edges
        outEdges    = (DAGEdge**)allocator->allocate(numEdges * sizeof(DAGEdge*));
        outNumEdges = numEdges;

        // Copy the outgoing edges to the output array
        u32 i = 0;
        edge  = _node->m_Outgoing;
        while (edge != nullptr)
        {
            outEdges[i++] = edge;
            edge          = edge->m_from.m_next;
        }
    }

} // namespace ncore
