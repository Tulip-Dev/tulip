#include <cassert>
#include <iomanip>
#include <fstream>
#include <tulip/vectorgraph.h>
#include <tulip/ForEach.h>
#include <tulip/Iterator.h>

#include "CppUnitIncludes.h"

using namespace std;

namespace tlp {

class VectorGraphTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(VectorGraphTest);
  CPPUNIT_TEST(testCreate);
  CPPUNIT_TEST(testDelEdge);
  CPPUNIT_TEST(testDelNode);
  CPPUNIT_TEST(testDelEdges);
  CPPUNIT_TEST(testDelAllNodes);
  CPPUNIT_TEST(testAddDelEdges);
  CPPUNIT_TEST(testReverseEdges);
  CPPUNIT_TEST(testSetSourcesAndTargets);
  CPPUNIT_TEST(testSetEnds);
  CPPUNIT_TEST(testMoreSetEnds);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() {}
  void tearDown() {}
  void testCreate();
  void testDelEdge();
  void testDelNode();
  void testDelEdges();
  void testDelAllNodes();
  void testAddDelEdges();
  void testReverseEdges();
  void testSetSourcesAndTargets();
  void testSetEnds();
  void testMoreSetEnds();
};

CPPUNIT_TEST_SUITE_REGISTRATION(VectorGraphTest);

// nb nodes must be a multiple of 10
#define NB_NODES 10
std::vector<node> nodes;
std::vector<edge> edges;
VectorGraph graph;

// Graph population
static void populateGraph(bool nodesOnly = false) {

  // clear vector
  nodes.clear();

  // reserve nodes space
  nodes.reserve(NB_NODES);

  // create nodes
  graph.addNodes(NB_NODES, &nodes);

  if (nodesOnly)
    return;

  // clear vector
  edges.clear();

  // reserve edges space
  edges.reserve(NB_NODES);

  // create edges
  for (unsigned int i = 0; i < NB_NODES; ++i) {
    if (i == NB_NODES - 1)
      edges.push_back(graph.addEdge(nodes[i], nodes[0]));
    else
      edges.push_back(graph.addEdge(nodes[i], nodes[i + 1]));
  }
}

//==========================================================
// Check graph after population
static void checkCreatedGraph(bool nodesOnly = false) {

  // check nodes
  CPPUNIT_ASSERT(graph.numberOfNodes() == NB_NODES);

  Iterator<node> *itn = graph.getNodes();
  OMP_ITER_TYPE i = 0;
  while (itn->hasNext()) {
    CPPUNIT_ASSERT(itn->next() == nodes[i]);
    ++i;
  }
  delete itn;

  if (nodesOnly)
    return;

  // check edges
  CPPUNIT_ASSERT(graph.numberOfEdges() == NB_NODES);

  const std::vector<edge> &gEdges = graph.edges();
  CPPUNIT_ASSERT(gEdges.size() == NB_NODES);
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < NB_NODES; ++i) {
    CPPUNIT_ASSERT(gEdges[i] == edges[i]);

    // check ends
    std::pair<node, node> ends = graph.ends(edges[i]);
    CPPUNIT_ASSERT(ends.first == nodes[i]);
    if (i < NB_NODES - 1) {
      CPPUNIT_ASSERT(ends.second == nodes[i + 1]);
      CPPUNIT_ASSERT(graph.existEdge(nodes[i], nodes[i + 1], true) == edges[i]);
      CPPUNIT_ASSERT(graph.existEdge(nodes[i + 1], nodes[i], false) == edges[i]);
    } else {
      CPPUNIT_ASSERT(ends.second == nodes[0]);
      CPPUNIT_ASSERT(graph.existEdge(nodes[i], nodes[0], true) == edges[i]);
      CPPUNIT_ASSERT(graph.existEdge(nodes[0], nodes[i], false) == edges[i]);
    }
    // check edge source
    CPPUNIT_ASSERT(graph.source(edges[i]) == nodes[i]);
    // check edge target
    if (i < NB_NODES - 1)
      CPPUNIT_ASSERT(graph.target(edges[i]) == nodes[i + 1]);
    else
      CPPUNIT_ASSERT(graph.target(edges[i]) == nodes[0]);
    // check opposite
    if (i < NB_NODES - 1) {
      CPPUNIT_ASSERT(graph.opposite(edges[i], nodes[i]) == nodes[i + 1]);
      CPPUNIT_ASSERT(graph.opposite(edges[i], nodes[i + 1]) == nodes[i]);
    } else {
      CPPUNIT_ASSERT(graph.opposite(edges[i], nodes[i]) == nodes[0]);
      CPPUNIT_ASSERT(graph.opposite(edges[i], nodes[0]) == nodes[i]);
    }
  }

  // check edges per node
  const std::vector<node> &gNodes = graph.nodes();
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (OMP_ITER_TYPE i = 0; i < NB_NODES; ++i) {
    CPPUNIT_ASSERT(gNodes[i] == nodes[i]);
    // check degree
    CPPUNIT_ASSERT(graph.deg(nodes[i]) == 2);
    // check out degree
    CPPUNIT_ASSERT(graph.outdeg(nodes[i]) == 1);
    // check in degree
    CPPUNIT_ASSERT(graph.indeg(nodes[i]) == 1);

    // check out edges
    Iterator<edge> *ite = graph.getOutEdges(nodes[i]);
    CPPUNIT_ASSERT(ite->hasNext());
    CPPUNIT_ASSERT(ite->next() == edges[i]);
    CPPUNIT_ASSERT(ite->hasNext() == false);
    delete ite;

    // check in edges
    ite = graph.getInEdges(nodes[i]);
    CPPUNIT_ASSERT(ite->hasNext());
    if (i != 0)
      CPPUNIT_ASSERT(ite->next() == edges[i - 1]);
    else
      CPPUNIT_ASSERT(ite->next() == edges[NB_NODES - 1]);
    CPPUNIT_ASSERT(ite->hasNext() == false);
    delete ite;
  }

#ifdef _OPENMP
#pragma omp parallel for
#endif
  // check neighbours per node
  for (OMP_ITER_TYPE i = 0; i < NB_NODES; ++i) {
    // check out neighbours
    Iterator<node> *itn = graph.getOutNodes(nodes[i]);
    CPPUNIT_ASSERT(itn->hasNext());
    if (i < NB_NODES - 1)
      CPPUNIT_ASSERT(itn->next() == nodes[i + 1]);
    else
      CPPUNIT_ASSERT(itn->next() == nodes[0]);
    CPPUNIT_ASSERT(itn->hasNext() == false);
    delete itn;

    // check in neighbours
    itn = graph.getInNodes(nodes[i]);
    CPPUNIT_ASSERT(itn->hasNext());
    if (i == 0)
      CPPUNIT_ASSERT(itn->next() == nodes[NB_NODES - 1]);
    else
      CPPUNIT_ASSERT(itn->next() == nodes[i - 1]);
    CPPUNIT_ASSERT(itn->hasNext() == false);
    delete itn;
  }
}

void VectorGraphTest::testCreate() {
  // first ensure the graph is empty
  CPPUNIT_ASSERT(graph.numberOfEdges() == 0);
  CPPUNIT_ASSERT(graph.numberOfNodes() == 0);

  // add only nodes
  populateGraph();

  // check nodes population
  checkCreatedGraph();

  // non regression test for deletion of the last edge
  // when there is no free edge and deletion of the last node
  // when there is no free node
  CPPUNIT_ASSERT(graph.isElement(edges[NB_NODES - 1]));
  CPPUNIT_ASSERT(graph.numberOfEdges() == NB_NODES);
  graph.delEdge(edges[NB_NODES - 1]);
  CPPUNIT_ASSERT(graph.numberOfEdges() == NB_NODES - 1);
  CPPUNIT_ASSERT(graph.isElement(edges[NB_NODES - 1]) == false);
  edge e = graph.addEdge(nodes[0], nodes[1]);
  CPPUNIT_ASSERT(graph.numberOfEdges() == NB_NODES);
  CPPUNIT_ASSERT(graph.isElement(e));
  CPPUNIT_ASSERT(graph.isElement(nodes[NB_NODES - 1]));
  CPPUNIT_ASSERT(graph.numberOfNodes() == NB_NODES);
  graph.delNode(nodes[NB_NODES - 1]);
  CPPUNIT_ASSERT(graph.numberOfNodes() == (NB_NODES - 1));
  CPPUNIT_ASSERT(graph.isElement(nodes[NB_NODES - 1]) == false);
  node n = graph.addNode();
  CPPUNIT_ASSERT(graph.isElement(n));
  CPPUNIT_ASSERT(graph.numberOfNodes() == NB_NODES);

  // clear graph
  graph.clear();
  CPPUNIT_ASSERT(graph.numberOfNodes() == 0);

  // recreate nodes and create edges
  populateGraph();

  // check nodes and edges
  checkCreatedGraph();
}

//==========================================================
// check graph after edges deletion
static void checkGraphAfterDelEdge() {
  CPPUNIT_ASSERT(graph.numberOfNodes() == NB_NODES);

  // check nodes
  OMP_ITER_TYPE i = 0;
  node n;
  forEach(n, graph.getNodes()) {
    CPPUNIT_ASSERT(n == nodes[i]);
    ++i;
  }

  // check edges
  CPPUNIT_ASSERT(graph.numberOfEdges() == NB_NODES / 2);
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < NB_NODES; ++i)
    CPPUNIT_ASSERT(graph.isElement(edges[i]) == ((i % 2) == 0));

  i = 0;
  edge e;
  forEach(e, graph.getEdges())++ i;
  CPPUNIT_ASSERT(i == NB_NODES / 2);

// check neighbours per node
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < NB_NODES; ++i) {
    // check out neighbours
    Iterator<node> *itn = graph.getOutNodes(nodes[i]);
    if (i % 2 == 0) {
      CPPUNIT_ASSERT(itn->hasNext());
      CPPUNIT_ASSERT(itn->next() == nodes[i + 1]);
    } else
      CPPUNIT_ASSERT(itn->hasNext() == false);
    delete itn;

    // check in neighbours
    itn = graph.getInNodes(nodes[i]);
    if (i % 2) {
      CPPUNIT_ASSERT(itn->hasNext());
      CPPUNIT_ASSERT(itn->next() == nodes[i - 1]);
    } else
      CPPUNIT_ASSERT(itn->hasNext() == false);
    delete itn;
  }
}

void VectorGraphTest::testDelEdge() {
  // check nodes and edges
  checkCreatedGraph();

  // delete some edges
  for (unsigned int i = 1; i < NB_NODES; i += 2) {
    CPPUNIT_ASSERT(graph.isElement(edges[i]));
    graph.delEdge(edges[i]);
    CPPUNIT_ASSERT(graph.isElement(edges[i]) == false);
  }

  // check nodes and remaining edges
  checkGraphAfterDelEdge();
}

//==========================================================
static void checkGraphAfterDelNode() {
  // check number of nodes
  CPPUNIT_ASSERT(graph.numberOfNodes() == 3 * NB_NODES / 4);
  OMP_ITER_TYPE i = 0;
  node n;
  forEach(n, graph.getNodes())++ i;
  CPPUNIT_ASSERT(i == 3 * NB_NODES / 4);

  // check number of edges
  CPPUNIT_ASSERT(graph.numberOfEdges() == NB_NODES / 4);
  i = 0;
  edge e;
  forEach(e, graph.getEdges())++ i;
  CPPUNIT_ASSERT(i == NB_NODES / 4);

// check nodes
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < NB_NODES; ++i)
    CPPUNIT_ASSERT(graph.isElement(nodes[i]) == ((i % 4) != 0));

// check edges
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < NB_NODES; ++i)
    CPPUNIT_ASSERT(graph.isElement(edges[i]) == ((i % 4) == 2));

// check neighbours per node
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < NB_NODES; ++i) {
    if (i % 4) {
      // check out neighbours
      Iterator<node> *itn = graph.getOutNodes(nodes[i]);
      if (i % 2 == 0) {
        CPPUNIT_ASSERT(itn->hasNext());
        CPPUNIT_ASSERT(itn->next() == nodes[i + 1]);
      } else
        CPPUNIT_ASSERT(itn->hasNext() == false);
      delete itn;

      // check in neighbours
      itn = graph.getInNodes(nodes[i]);
      if ((i % 4) == 3) {
        CPPUNIT_ASSERT(itn->hasNext());
        CPPUNIT_ASSERT(itn->next() == nodes[i - 1]);
      } else
        CPPUNIT_ASSERT(itn->hasNext() == false);
      delete itn;
    }
  }
}

void VectorGraphTest::testDelNode() {
  // check nodes and remaining edges after delEdge test
  checkGraphAfterDelEdge();

  // delete some nodes
  for (unsigned int i = 0; i < NB_NODES; i += 4) {
    CPPUNIT_ASSERT(graph.isElement(nodes[i]));
    graph.delNode(nodes[i]);
    CPPUNIT_ASSERT(graph.isElement(nodes[i]) == false);
  }

  // check remaining nodes and edges
  checkGraphAfterDelNode();
}

//==========================================================
void checkGraphAfterDelEdges() {
  // check number of nodes
  CPPUNIT_ASSERT(graph.numberOfNodes() == 3 * NB_NODES / 4);

  // check number of edges
  CPPUNIT_ASSERT(graph.numberOfEdges() == 0);
  OMP_ITER_TYPE i = 0;
  edge e;
  forEach(e, graph.getEdges())++ i;
  CPPUNIT_ASSERT(i == 0);

// check neighbours per node
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (i = 0; i < NB_NODES; ++i) {
    if (i % 4) {
      // check out neighbours
      Iterator<node> *itn = graph.getOutNodes(nodes[i]);
      CPPUNIT_ASSERT(itn->hasNext() == false);
      delete itn;
      // check in neighbours
      itn = graph.getInNodes(nodes[i]);
      CPPUNIT_ASSERT(itn->hasNext() == false);
      delete itn;
    }
  }
}

void VectorGraphTest::testDelEdges() {
  // check nodes and remaining edges after delNode test
  checkGraphAfterDelNode();

  // delete all edges
  node n;
  forEach(n, graph.getNodes()) graph.delEdges(n);

  // check remaining nodes and edges
  checkGraphAfterDelEdges();
}

//==========================================================
void VectorGraphTest::testDelAllNodes() {
  // check nodes and remaining edges after delEdge test
  checkGraphAfterDelEdges();

  // delete remaining nodes
  graph.delAllNodes();

  // check number of nodes
  CPPUNIT_ASSERT(graph.numberOfNodes() == 0);
  CPPUNIT_ASSERT(graph.nodes().empty());

  // check number of edges
  CPPUNIT_ASSERT(graph.numberOfEdges() == 0);
  CPPUNIT_ASSERT(graph.edges().empty());
}

//==========================================================
void VectorGraphTest::testAddDelEdges() {
  // check that graph is empty
  CPPUNIT_ASSERT(graph.numberOfEdges() == 0);
  CPPUNIT_ASSERT(graph.numberOfNodes() == 0);

  // populate graph with nodes only
  populateGraph(true);

  // check nodes population
  checkCreatedGraph(true);

  // add direct edges to nodes[0]
  std::vector<std::pair<node, node> > ends;
  ends.reserve(NB_NODES - 1);
  for (unsigned int i = 1; i < NB_NODES; ++i)
    ends.push_back(std::make_pair(nodes[0], nodes[i]));

  edges.clear();
  graph.addEdges(ends, &edges);

  CPPUNIT_ASSERT(graph.numberOfEdges() == NB_NODES - 1);

  // check edges
  const std::vector<edge> &nEdges = graph.star(nodes[0]);
  CPPUNIT_ASSERT(nEdges.size() == NB_NODES - 1);
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (OMP_ITER_TYPE i = 0; i < NB_NODES - 1; ++i) {
    edge e = nEdges[i];
    CPPUNIT_ASSERT(e == edges[i]);
    // check ends
    std::pair<node, node> ends = graph.ends(e);
    CPPUNIT_ASSERT(ends.first == nodes[0]);
    CPPUNIT_ASSERT(ends.second == nodes[i + 1]);
    CPPUNIT_ASSERT(graph.existEdge(nodes[0], nodes[i + 1], true) == e);
    CPPUNIT_ASSERT(graph.existEdge(nodes[i + 1], nodes[0], false) == e);
    // check edge source
    CPPUNIT_ASSERT(graph.source(e) == nodes[0]);
    // check edge target
    CPPUNIT_ASSERT(graph.target(e) == nodes[i + 1]);
    // check opposite
    CPPUNIT_ASSERT(graph.opposite(e, nodes[0]) == nodes[i + 1]);
    CPPUNIT_ASSERT(graph.opposite(e, nodes[i + 1]) == nodes[0]);
  }

  // check edges per node
  // check degree
  CPPUNIT_ASSERT(graph.deg(nodes[0]) == NB_NODES - 1);
  // check out degree
  CPPUNIT_ASSERT(graph.outdeg(nodes[0]) == NB_NODES - 1);
  // check in degree
  CPPUNIT_ASSERT(graph.indeg(nodes[0]) == 0);

  const std::vector<node> &nNodes = graph.adj(nodes[0]);
  CPPUNIT_ASSERT(nNodes.size() == NB_NODES - 1);
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (OMP_ITER_TYPE i = 0; i < NB_NODES - 1; ++i) {
    node n = nNodes[i];
    CPPUNIT_ASSERT(n == nodes[i + 1]);
    // check degree
    CPPUNIT_ASSERT(graph.deg(n) == 1);
    // check out degree
    CPPUNIT_ASSERT(graph.outdeg(n) == 0);
    // check in degree
    CPPUNIT_ASSERT(graph.indeg(n) == 1);

    // check out edges
    Iterator<edge> *ite = graph.getOutEdges(n);
    CPPUNIT_ASSERT(ite->hasNext() == false);
    delete ite;

    // check in edges
    ite = graph.getInEdges(n);
    CPPUNIT_ASSERT(ite->hasNext());
    edge e = ite->next();
    CPPUNIT_ASSERT(e == edges[i]);
    CPPUNIT_ASSERT(ite->hasNext() == false);
    delete ite;
  }

  // swap order of nodes[0] edges
  std::sort(edges.begin(), edges.end(), std::greater<edge>());
// check edge ordering
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (OMP_ITER_TYPE i = 0; i < NB_NODES - 2; ++i)
    CPPUNIT_ASSERT(edges[i] > edges[i + 1]);

  // swap edges of nodes[0]
  graph.setEdgeOrder(nodes[0], edges);

  // check edges
  const std::vector<edge> &n0Edges = graph.star(nodes[0]);
  CPPUNIT_ASSERT(n0Edges.size() == NB_NODES - 1);
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (OMP_ITER_TYPE i = 0; i < NB_NODES - 1; ++i) {
    edge e = n0Edges[i];
    CPPUNIT_ASSERT(e == edges[i]);
    // check ends
    std::pair<node, node> ends = graph.ends(e);
    CPPUNIT_ASSERT(ends.first == nodes[0]);
    CPPUNIT_ASSERT(ends.second == nodes[NB_NODES - i - 1]);
    CPPUNIT_ASSERT(graph.existEdge(nodes[0], nodes[NB_NODES - i - 1], true) == e);
    CPPUNIT_ASSERT(graph.existEdge(nodes[NB_NODES - i - 1], nodes[0], false) == e);
    // check edge source
    CPPUNIT_ASSERT(graph.source(e) == nodes[0]);
    // check edge target
    CPPUNIT_ASSERT(graph.target(e) == nodes[NB_NODES - i - 1]);
    // check opposite
    CPPUNIT_ASSERT(graph.opposite(e, nodes[0]) == nodes[NB_NODES - i - 1]);
    CPPUNIT_ASSERT(graph.opposite(e, nodes[NB_NODES - i - 1]) == nodes[0]);
  }

  // check edges per node
  // check degree
  CPPUNIT_ASSERT(graph.deg(nodes[0]) == NB_NODES - 1);
  // check out degree
  CPPUNIT_ASSERT(graph.outdeg(nodes[0]) == NB_NODES - 1);
  // check in degree
  CPPUNIT_ASSERT(graph.indeg(nodes[0]) == 0);
  // nNodes = graph.adj(nodes[0]);
  CPPUNIT_ASSERT(nNodes.size() == NB_NODES - 1);
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (OMP_ITER_TYPE i = 0; i < NB_NODES - 1; ++i) {
    node n = nNodes[i];
    CPPUNIT_ASSERT(n == nodes[NB_NODES - i - 1]);
    // check degree
    CPPUNIT_ASSERT(graph.deg(n) == 1);
    // check out degree
    CPPUNIT_ASSERT(graph.outdeg(n) == 0);
    // check in degree
    CPPUNIT_ASSERT(graph.indeg(n) == 1);

    // check out edges
    Iterator<edge> *ite = graph.getOutEdges(n);
    CPPUNIT_ASSERT(ite->hasNext() == false);
    delete ite;

    // check in edges
    ite = graph.getInEdges(n);
    CPPUNIT_ASSERT(ite->hasNext());
    CPPUNIT_ASSERT(ite->next() == edges[i]);
    CPPUNIT_ASSERT(ite->hasNext() == false);
    delete ite;
  }

  // delete all edges of nodes[0]
  graph.delEdges(nodes[0]);
  CPPUNIT_ASSERT(graph.numberOfEdges() == 0);

  // check edges
  Iterator<edge> *ite = graph.getEdges();
  CPPUNIT_ASSERT(ite->hasNext() == false);
  delete ite;

  // check edges per node
  // check degree
  CPPUNIT_ASSERT(graph.deg(nodes[0]) == 0);
  // check out degree
  CPPUNIT_ASSERT(graph.outdeg(nodes[0]) == 0);
  // check in degree
  CPPUNIT_ASSERT(graph.indeg(nodes[0]) == 0);
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (OMP_ITER_TYPE i = 1; i < NB_NODES; ++i) {
    node n = nodes[i];
    // check degree
    CPPUNIT_ASSERT(graph.deg(n) == 0);
    // check out degree
    CPPUNIT_ASSERT(graph.outdeg(n) == 0);
    // check in degree
    CPPUNIT_ASSERT(graph.indeg(n) == 0);

    // check out edges
    Iterator<edge> *ite = graph.getOutEdges(n);
    CPPUNIT_ASSERT(ite->hasNext() == false);
    delete ite;

    // check in edges
    ite = graph.getInEdges(n);
    CPPUNIT_ASSERT(ite->hasNext() == false);
    delete ite;
  }

  // finally clear all nodes
  graph.delAllNodes();
  CPPUNIT_ASSERT(graph.numberOfNodes() == 0);
}

//==========================================================
void checkGraphAfterReverseEdges() {
  // check nodes
  checkCreatedGraph(true);

  // check edges
  const std::vector<edge> &nEdges = graph.edges();
  CPPUNIT_ASSERT(nEdges.size() == NB_NODES);
#ifdef _OPENMP
#pragma omp parallel for
#endif
  for (OMP_ITER_TYPE i = 0; i < NB_NODES; ++i) {
    edge e = nEdges[i];
    CPPUNIT_ASSERT(e == edges[i]);

    // check ends
    std::pair<node, node> ends = graph.ends(e);
    CPPUNIT_ASSERT(ends.second == nodes[i]);
    if (i < NB_NODES - 1) {
      CPPUNIT_ASSERT(ends.first == nodes[i + 1]);
      CPPUNIT_ASSERT(graph.existEdge(nodes[i + 1], nodes[i], true) == e);
      CPPUNIT_ASSERT(graph.existEdge(nodes[i], nodes[i + 1], false) == e);
    } else {
      CPPUNIT_ASSERT(ends.first == nodes[0]);
      CPPUNIT_ASSERT(graph.existEdge(nodes[0], nodes[i], true) == e);
      CPPUNIT_ASSERT(graph.existEdge(nodes[i], nodes[0], false) == e);
    }
    // check edge source
    CPPUNIT_ASSERT(graph.target(e) == nodes[i]);
    // check edge target
    if (i < NB_NODES - 1)
      CPPUNIT_ASSERT(graph.source(e) == nodes[i + 1]);
    else
      CPPUNIT_ASSERT(graph.source(e) == nodes[0]);
    // check opposite
    if (i < NB_NODES - 1) {
      CPPUNIT_ASSERT(graph.opposite(e, nodes[i]) == nodes[i + 1]);
      CPPUNIT_ASSERT(graph.opposite(e, nodes[i + 1]) == nodes[i]);
    } else {
      CPPUNIT_ASSERT(graph.opposite(e, nodes[i]) == nodes[0]);
      CPPUNIT_ASSERT(graph.opposite(e, nodes[0]) == nodes[i]);
    }
  }
}

void VectorGraphTest::testReverseEdges() {
  // check that graph is empty
  CPPUNIT_ASSERT(graph.numberOfEdges() == 0);
  CPPUNIT_ASSERT(graph.numberOfNodes() == 0);

  // populate graph with nodes only
  populateGraph();

  // check nodes population
  checkCreatedGraph();

  // reverse all edges
  Iterator<edge> *ite = graph.getEdges();
  while (ite->hasNext()) {
    edge e = ite->next();
    std::pair<node, node> ends = graph.ends(e);
    unsigned int src_outdeg = graph.outdeg(ends.first);
    unsigned int tgt_outdeg = graph.outdeg(ends.second);
    graph.reverse(e);
    // check out degrees
    CPPUNIT_ASSERT(graph.outdeg(ends.second) - tgt_outdeg == 1);
    CPPUNIT_ASSERT(src_outdeg - graph.outdeg(ends.first) == 1);
  }
  delete ite;
  // check graph
  checkGraphAfterReverseEdges();
}

//==========================================================
void VectorGraphTest::testSetSourcesAndTargets() {
  // check graph is in afterReverseEdges state
  checkGraphAfterReverseEdges();

  // reverse each edge using setSource and setTarget
  Iterator<edge> *ite = graph.getEdges();
  while (ite->hasNext()) {
    edge e = ite->next();
    std::pair<node, node> ends = graph.ends(e);
    unsigned int src_outdeg = graph.outdeg(ends.first);
    unsigned int tgt_outdeg = graph.outdeg(ends.second);
    graph.setSource(e, ends.second);
    CPPUNIT_ASSERT(graph.outdeg(ends.first) == src_outdeg - 1);
    CPPUNIT_ASSERT(graph.outdeg(ends.second) == tgt_outdeg + 1);
    graph.setTarget(e, ends.first);
    CPPUNIT_ASSERT(graph.outdeg(ends.first) == src_outdeg - 1);
    CPPUNIT_ASSERT(graph.outdeg(ends.second) == tgt_outdeg + 1);
  }
  delete ite;

  // check nodes and edges
  checkCreatedGraph();
}

//==========================================================
void VectorGraphTest::testSetEnds() {
  // check nodes and edges
  checkCreatedGraph();

  // reverse each edge using setEnds
  Iterator<edge> *ite = graph.getEdges();
  while (ite->hasNext()) {
    edge e = ite->next();
    std::pair<node, node> ends = graph.ends(e);
    graph.setEnds(e, ends.second, ends.first);
  }
  delete ite;

  // check graph is in afterReverseEdges state
  checkGraphAfterReverseEdges();
}

//==========================================================
void VectorGraphTest::testMoreSetEnds() {
  // check graph is in afterReverseEdges state
  checkGraphAfterReverseEdges();

  // set ends for all edges
  unsigned int i = 0;
  Iterator<edge> *ite = graph.getEdges();
  while (ite->hasNext()) {
    CPPUNIT_ASSERT(ite->next() == edges[i]);
    if (i < NB_NODES - 1)
      graph.setEnds(edges[i], nodes[NB_NODES - i - 2], nodes[NB_NODES - i - 1]);
    else
      graph.setEnds(edges[i], nodes[NB_NODES - 1], nodes[0]);
    ++i;
  }

  // check edges
  i = 0;
  ite = graph.getEdges();
  while (ite->hasNext()) {
    CPPUNIT_ASSERT(ite->next() == edges[i]);

    // check ends
    std::pair<node, node> ends = graph.ends(edges[i]);
    CPPUNIT_ASSERT(ends.second == nodes[NB_NODES - i - 1]);
    if (i < NB_NODES - 1) {
      CPPUNIT_ASSERT(ends.first == nodes[NB_NODES - i - 2]);
      CPPUNIT_ASSERT(graph.existEdge(nodes[NB_NODES - i - 2], nodes[NB_NODES - i - 1], true) ==
                     edges[i]);
      CPPUNIT_ASSERT(graph.existEdge(nodes[NB_NODES - i - 1], nodes[NB_NODES - i - 2], false) ==
                     edges[i]);
    } else {
      CPPUNIT_ASSERT(ends.first == nodes[NB_NODES - 1]);
      CPPUNIT_ASSERT(graph.existEdge(nodes[NB_NODES - 1], nodes[0], true) == edges[i]);
      CPPUNIT_ASSERT(graph.existEdge(nodes[0], nodes[NB_NODES - 1], false) == edges[i]);
    }
    // check edge target
    CPPUNIT_ASSERT(graph.target(edges[i]) == nodes[NB_NODES - i - 1]);
    // check edge source
    if (i < NB_NODES - 1)
      CPPUNIT_ASSERT(graph.source(edges[i]) == nodes[NB_NODES - i - 2]);
    else
      CPPUNIT_ASSERT(graph.source(edges[i]) == nodes[NB_NODES - 1]);
    // check opposite
    if (i < NB_NODES - 1) {
      CPPUNIT_ASSERT(graph.opposite(edges[i], nodes[NB_NODES - i - 1]) == nodes[NB_NODES - i - 2]);
      CPPUNIT_ASSERT(graph.opposite(edges[i], nodes[NB_NODES - i - 2]) == nodes[NB_NODES - i - 1]);
    } else {
      CPPUNIT_ASSERT(graph.opposite(edges[i], nodes[NB_NODES - 1]) == nodes[0]);
      CPPUNIT_ASSERT(graph.opposite(edges[i], nodes[0]) == nodes[NB_NODES - 1]);
    }
    ++i;
  }
  delete ite;
}
}
