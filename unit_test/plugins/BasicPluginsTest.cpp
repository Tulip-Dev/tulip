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
#include <sstream>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include "BasicPluginsTest.h"
#include <tulip/TlpTools.h>
#include <tulip/ForEach.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>

using namespace std;
using namespace tlp;

CPPUNIT_TEST_SUITE_REGISTRATION( BasicPluginsTest );

//==========================================================
void BasicPluginsTest::setUp() {
  graph = tlp::newGraph();
}
//==========================================================
void BasicPluginsTest::tearDown() {
  delete graph;
}
//==========================================================
void BasicPluginsTest::initializeGraph(const string& type) {
  DataSet ds;
  Graph* g = importGraph(type, ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
template<typename PropType>
bool BasicPluginsTest::computeProperty(const std::string &algorithm,
                                       const std::string & graphType,
                                       PropType* prop) {
  initializeGraph(graphType);
  bool deleteProp = prop == NULL;

  if (prop == NULL)
    prop = new PropType(graph);

  string errorMsg;
  DataSet ds;
  bool result = graph->computeProperty(algorithm, prop, errorMsg);

  if (deleteProp)
    delete prop;

  return result;
}
//==========================================================
void BasicPluginsTest::testImportCompleteGraph() {
  DataSet ds;
  Graph* g = importGraph("Complete General Graph", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void BasicPluginsTest::testImportCompleteTree() {
  DataSet ds;
  Graph* g = importGraph("Complete Tree", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void BasicPluginsTest::testImportRandomGraph() {
  DataSet ds;
  Graph* g = importGraph("Random General Graph", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void BasicPluginsTest::testImportRandomTree() {
  DataSet ds;
  Graph* g = importGraph("Uniform Random Binary Tree", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void BasicPluginsTest::testImportRandomTreeGeneral() {
  DataSet ds;
  Graph* g = importGraph("Random General Tree", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void BasicPluginsTest::testImportRandomSimpleGraph() {
  DataSet ds;
  Graph* g = importGraph("Random Simple Graph", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void BasicPluginsTest::testImportPlanarGraph() {
  DataSet ds;
  Graph* g = importGraph("Planar Graph", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void BasicPluginsTest::testImportGrid() {
  DataSet ds;
  Graph* g = importGraph("Grid", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void BasicPluginsTest::testImportGridApproximation() {
  DataSet ds;
  Graph* g = importGraph("Grid Approximation", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void BasicPluginsTest::testImportDot() {
  DataSet ds;
  ds.set("file::filename", string("toto.dot"));
  Graph* g = importGraph("dot (graphviz)", ds, NULL, graph);
  CPPUNIT_ASSERT(g == NULL);
  ds.set("file::filename", string("graph.dot"));
  g = importGraph("dot (graphviz)", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void BasicPluginsTest::testImportGml() {
  DataSet ds;
  ds.set("file::filename", string("toto.gml"));
  Graph* g = importGraph("GML", ds, NULL, graph);
  CPPUNIT_ASSERT(g == NULL);
  ds.set("file::filename", string("CMPb.gml"));
  g = importGraph("GML", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void BasicPluginsTest::testExportGml() {
  DataSet ds;
  Graph* g = importGraph("Planar Graph", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
  std::stringstream os;
  CPPUNIT_ASSERT(exportGraph(graph, os, "GML", ds, NULL));
}
//==========================================================
void BasicPluginsTest::testImportFileSystem() {
  DataSet ds;
  ds.set("dir::directory", string("toto"));
  Graph* g = importGraph("File System Directory", ds, NULL, graph);
  CPPUNIT_ASSERT(g == NULL);
  ds.set("dir::directory", string(".."));
  g = importGraph("File System Directory", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void BasicPluginsTest::testImportAdjacencyMatrix() {
  DataSet ds;
  ds.set("file::name", string("toto.txt"));
  Graph* g = importGraph("Adjacency Matrix", ds, NULL, graph);
  CPPUNIT_ASSERT(g == NULL);
  ds.set("file::name", string("adj_mat.txt"));
  g = importGraph("Adjacency Matrix", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void BasicPluginsTest::testMetricColorMapping() {
  initializeGraph("Planar Graph");
  DoubleProperty metric(graph);
  string errorMsg;
  bool result = graph->computeProperty("Degree", &metric, errorMsg);
  CPPUNIT_ASSERT(result);

  DataSet ds;
  ds.set("linear/uniform\nproperty", &metric);
  ColorProperty color(graph);
  result = graph->computeProperty("Color Mapping", &color,
                                  errorMsg, NULL, &ds);
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testLoopSelection() {
  BooleanProperty selection(graph);
  bool result = computeProperty<BooleanProperty>("Loop Selection",
                "Planar Graph",
                &selection);
  CPPUNIT_ASSERT(result);
  node n;
  forEach(n, graph->getNodes()) {
    CPPUNIT_ASSERT(selection.getNodeValue(n) == false);
  }
}
//==========================================================
void BasicPluginsTest::testMultipleEdgeSelection() {
  BooleanProperty selection(graph);
  bool result = computeProperty<BooleanProperty>("Multiple Edge",
                "Planar Graph",
                &selection);
  CPPUNIT_ASSERT(result);
  node n;
  forEach(n, graph->getNodes()) {
    CPPUNIT_ASSERT(selection.getNodeValue(n) == false);
  }
}
//==========================================================
void BasicPluginsTest::testReachableSubGraphSelection() {
  bool result = computeProperty<BooleanProperty>("Reachable Sub-Graph");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testSpanningDagSelection() {
  BooleanProperty selection(graph);
  bool result = computeProperty<BooleanProperty>("Spanning Dag",
                "Planar Graph",
                &selection);
  CPPUNIT_ASSERT(result);
  node n;
  forEach(n, graph->getNodes()) {
    CPPUNIT_ASSERT(selection.getNodeValue(n));
  }
}
//==========================================================
void BasicPluginsTest::testSpanningTreeSelection() {
  BooleanProperty selection(graph);
  bool result = computeProperty<BooleanProperty>("Spanning Forest",
                "Planar Graph",
                &selection);
  CPPUNIT_ASSERT(result);
  node n;
  forEach(n, graph->getNodes()) {
    CPPUNIT_ASSERT(selection.getNodeValue(n));
  }
}
//==========================================================
void BasicPluginsTest::testAutoSize() {
  bool result = computeProperty<SizeProperty>("Auto Sizing");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testFitToLabel() {
  StringProperty* label = graph->getProperty<StringProperty>("viewLabel");
  label->setAllNodeValue("toto");
  bool result = computeProperty<SizeProperty>("Fit to label");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testMetricSizeMapping() {
  initializeGraph("Planar Graph");
  DoubleProperty metric(graph);
  string errorMsg;
  DataSet ds;
  bool result = graph->computeProperty("Degree", &metric, errorMsg);
  CPPUNIT_ASSERT(result);

  SizeProperty size(graph);
  ds.set("property", &metric);
  result = graph->computeProperty("Metric Mapping", &size,
                                  errorMsg, NULL, &ds);
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testEqualValueClustering() {
  string errorMsg;
  DataSet ds;
  bool result;
  const std::string algorithmName = "Equal Value";

  // check minimum call to computeEqualValueClustering
  // with an empty graph
  graph = tlp::newGraph();

  DoubleProperty* metric = graph->getProperty<DoubleProperty>("metric");
  ds.set("Property", metric);

  result = graph->applyAlgorithm(algorithmName, errorMsg, &ds);
  CPPUNIT_ASSERT_MESSAGE(errorMsg, result);

  // fill graph & metric
  vector<node> nodes;
  vector<edge> edges;
  unsigned int NB_ADD  = 100;
  unsigned int EDGE_RATIO = 100;

  for (unsigned int i=0; i<NB_ADD; ++i) {
    nodes.push_back(graph->addNode());
    metric->setNodeValue(nodes[i], (double) (rand()%NB_ADD));
  }

  unsigned int NB_EDGES = EDGE_RATIO * NB_ADD;

  for (unsigned int i=0; i< NB_EDGES; ++i)
    graph->addEdge(nodes[rand()%NB_ADD], nodes[rand()%NB_ADD]);

  // check dcall to computeEqualValueClustering
  result = graph->applyAlgorithm(algorithmName, errorMsg, &ds);
  CPPUNIT_ASSERT_MESSAGE(errorMsg, result);

  graph->clear();

  PluginProgress* progress = new SimplePluginProgress();
  initializeGraph("Planar Graph");
  result = graph->computeProperty("Degree", metric, errorMsg, progress);
  CPPUNIT_ASSERT_MESSAGE(errorMsg, result);
  result = graph->applyAlgorithm(algorithmName, errorMsg, &ds);
  CPPUNIT_ASSERT_MESSAGE(errorMsg, result);
}
//==========================================================
void BasicPluginsTest::testHierarchicalClustering() {
  initializeGraph("Planar Graph");
  DoubleProperty* metric = graph->getProperty<DoubleProperty>("viewMetric");
  string errorMsg;
  bool result = graph->computeProperty("Degree", metric, errorMsg);
  CPPUNIT_ASSERT(result);
  result = graph->applyAlgorithm("Hierarchical", errorMsg);
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testQuotientClustering() {
  initializeGraph("Planar Graph");
  DoubleProperty metric(graph);
  string errorMsg;
  DataSet ds;
  bool result = graph->computeProperty("Degree", &metric, errorMsg);
  CPPUNIT_ASSERT(result);
  ds.set("Property", &metric);
  result = graph->applyAlgorithm("Equal Value", errorMsg, &ds);
  CPPUNIT_ASSERT(result);
  result = graph->applyAlgorithm("Quotient Clustering", errorMsg);
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testStrengthClustering() {
  initializeGraph("Planar Graph");
  string errorMsg;
  DoubleProperty metric(graph);
  DataSet ds;
  bool result = graph->computeProperty("Degree", &metric, errorMsg);
  CPPUNIT_ASSERT(result);
  ds.set("metric", &metric);
  DoubleProperty resultMetric(graph);
  result = graph->computeProperty("Strength Clustering", &resultMetric,
                                  errorMsg);
  CPPUNIT_ASSERT(result);
}
