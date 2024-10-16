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
    using DAGObjectID   = nobject::handle_t;
    using DAGNodeID     = DAGObjectID;
    using DAGEdgeID     = DAGObjectID;
    using DAGEdgeNodeID = DAGObjectID;

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
        };
    }

    struct DAGEdge
    {
        DECLARE_OBJECT_TYPE(EDagObjects::Edge);
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
        DECLARE_OBJECT_TYPE(EDagObjects::Node);
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
        DECLARE_COMPONENT_TYPE(EDagComponents::VizComponent);
        DCORE_CLASS_PLACEMENT_NEW_DELETE
        EGraphColor::VALUE m_NodeColor;
        EGraphColor::VALUE m_EdgeColor;
        EGraphShape::VALUE m_Shape;
    };

    class DirectedAcyclicGraph
    {
    public:
        void setup(alloc_t* allocator);

        DAGNode*  GetNode(DAGNodeID id) const;
        DAGEdge*  GetEdge(DAGEdgeID id) const;
        DAGEdgeID FindEdge(DAGNodeID from, DAGNodeID to) const;
        bool      IsEdgeValid(DAGEdgeID edge) const;

        DAGNodeID CreateNode();
        DAGEdgeID CreateEdge(DAGNodeID from, DAGNodeID to);

        void ClearConnectivity();

        void LockNode(DAGNodeID node);
        bool IsNodeLocked(DAGNodeID node) const;
        void Cull(alloc_t* allocator);
        bool IsNodeCulled(DAGNodeID node) const;

        void GetIncomingEdges(DAGNodeID node, alloc_t* allocator, DAGEdgeID*& outEdges, u32& outNumEdges) const;
        void GetOutgoingEdges(DAGNodeID node, alloc_t* allocator, DAGEdgeID*& outEdges, u32& outNumEdges) const;

        template <typename T> T*   AddDecoration(DAGObjectID o);
        template <typename T> T*   GetDecoration(DAGObjectID o);
        template <typename T> void RemDecoration(DAGObjectID o);

        ascii::prune ExportGraphviz(alloc_t* allocator);

    private:
    alloc_t* m_allocator;
        nobject::nobjects_with_components::pool_t m_pool;
    };

} // namespace ncore

#endif
