/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#include <tulip/TlpTools.h>
#include "BooleanPropertyTest.h"

using namespace std;
using namespace tlp;

CPPUNIT_TEST_SUITE_REGISTRATION(BooleanPropertyTest);

//==========================================================
void buildGraph(Graph *graph) {
  // add three nodes
  node n1 = graph->addNode();
  node n2 = graph->addNode();
  node n3 = graph->addNode();
  // add three edges
  graph->addEdge(n2, n3);
  graph->addEdge(n1, n2);
  graph->addEdge(n3, n1);
}
//==========================================================
void BooleanPropertyTest::setUp() {
#ifndef NDEBUG
  // choose the seed to ease the debugging
  tlp::setSeedOfRandomSequence(1);
#endif
  graph = tlp::newGraph();
  selection = graph->getProperty<BooleanProperty>("Select Test");
  buildGraph(graph);
}
//==========================================================
void BooleanPropertyTest::tearDown() {
  delete graph;
}
//==========================================================
void BooleanPropertyTest::testIterators() {
  selection->setAllNodeValue(true);
  selection->setAllEdgeValue(true);
  {
    Iterator<node> *itSG = graph->getNodes();
    Iterator<node> *itSE = selection->getNodesEqualTo(true);

    while (itSG->hasNext() && itSE->hasNext()) {
      CPPUNIT_ASSERT_EQUAL(itSG->next(), itSE->next());
    }

    CPPUNIT_ASSERT(!itSG->hasNext());
    CPPUNIT_ASSERT(!itSE->hasNext());
    delete itSG;
    delete itSE;
  }
  {
    Iterator<edge> *itSG = graph->getEdges();
    Iterator<edge> *itSE = selection->getEdgesEqualTo(true);

    while (itSG->hasNext() && itSE->hasNext()) {
      CPPUNIT_ASSERT_EQUAL(itSG->next(), itSE->next());
    }

    CPPUNIT_ASSERT(!itSG->hasNext());
    CPPUNIT_ASSERT(!itSE->hasNext());
    delete itSG;
    delete itSE;
  }
}
//==========================================================
void BooleanPropertyTest::testSetAll(bool value) {
  selection->setAllNodeValue(value);
  selection->setAllEdgeValue(value);

  for (auto n : graph->nodes()) {
    CPPUNIT_ASSERT_EQUAL(value, selection->getNodeValue(n));
  }

  for (auto e : graph->edges()) {
    CPPUNIT_ASSERT_EQUAL(value, selection->getEdgeValue(e));
  }
}
//==========================================================
void BooleanPropertyTest::testSetAll() {
  testSetAll(true);
  testSetAll(false);
}
//==========================================================
void BooleanPropertyTest::testSetGet(bool value) {
  const vector<node> &nodes = graph->nodes();
  const vector<edge> &edges = graph->edges();

  selection->setAllNodeValue(value);
  selection->setAllEdgeValue(value);

  for (unsigned int i = 0; i < graph->numberOfNodes() * 10; ++i) {
    unsigned int rando = randomUnsignedInteger(graph->numberOfNodes() - 1);
    selection->setNodeValue(nodes[rando], !value);
    CPPUNIT_ASSERT_EQUAL(!value, selection->getNodeValue(nodes[rando]));
  }

  for (unsigned int i = 0; i < graph->numberOfEdges() * 10; ++i) {
    unsigned int rando = randomUnsignedInteger(graph->numberOfEdges() - 1);
    selection->setEdgeValue(edges[rando], !value);
    CPPUNIT_ASSERT_EQUAL(!value, selection->getEdgeValue(edges[rando]));
  }
}
//==========================================================
void BooleanPropertyTest::testCopy() {
  const vector<node> &nodes = graph->nodes();
  const vector<edge> &edges = graph->edges();

  bool value = true;
  selection->setAllNodeValue(value);
  selection->setAllEdgeValue(value);

  for (unsigned int i = 0; i < graph->numberOfNodes() * 10; ++i) {
    unsigned int rando = randomUnsignedInteger(graph->numberOfNodes() - 1);
    selection->setNodeValue(nodes[rando], !value);
    CPPUNIT_ASSERT_EQUAL(!value, selection->getNodeValue(nodes[rando]));
  }

  for (unsigned int i = 0; i < graph->numberOfEdges() * 10; ++i) {
    unsigned int rando = randomUnsignedInteger(graph->numberOfEdges() - 1);
    selection->setEdgeValue(edges[rando], !value);
    CPPUNIT_ASSERT_EQUAL(!value, selection->getEdgeValue(edges[rando]));
  }

  BooleanProperty tmp(graph);
  tmp = *selection;

  for (auto n : nodes) {
    CPPUNIT_ASSERT_EQUAL(selection->getNodeValue(n), tmp.getNodeValue(n));
  }

  for (auto e : edges) {
    CPPUNIT_ASSERT_EQUAL(selection->getEdgeValue(e), tmp.getEdgeValue(e));
  }
}
//==========================================================
void BooleanPropertyTest::testSetGet() {
  testSetGet(false);
  testSetGet(true);
}
//==========================================================
void BooleanPropertyTest::testDelete(bool value) {
  selection->setAllNodeValue(value);
  selection->setAllEdgeValue(value);

  node n = graph->getRandomNode();

  selection->setNodeValue(n, !value);
  CPPUNIT_ASSERT_EQUAL(!value, selection->getNodeValue(n));
  graph->delNode(n);
  n = graph->addNode();
  CPPUNIT_ASSERT_EQUAL(value, selection->getNodeValue(n));
  edge e = graph->addEdge(n, n);
  selection->setEdgeValue(e, !value);
  CPPUNIT_ASSERT_EQUAL(!value, selection->getEdgeValue(e));
  graph->delEdge(e);
  e = graph->addEdge(n, n);
  CPPUNIT_ASSERT_EQUAL(value, selection->getEdgeValue(e));
}
//==========================================================
void BooleanPropertyTest::testDelete() {
  testDelete(false);
  testDelete(true);
}
//==========================================================
void BooleanPropertyTest::testReverse() {
  selection->setAllNodeValue(false);
  selection->setAllEdgeValue(false);

  selection->setNodeValue(node(0), true);
  selection->setNodeValue(node(graph->numberOfNodes() - 1), true);
  selection->setEdgeValue(edge(0), true);
  selection->setEdgeValue(edge(graph->numberOfEdges() - 1), true);

  CPPUNIT_ASSERT_EQUAL(true, selection->getNodeValue(node(0)));
  CPPUNIT_ASSERT_EQUAL(true, selection->getNodeValue(node(graph->numberOfNodes() - 1)));
  for (unsigned int i = 1; i < graph->numberOfNodes() - 1; ++i)
    CPPUNIT_ASSERT_EQUAL(false, selection->getNodeValue(node(i)));
  CPPUNIT_ASSERT_EQUAL(true, selection->getEdgeValue(edge(0)));
  CPPUNIT_ASSERT_EQUAL(true, selection->getEdgeValue(edge(graph->numberOfEdges() - 1)));
  for (unsigned int i = 1; i < graph->numberOfEdges() - 1; ++i)
    CPPUNIT_ASSERT_EQUAL(false, selection->getEdgeValue(edge(i)));

  selection->reverse();

  CPPUNIT_ASSERT_EQUAL(false, selection->getNodeValue(node(0)));
  CPPUNIT_ASSERT_EQUAL(false, selection->getNodeValue(node(graph->numberOfNodes() - 1)));
  for (unsigned int i = 1; i < graph->numberOfNodes() - 1; ++i)
    CPPUNIT_ASSERT_EQUAL(true, selection->getNodeValue(node(i)));
  CPPUNIT_ASSERT_EQUAL(false, selection->getEdgeValue(edge(0)));
  CPPUNIT_ASSERT_EQUAL(false, selection->getEdgeValue(edge(graph->numberOfEdges() - 1)));
  for (unsigned int i = 1; i < graph->numberOfEdges() - 1; ++i)
    CPPUNIT_ASSERT_EQUAL(true, selection->getEdgeValue(edge(i)));

  selection->reverse();

  CPPUNIT_ASSERT_EQUAL(true, selection->getNodeValue(node(0)));
  CPPUNIT_ASSERT_EQUAL(true, selection->getNodeValue(node(graph->numberOfNodes() - 1)));
  for (unsigned int i = 1; i < graph->numberOfNodes() - 1; ++i)
    CPPUNIT_ASSERT_EQUAL(false, selection->getNodeValue(node(i)));
  CPPUNIT_ASSERT_EQUAL(true, selection->getEdgeValue(edge(0)));
  CPPUNIT_ASSERT_EQUAL(true, selection->getEdgeValue(edge(graph->numberOfEdges() - 1)));
  for (unsigned int i = 1; i < graph->numberOfEdges() - 1; ++i)
    CPPUNIT_ASSERT_EQUAL(false, selection->getEdgeValue(edge(i)));
}
