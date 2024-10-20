#ifndef __CDAG_DAG_H__
#define __CDAG_DAG_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "callocator/c_allocator_object.h"

namespace ncore
{
    struct DAGNode;
    struct DAGEdge;

    struct DAGEdgeNode
    {
        DAGNode* m_node;
        DAGEdge* m_next;
        DAGEdge* m_prev;
    };

    namespace EDagObjects
    {
        const u16 Node = 0;
        const u16 Edge = 1;
    } // namespace EDagObjects

    struct DAGEdge
    {
        D_DECLARE_OBJECT_TYPE(EDagObjects::Edge);
        DCORE_CLASS_PLACEMENT_NEW_DELETE

        DAGEdgeNode m_from;
        DAGEdgeNode m_to;
    };

    struct DAGNode
    {
        D_DECLARE_OBJECT_TYPE(EDagObjects::Node);
        DCORE_CLASS_PLACEMENT_NEW_DELETE

        DAGEdge* m_Outgoing;
        DAGEdge* m_Incoming;
    };

    namespace EDagComponents
    {
        const u16 Tags = 0;
    }

    namespace EDagTags
    {
        const s8 Locked = 0;
        const s8 Culled = 1;
    } // namespace EDagTags

    class DirectedAcyclicGraph
    {
    public:
        void Setup(alloc_t* allocator, u32 max_nodes, u32 max_edges, u32 max_node_attachments, u32 max_edge_attachments);
        void Teardown();

        DAGEdge* FindEdge(DAGNode const* from, DAGNode const* to) const;
        bool     IsEdgeValid(DAGEdge const* edge) const;

        DAGNode* CreateNode();
        DAGEdge* CreateEdge(DAGNode* from, DAGNode* to);

        void ClearConnectivity();

        void LockNode(DAGNode const* node);
        bool IsNodeLocked(DAGNode const* node) const;
        void Cull(alloc_t* allocator);
        void CullNode(DAGNode const* node);
        bool IsNodeCulled(DAGNode const* node) const;

        void GetAllNodes(alloc_t* allocator, DAGNode**& outNodes, u32& outNumNodes) const;
        void GetAllEdges(alloc_t* allocator, DAGEdge**& outEdges, u32& outNumEdges) const;
        void GetActiveNodes(alloc_t* allocator, DAGNode**& outNodes, u32& outNumNodes) const;
        void GetIncomingEdges(DAGNode const* node, alloc_t* allocator, DAGEdge**& outEdges, u32& outNumEdges) const;
        void GetOutgoingEdges(DAGNode const* node, alloc_t* allocator, DAGEdge**& outEdges, u32& outNumEdges) const;

        template <typename U, typename T> bool     HasAttachment(T const* obj) const;
        template <typename T, typename U> bool     RegisterAttachment();
        template <typename U, typename T> U*       AddAttachment(T* obj);
        template <typename U, typename T> U*       GetAttachment(T* obj);
        template <typename U, typename T> U const* GetAttachment(T* obj) const;
        template <typename U, typename T> void     RemAttachment(T* obj);

    private:
        alloc_t*                                  m_allocator;
        nobject::nobjects_with_components::pool_t m_pool;
    };

    template <typename U, typename T> bool     DirectedAcyclicGraph::HasAttachment(T const* obj) const { return m_pool.has_component<U, T>(obj); }
    template <typename T, typename U> bool     DirectedAcyclicGraph::RegisterAttachment() { return m_pool.register_component<T, U>(); }
    template <typename U, typename T> U*       DirectedAcyclicGraph::AddAttachment(T* obj) { return m_pool.allocate_component<U, T>(obj); }
    template <typename U, typename T> U*       DirectedAcyclicGraph::GetAttachment(T* obj) { return m_pool.get_component<U, T>(obj); }
    template <typename U, typename T> U const* DirectedAcyclicGraph::GetAttachment(T* obj) const { return m_pool.get_component<U, T>(obj); }
    template <typename U, typename T> void     DirectedAcyclicGraph::RemAttachment(T* obj) { m_pool.deallocate_component<U, T>(obj); }

} // namespace ncore

#endif
