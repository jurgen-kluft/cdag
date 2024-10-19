#include "ccore/c_target.h"

#include "cdag/c_dag.h"

namespace ncore
{
    struct DAGNodeTags
    {
        u64 m_tags;
    };

    void DirectedAcyclicGraph::Setup(alloc_t* allocator, u32 max_nodes, u32 max_edges, u32 max_node_attachments, u32 max_edge_attachments)
    {
        m_allocator = allocator;
        m_pool.setup(allocator, 128, 128);

        m_pool.register_object_type<DAGNode>(EDagObjects::Node, max_nodes, max_node_attachments);
        m_pool.register_object_type<DAGEdge>(EDagObjects::Edge, max_edges, max_edge_attachments);

        // DAGNodes have tags
        m_pool.register_component_type<DAGNodeTags>(EDagObjects::Node, EDagObjects::Tags);
    }

    DAGNode*       DirectedAcyclicGraph::GetNode(DAGNodeID id) { return m_pool.get_object<DAGNode>(id); }
    DAGNode const* DirectedAcyclicGraph::GetNode(DAGNodeID id) const { return m_pool.get_object<DAGNode>(id); }
    DAGEdge*       DirectedAcyclicGraph::GetEdge(DAGEdgeID id) { return m_pool.get_object<DAGEdge>(id); }
    DAGEdge const* DirectedAcyclicGraph::GetEdge(DAGEdgeID id) const { return m_pool.get_object<DAGEdge>(id); }

    DAGEdgeID DirectedAcyclicGraph::FindEdge(DAGNodeID from, DAGNodeID to) const
    {
        // This is a directed graph, so we have to look at the outgoing edges.
        DAGNode const* pNode = GetNode(from);

        // Iterate over the list of edges that are going out from the node with the id 'from'.
        // See if there are any edges that are connected to the node with the id 'to'.
        DAGEdgeID edge = pNode->m_Outgoing;
        while (edge != nobject::c_invalid_handle)
        {
            if (GetEdge(edge)->m_to.m_node == to)
            {
                return edge;
            }
            edge = GetEdge(edge)->m_from.m_next;
        }

        return nobject::c_invalid_handle;
    }

    bool DirectedAcyclicGraph::IsEdgeValid(DAGEdgeID edgeId) const
    {
        DAGEdge const* edge = GetEdge(edgeId);
        return !IsNodeCulled(edge->m_from.m_node) && !IsNodeCulled(edge->m_to.m_node);
    }

    DAGNodeID DirectedAcyclicGraph::CreateNode()
    {
        DAGNodeID nodeId = m_pool.allocate_object(EDagObjects::Node);
        DAGNode*  node   = m_pool.get_object<DAGNode>(nodeId);
        node->m_Incoming = nobject::c_invalid_handle;
        node->m_Outgoing = nobject::c_invalid_handle;
        m_pool.allocate_component(nodeId, EDagObjects::Tags);
        return nodeId;
    }

    DAGEdgeID DirectedAcyclicGraph::CreateEdge(DAGNodeID fromId, DAGNodeID toId)
    {
        DAGEdgeID edgeId = m_pool.allocate_object(EDagObjects::Edge);
        DAGEdge*  edge   = m_pool.get_object<DAGEdge>(edgeId);

        DAGNode* from               = GetNode(fromId);
        edge->m_from.m_next         = from->m_Outgoing;
        edge->m_from.m_prev         = nobject::c_invalid_handle;
        edge->m_from.m_node         = fromId;
        DAGEdge* fromOutgoing       = GetEdge(from->m_Outgoing);
        fromOutgoing->m_from.m_prev = edgeId;
        from->m_Outgoing            = edgeId;

        DAGNode* to             = GetNode(toId);
        edge->m_to.m_next       = to->m_Incoming;
        edge->m_to.m_prev       = nobject::c_invalid_handle;
        edge->m_to.m_node       = toId;
        DAGEdge* toIncoming     = GetEdge(to->m_Incoming);
        toIncoming->m_to.m_prev = edgeId;
        to->m_Incoming          = edgeId;

        return edgeId;
    }

    void DirectedAcyclicGraph::ClearConnectivity()
    {
        // todo: implement
    }

    void DirectedAcyclicGraph::LockNode(DAGNodeID node)
    {
        DAGNodeTags* tags = m_pool.get_component<DAGNodeTags>(node, EDagObjects::Tags);
        tags->m_tags |= (1 << EDagTags::Locked);
    }

    bool DirectedAcyclicGraph::IsNodeLocked(DAGNodeID node) const
    {
        DAGNodeTags const* tags = m_pool.get_component<DAGNodeTags>(node, EDagObjects::Tags);
        return (tags->m_tags & (1 << EDagTags::Locked)) != 0;
    }

    void DirectedAcyclicGraph::Cull(alloc_t* allocator)
    {
        // TODO Cull the nodes that end up with a reference count of 0
    }

    void DirectedAcyclicGraph::CullNode(DAGNodeID _node)
    {
        DAGNodeTags* tags = m_pool.get_component<DAGNodeTags>(_node, EDagObjects::Tags);
        tags->m_tags |= (1 << EDagTags::Culled);
    }

    bool DirectedAcyclicGraph::IsNodeCulled(DAGNodeID _node) const
    {
        DAGNodeTags const* tags = m_pool.get_component<DAGNodeTags>(_node, EDagObjects::Tags);
        return (tags->m_tags & (1 << EDagTags::Culled)) != 0;
    }

    void DirectedAcyclicGraph::GetActiveNodes(alloc_t* allocator, DAGNodeID*& outNodes, u32& outNumNodes) const
    {
        outNumNodes = 0;
        outNodes    = (DAGNodeID*)allocator->allocate(m_pool.get_number_of_objects(EDagObjects::Node) * sizeof(DAGNodeID));

        nobject::handle_t handle = m_pool.iterate_objects<DAGNode>(EDagObjects::Node, nullptr);
        while (handle != nobject::c_invalid_handle)
        {
            if (!IsNodeCulled(handle))
            {
                outNodes[outNumNodes++] = handle;
            }
        }
    }

    void DirectedAcyclicGraph::GetIncomingEdges(DAGNodeID _node, alloc_t* allocator, DAGEdgeID*& outEdges, u32& outNumEdges) const
    {
        DAGNode const* node = GetNode(_node);

        // Count the number of incoming edges on this node
        u32       numEdges = 0;
        DAGEdgeID edge     = node->m_Incoming;
        while (edge != nobject::c_invalid_handle)
        {
            numEdges++;
            edge = GetEdge(edge)->m_to.m_next;
        }

        // Allocate memory for the outgoing edges
        outEdges    = (DAGEdgeID*)allocator->allocate(numEdges * sizeof(DAGEdgeID));
        outNumEdges = numEdges;

        // Copy the incoming edges to the output array
        u32 i = 0;
        edge  = node->m_Incoming;
        while (edge != nobject::c_invalid_handle)
        {
            outEdges[i++] = edge;
            edge          = GetEdge(edge)->m_to.m_next;
        }
    }

    void DirectedAcyclicGraph::GetOutgoingEdges(DAGNodeID _node, alloc_t* allocator, DAGEdgeID*& outEdges, u32& outNumEdges) const
    {
        DAGNode const* node = GetNode(_node);

        // Count the number of outgoing edges on this node
        u32       numEdges = 0;
        DAGEdgeID edge     = node->m_Outgoing;
        while (edge != nobject::c_invalid_handle)
        {
            numEdges++;
            edge = GetEdge(edge)->m_from.m_next;
        }

        // Allocate memory for the outgoing edges
        outEdges    = (DAGEdgeID*)allocator->allocate(numEdges * sizeof(DAGEdgeID));
        outNumEdges = numEdges;

        // Copy the outgoing edges to the output array
        u32 i = 0;
        edge  = node->m_Outgoing;
        while (edge != nobject::c_invalid_handle)
        {
            outEdges[i++] = edge;
            edge          = GetEdge(edge)->m_from.m_next;
        }
    }

} // namespace ncore
