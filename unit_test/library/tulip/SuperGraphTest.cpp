/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include "SuperGraphTest.h"
#include <tulip/BooleanProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>

using namespace std;
using namespace tlp;

#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION( SuperGraphTest );

//==========================================================
void SuperGraphTest::setUp() {
  graph = tlp::newGraph();
}
//==========================================================
void SuperGraphTest::tearDown() {
  delete graph;
}
//==========================================================
void SuperGraphTest::build(unsigned int nbNodes, unsigned int edgeRatio) {
  vector<node> nodes;
  vector<edge> edges;
  unsigned int NB_ADD  = nbNodes;
  unsigned int EDGE_RATIO = edgeRatio;
  for (unsigned int i=0; i<NB_ADD; ++i)
    nodes.push_back(graph->addNode());
  unsigned int NB_EDGES = EDGE_RATIO * NB_ADD;
  for (unsigned int i=0; i< NB_EDGES; ++i)
    graph->addEdge(nodes[rand()%NB_ADD], nodes[rand()%NB_ADD]);
}
//==========================================================
void SuperGraphTest::testIterators() {
  graph->clear();
  vector<node> nodes;
  vector<edge> edges;
  unsigned int NB_NODES  = 100;
  unsigned int EDGE_RATIO = 100;
  for (unsigned int i=0; i<NB_NODES; ++i)
    nodes.push_back(graph->addNode());
  unsigned int NB_EDGES = EDGE_RATIO * NB_NODES;
  for (unsigned int i=0; i< NB_EDGES; ++i)
    edges.push_back(graph->addEdge(nodes[rand()%NB_NODES], nodes[rand()%NB_NODES]));
  {
  Iterator<node> *itN = graph->getNodes();
  unsigned int i = 0;
  while(itN->hasNext()){
    CPPUNIT_ASSERT_EQUAL( nodes[i].id , itN->next().id );
    ++i;
  }delete itN;
  CPPUNIT_ASSERT_EQUAL( i , NB_NODES );
  }
  {
  Iterator<edge> *itE = graph->getEdges();
  unsigned int i = 0;
  while(itE->hasNext()){
    CPPUNIT_ASSERT_EQUAL( edges[i].id , itE->next().id );
    ++i;
  }delete itE;
  CPPUNIT_ASSERT_EQUAL( i , NB_EDGES );
  }
  graph->clear();
  node n1 = graph->addNode();
  node n2 = graph->addNode();
  node n3 = graph->addNode();
  edge e1 = graph->addEdge(n1, n1); //loop
  edge e2 = graph->addEdge(n1, n2); 
  edge e3 = graph->addEdge(n2, n1);//parallel edge
  edge e4 = graph->addEdge(n1, n3);

  Iterator<node> *it1 = graph->getOutNodes(n1);
  CPPUNIT_ASSERT_EQUAL(n1.id, it1->next().id);
  CPPUNIT_ASSERT_EQUAL(n2.id, it1->next().id);
  CPPUNIT_ASSERT_EQUAL(n3.id, it1->next().id);
  CPPUNIT_ASSERT_EQUAL(false, it1->hasNext());
  delete it1;
  Iterator<node> *it2 = graph->getInNodes(n1);
  CPPUNIT_ASSERT_EQUAL(n1.id, it2->next().id);
  CPPUNIT_ASSERT_EQUAL(n2.id, it2->next().id);
  CPPUNIT_ASSERT_EQUAL(false, it2->hasNext());
  delete it2;
  Iterator<node> *it3 = graph->getInOutNodes(n1);
  CPPUNIT_ASSERT_EQUAL(n1.id, it3->next().id);
  CPPUNIT_ASSERT_EQUAL(n1.id, it3->next().id);
  CPPUNIT_ASSERT_EQUAL(n2.id, it3->next().id);
  CPPUNIT_ASSERT_EQUAL(n2.id, it3->next().id);
  CPPUNIT_ASSERT_EQUAL(n3.id, it3->next().id);
  CPPUNIT_ASSERT_EQUAL(false, it3->hasNext());
  delete it3;

  Iterator<edge> *it4 = graph->getOutEdges(n1);
  CPPUNIT_ASSERT_EQUAL(e1.id, it4->next().id);
  CPPUNIT_ASSERT_EQUAL(e2.id, it4->next().id);
  CPPUNIT_ASSERT_EQUAL(e4.id, it4->next().id);
  CPPUNIT_ASSERT_EQUAL(false, it4->hasNext());
  delete it4;
  Iterator<edge> *it5 = graph->getInEdges(n1);
  CPPUNIT_ASSERT_EQUAL(e1.id, it5->next().id);
  CPPUNIT_ASSERT_EQUAL(e3.id, it5->next().id);
  CPPUNIT_ASSERT_EQUAL(false, it5->hasNext());
  delete it5;
  Iterator<edge> *it6 = graph->getInOutEdges(n1);
  CPPUNIT_ASSERT_EQUAL(e1.id, it6->next().id);
  CPPUNIT_ASSERT_EQUAL(e1.id, it6->next().id);
  CPPUNIT_ASSERT_EQUAL(e2.id, it6->next().id);
  CPPUNIT_ASSERT_EQUAL(e3.id, it6->next().id);
  CPPUNIT_ASSERT_EQUAL(e4.id, it6->next().id);
  CPPUNIT_ASSERT_EQUAL(false, it6->hasNext());
  delete it6;
}
//==========================================================
void degreeCheck(Graph *graph) {
  Iterator<node> *itN = graph->getNodes();
  while (itN->hasNext()) {
    node n = itN->next();
    unsigned int outdeg = 0;
    unsigned int indeg = 0;
    unsigned int deg = 0;
    Iterator<edge> *it = graph->getOutEdges(n);
    for (;it->hasNext();it->next())
      ++outdeg;
    delete it;
    it = graph->getInEdges(n);
    for (;it->hasNext();it->next())
      ++indeg;
    delete it;
    it = graph->getInOutEdges(n);
    for (;it->hasNext();it->next())
      ++deg;
    delete it;
    CPPUNIT_ASSERT_EQUAL(graph->indeg(n) , indeg);
    CPPUNIT_ASSERT_EQUAL(graph->outdeg(n), outdeg);
    CPPUNIT_ASSERT_EQUAL(graph->deg(n)   , deg);
    CPPUNIT_ASSERT_EQUAL(indeg + outdeg  , deg);
  } delete itN;  
}
//==========================================================
void SuperGraphTest::testDegree() {
  graph->clear();
  build(100, 100);
  degreeCheck(graph);
  Graph *gr = tlp::newCloneSubGraph(graph);
  Graph *gr1 = tlp::newCloneSubGraph(graph);
  Graph *gr2 = tlp::newCloneSubGraph(gr1);

  degreeCheck(graph);
  degreeCheck(gr);
  degreeCheck(gr1);
  degreeCheck(gr2);
  Iterator<edge> *it = graph->getEdges();
  while(it->hasNext()) {
    graph->reverse(it->next());
  } delete it;
  degreeCheck(graph);
  degreeCheck(gr);
  degreeCheck(gr1);
  degreeCheck(gr2);
  it = gr2->getEdges();
  while(it->hasNext()) {
    gr2->reverse(it->next());
  } delete it;
  degreeCheck(graph);
  degreeCheck(gr);
  degreeCheck(gr1);
  degreeCheck(gr2);

  graph->clear();
  gr1 = tlp::newCloneSubGraph(graph);
  gr2 = tlp::newCloneSubGraph(gr1);
  node n1 = graph->addNode();
  node n2 = graph->addNode();
  node n3 = graph->addNode();
  gr2->addNode(n1);
  gr2->addNode(n2);
  gr2->addNode(n3);
  gr2->addEdge(graph->addEdge(n1, n2));
  gr2->addEdge(graph->addEdge(n1, n3));
  gr2->delNode(n3);
  CPPUNIT_ASSERT_EQUAL(3u, graph->numberOfNodes());
  CPPUNIT_ASSERT_EQUAL(2u, graph->numberOfEdges());
  CPPUNIT_ASSERT_EQUAL(3u, gr1->numberOfNodes());
  CPPUNIT_ASSERT_EQUAL(2u, gr1->numberOfEdges());
  CPPUNIT_ASSERT_EQUAL(2u, gr2->numberOfNodes());
  CPPUNIT_ASSERT_EQUAL(1u, gr2->numberOfEdges());
  CPPUNIT_ASSERT_EQUAL(0u, graph->indeg(n1));
  CPPUNIT_ASSERT_EQUAL(1u, graph->indeg(n2));
  CPPUNIT_ASSERT_EQUAL(1u, graph->indeg(n3));
  CPPUNIT_ASSERT_EQUAL(2u, graph->outdeg(n1));
  CPPUNIT_ASSERT_EQUAL(0u, graph->outdeg(n2));
  CPPUNIT_ASSERT_EQUAL(2u, graph->deg(n1));
  CPPUNIT_ASSERT_EQUAL(1u, graph->deg(n2));
  CPPUNIT_ASSERT_EQUAL(1u, graph->deg(n3));
  CPPUNIT_ASSERT_EQUAL(0u, gr1->indeg(n1));
  CPPUNIT_ASSERT_EQUAL(1u, gr1->indeg(n2));
  CPPUNIT_ASSERT_EQUAL(1u, gr1->indeg(n3));
  CPPUNIT_ASSERT_EQUAL(2u, gr1->outdeg(n1));
  CPPUNIT_ASSERT_EQUAL(0u, gr1->outdeg(n2));
  CPPUNIT_ASSERT_EQUAL(2u, gr1->deg(n1));
  CPPUNIT_ASSERT_EQUAL(1u, gr1->deg(n2));
  CPPUNIT_ASSERT_EQUAL(1u, gr1->deg(n3));
  CPPUNIT_ASSERT_EQUAL(0u, gr2->indeg(n1));
  CPPUNIT_ASSERT_EQUAL(1u, gr2->indeg(n2));
  CPPUNIT_ASSERT_EQUAL(1u, gr2->outdeg(n1));
  CPPUNIT_ASSERT_EQUAL(0u, gr2->outdeg(n2));
  CPPUNIT_ASSERT_EQUAL(1u, gr2->deg(n1));
  CPPUNIT_ASSERT_EQUAL(1u, gr2->deg(n2));

}
//==========================================================
void SuperGraphTest::testAddDel() {
  //  cerr << __PRETTY_FUNCTION__ << endl;
  vector<node> nodes;
  vector<edge> edges;
  unsigned int NB_ADD = 100;
  unsigned int EDGE_RATIO = 100;

  unsigned int NB_EDGES = EDGE_RATIO * NB_ADD;
  vector<bool> edgePresent(NB_EDGES);
  vector<bool> nodePresent(NB_ADD);

  for (unsigned int i=0; i<NB_ADD; ++i) {
    nodes.push_back(graph->addNode());
    CPPUNIT_ASSERT(graph->isElement(nodes[i]));
    nodePresent[i] = true;
  }
  CPPUNIT_ASSERT(graph->numberOfNodes() == NB_ADD);


  for (unsigned int i=0; i< NB_EDGES; ++i) {
    edges.push_back(graph->addEdge(nodes[rand()%NB_ADD],nodes[rand()%NB_ADD]));
    CPPUNIT_ASSERT(graph->isElement(edges[i]));
    edgePresent[i] = true;
  }
  CPPUNIT_ASSERT(graph->numberOfEdges() == NB_EDGES);

  for (unsigned int i=0; i < NB_EDGES / 2; ++i) {
    graph->delEdge(edges[i]);
    CPPUNIT_ASSERT(!graph->isElement(edges[i]));
  }
  CPPUNIT_ASSERT(graph->numberOfEdges() == NB_EDGES - NB_EDGES/2);

  for (unsigned int i=0; i<NB_ADD; ++i) {
    Iterator<edge> *itE = graph->getInOutEdges(nodes[i]);
    while (itE->hasNext())
      CPPUNIT_ASSERT(graph->isElement(itE->next()));
    delete itE;
    graph->delNode(nodes[i]);
    CPPUNIT_ASSERT(!graph->isElement(nodes[i]));
  }
  CPPUNIT_ASSERT(graph->numberOfNodes() == 0);
  CPPUNIT_ASSERT(graph->numberOfEdges() == 0);
}
//==========================================================
void SuperGraphTest::testClear() {
  build(100, 100);
  graph->clear();
  CPPUNIT_ASSERT(graph->numberOfNodes() == 0);
  CPPUNIT_ASSERT(graph->numberOfEdges() == 0);
}
//==========================================================
void SuperGraphTest::testOrderEdgeAndSwap() {
  graph->clear();
  vector<node> nodes;
  vector<edge> edges;
  unsigned int NB_NODES = 1000;
  for (unsigned int i=0; i<NB_NODES; ++i)
    nodes.push_back(graph->addNode());
  unsigned int NB_EDGES = NB_NODES - 1;
  for (unsigned int i=0; i<NB_EDGES; ++i)
    edges.push_back(graph->addEdge(nodes[0],nodes[i+1]));
  {
    Iterator<edge> *it = graph->getInOutEdges(nodes[0]);
    unsigned int i = 0;
    while(it->hasNext()) {
      CPPUNIT_ASSERT( it->next() == edges[i]);
      ++i;
    }delete it;
  }
  //change order
  for (unsigned int j=0;j < NB_EDGES; ++j) {
    unsigned int u = rand()%NB_EDGES;
    unsigned int v = rand()%NB_EDGES;
    edge tmp = edges[u];
    edges[u] = edges[v];
    edges[v] = tmp;
  }
  graph->setEdgeOrder(nodes[0],edges);
  {
    Iterator<edge> *it = graph->getInOutEdges(nodes[0]);
    unsigned int i = 0;
    while(it->hasNext()) {
      CPPUNIT_ASSERT(it->next() == edges[i]);
      ++i;
    }delete it;
  }
  //Swap two edge
  for (unsigned int j=0;j < NB_EDGES; ++j) {
    unsigned int u = rand()%NB_EDGES;
    unsigned int v = rand()%NB_EDGES;
    graph->swapEdgeOrder(nodes[0],edges[u],edges[v]);
    edge tmp = edges[u];
    edges[u] = edges[v];
    edges[v] = tmp;
  }
  {
    Iterator<edge> *it = graph->getInOutEdges(nodes[0]);
    unsigned int i = 0;
    while(it->hasNext()) {
      CPPUNIT_ASSERT(it->next() == edges[i]);
      ++i;
    }delete it;
  }
  graph->clear();
}
//==========================================================
void SuperGraphTest::testDeleteSubgraph() {
  graph->clear();
  Graph *g1, *g2, *g3, *g4;
  g1 = graph->addSubGraph();
  g2 = graph->addSubGraph();
  g3 = g2->addSubGraph();
  g4 = g2->addSubGraph();

  CPPUNIT_ASSERT(graph->getSuperGraph()==graph);
  CPPUNIT_ASSERT(g1->getSuperGraph()==graph);
  CPPUNIT_ASSERT(g2->getSuperGraph()==graph);
  CPPUNIT_ASSERT(g3->getSuperGraph()==g2);
  CPPUNIT_ASSERT(g4->getSuperGraph()==g2);

  CPPUNIT_ASSERT(graph->getRoot()==graph);
  CPPUNIT_ASSERT(g1->getRoot()==graph);
  CPPUNIT_ASSERT(g2->getRoot()==graph);
  CPPUNIT_ASSERT(g3->getRoot()==graph);
  CPPUNIT_ASSERT(g4->getRoot()==graph);

  graph->delSubGraph(g2);
  CPPUNIT_ASSERT(graph->getSuperGraph()==graph);
  CPPUNIT_ASSERT(g1->getSuperGraph()==graph);
  CPPUNIT_ASSERT(g3->getSuperGraph()==graph);
  CPPUNIT_ASSERT(g4->getSuperGraph()==graph);

  graph->delSubGraph(g3);
  graph->delSubGraph(g4);
  g2 = g1->addSubGraph();
  g3 = g2->addSubGraph();
  g4 = g3->addSubGraph();
  // try to delete its father
  // fail because of an assertion in GraphAbstract::delSubGraph
  //g3->delSubGraph(g2);
  g1->delAllSubGraphs(g2);
  Iterator<Graph *> *itGr = g1->getSubGraphs();
  CPPUNIT_ASSERT(!itGr->hasNext());
  delete itGr;
  graph->clear();
}
//==========================================================
void SuperGraphTest::testSubgraphId() {
  graph->clear();
  BooleanProperty sel(graph);
  Graph *g1 = graph->addSubGraph(&sel);
  unsigned int id = g1->getId();
  for (unsigned int i = 1; i<1000; ++i) {
    g1 = graph->addSubGraph(&sel);
    CPPUNIT_ASSERT(g1->getId() == id + i);
    CPPUNIT_ASSERT(graph->getSubGraph(id + i) == g1);
  }
  Graph *g;
  unsigned int i = 0; //if the graph are not ordered that test can fail.
  forEach(g, graph->getSubGraphs()) {
    CPPUNIT_ASSERT(g->getId() == id + i);
    CPPUNIT_ASSERT(graph->getSubGraph(id + i) == g);
    ++i;
  }
}
//==========================================================
void SuperGraphTest::testSubgraph() {
  graph->clear();
  Graph *g1, *g2, *g3, *g4;
  g1 = graph->addSubGraph();
  g2 = graph->addSubGraph();
  g3 = g2->addSubGraph();
  g4 = g2->addSubGraph();

  CPPUNIT_ASSERT(graph->getSuperGraph()==graph);
  CPPUNIT_ASSERT(g1->getSuperGraph()==graph);
  CPPUNIT_ASSERT(g2->getSuperGraph()==graph);
  CPPUNIT_ASSERT(g3->getSuperGraph()==g2);
  CPPUNIT_ASSERT(g4->getSuperGraph()==g2);  
  CPPUNIT_ASSERT(graph->getSubGraph(g1->getId()) == g1);
  CPPUNIT_ASSERT(graph->getSubGraph(g2->getId()) == g2);
  CPPUNIT_ASSERT(graph->getSubGraph(g3->getId()) == NULL);
  CPPUNIT_ASSERT(graph->getSubGraph(g4->getId()) == NULL);
  CPPUNIT_ASSERT(g2->getSubGraph(g3->getId()) == g3);
  CPPUNIT_ASSERT(g2->getSubGraph(g4->getId()) == g4);
  CPPUNIT_ASSERT(graph->isSubGraph(g1));
  CPPUNIT_ASSERT(graph->isSubGraph(g2));
  CPPUNIT_ASSERT(graph->isSubGraph(g3) == false);
  CPPUNIT_ASSERT(graph->isSubGraph(g4) == false);
  CPPUNIT_ASSERT(g2->isSubGraph(g3));
  CPPUNIT_ASSERT(g2->isSubGraph(g4));
  CPPUNIT_ASSERT(graph->getDescendantGraph(g1->getId()) == g1);
  CPPUNIT_ASSERT(graph->getDescendantGraph(g2->getId()) == g2);
  CPPUNIT_ASSERT(graph->getDescendantGraph(g3->getId()) == g3);
  CPPUNIT_ASSERT(graph->getDescendantGraph(g4->getId()) == g4);
  CPPUNIT_ASSERT(g2->getDescendantGraph(g3->getId()) == g3);
  CPPUNIT_ASSERT(g2->getDescendantGraph(g4->getId()) == g4);
  CPPUNIT_ASSERT(graph->isDescendantGraph(g1));
  CPPUNIT_ASSERT(graph->isDescendantGraph(g2));
  CPPUNIT_ASSERT(graph->isDescendantGraph(g3));
  CPPUNIT_ASSERT(graph->isDescendantGraph(g4));
  CPPUNIT_ASSERT(g2->isDescendantGraph(g3));
  CPPUNIT_ASSERT(g2->isDescendantGraph(g4));
  

  CPPUNIT_ASSERT(graph->getRoot()==graph);
  CPPUNIT_ASSERT(g1->getRoot()==graph);
  CPPUNIT_ASSERT(g2->getRoot()==graph);
  CPPUNIT_ASSERT(g3->getRoot()==graph);
  CPPUNIT_ASSERT(g4->getRoot()==graph);

  Iterator<Graph *> *it= g2->getSubGraphs();
  Graph *a,*b;
  CPPUNIT_ASSERT(it->hasNext());
  a = it->next();
  CPPUNIT_ASSERT(it->hasNext());
  b = it->next();
  CPPUNIT_ASSERT((a==g3 && b==g4) || (a==g4 && b==g3));
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;
  g2->clear();
  it= g2->getSubGraphs();
  CPPUNIT_ASSERT(!it->hasNext());
  delete it;

  g3 = g2->addSubGraph();
  g4 = g2->addSubGraph();  
  node n1 = g3->addNode();
  node n2 = g3->addNode();
  CPPUNIT_ASSERT(g3->isElement(n1) && g3->isElement(n2));
  CPPUNIT_ASSERT(g2->isElement(n1) && graph->isElement(n1));
  CPPUNIT_ASSERT(g2->isElement(n2) && graph->isElement(n2));
  CPPUNIT_ASSERT(!g4->isElement(n1) && !g1->isElement(n1));  
  CPPUNIT_ASSERT(!g4->isElement(n2) && !g1->isElement(n2));  
  
  edge e = g2->addEdge(n1,n2);
  CPPUNIT_ASSERT(!g3->isElement(e) && !g4->isElement(e) && !g1->isElement(e));
  CPPUNIT_ASSERT(g2->isElement(e) && graph->isElement(e) && g2->isElement(n2));
  g2->delNode(n2);
  CPPUNIT_ASSERT(!g2->isElement(n2) && !g2->isElement(e));
  CPPUNIT_ASSERT(graph->isElement(n2) && graph->isElement(e));
  g2->addNode(n2);
  g2->addEdge(e);
  CPPUNIT_ASSERT(g2->isElement(n2) && g2->isElement(e));
  g2->delAllNode(n2);
  CPPUNIT_ASSERT(!g2->isElement(n2) && !g2->isElement(e));
  CPPUNIT_ASSERT(!graph->isElement(n2) && !graph->isElement(e));
  graph->clear();

  g1 = graph->addSubGraph();
  g2 = graph;
  graph = g1;
  testAddDel();
  testClear();
  testOrderEdgeAndSwap();
  testIterators();
  graph = g2;
  graph->clear();
}
//==========================================================
void SuperGraphTest::testInheritance() {
  graph->clear();
  Graph *g1, *g2, *g3, *g4;

  g1 = graph->addSubGraph();
  g2 = graph->addSubGraph();
  g3 = g2->addSubGraph();
  g4 = g2->addSubGraph();
  DoubleProperty *m = graph->getProperty<DoubleProperty>("metric");
  CPPUNIT_ASSERT(graph->existProperty("metric"));
  CPPUNIT_ASSERT(g1->existProperty("metric"));
  CPPUNIT_ASSERT(g2->existProperty("metric"));
  CPPUNIT_ASSERT(g3->existProperty("metric"));
  CPPUNIT_ASSERT(g4->existProperty("metric"));

  graph->delLocalProperty("metric");
  CPPUNIT_ASSERT(!graph->existProperty("metric"));
  CPPUNIT_ASSERT(!g1->existProperty("metric"));
  CPPUNIT_ASSERT(!g2->existProperty("metric"));
  CPPUNIT_ASSERT(!g3->existProperty("metric"));
  CPPUNIT_ASSERT(!g4->existProperty("metric"));


  DoubleProperty *m2 = g2->getLocalProperty<DoubleProperty>("metric");
  CPPUNIT_ASSERT(!graph->existProperty("metric"));
  CPPUNIT_ASSERT(!g1->existProperty("metric"));
  CPPUNIT_ASSERT(g2->existProperty("metric"));
  CPPUNIT_ASSERT(g3->existProperty("metric"));
  CPPUNIT_ASSERT(g4->existProperty("metric"));

  m = graph->getProperty<DoubleProperty>("metric");
  CPPUNIT_ASSERT(graph->getProperty("metric") == m);
  CPPUNIT_ASSERT(g1->getProperty("metric") == m);
  CPPUNIT_ASSERT(g2->getProperty("metric") == m2);
  CPPUNIT_ASSERT(g3->getProperty("metric") == m2);
  CPPUNIT_ASSERT(g4->getProperty("metric") == m2);

  g2->delLocalProperty("metric");
  CPPUNIT_ASSERT(graph->getProperty("metric") == m);
  CPPUNIT_ASSERT(g1->getProperty("metric") == m);
  CPPUNIT_ASSERT(g2->getProperty("metric") == m);
  CPPUNIT_ASSERT(g3->getProperty("metric") == m);
  CPPUNIT_ASSERT(g4->getProperty("metric") == m);

  graph->clear();
}
//==========================================================
void SuperGraphTest::testPropertiesIteration() {
  graph->clear();
  Graph *g1, *g2, *g3, *g4;
  set<string> propList1;
  set<string> propList2;
  propList1.insert("m1");
  propList1.insert("m2");
  propList1.insert("m3");
  propList2.insert("m4");
  propList2.insert("m5");
  propList2.insert("m6");
  g1 = graph->addSubGraph();
  g2 = graph->addSubGraph();
  g3 = g2->addSubGraph();
  g4 = g2->addSubGraph();

  set<string>::const_iterator it;
  for (it=propList1.begin();it!=propList1.end();++it) {
    graph->getProperty<IntegerProperty>(*it);
    CPPUNIT_ASSERT(g4->existProperty(*it));
  }
  Iterator<string> *itS = graph->getProperties();
  while(itS->hasNext()) {
    CPPUNIT_ASSERT(propList1.find(itS->next())!=propList1.end());
  } delete itS;
  itS = g2->getProperties();
  while(itS->hasNext()) {
    CPPUNIT_ASSERT(propList1.find(itS->next())!=propList1.end());
  } delete itS;
  itS = g1->getLocalProperties();
  while(itS->hasNext()) {
    CPPUNIT_ASSERT(propList1.find(itS->next())==propList1.end());
  } delete itS;
  itS = g1->getLocalProperties();
  while(itS->hasNext()) {
    CPPUNIT_ASSERT(propList1.find(itS->next())==propList1.end());
  } delete itS;
  itS = g4->getInheritedProperties();
  while(itS->hasNext()) {
    CPPUNIT_ASSERT(propList1.find(itS->next())!=propList1.end());
  } delete itS;

  for (it=propList2.begin();it!=propList2.end();++it) {
    g2->getProperty<IntegerProperty>(*it);
    CPPUNIT_ASSERT(g4->existProperty(*it));
  }

  itS = graph->getProperties();
  while(itS->hasNext()) {
    string str = itS->next();
    CPPUNIT_ASSERT(propList1.find(str)!=propList1.end()  && propList2.find(str)==propList2.end());
  } delete itS;
  itS = g1->getProperties();
  while(itS->hasNext()) {
    string str = itS->next();
    CPPUNIT_ASSERT(propList1.find(str)!=propList1.end()  && propList2.find(str)==propList2.end());
  } delete itS;
  itS = g2->getProperties();
  while(itS->hasNext()) {
    string str = itS->next();
    CPPUNIT_ASSERT(propList1.find(str)!=propList1.end()  || propList2.find(str)!=propList2.end());
  } delete itS;
  itS = g3->getProperties();
  while(itS->hasNext()) {
    string str = itS->next();
    CPPUNIT_ASSERT(propList1.find(str)!=propList1.end()  || propList2.find(str)!=propList2.end());
  } delete itS;
  itS = g4->getProperties();
  while(itS->hasNext()) {
    string str = itS->next();
    CPPUNIT_ASSERT(propList1.find(str)!=propList1.end()  || propList2.find(str)!=propList2.end());
  } delete itS;
  graph->clear();
}

//==========================================================
void SuperGraphTest::testAttributes() {
  graph->clear();
  float f = 12.12;
  CPPUNIT_ASSERT(!graph->getAttribute("f", f));
  graph->setAttribute("name", string("The Graph"));
  string name = graph->getAttribute<string>("name");
  CPPUNIT_ASSERT(name == string("The Graph"));
  CPPUNIT_ASSERT(graph->getAttribute("name", name));
  CPPUNIT_ASSERT(name == string("The Graph"));
  DataType* dt = graph->getAttribute("name");
  CPPUNIT_ASSERT(dt != NULL);
  delete dt;
  TypedData<string> dtc(new string("test"));
  graph->setAttribute("name", (const DataType *) &dtc);
  CPPUNIT_ASSERT(graph->attributeExist("name"));
  DataType *dataType = graph->getAttribute("name");
  CPPUNIT_ASSERT(dataType->getTypeName().compare(typeid(string).name())==0);
}
//==========================================================
void SuperGraphTest::testGetNodesEqualTo() {
  for (int i = 0; i != 10; ++i) {
    graph->addNode();
  }
  BooleanProperty property(graph);
  property.setAllNodeValue(false);
  Graph* subGraph = graph->addSubGraph();

  Iterator<node>* it = graph->getNodes();
  /*for (int i = 0; i < 5; ++i) {
    subGraph->addNode(it->next());
    }*/
  delete it;

  it = property.getNodesEqualTo(false, subGraph);
  while(it->hasNext()) {
    node n = it->next();
    CPPUNIT_ASSERT(subGraph->isElement(n));
  }
  delete it;
}


//==========================================================
CppUnit::Test * SuperGraphTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : Graph" );
  suiteOfTests->addTest( new CppUnit::TestCaller<SuperGraphTest>( "Add/Del operations (Simple Graph)", 
								  &SuperGraphTest::testAddDel) );
  suiteOfTests->addTest( new CppUnit::TestCaller<SuperGraphTest>( "Clean operations (Simple Graph)", 
								  &SuperGraphTest::testClear) );
  suiteOfTests->addTest( new CppUnit::TestCaller<SuperGraphTest>( "Graph structure Iterators", 
								  &SuperGraphTest::testIterators) );
  suiteOfTests->addTest( new CppUnit::TestCaller<SuperGraphTest>( "Edge ordering and swap(Simple Graph)", 
								  &SuperGraphTest::testOrderEdgeAndSwap) );
  suiteOfTests->addTest( new CppUnit::TestCaller<SuperGraphTest>( "Outdeg, indeg, deg on graph and sub-graph", 
								  &SuperGraphTest::testDegree) );
  suiteOfTests->addTest( new CppUnit::TestCaller<SuperGraphTest>( "Sub Graph operations (add/del/clean/iterators)", 
								  &SuperGraphTest::testSubgraph) );
  suiteOfTests->addTest( new CppUnit::TestCaller<SuperGraphTest>( "Test Sub Graph delete", 
								  &SuperGraphTest::testDeleteSubgraph) );
  suiteOfTests->addTest( new CppUnit::TestCaller<SuperGraphTest>( "Inheritance of properties", 
								  &SuperGraphTest::testInheritance) );
  suiteOfTests->addTest( new CppUnit::TestCaller<SuperGraphTest>( "Iteration of properties", 
								  &SuperGraphTest::testPropertiesIteration) );
  suiteOfTests->addTest( new CppUnit::TestCaller<SuperGraphTest>( "Test subgraph id", 
								  &SuperGraphTest::testSubgraphId) );
  suiteOfTests->addTest( new CppUnit::TestCaller<SuperGraphTest>( "Test attributes", 
								  &SuperGraphTest::testAttributes) );
  suiteOfTests->addTest( new CppUnit::TestCaller<SuperGraphTest>( "Test getNodesEqualTo", 
								  &SuperGraphTest::testGetNodesEqualTo) );
  return suiteOfTests;
}
//==========================================================
