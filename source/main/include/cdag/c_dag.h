#ifndef __CDAG_DAG_H__
#define __CDAG_DAG_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cbase/c_runes.h"
#include "callocator/c_allocator_object.h"

namespace ncore
{
    using DAGObjectID     = nobject::handle_t;
    using DAGNodeID       = DAGObjectID;
    using DAGEdgeID       = DAGObjectID;
    using DAGEdgeNodeID   = DAGObjectID;
    using DAGAttachmentID = DAGObjectID;

    struct DAGEdgeNode
    {
        DAGNodeID m_node;
        DAGEdgeID m_next;
        DAGEdgeID m_prev;
    };

    namespace EDagObjects
    {
        enum VALUE
        {
            Edge = 0,
            Node = 1,
            Tags = 2,
        };
    }

    struct DAGEdge
    {
        DCORE_CLASS_PLACEMENT_NEW_DELETE

        DAGEdgeNode m_from;
        DAGEdgeNode m_to;

        DAGNodeID const& Get(DAGNodeID node) const
        {
            if (m_from.m_node == node)
                return m_to.m_node;
            return m_from.m_node;
        }
    };

    struct DAGNode
    {
        DCORE_CLASS_PLACEMENT_NEW_DELETE

        DAGEdgeID m_Outgoing;
        DAGEdgeID m_Incoming;
    };

    namespace EGraphColor
    {
        enum VALUE
        {
            DarkOliveGreen,
            Max
        };

        const char* ToString(VALUE value);
    } // namespace EGraphColor

    namespace EGraphShape
    {
        enum VALUE
        {
            Rectangle,
            Max
        };

        const char* ToString(VALUE value);
    } // namespace EGraphShape

    namespace EDagComponents
    {
        enum VALUE
        {
            VizComponent = 0
        };
    }

    namespace EDagTags
    {
        enum VALUE
        {
            Locked = 0,
            Culled = 1
        };
    }

    struct DAGNodeViz
    {
        DCORE_CLASS_PLACEMENT_NEW_DELETE
        EGraphColor::VALUE m_NodeColor;
        EGraphColor::VALUE m_EdgeColor;
        EGraphShape::VALUE m_Shape;
    };

    class DirectedAcyclicGraph
    {
    public:
        void Setup(alloc_t* allocator, u32 max_nodes, u32 max_edges, u32 max_node_attachments, u32 max_edge_attachments);

        DAGNode*       GetNode(DAGNodeID id);
        DAGNode const* GetNode(DAGNodeID id) const;
        DAGEdge*       GetEdge(DAGEdgeID id);
        DAGEdge const* GetEdge(DAGEdgeID id) const;
        DAGEdgeID      FindEdge(DAGNodeID from, DAGNodeID to) const;
        bool           IsEdgeValid(DAGEdgeID edge) const;

        DAGNodeID CreateNode();
        DAGEdgeID CreateEdge(DAGNodeID from, DAGNodeID to);

        void ClearConnectivity();

        void LockNode(DAGNodeID node);
        bool IsNodeLocked(DAGNodeID node) const;
        void Cull(alloc_t* allocator);
        void CullNode(DAGNodeID node);
        bool IsNodeCulled(DAGNodeID node) const;

        void GetActiveNodes(alloc_t* allocator, DAGNodeID*& outNodes, u32& outNumNodes) const;
        void GetIncomingEdges(DAGNodeID node, alloc_t* allocator, DAGEdgeID*& outEdges, u32& outNumEdges) const;
        void GetOutgoingEdges(DAGNodeID node, alloc_t* allocator, DAGEdgeID*& outEdges, u32& outNumEdges) const;

        bool                                  HasAttachment(DAGObjectID o, u16 const attachment_id) const;
        template <typename T> bool            RegisterAttachment(DAGObjectID o, u16 const attachment_id);
        template <typename T> DAGAttachmentID AddAttachment(DAGObjectID o, u16 const attachment_id);
        template <typename T> T*              GetAttachment(DAGObjectID o, u16 const attachment_id);
        template <typename T> void            RemAttachment(DAGObjectID o, u16 const attachment_id);

        ascii::prune ExportGraphviz(alloc_t* allocator);

    private:
        alloc_t*                                  m_allocator;
        nobject::nobjects_with_components::pool_t m_pool;
    };

    bool                                  DirectedAcyclicGraph::HasAttachment(DAGObjectID o, u16 const attachment_id) const { return m_pool.has_component(o, attachment_id); }
    template <typename T> bool            DirectedAcyclicGraph::RegisterAttachment(DAGObjectID o, u16 const attachment_id) { return m_pool.register_component_type<T>(o, attachment_id); }
    template <typename T> DAGAttachmentID DirectedAcyclicGraph::AddAttachment(DAGObjectID o, u16 const attachment_id) { return m_pool.allocate_component(o, attachment_id); }
    template <typename T> T*              DirectedAcyclicGraph::GetAttachment(DAGObjectID o, u16 const attachment_id) { return m_pool.get_component<T>(o, attachment_id); }
    template <typename T> void            DirectedAcyclicGraph::RemAttachment(DAGObjectID o, u16 const attachment_id) { m_pool.deallocate_component(o, attachment_id); }

} // namespace ncore

#endif
