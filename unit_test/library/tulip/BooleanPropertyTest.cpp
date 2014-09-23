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
#include <tulip/TlpTools.h>
#include "BooleanPropertyTest.h"

using namespace std;
using namespace tlp;

CPPUNIT_TEST_SUITE_REGISTRATION( BooleanPropertyTest );

//==========================================================
void buildGraph(Graph *graph) {
  //add three nodes
  node n1=graph->addNode();
  node n2=graph->addNode();
  node n3=graph->addNode();
  //add three edges
  graph->addEdge(n2,n3);
  graph->addEdge(n1,n2);
  graph->addEdge(n3,n1);
}
//==========================================================
void BooleanPropertyTest::setUp() {
  graph    = tlp::newGraph();
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
  Iterator<node> *itN=graph->getNodes();

  while(itN->hasNext()) {
    CPPUNIT_ASSERT_EQUAL(value, selection->getNodeValue(itN->next()));
  }

  delete itN;
  Iterator<edge> *itE=graph->getEdges();

  while(itE->hasNext()) {
    CPPUNIT_ASSERT_EQUAL(value, selection->getEdgeValue(itE->next()));
  }

  delete itE;
}
//==========================================================
void BooleanPropertyTest::testSetAll() {
  testSetAll(true);
  testSetAll(false);
}
//==========================================================
void BooleanPropertyTest::testSetGet(bool value) {
  vector<node> nodes(graph->numberOfNodes());
  vector<edge> edges(graph->numberOfEdges());
  unsigned int i=0;
  Iterator<node> *itN=graph->getNodes();

  while(itN->hasNext()) {
    nodes[i++]=itN->next();
  }

  delete itN;
  Iterator<edge> *itE=graph->getEdges();
  i=0;

  while(itE->hasNext()) {
    edges[i++]=itE->next();
  }

  delete itE;

  selection->setAllNodeValue(value);
  selection->setAllEdgeValue(value);

  for (unsigned int i=0; i<graph->numberOfNodes()*10; ++i) {
    unsigned int rando=rand()%graph->numberOfNodes();
    selection->setNodeValue(nodes[rando],!value);
    CPPUNIT_ASSERT_EQUAL(!value, selection->getNodeValue(nodes[rando]));
  }

  for (unsigned int i=0; i<graph->numberOfEdges()*10; ++i) {
    unsigned int rando=rand()%graph->numberOfEdges();
    selection->setEdgeValue(edges[rando],!value);
    CPPUNIT_ASSERT_EQUAL(!value, selection->getEdgeValue(edges[rando]));
  }
}
//==========================================================
void BooleanPropertyTest::testCopy() {
  vector<node> nodes(graph->numberOfNodes());
  vector<edge> edges(graph->numberOfEdges());
  unsigned int i=0;
  Iterator<node> *itN=graph->getNodes();

  while(itN->hasNext()) {
    nodes[i++]=itN->next();
  }

  delete itN;
  Iterator<edge> *itE=graph->getEdges();
  i=0;

  while(itE->hasNext()) {
    edges[i++]=itE->next();
  }

  delete itE;
  bool value = true;
  selection->setAllNodeValue(value);
  selection->setAllEdgeValue(value);

  for (unsigned int i=0; i<graph->numberOfNodes()*10; ++i) {
    unsigned int rando=rand()%graph->numberOfNodes();
    selection->setNodeValue(nodes[rando],!value);
    CPPUNIT_ASSERT_EQUAL(!value, selection->getNodeValue(nodes[rando]));
  }

  for (unsigned int i=0; i<graph->numberOfEdges()*10; ++i) {
    unsigned int rando=rand()%graph->numberOfEdges();
    selection->setEdgeValue(edges[rando],!value);
    CPPUNIT_ASSERT_EQUAL(!value, selection->getEdgeValue(edges[rando]));
  }

  BooleanProperty tmp(graph);
  tmp = *selection;
  itN=graph->getNodes();

  while(itN->hasNext()) {
    node n  = itN->next();
    CPPUNIT_ASSERT_EQUAL( selection->getNodeValue(n), tmp.getNodeValue(n) );
  }

  delete itN;
  itE=graph->getEdges();

  while(itE->hasNext()) {
    edge e = itE->next();
    CPPUNIT_ASSERT_EQUAL( selection->getEdgeValue(e), tmp.getEdgeValue(e) );
  }

  delete itE;
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
  Iterator<node> *itN=graph->getNodes();
  node n;

  while(itN->hasNext()) {
    n = itN->next();
  }

  delete itN;
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
