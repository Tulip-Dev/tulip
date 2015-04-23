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
#include "SuperGraphTest.h"
#include <tulip/BooleanProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>

using namespace std;
using namespace tlp;

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

    while(itN->hasNext()) {
      CPPUNIT_ASSERT_EQUAL( nodes[i].id , itN->next().id );
      ++i;
    }

    delete itN;
    CPPUNIT_ASSERT_EQUAL( i , NB_NODES );
  }
  {
    Iterator<edge> *itE = graph->getEdges();
    unsigned int i = 0;

    while(itE->hasNext()) {
      CPPUNIT_ASSERT_EQUAL( edges[i].id , itE->next().id );
      ++i;
    }

    delete itE;
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
  CPPUNIT_ASSERT(!it1->hasNext());
  delete it1;
  Iterator<node> *it2 = graph->getInNodes(n1);
  CPPUNIT_ASSERT_EQUAL(n1.id, it2->next().id);
  CPPUNIT_ASSERT_EQUAL(n2.id, it2->next().id);
  CPPUNIT_ASSERT(!it2->hasNext());
  delete it2;
  Iterator<node> *it3 = graph->getInOutNodes(n1);
  CPPUNIT_ASSERT_EQUAL(n1.id, it3->next().id);
  CPPUNIT_ASSERT_EQUAL(n1.id, it3->next().id);
  CPPUNIT_ASSERT_EQUAL(n2.id, it3->next().id);
  CPPUNIT_ASSERT_EQUAL(n2.id, it3->next().id);
  CPPUNIT_ASSERT_EQUAL(n3.id, it3->next().id);
  CPPUNIT_ASSERT(!it3->hasNext());
  delete it3;

  Iterator<edge> *it4 = graph->getOutEdges(n1);
  CPPUNIT_ASSERT_EQUAL(e1.id, it4->next().id);
  CPPUNIT_ASSERT_EQUAL(e2.id, it4->next().id);
  CPPUNIT_ASSERT_EQUAL(e4.id, it4->next().id);
  CPPUNIT_ASSERT(!it4->hasNext());
  delete it4;
  Iterator<edge> *it5 = graph->getInEdges(n1);
  CPPUNIT_ASSERT_EQUAL(e1.id, it5->next().id);
  CPPUNIT_ASSERT_EQUAL(e3.id, it5->next().id);
  CPPUNIT_ASSERT(!it5->hasNext());
  delete it5;
  Iterator<edge> *it6 = graph->getInOutEdges(n1);
  CPPUNIT_ASSERT_EQUAL(e1.id, it6->next().id);
  CPPUNIT_ASSERT_EQUAL(e1.id, it6->next().id);
  CPPUNIT_ASSERT_EQUAL(e2.id, it6->next().id);
  CPPUNIT_ASSERT_EQUAL(e3.id, it6->next().id);
  CPPUNIT_ASSERT_EQUAL(e4.id, it6->next().id);
  CPPUNIT_ASSERT(!it6->hasNext());
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

    for (; it->hasNext(); it->next())
      ++outdeg;

    delete it;
    it = graph->getInEdges(n);

    for (; it->hasNext(); it->next())
      ++indeg;

    delete it;
    it = graph->getInOutEdges(n);

    for (; it->hasNext(); it->next())
      ++deg;

    delete it;
    CPPUNIT_ASSERT_EQUAL(graph->indeg(n) , indeg);
    CPPUNIT_ASSERT_EQUAL(graph->outdeg(n), outdeg);
    CPPUNIT_ASSERT_EQUAL(graph->deg(n)   , deg);
    CPPUNIT_ASSERT_EQUAL(indeg + outdeg  , deg);
  }

  delete itN;
}
//==========================================================
void SuperGraphTest::testDegree() {
  graph->clear();
  build(100, 100);
  degreeCheck(graph);
  Graph *gr = graph->addCloneSubGraph();
  CPPUNIT_ASSERT_EQUAL(graph, gr->getSuperGraph());
  Graph *gr1 = gr->addCloneSubGraph("gr1", true);
  CPPUNIT_ASSERT_EQUAL(graph, gr1->getSuperGraph());
  Graph *gr2 = gr1->addCloneSubGraph();
  CPPUNIT_ASSERT_EQUAL(gr1, gr2->getSuperGraph());

  degreeCheck(graph);
  degreeCheck(gr);
  degreeCheck(gr1);
  degreeCheck(gr2);
  Iterator<edge> *it = graph->getEdges();

  while(it->hasNext()) {
    graph->reverse(it->next());
  }

  delete it;
  degreeCheck(graph);
  degreeCheck(gr);
  degreeCheck(gr1);
  degreeCheck(gr2);
  it = gr2->getEdges();

  while(it->hasNext()) {
    gr2->reverse(it->next());
  }

  delete it;
  degreeCheck(graph);
  degreeCheck(gr);
  degreeCheck(gr1);
  degreeCheck(gr2);

  gr1->delSubGraph(gr2);
  graph->delSubGraph(gr1);
  graph->delSubGraph(gr);

  graph->clear();
  gr1 = graph->addCloneSubGraph();
  gr2 = gr1->addCloneSubGraph();
  node n1 = graph->addNode();
  node n2 = graph->addNode();
  node n3 = graph->addNode();
  edge e = graph->addEdge(n1, n2);
  gr2->addNode(n1);
  gr2->addNode(n2);
  gr2->addNode(n3);
  gr2->addEdge(e);
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
  graph->setEnds(e, n3, n1);
  CPPUNIT_ASSERT_EQUAL(3u, graph->numberOfNodes());
  CPPUNIT_ASSERT_EQUAL(2u, graph->numberOfEdges());
  CPPUNIT_ASSERT_EQUAL(3u, gr1->numberOfNodes());
  CPPUNIT_ASSERT_EQUAL(2u, gr1->numberOfEdges());
  CPPUNIT_ASSERT_EQUAL(2u, gr2->numberOfNodes());
  CPPUNIT_ASSERT_EQUAL(0u, gr2->numberOfEdges());
  CPPUNIT_ASSERT(graph->isElement(e));
  CPPUNIT_ASSERT(gr1->isElement(e));
  CPPUNIT_ASSERT(!gr2->isElement(e));
  CPPUNIT_ASSERT_EQUAL(1u, graph->indeg(n1));
  CPPUNIT_ASSERT_EQUAL(0u, graph->indeg(n2));
  CPPUNIT_ASSERT_EQUAL(1u, graph->indeg(n3));
  CPPUNIT_ASSERT_EQUAL(1u, graph->outdeg(n1));
  CPPUNIT_ASSERT_EQUAL(0u, graph->outdeg(n2));
  CPPUNIT_ASSERT_EQUAL(2u, graph->deg(n1));
  CPPUNIT_ASSERT_EQUAL(0u, graph->deg(n2));
  CPPUNIT_ASSERT_EQUAL(2u, graph->deg(n3));
  CPPUNIT_ASSERT_EQUAL(1u, gr1->indeg(n1));
  CPPUNIT_ASSERT_EQUAL(0u, gr1->indeg(n2));
  CPPUNIT_ASSERT_EQUAL(1u, gr1->indeg(n3));
  CPPUNIT_ASSERT_EQUAL(1u, gr1->outdeg(n1));
  CPPUNIT_ASSERT_EQUAL(0u, gr1->outdeg(n2));
  CPPUNIT_ASSERT_EQUAL(2u, gr1->deg(n1));
  CPPUNIT_ASSERT_EQUAL(0u, gr1->deg(n2));
  CPPUNIT_ASSERT_EQUAL(2u, gr1->deg(n3));
}
//==========================================================
void SuperGraphTest::testAddDel() {
  //  qWarning() << __PRETTY_FUNCTION__ << endl;
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

  CPPUNIT_ASSERT_EQUAL(NB_ADD, graph->numberOfNodes());

  graph->delNodes(nodes);

  CPPUNIT_ASSERT_EQUAL(0u, graph->numberOfNodes());

  graph->addNodes(NB_ADD, nodes);

  for (unsigned int i=0; i<NB_ADD; ++i) {
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

  for (unsigned int i=0; i < NB_EDGES; ++i) {
    graph->delEdge(edges[i]);
    CPPUNIT_ASSERT(!graph->isElement(edges[i]));
  }

  CPPUNIT_ASSERT_EQUAL(0u, graph->numberOfEdges());

  vector<pair<node, node> > ends;

  for (unsigned int i=0; i< NB_EDGES; ++i) {
    ends.push_back(pair<node, node>(nodes[rand()%NB_ADD],nodes[rand()%NB_ADD]));
  }

  graph->addEdges(ends, edges);
  CPPUNIT_ASSERT_EQUAL(NB_EDGES, graph->numberOfEdges());


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
      CPPUNIT_ASSERT_EQUAL(edges[i], it->next());
      ++i;
    }

    delete it;
  }

  //change order
  for (unsigned int j=0; j < NB_EDGES; ++j) {
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
      CPPUNIT_ASSERT_EQUAL(edges[i], it->next());
      ++i;
    }

    delete it;
  }

  //Swap two edge
  for (unsigned int j=0; j < NB_EDGES; ++j) {
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
      CPPUNIT_ASSERT_EQUAL(edges[i], it->next());
      ++i;
    }

    delete it;
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

  CPPUNIT_ASSERT_EQUAL(graph, graph->getSuperGraph());
  CPPUNIT_ASSERT_EQUAL(graph, g1->getSuperGraph());
  CPPUNIT_ASSERT_EQUAL(graph, g2->getSuperGraph());
  CPPUNIT_ASSERT_EQUAL(g2, g3->getSuperGraph());
  CPPUNIT_ASSERT_EQUAL(g2, g4->getSuperGraph());

  CPPUNIT_ASSERT_EQUAL(graph, graph->getRoot());
  CPPUNIT_ASSERT_EQUAL(graph, g1->getRoot());
  CPPUNIT_ASSERT_EQUAL(graph, g2->getRoot());
  CPPUNIT_ASSERT_EQUAL(graph, g3->getRoot());
  CPPUNIT_ASSERT_EQUAL(graph, g4->getRoot());

  graph->delSubGraph(g2);
  CPPUNIT_ASSERT_EQUAL(graph, graph->getSuperGraph());
  CPPUNIT_ASSERT_EQUAL(graph, g1->getSuperGraph());
  CPPUNIT_ASSERT_EQUAL(graph, g3->getSuperGraph());
  CPPUNIT_ASSERT_EQUAL(graph, g4->getSuperGraph());

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
    CPPUNIT_ASSERT_EQUAL(id + i, g1->getId());
    CPPUNIT_ASSERT_EQUAL(g1, graph->getSubGraph(id + i));
  }

  Graph *g;
  unsigned int i = 0; //if the graph are not ordered that test can fail.
  forEach(g, graph->getSubGraphs()) {
    CPPUNIT_ASSERT_EQUAL(id + i, g->getId());
    CPPUNIT_ASSERT_EQUAL(g, graph->getSubGraph(id + i));
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

  CPPUNIT_ASSERT_EQUAL(graph, graph->getSuperGraph());
  CPPUNIT_ASSERT_EQUAL(graph, g1->getSuperGraph());
  CPPUNIT_ASSERT_EQUAL(graph, g2->getSuperGraph());
  CPPUNIT_ASSERT_EQUAL(g2, g3->getSuperGraph());
  CPPUNIT_ASSERT_EQUAL(g2, g4->getSuperGraph());
  CPPUNIT_ASSERT_EQUAL(g1, graph->getSubGraph(g1->getId()));
  CPPUNIT_ASSERT_EQUAL(g2, graph->getSubGraph(g2->getId()));
  CPPUNIT_ASSERT(graph->getSubGraph(g3->getId()) == NULL);
  CPPUNIT_ASSERT(graph->getSubGraph(g4->getId()) == NULL);
  CPPUNIT_ASSERT_EQUAL(g3, g2->getSubGraph(g3->getId()));
  CPPUNIT_ASSERT_EQUAL(g4, g2->getSubGraph(g4->getId()));
  CPPUNIT_ASSERT(graph->isSubGraph(g1));
  CPPUNIT_ASSERT(graph->isSubGraph(g2));
  CPPUNIT_ASSERT(!graph->isSubGraph(g3));
  CPPUNIT_ASSERT(!graph->isSubGraph(g4));
  CPPUNIT_ASSERT(g2->isSubGraph(g3));
  CPPUNIT_ASSERT(g2->isSubGraph(g4));
  CPPUNIT_ASSERT_EQUAL(g1, graph->getDescendantGraph(g1->getId()));
  CPPUNIT_ASSERT_EQUAL(g2, graph->getDescendantGraph(g2->getId()));
  CPPUNIT_ASSERT_EQUAL(g3, graph->getDescendantGraph(g3->getId()));
  CPPUNIT_ASSERT_EQUAL(g4, graph->getDescendantGraph(g4->getId()));
  CPPUNIT_ASSERT_EQUAL(g3, g2->getDescendantGraph(g3->getId()));
  CPPUNIT_ASSERT_EQUAL(g4, g2->getDescendantGraph(g4->getId()));
  CPPUNIT_ASSERT(graph->isDescendantGraph(g1));
  CPPUNIT_ASSERT(graph->isDescendantGraph(g2));
  CPPUNIT_ASSERT(graph->isDescendantGraph(g3));
  CPPUNIT_ASSERT(graph->isDescendantGraph(g4));
  CPPUNIT_ASSERT(g2->isDescendantGraph(g3));
  CPPUNIT_ASSERT(g2->isDescendantGraph(g4));


  CPPUNIT_ASSERT_EQUAL(graph, graph->getRoot());
  CPPUNIT_ASSERT_EQUAL(graph, g1->getRoot());
  CPPUNIT_ASSERT_EQUAL(graph, g2->getRoot());
  CPPUNIT_ASSERT_EQUAL(graph, g3->getRoot());
  CPPUNIT_ASSERT_EQUAL(graph, g4->getRoot());

  // graph's descendant graphs loop
  Iterator<Graph *>* it = graph->getDescendantGraphs();
  CPPUNIT_ASSERT(it->hasNext());
  set<Graph*> dg;
  dg.insert(g1);
  dg.insert(g2);
  dg.insert(g3);
  dg.insert(g4);
  while(it->hasNext())  {
    Graph *g = it->next();
    set<Graph*>::iterator itdg = dg.find(g);
    CPPUNIT_ASSERT(itdg != dg.end());
    dg.erase(itdg);
  }
  delete it;
  CPPUNIT_ASSERT(dg.empty());

  it= g2->getSubGraphs();
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
  g2->delNode(n2, true);
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
  CPPUNIT_ASSERT_EQUAL(m, graph->getProperty<DoubleProperty>("metric"));
  CPPUNIT_ASSERT_EQUAL(m, g1->getProperty<DoubleProperty>("metric"));
  CPPUNIT_ASSERT_EQUAL(m2, g2->getProperty<DoubleProperty>("metric"));
  CPPUNIT_ASSERT_EQUAL(m2, g3->getProperty<DoubleProperty>("metric"));
  CPPUNIT_ASSERT_EQUAL(m2, g4->getProperty<DoubleProperty>("metric"));

  m2->rename("metric1");
  CPPUNIT_ASSERT_EQUAL(m, graph->getProperty<DoubleProperty>("metric"));
  CPPUNIT_ASSERT_EQUAL(m, g1->getProperty<DoubleProperty>("metric"));
  CPPUNIT_ASSERT_EQUAL(m2, g2->getProperty<DoubleProperty>("metric1"));
  CPPUNIT_ASSERT_EQUAL(m, g3->getProperty<DoubleProperty>("metric"));
  CPPUNIT_ASSERT_EQUAL(m, g4->getProperty<DoubleProperty>("metric"));
  CPPUNIT_ASSERT_EQUAL(m2, g3->getProperty<DoubleProperty>("metric1"));
  CPPUNIT_ASSERT_EQUAL(m2, g4->getProperty<DoubleProperty>("metric1"));

  g2->delLocalProperty("metric1");
  CPPUNIT_ASSERT_EQUAL(g2->existLocalProperty("metric1"), false);
  CPPUNIT_ASSERT_EQUAL(g3->existLocalProperty("metric1"), false);
  CPPUNIT_ASSERT_EQUAL(g4->existLocalProperty("metric1"), false);

  
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

  for (it=propList1.begin(); it!=propList1.end(); ++it) {
    graph->getProperty<IntegerProperty>(*it);
    CPPUNIT_ASSERT(g4->existProperty(*it));
  }

  Iterator<string> *itS = graph->getProperties();

  while(itS->hasNext()) {
    string str = itS->next();
    if (str.size() == 2)
      CPPUNIT_ASSERT(propList1.find(str)!=propList1.end());
  }

  delete itS;
  itS = g2->getProperties();

  while(itS->hasNext()) {
    string str = itS->next();
    if (str.size() == 2)
      CPPUNIT_ASSERT(propList1.find(str)!=propList1.end());
  }

  delete itS;
  itS = g1->getLocalProperties();

  while(itS->hasNext()) {
    string str = itS->next();
    if (str.size() == 2)
      CPPUNIT_ASSERT(propList1.find(str)!=propList1.end());
  }

  delete itS;
  itS = g1->getLocalProperties();

  while(itS->hasNext()) {
    string str = itS->next();
    if (str.size() == 2)
      CPPUNIT_ASSERT(propList1.find(str)!=propList1.end());
  }

  delete itS;
  itS = g4->getInheritedProperties();

  while(itS->hasNext()) {
    string str = itS->next();
    if (str.size() == 2)
      CPPUNIT_ASSERT(propList1.find(str)!=propList1.end());
  }

  delete itS;

  for (it=propList2.begin(); it!=propList2.end(); ++it) {
    g2->getProperty<IntegerProperty>(*it);
    CPPUNIT_ASSERT(g4->existProperty(*it));
  }

  itS = graph->getProperties();

  while(itS->hasNext()) {
    string str = itS->next();
    if (str.size() == 2)
      CPPUNIT_ASSERT(propList1.find(str)!=propList1.end()  && propList2.find(str)==propList2.end());
  }

  delete itS;
  itS = g1->getProperties();

  while(itS->hasNext()) {
    string str = itS->next();
    if (str.size() == 2)
      CPPUNIT_ASSERT(propList1.find(str)!=propList1.end()  && propList2.find(str)==propList2.end());
  }

  delete itS;
  itS = g2->getProperties();

  while(itS->hasNext()) {
    string str = itS->next();
    if (str.size() == 2)
      CPPUNIT_ASSERT(propList1.find(str)!=propList1.end()  || propList2.find(str)!=propList2.end());
  }

  delete itS;
  itS = g3->getProperties();

  while(itS->hasNext()) {
    string str = itS->next();
    if (str.size() == 2)
      CPPUNIT_ASSERT(propList1.find(str)!=propList1.end()  || propList2.find(str)!=propList2.end());
  }

  delete itS;
  itS = g4->getProperties();

  while(itS->hasNext()) {
    string str = itS->next();
    if (str.size() == 2)
      CPPUNIT_ASSERT(propList1.find(str)!=propList1.end()  || propList2.find(str)!=propList2.end());
  }

  delete itS;
  graph->clear();
}

//==========================================================
void SuperGraphTest::testAttributes() {
  graph->clear();
  float f = 12.12;
  CPPUNIT_ASSERT(!graph->getAttribute("f", f));
  graph->setAttribute("name", string("The Graph"));
  string name;
  graph->getAttribute<string>("name", name);
  CPPUNIT_ASSERT_EQUAL(string("The Graph"), name);
  CPPUNIT_ASSERT(graph->getAttribute("name", name));
  CPPUNIT_ASSERT_EQUAL(string("The Graph"), name);
  DataType* dt = graph->getAttribute("name");
  CPPUNIT_ASSERT(dt != NULL);
  delete dt;
  TypedData<string> dtc(new string("test"));
  graph->setAttribute("name", (const DataType *) &dtc);
  CPPUNIT_ASSERT(graph->attributeExist("name"));
  dt = graph->getAttribute("name");
  CPPUNIT_ASSERT_EQUAL(0, dt->getTypeName().compare(typeid(string).name()));
  delete dt;
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
