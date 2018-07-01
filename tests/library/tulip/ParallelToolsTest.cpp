#include <tuple>

#include <tulip/GraphParallelTools.h>
#include <tulip/StaticProperty.h>

#include "ParallelToolsTest.h"

const unsigned int NB_NODES = 10000;
const unsigned int NB_EDGES = 20000;

CPPUNIT_TEST_SUITE_REGISTRATION(ParallelToolsTest);

void ParallelToolsTest::setUp() {
  _graph = tlp::newGraph();
  _graph->addNodes(NB_NODES);
  for (unsigned int i = 0; i < NB_EDGES; ++i) {
    std::ignore = i;
    _graph->addEdge(_graph->getRandomNode(), _graph->getRandomNode());
  }
  for (unsigned int i = 0; i < NB_NODES / 3; ++i) {
    _graph->delNode(_graph->getRandomNode());
  }
}

void ParallelToolsTest::tearDown() {
  delete _graph;
}

void ParallelToolsTest::testParallelMapIndices() {
  std::vector<unsigned int> v(100);
  tlp::TLP_PARALLEL_MAP_INDICES(v.size(), [&](unsigned int i) { v[i] = 2 * i; });
  for (unsigned int i = 0; i < v.size(); ++i) {
    CPPUNIT_ASSERT_EQUAL(v[i], 2 * i);
  }
}

void ParallelToolsTest::testParallelMapNodes() {
  tlp::NodeStaticProperty<unsigned int> deg(_graph);
  TLP_PARALLEL_MAP_NODES(_graph, [&](const tlp::node &n) { deg[n] = _graph->deg(n); });
  for (auto n : _graph->nodes()) {
    CPPUNIT_ASSERT_EQUAL(deg[n], _graph->deg(n));
  }
}

void ParallelToolsTest::testParallelMapNodesAndIndices() {
  std::vector<tlp::node> nodes(_graph->numberOfNodes());
  TLP_PARALLEL_MAP_NODES_AND_INDICES(_graph,
                                     [&](const tlp::node &n, unsigned int i) { nodes[i] = n; });
  unsigned int i = 0;
  for (auto n : _graph->nodes()) {
    CPPUNIT_ASSERT_EQUAL(n, nodes[i++]);
  }
}

void ParallelToolsTest::testParallelMapEdges() {
  tlp::EdgeStaticProperty<int> selfLoop(_graph);
  TLP_PARALLEL_MAP_EDGES(
      _graph, [&](const tlp::edge &e) { selfLoop[e] = _graph->source(e) == _graph->target(e); });
  for (auto e : _graph->edges()) {
    CPPUNIT_ASSERT_EQUAL(selfLoop[e], int(_graph->source(e) == _graph->target(e)));
  }
}

void ParallelToolsTest::testParallelMapEdgesAndIndices() {
  std::vector<tlp::edge> edges(_graph->numberOfEdges());
  TLP_PARALLEL_MAP_EDGES_AND_INDICES(_graph,
                                     [&](const tlp::edge &e, unsigned int i) { edges[i] = e; });
  unsigned int i = 0;
  for (auto e : _graph->edges()) {
    CPPUNIT_ASSERT_EQUAL(e, edges[i++]);
  }
}

void ParallelToolsTest::testCriticalSection() {
  unsigned int maxDegPar = 0;
  TLP_PARALLEL_MAP_NODES(_graph, [&](const tlp::node &n) {
    unsigned int deg = _graph->deg(n);
    TLP_LOCK_SECTION(maxDeg) {
      maxDegPar = std::max(deg, maxDegPar);
    }
    TLP_UNLOCK_SECTION(maxDeg);
  });
  unsigned int maxDegSeq = 0;
  for (auto n : _graph->nodes()) {
    maxDegSeq = std::max(maxDegSeq, _graph->deg(n));
  }
  CPPUNIT_ASSERT_EQUAL(maxDegPar, maxDegSeq);
}

void ParallelToolsTest::testNumberOfThreads() {
  const unsigned int vSize = 100;
  const unsigned int nbThreads = 32;
  tlp::ThreadManager::setNumberOfThreads(nbThreads);
  std::vector<unsigned int> tids(vSize);
  tlp::TLP_PARALLEL_MAP_INDICES(
      nbThreads, [&](unsigned int i) { tids[i] = tlp::ThreadManager::getThreadNumber(); });

  for (unsigned int tid : tids) {
    CPPUNIT_ASSERT(tid <= tlp::ThreadManager::getNumberOfThreads() - 1);
  }
}
