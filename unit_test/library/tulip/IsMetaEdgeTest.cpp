#include <tulip/BooleanProperty.h>
#include <tulip/GraphProperty.h>
#include <tulip/ForEach.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>

#include "IsMetaEdgeTest.h"

using namespace tlp;
using namespace std;

const unsigned int nbNodesPerCluster = 5;

void createCompleteGraphAndSelect(Graph *graph, BooleanProperty *selection) {

	vector<node> addedNodes;
	for (unsigned int j=0; j<nbNodesPerCluster; ++j) {
		node n = graph->addNode();
		addedNodes.push_back(n);
		selection->setNodeValue(n, true);
	}

	for (size_t i = 0 ; i < addedNodes.size() ; ++i) {
		for (size_t j = 0 ; j < addedNodes.size() ; ++j) {
			if (addedNodes[i] != addedNodes[j]) {
				edge e = graph->addEdge(addedNodes[i], addedNodes[j]);
				selection->setEdgeValue(e, true);
			}
		}
	} 
}

#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION( IsMetaEdgeTest );

void IsMetaEdgeTest::setUp() {
	
	graph = tlp::newGraph();
	BooleanProperty selec(graph);
	
	// create a first complete graph that will be our first cluster
	createCompleteGraphAndSelect(graph, &selec);
	Graph *sg1 = graph->addSubGraph(&selec);
	
	selec.setAllNodeValue(false);
	selec.setAllEdgeValue(false);
	
	// create another complete graph that will be our second cluster
	createCompleteGraphAndSelect(graph, &selec);
	Graph *sg2 = graph->addSubGraph(&selec);

	// link each node of the first cluster to each node of the second cluster
	node n1;
	forEach(n1, sg1->getNodes()) {
		node n2;
		forEach(n2, sg2->getNodes()) {	
			graph->addEdge(n1, n2);
		}
	}

	// Create our quotient graph, should contain two meta-nodes and one meta-edge
	quotientGraph = tlp::newSubGraph(graph);
	Iterator<Graph *> *itS = graph->getSubGraphs();
	vector<node> mNodes;
	graph->createMetaNodes(itS, quotientGraph, mNodes);
	delete itS;

}

void IsMetaEdgeTest::tearDown() {
	graph->delAllSubGraphs(quotientGraph);
	delete graph;
}

void IsMetaEdgeTest::testIsMetaEdge() {
	unsigned int nbMetaNodes = 0;
	unsigned int nbMetaEdges = 0;

	// check if we have two nodes and one edge in the quotient
	CPPUNIT_ASSERT(quotientGraph->numberOfNodes() == 2);
	CPPUNIT_ASSERT(quotientGraph->numberOfEdges() == 1);
	
	node n;
	forEach(n, quotientGraph->getNodes()) {
		if (quotientGraph->isMetaNode(n)) {
			++nbMetaNodes;
		}
	}

	edge e;
	forEach(e, quotientGraph->getEdges()) {
		if (quotientGraph->isMetaEdge(e)) {
			++nbMetaEdges;
		}
	}
	
	// check that the two quotient nodes are meta nodes
	CPPUNIT_ASSERT(nbMetaNodes == 2);
	
	
	set<edge> underlyingEdgesInMetaEdge = quotientGraph->getProperty<GraphProperty>("viewMetaGraph")->getEdgeValue(quotientGraph->getOneEdge());

	// check the number of underlying edges in meta edge	
	CPPUNIT_ASSERT(underlyingEdgesInMetaEdge.size() == nbNodesPerCluster * nbNodesPerCluster);

	// check if the quotient edge is a meta edge
	CPPUNIT_ASSERT(nbMetaEdges == 1);
	
}

CppUnit::Test *IsMetaEdgeTest::suite() {
	CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : MetaEdge" );
 	suiteOfTests->addTest( new CppUnit::TestCaller<IsMetaEdgeTest>( "isMetaEdge test", 
							      &IsMetaEdgeTest::testIsMetaEdge));
	return suiteOfTests;
}
