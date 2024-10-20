#include "ccore/c_target.h"
#include "ccore/c_allocator.h"

#include "cdag/c_dag.h"
#include "cdag/c_dag_viz.h"

#include "cunittest/cunittest.h"

using namespace ncore;

UNITTEST_SUITE_BEGIN(dag)
{
    UNITTEST_FIXTURE(main)
    {
        UNITTEST_ALLOCATOR;

        UNITTEST_FIXTURE_SETUP() {}
        UNITTEST_FIXTURE_TEARDOWN() {}

        UNITTEST_TEST(setup)
        {
            DirectedAcyclicGraph dag;

            dag.Setup(Allocator, 64, 64, 16, 16);

            // ...

            dag.Teardown();
        }

        UNITTEST_TEST(create_nodes)
        {
            DirectedAcyclicGraph dag;

            dag.Setup(Allocator, 64, 64, 16, 16);

            DAGNode* nodeA = dag.CreateNode();
            DAGNode* nodeB = dag.CreateNode();
            DAGNode* nodeC = dag.CreateNode();

            dag.Teardown();
        }

        UNITTEST_TEST(create_edges)
        {
            DirectedAcyclicGraph dag;

            dag.Setup(Allocator, 64, 64, 16, 16);

            DAGNode* nodeA = dag.CreateNode();
            DAGNode* nodeB = dag.CreateNode();
            DAGNode* nodeC = dag.CreateNode();

            DAGEdge* edgeAB = dag.CreateEdge(nodeA, nodeB);
            DAGEdge* edgeBC = dag.CreateEdge(nodeB, nodeC);

            dag.Teardown();
        }

        UNITTEST_TEST(find_edge)
        {
            DirectedAcyclicGraph dag;

            dag.Setup(Allocator, 64, 64, 16, 16);

            DAGNode* nodeA = dag.CreateNode();
            DAGNode* nodeB = dag.CreateNode();
            DAGNode* nodeC = dag.CreateNode();
            DAGNode* nodeD = dag.CreateNode();

            DAGEdge* edgeAB = dag.CreateEdge(nodeA, nodeB);
            DAGEdge* edgeAD = dag.CreateEdge(nodeA, nodeD);
            DAGEdge* edgeBC = dag.CreateEdge(nodeB, nodeC);
            DAGEdge* edgeBA = dag.CreateEdge(nodeB, nodeA);

            DAGEdge* findAB = dag.FindEdge(nodeA, nodeB);
            DAGEdge* findAD = dag.FindEdge(nodeA, nodeD);
            DAGEdge* findBC = dag.FindEdge(nodeB, nodeC);
            DAGEdge* findBA = dag.FindEdge(nodeB, nodeA);

            CHECK_TRUE(edgeAB == findAB);
            CHECK_TRUE(edgeBC == findBC);
            CHECK_TRUE(edgeAD == findAD);
            CHECK_TRUE(edgeBA == findBA);

            dag.Teardown();
        }

        static DAGNode* CreateNode(DirectedAcyclicGraph & dag, EGraphColor::rgb_t color, EGraphShape::shape_t shape, const char* label)
        {
            DAGNode*    node = dag.CreateNode();
            DAGNodeViz* viz  = dag.AddAttachment<DAGNodeViz>(node);
            viz->m_Color     = color;
            viz->m_Shape     = shape;
            SetLabel(viz, label);
            return node;
        }

        static DAGEdge* CreateEdge(DirectedAcyclicGraph & dag, DAGNode * from, DAGNode * to, EGraphColor::rgb_t color, const char* label)
        {
            DAGEdge*    edge = dag.CreateEdge(from, to);
            DAGEdgeViz* viz  = dag.AddAttachment<DAGEdgeViz>(edge);
            viz->m_EdgeColor = color;
            SetLabel(viz, label);
            return edge;
        }

        UNITTEST_TEST(export)
        {
            // Build an example graph
            // A -> B -> C
            DirectedAcyclicGraph dag;

            dag.Setup(Allocator, 64, 64, 16, 16);

            dag.RegisterAttachment<DAGNode, DAGNodeViz>();
            dag.RegisterAttachment<DAGEdge, DAGEdgeViz>();

            DAGNode* nodeA = CreateNode(dag, EGraphColor::DarkOliveGreen, EGraphShape::ShapeRectangle, "Node A");
            DAGNode* nodeB = CreateNode(dag, EGraphColor::LawnGreen, EGraphShape::ShapeRectangle, "Node B");
            DAGNode* nodeC = CreateNode(dag, EGraphColor::PalegoldenRod, EGraphShape::ShapeRectangle, "Node C");

            CreateEdge(dag, nodeA, nodeB, EGraphColor::Honeydew, "Edge A -> B");
            CreateEdge(dag, nodeB, nodeC, EGraphColor::DarkBlue, "Edge B -> C");

            byte* data = nullptr;
            u32   size = 0;
            CHECK_TRUE(Export(dag, Allocator, Allocator, data, size));
            UnitTest::gFileWrite("dag_viz.bin", data, (s32)size);

            Allocator->deallocate(data);

            dag.Teardown();
        }
    }
}
UNITTEST_SUITE_END
