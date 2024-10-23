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

            dag.Setup(Allocator, 64, 16, 8, 64, 16, 8);

            // ...

            dag.Teardown();
        }

        UNITTEST_TEST(create_nodes)
        {
            DirectedAcyclicGraph dag;

            dag.Setup(Allocator, 64, 16, 8, 64, 16, 8);

            DAGNode* nodeA = dag.CreateNode();
            DAGNode* nodeB = dag.CreateNode();
            DAGNode* nodeC = dag.CreateNode();

            dag.Teardown();
        }

        UNITTEST_TEST(create_edges)
        {
            DirectedAcyclicGraph dag;

            dag.Setup(Allocator, 64, 16, 8, 64, 16, 8);

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

            dag.Setup(Allocator, 64, 16, 8, 64, 16, 8);

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

        UNITTEST_TEST(iterate_incoming)
        {
            DirectedAcyclicGraph dag;

            dag.Setup(Allocator, 64, 16, 8, 64, 16, 8);

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

            DAGEdge** incomingEdges;
            u32       numIncomingEdges = 0;
            dag.GetIncomingEdges(nodeB, Allocator, incomingEdges, numIncomingEdges);

            CHECK_TRUE(numIncomingEdges == 1);
            CHECK_TRUE(incomingEdges[0] == edgeAB);

            Allocator->deallocate(incomingEdges);

            DAGEdge** incomingEdgesA;
            u32       numIncomingEdgesA = 0;
            dag.GetIncomingEdges(nodeA, Allocator, incomingEdgesA, numIncomingEdgesA);

            CHECK_TRUE(numIncomingEdgesA == 1);
            CHECK_TRUE(incomingEdgesA[0] == edgeBA);

            Allocator->deallocate(incomingEdgesA);

            dag.Teardown();
        }

        UNITTEST_TEST(iterate_outgoing)
        {
            DirectedAcyclicGraph dag;

            dag.Setup(Allocator, 64, 16, 8, 64, 16, 8);

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

            DAGEdge** outgoingEdges;
            u32       numOutgoingEdges = 0;
            dag.GetOutgoingEdges(nodeB, Allocator, outgoingEdges, numOutgoingEdges);

            CHECK_TRUE(numOutgoingEdges == 2);
            CHECK_TRUE(outgoingEdges[0] == edgeBC || outgoingEdges[0] == edgeBA);
            CHECK_TRUE(outgoingEdges[1] == edgeBC || outgoingEdges[1] == edgeBA);

            Allocator->deallocate(outgoingEdges);

            DAGEdge** outgoingEdgesA;
            u32       numOutgoingEdgesA = 0;
            dag.GetOutgoingEdges(nodeA, Allocator, outgoingEdgesA, numOutgoingEdgesA);

            CHECK_TRUE(numOutgoingEdgesA == 2);
            CHECK_TRUE(outgoingEdgesA[0] == edgeAB || outgoingEdgesA[0] == edgeAD);
            CHECK_TRUE(outgoingEdgesA[1] == edgeAD || outgoingEdgesA[1] == edgeAB);

            Allocator->deallocate(outgoingEdgesA);

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

            dag.Setup(Allocator, 64, 16, 8, 64, 16, 8);

            // Make sure we can decorate the nodes and edges with visual information
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
