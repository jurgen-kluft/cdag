#ifndef __CDAG_DAG_VIZ_H__
#define __CDAG_DAG_VIZ_H__
#include "ccore/c_target.h"
#ifdef USE_PRAGMA_ONCE
#    pragma once
#endif

#include "cdag/c_dag.h"

namespace ncore
{
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
        const u16 NodeVizComponent = 0;
        const u16 EdgeVizComponent = 1;
    }

    struct DAGNodeViz
    {
        DCORE_CLASS_PLACEMENT_NEW_DELETE
        EGraphColor::VALUE m_NodeColor;
        EGraphColor::VALUE m_EdgeColor;
        EGraphShape::VALUE m_Shape;
        char               m_Label[64];
    };

    struct DAGEdgeViz
    {
        DCORE_CLASS_PLACEMENT_NEW_DELETE
        EGraphColor::VALUE m_EdgeColor;
        char               m_Label[64];
    };

    bool Export(DirectedAcyclicGraph const& dag, alloc_t* stack, alloc_t* heap, void*& outData, u32& outSize);

} // namespace ncore

#endif
