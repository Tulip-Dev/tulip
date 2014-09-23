/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <tulip/ForEach.h>
#include "PushPopTest.h"
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/ConnectedTest.h>

using namespace std;
using namespace tlp;

CPPUNIT_TEST_SUITE_REGISTRATION( PushPopTest );

//==========================================================
void PushPopTest::setUp() {
  graph = tlp::newGraph();
}
//==========================================================
void PushPopTest::tearDown() {
  //graph->pop();
  delete graph;
}
//==========================================================
void PushPopTest::build(unsigned int nbNodes, unsigned int edgeRatio) {
  vector<node> nodes;
  vector<edge> edges;
  unsigned int NB_ADD  = nbNodes;
  unsigned int EDGE_RATIO = edgeRatio;

  for (unsigned int i=0; i<NB_ADD; ++i)
    nodes.push_back(graph->addNode());

  unsigned int NB_EDGES = EDGE_RATIO * NB_ADD;

  for (unsigned int i=0; i< NB_EDGES; ++i)
    edges.push_back(graph->addEdge(nodes[rand()%NB_ADD], nodes[rand()%NB_ADD]));
}
//==========================================================
void PushPopTest::testAddDel() {
  node n0 = graph->addNode();
  node n1 = graph->addNode();

  edge e0 = graph->addEdge(n0, n1);

  graph->push();
  node n2 = graph->addNode();
  node n3 = graph->addNode();

  edge e1 = graph->addEdge(n2, n3);
  CPPUNIT_ASSERT_EQUAL(e1, graph->existEdge(n2, n3));

  graph->reverse(e0);
  CPPUNIT_ASSERT_EQUAL(e0, graph->existEdge(n1, n0));

  graph->delNode(n0);
  CPPUNIT_ASSERT(!graph->isElement(n0));
  CPPUNIT_ASSERT(!graph->isElement(e0));

  vector<node> nodes;
  vector<pair <node, node> > ends;
  vector<edge> edges;

  graph->addNodes(3, nodes);
  CPPUNIT_ASSERT_EQUAL(size_t(3), nodes.size());
  ends.push_back(pair<node, node>(nodes[0], nodes[1]));
  ends.push_back(pair<node, node>(nodes[0], nodes[2]));
  ends.push_back(pair<node, node>(nodes[1], nodes[2]));
  graph->addEdges(ends, edges);
  CPPUNIT_ASSERT_EQUAL(size_t(3), edges.size());

  graph->pop();

  CPPUNIT_ASSERT(!graph->isElement(e1));
  CPPUNIT_ASSERT_EQUAL(e0, graph->existEdge(n0, n1));
  CPPUNIT_ASSERT(!graph->isElement(nodes[0]));
  CPPUNIT_ASSERT(!graph->isElement(nodes[1]));
  CPPUNIT_ASSERT(!graph->isElement(nodes[2]));

  graph->unpop();
  CPPUNIT_ASSERT_EQUAL(e1, graph->existEdge(n2, n3));
  CPPUNIT_ASSERT(!graph->isElement(e0));
  CPPUNIT_ASSERT(graph->isElement(nodes[0]));
  CPPUNIT_ASSERT(graph->isElement(nodes[1]));
  CPPUNIT_ASSERT(graph->isElement(nodes[2]));
  CPPUNIT_ASSERT_EQUAL(edges[0], graph->existEdge(nodes[0], nodes[1]));
  CPPUNIT_ASSERT_EQUAL(edges[1], graph->existEdge(nodes[0], nodes[2]));
  CPPUNIT_ASSERT_EQUAL(edges[2], graph->existEdge(nodes[1], nodes[2]));

  graph->reverse(e1);
  CPPUNIT_ASSERT_EQUAL(e1, graph->existEdge(n3, n2));

  graph->delEdges(edges);
  CPPUNIT_ASSERT(!graph->isElement(edges[0]));
  CPPUNIT_ASSERT(!graph->isElement(edges[1]));
  CPPUNIT_ASSERT(!graph->isElement(edges[2]));

  graph->pop();
  CPPUNIT_ASSERT(!graph->isElement(e1));
  CPPUNIT_ASSERT(!graph->isElement(n2));
  CPPUNIT_ASSERT(!graph->isElement(n3));
  CPPUNIT_ASSERT(!graph->isElement(nodes[0]));
  CPPUNIT_ASSERT(!graph->isElement(nodes[1]));
  CPPUNIT_ASSERT(!graph->isElement(nodes[2]));

  graph->unpop();
  CPPUNIT_ASSERT(graph->isElement(e1));
  CPPUNIT_ASSERT(graph->isElement(n2));
  CPPUNIT_ASSERT(graph->isElement(n3));
  CPPUNIT_ASSERT_EQUAL(e1, graph->existEdge(n3, n2));
  CPPUNIT_ASSERT(graph->isElement(nodes[0]));
  CPPUNIT_ASSERT(graph->isElement(nodes[1]));
  CPPUNIT_ASSERT(graph->isElement(nodes[2]));
  CPPUNIT_ASSERT(!graph->isElement(edges[0]));
  CPPUNIT_ASSERT(!graph->isElement(edges[1]));
  CPPUNIT_ASSERT(!graph->isElement(edges[2]));

  graph->delEdge(e1);
  CPPUNIT_ASSERT(!graph->isElement(e1));
  CPPUNIT_ASSERT(graph->isElement(n2));
  CPPUNIT_ASSERT(graph->isElement(n3));

  graph->delNode(n2);
  CPPUNIT_ASSERT(!graph->isElement(e1));
  CPPUNIT_ASSERT(!graph->isElement(n2));
  CPPUNIT_ASSERT(graph->isElement(n3));

  graph->pop();
  CPPUNIT_ASSERT(!graph->isElement(e1));
  CPPUNIT_ASSERT(!graph->isElement(n2));
  CPPUNIT_ASSERT(!graph->isElement(n3));

  graph->unpop();
  CPPUNIT_ASSERT(!graph->isElement(e1));
  CPPUNIT_ASSERT(!graph->isElement(n2));
  CPPUNIT_ASSERT(graph->isElement(n3));

}

//==========================================================
void PushPopTest::testSetEnds() {
  node n0 = graph->addNode();
  node n1 = graph->addNode();

  edge e0 = graph->addEdge(n0, n1);

  graph->push();
  node n2 = graph->addNode();
  node n3 = graph->addNode();

  graph->setEnds(e0, n2, n3);
  CPPUNIT_ASSERT_EQUAL(e0, graph->existEdge(n2, n3));

  graph->reverse(e0);
  CPPUNIT_ASSERT_EQUAL(e0, graph->existEdge(n3, n2));

  graph->pop();

  CPPUNIT_ASSERT_EQUAL(e0, graph->existEdge(n0, n1));

  graph->unpop();

  CPPUNIT_ASSERT_EQUAL(e0, graph->existEdge(n3, n2));

  graph->delEdge(e0);
  CPPUNIT_ASSERT(!graph->isElement(e0));
  CPPUNIT_ASSERT(!graph->existEdge(n3, n2).isValid());

  graph->pop();
  CPPUNIT_ASSERT(graph->isElement(e0));
  CPPUNIT_ASSERT_EQUAL(e0, graph->existEdge(n0, n1));

  Graph* g1 = graph->addSubGraph();
  g1->addNode(n0);
  g1->addNode(n1);
  g1->addEdge(e0);

  graph->push();

  g1->reverse(e0);
  CPPUNIT_ASSERT(graph->isElement(e0));
  CPPUNIT_ASSERT_EQUAL(e0, graph->existEdge(n1, n0));

  n2 = g1->addNode();
  g1->setTarget(e0, n2);
  CPPUNIT_ASSERT(graph->isElement(e0));
  CPPUNIT_ASSERT_EQUAL(e0, graph->existEdge(n1, n2));

  graph->pop();
  CPPUNIT_ASSERT(graph->isElement(e0));
  CPPUNIT_ASSERT_EQUAL(e0, graph->existEdge(n0, n1));

  graph->unpop();
  CPPUNIT_ASSERT(graph->isElement(e0));
  CPPUNIT_ASSERT_EQUAL(e0, graph->existEdge(n1, n2));

  g1->setSource(e0, n0);
  CPPUNIT_ASSERT(graph->isElement(e0));
  CPPUNIT_ASSERT_EQUAL(e0, graph->existEdge(n0, n2));

  g1->delEdge(e0);

  graph->pop();
  CPPUNIT_ASSERT(graph->isElement(e0));
  CPPUNIT_ASSERT_EQUAL(e0, graph->existEdge(n0, n1));

  graph->unpop();
  CPPUNIT_ASSERT(graph->isElement(e0));
  CPPUNIT_ASSERT_EQUAL(e0, graph->existEdge(n0, n2));

  g1->addEdge(e0);
  CPPUNIT_ASSERT(g1->isElement(e0));
  CPPUNIT_ASSERT_EQUAL(e0, g1->existEdge(n0, n2));

  g1->setEnds(e0, n2, n1);
  CPPUNIT_ASSERT_EQUAL(e0, g1->existEdge(n2, n1));
  CPPUNIT_ASSERT_EQUAL(e0, graph->existEdge(n2, n1));

  graph->pop();
  CPPUNIT_ASSERT(graph->isElement(e0));
  CPPUNIT_ASSERT_EQUAL(e0, graph->existEdge(n0, n1));
}

//==========================================================
void PushPopTest::testSetValue() {
  node n0 = graph->addNode();
  node n1 = graph->addNode();

  edge e0 = graph->addEdge(n0, n1);

  DoubleProperty* prop = graph->getProperty<DoubleProperty>("prop");

  prop->setNodeValue(n1, 1.0);
  prop->setEdgeValue(e0, 1.0);

  graph->push();
  node n2 = graph->addNode();
  edge e1 = graph->addEdge(n0, n2);

  prop->setNodeValue(n2, 2.0);
  prop->setEdgeValue(e1, 2.0);
  CPPUNIT_ASSERT_EQUAL(0.0, prop->getNodeValue(n0));
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getEdgeValue(e0));
  CPPUNIT_ASSERT_EQUAL(2.0, prop->getNodeValue(n2));
  CPPUNIT_ASSERT_EQUAL(2.0, prop->getEdgeValue(e1));

  prop->setNodeValue(n1, 2.0);
  prop->setEdgeValue(e0, 2.0);
  CPPUNIT_ASSERT_EQUAL(2.0, prop->getNodeValue(n1));
  CPPUNIT_ASSERT_EQUAL(2.0, prop->getEdgeValue(e0));
  prop->setNodeValue(n1, 4.0);
  prop->setEdgeValue(e0, 4.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 4.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 4.0);

  graph->pop();
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 1.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 1.0);

  graph->unpop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0) == 0.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e0) == 4.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n2) == 2.0);
  CPPUNIT_ASSERT(prop->getEdgeValue(e1) == 2.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n1) == 4.0);

  graph->push();
  prop->setAllNodeValue(3.0);
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getNodeValue(n0));
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getNodeValue(n1));
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getNodeValue(n2));
  graph->pop();
  CPPUNIT_ASSERT_EQUAL(0.0, prop->getNodeValue(n0));
  CPPUNIT_ASSERT_EQUAL(4.0, prop->getNodeValue(n1));
  CPPUNIT_ASSERT_EQUAL(2.0, prop->getNodeValue(n2));

  graph->unpop();
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getNodeValue(n0));
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getNodeValue(n1));
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getNodeValue(n2));

  prop->setAllEdgeValue(3.0);
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getEdgeValue(e0));

  graph->delNode(n1);
  CPPUNIT_ASSERT(!graph->isElement(n1));
  CPPUNIT_ASSERT(!graph->isElement(e0));

  graph->pop();
  CPPUNIT_ASSERT_EQUAL(0.0, prop->getNodeValue(n0));
  CPPUNIT_ASSERT_EQUAL(4.0, prop->getNodeValue(n1));
  CPPUNIT_ASSERT_EQUAL(4.0, prop->getEdgeValue(e0));

  graph->unpop();
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getNodeValue(n0));
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getNodeValue(n1));
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getEdgeValue(e0));
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getEdgeValue(e1));

  prop->setNodeValue(n0, 2.0);
  CPPUNIT_ASSERT_EQUAL(2.0, prop->getNodeValue(n0));
  prop->setEdgeValue(e0, 1.0);
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getEdgeValue(e0));

  graph->pop();
  CPPUNIT_ASSERT_EQUAL(0.0, prop->getNodeValue(n0));
  CPPUNIT_ASSERT_EQUAL(4.0, prop->getNodeValue(n1));
  CPPUNIT_ASSERT_EQUAL(4.0, prop->getEdgeValue(e0));
}

//==========================================================
void PushPopTest::testSetEltValue() {
  node n0 = graph->addNode();
  node n1 = graph->addNode();

  edge e0 = graph->addEdge(n0, n1);

  DoubleVectorProperty* prop =
    graph->getProperty<DoubleVectorProperty>("prop");

  vector<double> vv;
  vv.push_back(1.0);
  vv.push_back(1.0);

  prop->setNodeValue(n1, vv);
  prop->setEdgeValue(e0, vv);

  graph->push();
  node n2 = graph->addNode();
  edge e1 = graph->addEdge(n0, n2);

  CPPUNIT_ASSERT(prop->getEdgeValue(e1).empty());

  prop->setNodeValue(n2, vv);
  prop->setEdgeValue(e1, vv);
  CPPUNIT_ASSERT(prop->getNodeValue(n0).empty());
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getEdgeEltValue(e0, 0));
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getNodeEltValue(n2, 0));
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getEdgeEltValue(e1, 1));

  prop->setNodeEltValue(n1, 1, 2.0);
  prop->setEdgeEltValue(e0, 1, 2.0);
  CPPUNIT_ASSERT_EQUAL(2.0, prop->getNodeEltValue(n1, 1));
  CPPUNIT_ASSERT_EQUAL(2.0, prop->getEdgeEltValue(e0, 1));

  graph->push();
  vector<double> dv;
  dv.push_back(3.0);
  prop->setAllNodeValue(dv);
  prop->pushBackNodeEltValue(n1, 3.0);
  prop->pushBackNodeEltValue(n2, 3.0);
  prop->pushBackNodeEltValue(n2, 3.0);
  CPPUNIT_ASSERT_EQUAL(size_t(1), prop->getNodeValue(n0).size());
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getNodeEltValue(n0, 0));
  CPPUNIT_ASSERT_EQUAL(size_t(2), prop->getNodeValue(n1).size());
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getNodeEltValue(n1, 1));
  CPPUNIT_ASSERT_EQUAL(size_t(3), prop->getNodeValue(n2).size());
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getNodeEltValue(n2, 2));

  prop->setNodeEltValue(n0, 0, 4.0);
  CPPUNIT_ASSERT_EQUAL(size_t(1), prop->getNodeValue(n0).size());
  CPPUNIT_ASSERT_EQUAL(4.0, prop->getNodeEltValue(n0, 0));

  prop->popBackNodeEltValue(n0);
  CPPUNIT_ASSERT(prop->getNodeValue(n0).empty());
  prop->pushBackNodeEltValue(n0, 3.0);

  graph->pop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0).empty());
  CPPUNIT_ASSERT_EQUAL(2.0, prop->getNodeEltValue(n1, 1));
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getNodeEltValue(n2, 1));

  graph->unpop();
  CPPUNIT_ASSERT_EQUAL(size_t(1), prop->getNodeValue(n0).size());
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getNodeEltValue(n0, 0));
  CPPUNIT_ASSERT_EQUAL(size_t(2), prop->getNodeValue(n1).size());
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getNodeEltValue(n1, 1));
  CPPUNIT_ASSERT_EQUAL(size_t(3), prop->getNodeValue(n2).size());
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getNodeEltValue(n2, 2));

  prop->setAllEdgeValue(dv);
  prop->pushBackEdgeEltValue(e1, 3.0);
  prop->pushBackEdgeEltValue(e1, 3.0);
  CPPUNIT_ASSERT_EQUAL(size_t(1), prop->getEdgeValue(e0).size());
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getEdgeEltValue(e0, 0));
  CPPUNIT_ASSERT_EQUAL(size_t(3), prop->getEdgeValue(e1).size());
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getEdgeEltValue(e1, 1));
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getEdgeEltValue(e1, 2));

  prop->setEdgeEltValue(e0, 0, 4.0);
  CPPUNIT_ASSERT_EQUAL(size_t(1), prop->getEdgeValue(e0).size());
  CPPUNIT_ASSERT_EQUAL(4.0, prop->getEdgeEltValue(e0, 0));

  graph->delNode(n1);
  CPPUNIT_ASSERT(!graph->isElement(n1));
  CPPUNIT_ASSERT(!graph->isElement(e0));

  graph->pop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0).empty());
  CPPUNIT_ASSERT_EQUAL(2.0, prop->getNodeEltValue(n1, 1));
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getNodeEltValue(n2, 1));

  graph->unpop();
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getNodeEltValue(n0, 0));
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getEdgeEltValue(e0, 0));
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getEdgeEltValue(e1, 1));

  prop->pushBackNodeEltValue(n0, 3.0);
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getNodeEltValue(n0, 1));
  prop->pushBackEdgeEltValue(e0, 3.0);
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getEdgeEltValue(e0, 1));

  graph->pop();
  CPPUNIT_ASSERT(prop->getNodeValue(n0).empty());
  CPPUNIT_ASSERT_EQUAL(2.0, prop->getNodeEltValue(n1, 1));
  CPPUNIT_ASSERT_EQUAL(2.0, prop->getEdgeEltValue(e0, 1));
}

//==========================================================
void PushPopTest::testVectorValue() {
  node n0 = graph->addNode();
  node n1 = graph->addNode();

  edge e0 = graph->addEdge(n0, n1);

  DoubleVectorProperty* prop =
    graph->getProperty<DoubleVectorProperty>("prop");

  vector<double> vv;
  vv.push_back(1.0);
  vv.push_back(1.0);

  prop->setNodeValue(n1, vv);
  prop->setEdgeValue(e0, vv);

  graph->push();
  node n2 = graph->addNode();
  edge e1 = graph->addEdge(n0, n2);

  prop->setNodeValue(n2, vv);
  prop->setEdgeValue(e1, vv);
  CPPUNIT_ASSERT(prop->getNodeValue(n0).empty());
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getEdgeEltValue(e0, 0));
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getNodeEltValue(n2, 0));
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getEdgeEltValue(e1, 1));

  prop->setNodeEltValue(n1, 1, 2.0);
  prop->setEdgeEltValue(e0, 1, 2.0);
  CPPUNIT_ASSERT_EQUAL(2.0, prop->getNodeEltValue(n1, 1));
  CPPUNIT_ASSERT_EQUAL(2.0, prop->getEdgeEltValue(e0, 1));

  graph->push();
  prop->resizeNodeValue(n1, 2);
  prop->resizeNodeValue(n2, 3, 3.0);
  prop->resizeEdgeValue(e1, 3, 3.0);
  CPPUNIT_ASSERT_EQUAL(size_t(2), prop->getNodeValue(n1).size());
  CPPUNIT_ASSERT_EQUAL(2.0, prop->getNodeEltValue(n1, 1));
  CPPUNIT_ASSERT_EQUAL(size_t(3), prop->getNodeValue(n2).size());
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getNodeEltValue(n2, 2));
  CPPUNIT_ASSERT_EQUAL(size_t(3), prop->getEdgeValue(e1).size());
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getEdgeEltValue(e1, 2));

  graph->pop();
  CPPUNIT_ASSERT_EQUAL(size_t(2), prop->getNodeValue(n1).size());
  CPPUNIT_ASSERT_EQUAL(2.0, prop->getNodeEltValue(n1, 1));
  CPPUNIT_ASSERT_EQUAL(size_t(2), prop->getNodeValue(n2).size());
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getNodeEltValue(n2, 1));
  CPPUNIT_ASSERT_EQUAL(size_t(2), prop->getEdgeValue(e1).size());
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getEdgeEltValue(e1, 1));

  graph->unpop();
  CPPUNIT_ASSERT_EQUAL(size_t(2), prop->getNodeValue(n1).size());
  CPPUNIT_ASSERT_EQUAL(2.0, prop->getNodeEltValue(n1, 1));
  CPPUNIT_ASSERT_EQUAL(size_t(3), prop->getNodeValue(n2).size());
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getNodeEltValue(n2, 2));
  CPPUNIT_ASSERT_EQUAL(size_t(3), prop->getEdgeValue(e1).size());
  CPPUNIT_ASSERT_EQUAL(3.0, prop->getEdgeEltValue(e1, 2));

  prop->popBackNodeEltValue(n1);
  prop->popBackNodeEltValue(n2);
  prop->popBackEdgeEltValue(e1);
  CPPUNIT_ASSERT_EQUAL(size_t(1), prop->getNodeValue(n1).size());
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getNodeEltValue(n1, 0));
  CPPUNIT_ASSERT_EQUAL(size_t(2), prop->getNodeValue(n2).size());
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getNodeEltValue(n2, 1));
  CPPUNIT_ASSERT_EQUAL(size_t(2), prop->getEdgeValue(e1).size());
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getEdgeEltValue(e1, 1));
}

//==========================================================
void PushPopTest::testCopyProperty() {
  node n0 = graph->addNode();
  node n1 = graph->addNode();

  edge e0 = graph->addEdge(n0, n1);
  edge e1 = graph->addEdge(n1, n0);

  DoubleProperty* prop = graph->getProperty<DoubleProperty>("prop");

  prop->setNodeValue(n0, 1.0);
  prop->setEdgeValue(e0, 1.0);
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getNodeValue(n0));
  CPPUNIT_ASSERT_EQUAL(0.0, prop->getNodeValue(n1));
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getEdgeValue(e0));
  CPPUNIT_ASSERT_EQUAL(0.0, prop->getEdgeValue(e1));

  DoubleProperty p(graph);

  p.setNodeValue(n1, 1.0);
  p.setEdgeValue(e1, 1.0);
  CPPUNIT_ASSERT_EQUAL(0.0, p.getNodeValue(n0));
  CPPUNIT_ASSERT_EQUAL(1.0, p.getNodeValue(n1));
  CPPUNIT_ASSERT_EQUAL(0.0, p.getEdgeValue(e0));
  CPPUNIT_ASSERT_EQUAL(1.0, p.getEdgeValue(e1));

  graph->push();

  *prop = p;
  CPPUNIT_ASSERT_EQUAL(0.0, prop->getNodeValue(n0));
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getNodeValue(n1));
  CPPUNIT_ASSERT_EQUAL(0.0, prop->getEdgeValue(e0));
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getEdgeValue(e1));

  graph->pop();
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getNodeValue(n0));
  CPPUNIT_ASSERT_EQUAL(0.0, prop->getNodeValue(n1));
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getEdgeValue(e0));
  CPPUNIT_ASSERT_EQUAL(0.0, prop->getEdgeValue(e1));

  graph->push();
  prop->setAllNodeValue(0.0);
  prop->setAllEdgeValue(0.0);
  prop->setNodeValue(n1, 1.0);
  prop->setEdgeValue(e1, 1.0);
  CPPUNIT_ASSERT_EQUAL(0.0, prop->getNodeValue(n0));
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getNodeValue(n1));
  CPPUNIT_ASSERT_EQUAL(0.0, prop->getEdgeValue(e0));
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getEdgeValue(e1));

  graph->pop();
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getNodeValue(n0));
  CPPUNIT_ASSERT_EQUAL(0.0, prop->getNodeValue(n1));
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getEdgeValue(e0));
  CPPUNIT_ASSERT_EQUAL(0.0, prop->getEdgeValue(e1));

  graph->unpop();
  CPPUNIT_ASSERT_EQUAL(0.0, prop->getNodeValue(n0));
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getNodeValue(n1));
  CPPUNIT_ASSERT_EQUAL(0.0, prop->getEdgeValue(e0));
  CPPUNIT_ASSERT_EQUAL(1.0, prop->getEdgeValue(e1));
}

void PushPopTest::testSubgraph() {
  Graph *g1, *g2, *g3, *g4;

  vector<node> n;
  n.push_back(graph->addNode());
  n.push_back(graph->addNode());

  edge e0 = graph->addEdge(n[0], n[1]);

  g1 = graph->addSubGraph();
  g2 = graph->addSubGraph();
  g2->addNodes(n);
  g2->addEdge(e0);

  graph->push();

  g3 = g2->addSubGraph();
  g4 = g2->addSubGraph();

  Iterator<Graph *> *it= g2->getSubGraphs();
  Graph *a,*b;
  CPPUNIT_ASSERT(it->hasNext());
  a = it->next();
  CPPUNIT_ASSERT(it->hasNext());
  b = it->next();
  CPPUNIT_ASSERT((a==g3 && b==g4) || (a==g4 && b==g3));
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  graph->pop();

  it= g2->getSubGraphs();
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  graph->unpop();

  it= g2->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  a = it->next();
  CPPUNIT_ASSERT(it->hasNext());
  b = it->next();
  CPPUNIT_ASSERT((a==g3 && b==g4) || (a==g4 && b==g3));
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  node n2 = g3->addNode();
  node n3 = g3->addNode();
  edge e = g2->addEdge(n2,n3);

  CPPUNIT_ASSERT(g3->isElement(n2));
  CPPUNIT_ASSERT(g3->isElement(n3));
  CPPUNIT_ASSERT(g2->isElement(e));
  CPPUNIT_ASSERT(g2->isElement(n2));
  CPPUNIT_ASSERT(g2->isElement(n3));
  CPPUNIT_ASSERT(graph->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n2));
  CPPUNIT_ASSERT(graph->isElement(n3));

  graph->pop();

  CPPUNIT_ASSERT(!g2->isElement(e));
  CPPUNIT_ASSERT(!g2->isElement(n2));
  CPPUNIT_ASSERT(!g2->isElement(n3));
  CPPUNIT_ASSERT(!graph->isElement(e));
  CPPUNIT_ASSERT(!graph->isElement(n2));
  CPPUNIT_ASSERT(!graph->isElement(n3));

  graph->unpop();

  CPPUNIT_ASSERT(g3->isElement(n2));
  CPPUNIT_ASSERT(g3->isElement(n3));
  CPPUNIT_ASSERT(g2->isElement(e));
  CPPUNIT_ASSERT(g2->isElement(n2));
  CPPUNIT_ASSERT(g2->isElement(n3));
  CPPUNIT_ASSERT(graph->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n2));
  CPPUNIT_ASSERT(graph->isElement(n3));

  g2->delNode(n3);

  CPPUNIT_ASSERT(g3->isElement(n2));
  CPPUNIT_ASSERT(!g3->isElement(n3));
  CPPUNIT_ASSERT(!g2->isElement(n3));
  CPPUNIT_ASSERT(!g2->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n3));
  CPPUNIT_ASSERT(graph->isElement(e));

  graph->delNode(n[0]);

  CPPUNIT_ASSERT(!g2->isElement(n[0]));
  CPPUNIT_ASSERT(!g2->isElement(e0));
  CPPUNIT_ASSERT(!graph->isElement(n[0]));
  CPPUNIT_ASSERT(!graph->isElement(e0));

  graph->pop();

  CPPUNIT_ASSERT(!g2->isElement(n3));
  CPPUNIT_ASSERT(!g2->isElement(e));
  CPPUNIT_ASSERT(!graph->isElement(n3));
  CPPUNIT_ASSERT(!graph->isElement(e));
  CPPUNIT_ASSERT(g2->isElement(n[0]));
  CPPUNIT_ASSERT(g2->isElement(e0));
  CPPUNIT_ASSERT(graph->isElement(n[0]));
  CPPUNIT_ASSERT(graph->isElement(e0));

  graph->unpop();

  CPPUNIT_ASSERT(g3->isElement(n2));
  CPPUNIT_ASSERT(!g3->isElement(n3));
  CPPUNIT_ASSERT(!g2->isElement(n3));
  CPPUNIT_ASSERT(!g2->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n3));
  CPPUNIT_ASSERT(graph->isElement(e));
  CPPUNIT_ASSERT(!g2->isElement(n[0]));
  CPPUNIT_ASSERT(!g2->isElement(e0));
  CPPUNIT_ASSERT(!graph->isElement(n[0]));
  CPPUNIT_ASSERT(!graph->isElement(e0));

  g3->addNode(n[1]);
  e = g3->addEdge(n[1], n2);
  CPPUNIT_ASSERT(g3->isElement(e));
  CPPUNIT_ASSERT(g3->isElement(n[1]));
  CPPUNIT_ASSERT(g3->isElement(n2));
  CPPUNIT_ASSERT(g2->isElement(e));
  CPPUNIT_ASSERT(g2->isElement(n[1]));
  CPPUNIT_ASSERT(g2->isElement(n2));
  CPPUNIT_ASSERT(graph->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n[1]));
  CPPUNIT_ASSERT(graph->isElement(n2));

  graph->pop();
  CPPUNIT_ASSERT(!g2->isElement(e));
  CPPUNIT_ASSERT(g2->isElement(n[1]));
  CPPUNIT_ASSERT(!g2->isElement(n2));
  CPPUNIT_ASSERT(!graph->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n[1]));
  CPPUNIT_ASSERT(!graph->isElement(n2));

  graph->unpop();
  CPPUNIT_ASSERT(g3->isElement(e));
  CPPUNIT_ASSERT(g3->isElement(n[1]));
  CPPUNIT_ASSERT(g3->isElement(n2));
  CPPUNIT_ASSERT(g2->isElement(e));
  CPPUNIT_ASSERT(g2->isElement(n[1]));
  CPPUNIT_ASSERT(g2->isElement(n2));
  CPPUNIT_ASSERT(graph->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n[1]));
  CPPUNIT_ASSERT(graph->isElement(n2));

  graph->push();

  g2->delSubGraph(g3);
  g2->delSubGraph(g4);

  it= g2->getSubGraphs();
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  graph->pop();

  it= g2->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  a = it->next();
  CPPUNIT_ASSERT(it->hasNext());
  b = it->next();
  CPPUNIT_ASSERT((a==g3 && b==g4) || (a==g4 && b==g3));
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  CPPUNIT_ASSERT(g3->isElement(e));
  CPPUNIT_ASSERT(g3->isElement(n[1]));
  CPPUNIT_ASSERT(g3->isElement(n2));

  g2->delSubGraph(g4);
  it= g2->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  a = it->next();
  CPPUNIT_ASSERT(!it->hasNext());
  CPPUNIT_ASSERT_EQUAL(g3, a);
  delete it;

  graph->push();
  graph->delSubGraph(g2);
  it= graph->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  a = it->next();
  CPPUNIT_ASSERT(it->hasNext());
  b = it->next();
  CPPUNIT_ASSERT((a==g1 && b==g3) || (a==g3 && b==g1));
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  graph->pop();
  it= graph->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  a = it->next();
  CPPUNIT_ASSERT(it->hasNext());
  b = it->next();
  CPPUNIT_ASSERT((a==g1 && b==g2) || (a==g2 && b==g1));
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;
}

//==========================================================
void PushPopTest::testPushAddSubgraphsPop() {
  graph->push();
  Graph *g1, *g2;
  g1 = graph->addSubGraph();
  g2 = g1->addSubGraph();
  CPPUNIT_ASSERT(g2->getSuperGraph()==g1);
  CPPUNIT_ASSERT(graph->numberOfSubGraphs()==1);
  graph->pop();
  CPPUNIT_ASSERT(graph->numberOfSubGraphs()==0);
  graph->unpop();
  CPPUNIT_ASSERT(graph->numberOfSubGraphs()==1);
  graph->delSubGraph(g1);
  CPPUNIT_ASSERT(graph->numberOfSubGraphs()==1);
  CPPUNIT_ASSERT(g2->getSuperGraph()==graph);
  graph->pop();
  CPPUNIT_ASSERT(graph->numberOfSubGraphs()==0);
  graph->unpop();
  CPPUNIT_ASSERT(graph->numberOfSubGraphs()==1);
  CPPUNIT_ASSERT(g2->getSuperGraph()==graph);
}

//==========================================================
void PushPopTest::testDelSubgraph() {
  Graph *g1, *g2, *g3, *g4;
  g1 = graph->addSubGraph();
  g2 = graph->addSubGraph();
  g3 = g2->addSubGraph();
  g4 = g2->addSubGraph();

  CPPUNIT_ASSERT(graph->getSuperGraph()==graph);
  CPPUNIT_ASSERT(g1->getSuperGraph()==graph);
  CPPUNIT_ASSERT(g2->getSuperGraph()==graph);
  CPPUNIT_ASSERT(graph->getNthSubGraph(1) == g2);
  CPPUNIT_ASSERT(g3->getSuperGraph()==g2);
  CPPUNIT_ASSERT(g2->getNthSubGraph(0) == g3);
  CPPUNIT_ASSERT(g4->getSuperGraph()==g2);
  CPPUNIT_ASSERT(g2->getNthSubGraph(1) == g4);

  CPPUNIT_ASSERT(graph->getRoot()==graph);
  CPPUNIT_ASSERT(g1->getRoot()==graph);
  CPPUNIT_ASSERT(g2->getRoot()==graph);
  CPPUNIT_ASSERT(g3->getRoot()==graph);
  CPPUNIT_ASSERT(g4->getRoot()==graph);

  graph->push();

  graph->delSubGraph(g2);
  CPPUNIT_ASSERT(graph->getSuperGraph()==graph);
  CPPUNIT_ASSERT(g1->getSuperGraph()==graph);
  CPPUNIT_ASSERT(g3->getSuperGraph()==graph);
  CPPUNIT_ASSERT(graph->getNthSubGraph(1) == g3);
  CPPUNIT_ASSERT(g4->getSuperGraph()==graph);
  CPPUNIT_ASSERT(graph->getNthSubGraph(2) == g4);

  graph->pop();

  CPPUNIT_ASSERT(graph->getSuperGraph()==graph);
  CPPUNIT_ASSERT(g1->getSuperGraph()==graph);
  CPPUNIT_ASSERT(g2->getSuperGraph()==graph);
  CPPUNIT_ASSERT(graph->getNthSubGraph(1) == g2);
  CPPUNIT_ASSERT(g3->getSuperGraph()==g2);
  CPPUNIT_ASSERT(g2->getNthSubGraph(0) == g3);
  CPPUNIT_ASSERT(g4->getSuperGraph()==g2);
  CPPUNIT_ASSERT(g2->getNthSubGraph(1) == g4);

  graph->unpop();
  CPPUNIT_ASSERT(graph->getSuperGraph()==graph);
  CPPUNIT_ASSERT(g1->getSuperGraph()==graph);
  CPPUNIT_ASSERT(g3->getSuperGraph()==graph);
  CPPUNIT_ASSERT(graph->getNthSubGraph(1) == g3);
  CPPUNIT_ASSERT(g4->getSuperGraph()==graph);
  CPPUNIT_ASSERT(graph->getNthSubGraph(2) == g4);
}

static void createSubgraphs(Graph *graph, int m, int n) {
    if (n == 0)
        return;
    for (int i = 0 ; i < m ; ++i) {
        Graph *sg = graph->addCloneSubGraph();
        createSubgraphs(sg, m, n-1);
    }
}

void PushPopTest::testDelAllSgPopUnpop() {
    // create a hierarchy of sub-graphs
    // the hierachy has 3 levels with 4 sub-graph per level
    createSubgraphs(graph, 4, 3);

    // We should have 4 subgraphs for the root graph
    CPPUNIT_ASSERT(graph->numberOfSubGraphs()==4);

    graph->push();

    Graph *sg = graph->getNthSubGraph(3);
    graph->delAllSubGraphs(sg);

    // We should have 3 subgraphs for the root graph
    CPPUNIT_ASSERT(graph->numberOfSubGraphs()==3);

    graph->pop();

    // We should have 4 subgraphs for the root graph
    CPPUNIT_ASSERT(graph->numberOfSubGraphs()==4);

    graph->unpop();

    // need to grab the value and pop otherwise segfault when tearing down
    unsigned int nbSg = graph->numberOfSubGraphs();
    graph->pop();

    // We should have 3 subgraphs for the root graph before the last call to pop
    CPPUNIT_ASSERT(nbSg==3);
}


void PushPopTest::testTests() {
  node n0 = graph->addNode();
  node n1 = graph->addNode();
  node n2 = graph->addNode();

  graph->addEdge(n0, n1);
  graph->addEdge(n0, n2);

  graph->addNode();

  CPPUNIT_ASSERT(!ConnectedTest::isConnected(graph));

  graph->push();
  vector<edge> addedEdges;
  ConnectedTest::makeConnected(graph, addedEdges);
  CPPUNIT_ASSERT(ConnectedTest::isConnected(graph));

  graph->pop();
  CPPUNIT_ASSERT(!ConnectedTest::isConnected(graph));
}

void PushPopTest::testAddDelProps() {
  CPPUNIT_ASSERT(!graph->existProperty("boolean"));

  BooleanProperty* bProp = graph->getProperty<BooleanProperty>("boolean");
  CPPUNIT_ASSERT(graph->existProperty("boolean"));
  CPPUNIT_ASSERT(!graph->existProperty("double"));

  graph->push();

  DoubleProperty* dProp = graph->getProperty<DoubleProperty>("double");
  CPPUNIT_ASSERT(graph->existProperty("double"));
  node n = graph->addNode();
  dProp->setNodeValue(n, 123456789.);

  graph->pop();

  CPPUNIT_ASSERT(!graph->existProperty("double"));
  CPPUNIT_ASSERT(!graph->isElement(n));

  graph->unpop();
  CPPUNIT_ASSERT(graph->existProperty("double"));
  CPPUNIT_ASSERT_EQUAL(dProp, graph->getProperty<DoubleProperty>("double"));
  CPPUNIT_ASSERT(graph->existProperty("boolean"));
  CPPUNIT_ASSERT(graph->isElement(n));
  CPPUNIT_ASSERT_EQUAL(123456789., dProp->getNodeValue(n));

  graph->delLocalProperty("boolean");
  CPPUNIT_ASSERT(!graph->existProperty("boolean"));
  graph->delLocalProperty("double");
  CPPUNIT_ASSERT(!graph->existProperty("double"));
  graph->getProperty<DoubleProperty>("double");
  CPPUNIT_ASSERT(graph->existProperty("double"));
  CPPUNIT_ASSERT(graph->isElement(n));
  CPPUNIT_ASSERT(graph->getProperty<DoubleProperty>("double")->getNodeValue(n) != 123456789.);

  graph->push();
  CPPUNIT_ASSERT(graph->existProperty("double"));
  graph->delLocalProperty("double");
  CPPUNIT_ASSERT(!graph->existProperty("double"));

  graph->pop();
  CPPUNIT_ASSERT(!graph->existProperty("boolean"));
  CPPUNIT_ASSERT(graph->existProperty("double"));
  CPPUNIT_ASSERT(graph->isElement(n));
  CPPUNIT_ASSERT(graph->getProperty<DoubleProperty>("double")->getNodeValue(n) != 123456789.);

  graph->pop();
  CPPUNIT_ASSERT(!graph->existProperty("double"));
  CPPUNIT_ASSERT(graph->existProperty("boolean"));
  CPPUNIT_ASSERT_EQUAL(bProp, graph->getProperty<BooleanProperty>("boolean"));
}

void PushPopTest::testRenameProps() {
  CPPUNIT_ASSERT(!graph->existProperty("boolean"));

  BooleanProperty* bProp = graph->getProperty<BooleanProperty>("boolean");
  CPPUNIT_ASSERT(graph->existProperty("boolean"));
  CPPUNIT_ASSERT(!graph->existProperty("double"));

  graph->push();

  DoubleProperty* dProp = graph->getProperty<DoubleProperty>("double");
  CPPUNIT_ASSERT(graph->existProperty("double"));
  node n = graph->addNode();
  dProp->setNodeValue(n, 123456789.);

  dProp->rename("dbl");
  CPPUNIT_ASSERT(!graph->existProperty("double"));
  CPPUNIT_ASSERT(graph->existProperty("dbl"));
  
  graph->pop();

  CPPUNIT_ASSERT(!graph->existProperty("double"));
  CPPUNIT_ASSERT(!graph->existProperty("dbl"));
  CPPUNIT_ASSERT(!graph->isElement(n));

  graph->unpop();
  CPPUNIT_ASSERT(!graph->existProperty("double"));
  CPPUNIT_ASSERT(graph->existProperty("dbl"));
  CPPUNIT_ASSERT(dProp == graph->getProperty("dbl"));
  CPPUNIT_ASSERT(graph->existProperty("boolean"));
  CPPUNIT_ASSERT(graph->isElement(n));
  CPPUNIT_ASSERT_EQUAL(123456789., dProp->getNodeValue(n));

  bProp->rename("bool");
  CPPUNIT_ASSERT(!graph->existProperty("boolean"));
  CPPUNIT_ASSERT(graph->existProperty("bool"));
  CPPUNIT_ASSERT(bProp == graph->getProperty("bool"));

  graph->pop();
  CPPUNIT_ASSERT(graph->existProperty("boolean"));
  CPPUNIT_ASSERT(!graph->existProperty("bool"));
  CPPUNIT_ASSERT(bProp == graph->getProperty("boolean"));
  CPPUNIT_ASSERT(!graph->existProperty("double"));
  CPPUNIT_ASSERT(!graph->existProperty("dbl"));
  CPPUNIT_ASSERT(!graph->isElement(n));

  graph->unpop();
  CPPUNIT_ASSERT(!graph->existProperty("boolean"));
  CPPUNIT_ASSERT(graph->existProperty("bool"));
  CPPUNIT_ASSERT(bProp == graph->getProperty("bool"));
  CPPUNIT_ASSERT(!graph->existProperty("double"));
  CPPUNIT_ASSERT(graph->existProperty("dbl"));

  graph->delLocalProperty("bool");
  CPPUNIT_ASSERT(!graph->existProperty("bool"));
  CPPUNIT_ASSERT(graph->existProperty("dbl"));
  dProp->rename("bool");
  CPPUNIT_ASSERT(!graph->existProperty("dbl"));
  CPPUNIT_ASSERT(graph->existProperty("bool"));
  CPPUNIT_ASSERT(dProp == graph->getProperty("bool"));

  graph->pop();
  CPPUNIT_ASSERT(graph->existProperty("boolean"));
  CPPUNIT_ASSERT(!graph->existProperty("bool"));
  CPPUNIT_ASSERT(bProp == graph->getProperty("boolean"));
  CPPUNIT_ASSERT(!graph->existProperty("double"));
  CPPUNIT_ASSERT(!graph->existProperty("dbl"));
  CPPUNIT_ASSERT(!graph->isElement(n));

  graph->unpop();
  CPPUNIT_ASSERT(!graph->existProperty("double"));
  CPPUNIT_ASSERT(!graph->existProperty("dbl"));
  CPPUNIT_ASSERT(!graph->existProperty("boolean"));
  CPPUNIT_ASSERT(graph->existProperty("bool"));
  CPPUNIT_ASSERT(dProp == graph->getProperty("bool"));  
}

// this class will capture
// everything that will happen to our properties
class PropertyObserverForTest :public Observable {
public:
  std::set<PropertyInterface*> properties;

  PropertyObserverForTest() {}

  void reset() {
    properties.clear();
  }

  unsigned int nbProperties() {
    return properties.size();
  }

  bool found(PropertyInterface *prop) {
    return properties.find(prop) != properties.end();
  }

  virtual void destroy(PropertyInterface* prop) {
    properties.insert(prop);
  }

  virtual void treatEvent(const Event& evt) {
    PropertyInterface* prop = dynamic_cast<PropertyInterface *>(evt.sender());

    if (prop && evt.type() == Event::TLP_DELETE)
      destroy(prop);
  }

};

void PushPopTest::testObserveDelProps() {
  CPPUNIT_ASSERT(!graph->existProperty("boolean"));

  BooleanProperty* bProp = graph->getProperty<BooleanProperty>("boolean");
  CPPUNIT_ASSERT(graph->existProperty("boolean"));
  CPPUNIT_ASSERT(!graph->existProperty("double"));

  PropertyObserverForTest pObserver;
  pObserver.reset();
  bProp->addListener(&pObserver);
  CPPUNIT_ASSERT_EQUAL(0u, pObserver.nbProperties());

  graph->push();

  graph->delLocalProperty("boolean");
  CPPUNIT_ASSERT(!graph->existProperty("boolean"));
  CPPUNIT_ASSERT_EQUAL(1u, pObserver.nbProperties());
  CPPUNIT_ASSERT(pObserver.found(bProp));

  DoubleProperty* dProp = graph->getProperty<DoubleProperty>("double");
  dProp->addListener(&pObserver);
  pObserver.reset();
  CPPUNIT_ASSERT_EQUAL(0u, pObserver.nbProperties());
  graph->pop();
  CPPUNIT_ASSERT(graph->existProperty("boolean"));
  CPPUNIT_ASSERT(!graph->existProperty("double"));
  CPPUNIT_ASSERT_EQUAL(1u, pObserver.nbProperties());
  CPPUNIT_ASSERT(pObserver.found(dProp));
  pObserver.reset();

  graph->unpop();
  CPPUNIT_ASSERT(graph->existProperty("double"));
  CPPUNIT_ASSERT_EQUAL(dProp, graph->getProperty<DoubleProperty>("double"));
  CPPUNIT_ASSERT(!graph->existProperty("boolean"));
  CPPUNIT_ASSERT_EQUAL(1u, pObserver.nbProperties());
  CPPUNIT_ASSERT(pObserver.found(bProp));

  graph->delLocalProperty("double");
  CPPUNIT_ASSERT(!graph->existProperty("double"));
  CPPUNIT_ASSERT_EQUAL(2u, pObserver.nbProperties());
  CPPUNIT_ASSERT(pObserver.found(dProp));

  graph->pop();
  CPPUNIT_ASSERT(graph->existProperty("boolean"));
  CPPUNIT_ASSERT_EQUAL(bProp, graph->getProperty<BooleanProperty>("boolean"));
  CPPUNIT_ASSERT(!graph->existProperty("double"));
}


void PushPopTest::testAddSubgraphProp() {
  Graph *sg;

  Iterator<Graph *> *it = graph->getSubGraphs();
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  graph->push();
  sg = graph->addSubGraph();

  it = graph->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  CPPUNIT_ASSERT_EQUAL(sg, it->next());
  delete it;

  sg->push();
  DoubleProperty* prop = sg->getLocalProperty<DoubleProperty>("test");
  CPPUNIT_ASSERT(sg->existProperty("test"));

  sg->pop();
  CPPUNIT_ASSERT(!sg->existProperty("test"));

  sg->unpop();
  CPPUNIT_ASSERT(sg->existProperty("test"));
  CPPUNIT_ASSERT_EQUAL(prop, sg->getLocalProperty<DoubleProperty>("test"));

  sg->pop();
  CPPUNIT_ASSERT(!sg->existProperty("test"));
  it = graph->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  CPPUNIT_ASSERT_EQUAL(sg, it->next());
  delete it;

  graph->pop();
  it = graph->getSubGraphs();
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  graph->unpop();
  it = graph->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  CPPUNIT_ASSERT_EQUAL(sg, it->next());
  delete it;

  sg->unpop();
  CPPUNIT_ASSERT(sg->existProperty("test"));
  CPPUNIT_ASSERT_EQUAL(prop, sg->getLocalProperty<DoubleProperty>("test"));

  sg->pop();
  CPPUNIT_ASSERT(!sg->existProperty("test"));
  it = graph->getSubGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  CPPUNIT_ASSERT_EQUAL(sg, it->next());
  delete it;

  graph->pop();
  it = graph->getSubGraphs();
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;
  CPPUNIT_ASSERT(!graph->canPop());
  CPPUNIT_ASSERT(graph->canUnpop());

  graph->push();
  it = graph->getSubGraphs();
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;
  CPPUNIT_ASSERT(graph->canPop());
  CPPUNIT_ASSERT(!graph->canUnpop());
}

void PushPopTest::testMetaNode() {
  node n0 = graph->addNode();
  LayoutProperty* layout = graph->getProperty<LayoutProperty>("viewLayout");
  Coord coord0(-1, -1, -1);
  layout->setNodeValue(n0, coord0);

  set<node> tmp;
  tmp.insert(n0);

  graph->push();

  // create clone graph
  Graph* clone = graph->addCloneSubGraph("groups");

  // create meta node
  node metaNode = clone->createMetaNode(tmp);
  Color mColor(255, 255, 255, 127);
  ColorProperty* color = graph->getProperty<ColorProperty>("viewColor");
  color->setNodeValue(metaNode, mColor);

  CPPUNIT_ASSERT(graph->isElement(metaNode));
  CPPUNIT_ASSERT(graph->isMetaNode(metaNode));
  CPPUNIT_ASSERT_EQUAL(coord0,  layout->getNodeValue(metaNode));
  CPPUNIT_ASSERT_EQUAL(mColor,  color->getNodeValue(metaNode));

  graph->pop();
  CPPUNIT_ASSERT(!graph->isElement(metaNode));
  CPPUNIT_ASSERT(coord0 != layout->getNodeValue(metaNode));

  graph->unpop();
  CPPUNIT_ASSERT(graph->isElement(metaNode));
  CPPUNIT_ASSERT(graph->isMetaNode(metaNode));
  CPPUNIT_ASSERT_EQUAL(coord0,  layout->getNodeValue(metaNode));
  CPPUNIT_ASSERT_EQUAL(mColor,  color->getNodeValue(metaNode));
}

class DeleteObjectsObserver : public Observable {

public :

    vector<Graph *> deletedGraphs;
    vector<PropertyInterface *> deletedProperties;


    void treatEvent(const Event &e) {
        if (e.type() == Event::TLP_DELETE) {
            Graph* graph = dynamic_cast<Graph *>(e.sender());
            if (graph) {
                deletedGraphs.push_back(graph);
            }
            PropertyInterface *prop = dynamic_cast<PropertyInterface *>(e.sender());
            if (prop) {
                deletedProperties.push_back(prop);
            }
        }
    }

};

void PushPopTest::testDeletePushPopFalse() {

    // to track object deletion
    DeleteObjectsObserver delObserver;

    // create a new subgraph in graph and a local property to it
    // create a new subgraph in the previously created one and a local property to it
    graph->push();
    Graph *g1 = graph->addSubGraph("toto");
    g1->addListener(&delObserver);
    DoubleProperty *testProp = g1->getLocalProperty<DoubleProperty>("test1");
    testProp->addListener(&delObserver);
    Graph *g2 = g1->addSubGraph("titi");
    g2->addListener(&delObserver);
    LayoutProperty *testProp2 = g2->getLocalProperty<LayoutProperty>("test2");
    testProp2->addListener(&delObserver);

    // this call should delete the two created sub-graphs
    // and the two created properties
    graph->pop(false);

    // should be equal to 4 as two delete events are sent by deleted sub-graphs : the first one when
    // the GraphUpdatesRecorder do the updates (removing the newly added sub-graph from the hierarchy),
    // the second one when the GraphUpdatedsRecorder destructor is called (as we forbid to unpop) as it
    // will really delete the sub-graph
    CPPUNIT_ASSERT_EQUAL(size_t(4), delObserver.deletedGraphs.size());

    // same thing for the two deleted properties
    CPPUNIT_ASSERT_EQUAL(size_t(4), delObserver.deletedProperties.size());
}
