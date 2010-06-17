/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include <tulip/PluginLoaderTxt.h>

using namespace std;
using namespace tlp;

#include <cppunit/extensions/HelperMacros.h>
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
void BasicPluginsTest::testArityMetric() {
  bool result = computeProperty<DoubleProperty>("Degree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testBetweennessCentrality() {
  bool result = computeProperty<DoubleProperty>("Betweenness Centrality");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testBiconnectedComponent() {
  bool result = computeProperty<DoubleProperty>("Biconnected Component");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testClusterMetric() {
  bool result = computeProperty<DoubleProperty>("Cluster");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testConnectedComponent() {
  bool result = computeProperty<DoubleProperty>("Connected Component");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testDagLevelMetric() {
  bool result = computeProperty<DoubleProperty>("Dag Level");
  CPPUNIT_ASSERT(result == false);
  graph->clear();
  result = computeProperty<DoubleProperty>("Dag Level", "Random General Tree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testDepthMetric() {
  bool result = computeProperty<DoubleProperty>("Depth");
  CPPUNIT_ASSERT(result == false);
  graph->clear();
  result = computeProperty<DoubleProperty>("Depth", "Random General Tree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testEccentricity() {
  bool result = computeProperty<DoubleProperty>("Eccentricity");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testIdMetric() {
  bool result = computeProperty<DoubleProperty>("Id");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testLeafMetric() {
  bool result = computeProperty<DoubleProperty>("Leaf");
  CPPUNIT_ASSERT(result == false);
  graph->clear();
  result = computeProperty<DoubleProperty>("Leaf", "Random General Tree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testNodeMetric() {
  bool result = computeProperty<DoubleProperty>("Node");
  CPPUNIT_ASSERT(result == false);
  graph->clear();
  result = computeProperty<DoubleProperty>("Node", "Random General Tree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testPathLengthMetric() {
  bool result = computeProperty<DoubleProperty>("Path Length");
  CPPUNIT_ASSERT(result == false);
  graph->clear();
  result = computeProperty<DoubleProperty>("Path Length",
					   "Random General Tree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testRandomMetric() {
  bool result = computeProperty<DoubleProperty>("Random");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testStrahlerMetric() {
  bool result = computeProperty<DoubleProperty>("Strahler");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testStrengthMetric() {
  bool result = computeProperty<DoubleProperty>("Strength");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testStrongComponent() {
  bool result = computeProperty<DoubleProperty>("Strongly Connected Component");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testMetricColorMapping() {
  initializeGraph("Planar Graph");
  DoubleProperty metric(graph);
  string errorMsg;
  DataSet ds;
  bool result = graph->computeProperty("Degree", &metric, errorMsg);
  CPPUNIT_ASSERT(result);

  ColorProperty color(graph);
  ds.set("property", &metric);
  result = graph->computeProperty("Metric Mapping", &color,
				  errorMsg, NULL, &ds); 
  CPPUNIT_ASSERT(result);
}

//==========================================================
void BasicPluginsTest::testEnumeratedColorValuesMapping() {
  initializeGraph("Random General Tree");
  DoubleProperty metric(graph);
  string errorMsg;
  bool result = graph->computeProperty("Degree", &metric, errorMsg);
  CPPUNIT_ASSERT(result);

  /*ColorProperty color(graph);
  ds.set("Property", &metric);
  result = graph->computeProperty("Enumerated Values Mapping",
				  &color, errorMsg); 
				  CPPUNIT_ASSERT(result);*/
}
//==========================================================
void BasicPluginsTest::testBubbleTree() {
  bool result = computeProperty<LayoutProperty>("Bubble Tree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testCircular() {
  bool result = computeProperty<LayoutProperty>("Circular");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testConeTreeExtended() {
  bool result = computeProperty<LayoutProperty>("Cone Tree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testConnectedComponentPacking() {
  bool result = computeProperty<LayoutProperty>("Connected Component Packing");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testDendrogram() {
  initializeGraph("Planar Graph");
  DataSet ds;
  SizeProperty size(graph);
  ds.set("node size", &size);
  LayoutProperty layout(graph);
  string errorMsg;
  bool result =
    graph->computeProperty("Dendrogram", &layout, errorMsg, NULL, &ds);
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testGEMLayout() {
  bool result = computeProperty<LayoutProperty>("GEM (Frick)");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testHierarchicalGraph() {
  bool result = computeProperty<LayoutProperty>("Hierarchical Graph");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testImprovedWalker() {
  bool result = computeProperty<LayoutProperty>("Improved Walker");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testMixedModel() {
  initializeGraph("Planar Graph");
  DataSet ds;
  SizeProperty size(graph);
  ds.set("node size", &size);
  LayoutProperty layout(graph);
  string errorMsg;
  bool result =
    graph->computeProperty("Mixed Model", &layout, errorMsg, NULL, &ds);
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testRandomLayout() {
  bool result = computeProperty<LayoutProperty>("Random");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testSquarifiedTreeMap() {
  initializeGraph("Random General Tree");
  DoubleProperty metric(graph);
  string errorMsg;
  DataSet ds;
  bool result = graph->computeProperty("Degree", &metric, errorMsg);
  CPPUNIT_ASSERT(result);

  LayoutProperty layout(graph);
  ds.set("metric", &metric);
  result = graph->computeProperty("Squarified Tree Map", &layout,
				  errorMsg, NULL, &ds); 
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testTreeLeaf() {
  initializeGraph("Planar Graph");
  DataSet ds;
  SizeProperty size(graph);
  ds.set("node size", &size);
  LayoutProperty layout(graph);
  string errorMsg;
  bool result =
    graph->computeProperty("Tree Leaf", &layout, errorMsg, NULL, &ds);
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testTreeMap() {
}
//==========================================================
void BasicPluginsTest::testTreeRadial() {
  bool result = computeProperty<LayoutProperty>("Tree Radial");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testTreeReingoldAndTilfordExtended() {
  bool result = computeProperty<LayoutProperty>("Hierarchical Tree (R-T Extended)");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testTutte() {
  bool result = computeProperty<LayoutProperty>("3-Connected (Tutte)",
						"Complete General Graph");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testInducedSubGraphSelection() {
  bool result = computeProperty<BooleanProperty>("Induced Sub-Graph");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testKruskal() {
  BooleanProperty selection(graph);
  bool result = computeProperty<BooleanProperty>("Kruskal", "Planar Graph",
                                                 &selection);
  CPPUNIT_ASSERT(result);
  node n;
  forEach(n, graph->getNodes()) {
    CPPUNIT_ASSERT(selection.getNodeValue(n));
  }
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
  initializeGraph("Planar Graph");
  DoubleProperty metric(graph);
  string errorMsg;
  DataSet ds;
  bool result = graph->computeProperty("Degree", &metric, errorMsg);
  CPPUNIT_ASSERT(result);
  ds.set("Property", &metric);
  result = tlp::applyAlgorithm(graph, errorMsg, &ds, "Equal Value", NULL); 
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testHierarchicalClustering() {
  initializeGraph("Planar Graph");
  DoubleProperty* metric = graph->getProperty<DoubleProperty>("viewMetric");
  string errorMsg;
  bool result = graph->computeProperty("Degree", metric, errorMsg);
  CPPUNIT_ASSERT(result);
  result = tlp::applyAlgorithm(graph, errorMsg, NULL, "Hierarchical", NULL); 
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
  result = tlp::applyAlgorithm(graph, errorMsg, &ds, "Equal Value", NULL); 
  CPPUNIT_ASSERT(result);
  result = tlp::applyAlgorithm(graph, errorMsg, NULL,
			       "Quotient Clustering", NULL); 
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicPluginsTest::testStrengthClustering() {
  initializeGraph("Planar Graph");
  string errorMsg;
  bool result = tlp::applyAlgorithm(graph, errorMsg, NULL,
				       "Strength Clustering", NULL); 
  CPPUNIT_ASSERT(result);
}

CppUnit::Test * BasicPluginsTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("Tulip lib : Plugins mechanism" );
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("Import CompleteGraph", &BasicPluginsTest::testImportCompleteGraph));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("Import CompleteTree", &BasicPluginsTest::testImportCompleteTree));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("Import RandomGraph", &BasicPluginsTest::testImportRandomGraph));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("Import RandomTree", &BasicPluginsTest::testImportRandomTree));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("Import RandomTreeGeneral", &BasicPluginsTest::testImportRandomTreeGeneral));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("Import RandomSimpleGraph", &BasicPluginsTest::testImportRandomSimpleGraph));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("Import PlanarGraph", &BasicPluginsTest::testImportPlanarGraph));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("Import Grid", &BasicPluginsTest::testImportGrid));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("Import GridApproximation", &BasicPluginsTest::testImportGridApproximation));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("Import Dot", &BasicPluginsTest::testImportDot));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("Import Gml", &BasicPluginsTest::testImportGml));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("Import FileSystem", &BasicPluginsTest::testImportFileSystem));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("Import AdjacencyMatrix", &BasicPluginsTest::testImportAdjacencyMatrix));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("ArityMetric", &BasicPluginsTest::testArityMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("BetweennessCentrality", &BasicPluginsTest::testBetweennessCentrality));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("BiconnectedComponent", &BasicPluginsTest::testBiconnectedComponent));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("ClusterMetric", &BasicPluginsTest::testClusterMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("ConnectedComponent", &BasicPluginsTest::testConnectedComponent));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("DagLevelMetric", &BasicPluginsTest::testDagLevelMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("DepthMetric", &BasicPluginsTest::testDepthMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("Eccentricity", &BasicPluginsTest::testEccentricity));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("IdMetric", &BasicPluginsTest::testIdMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("LeafMetric", &BasicPluginsTest::testLeafMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("NodeMetric", &BasicPluginsTest::testNodeMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("PathLengthMetric", &BasicPluginsTest::testPathLengthMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("Random Metric", &BasicPluginsTest::testRandomMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("StrahlerMetric", &BasicPluginsTest::testStrahlerMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("StrengthMetric", &BasicPluginsTest::testStrengthMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("StrongComponent", &BasicPluginsTest::testStrongComponent));;
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("MetricColorMapping", &BasicPluginsTest::testMetricColorMapping));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("EnumeratedColorValuesMapping", &BasicPluginsTest::testEnumeratedColorValuesMapping));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("BubbleTree", &BasicPluginsTest::testBubbleTree));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("Circular", &BasicPluginsTest::testCircular));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("ConeTreeExtended", &BasicPluginsTest::testConeTreeExtended));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("ConnectedComponentPacking", &BasicPluginsTest::testConnectedComponentPacking));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("Dendrogram", &BasicPluginsTest::testDendrogram));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("GEMLayout", &BasicPluginsTest::testGEMLayout));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("HierarchicalGraph", &BasicPluginsTest::testHierarchicalGraph));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("ImprovedWalker", &BasicPluginsTest::testImprovedWalker));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("MixedModel", &BasicPluginsTest::testMixedModel));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("Random Layout", &BasicPluginsTest::testRandomLayout));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("SquarifiedTreeMap", &BasicPluginsTest::testSquarifiedTreeMap));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("TreeLeaf", &BasicPluginsTest::testTreeLeaf));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("TreeMap", &BasicPluginsTest::testTreeMap));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("TreeRadial", &BasicPluginsTest::testTreeRadial));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("TreeReingoldAndTilfordExtended", &BasicPluginsTest::testTreeReingoldAndTilfordExtended));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("Tutte", &BasicPluginsTest::testTutte));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("InducedSubGraphSelection", &BasicPluginsTest::testInducedSubGraphSelection));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("Kruskal", &BasicPluginsTest::testKruskal));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("LoopSelection", &BasicPluginsTest::testLoopSelection));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("MultipleEdgeSelection", &BasicPluginsTest::testMultipleEdgeSelection));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("ReachableSubGraphSelection", &BasicPluginsTest::testReachableSubGraphSelection));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("SpanningDagSelection", &BasicPluginsTest::testSpanningDagSelection));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("SpanningTreeSelection", &BasicPluginsTest::testSpanningTreeSelection));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("AutoSize", &BasicPluginsTest::testAutoSize));
  //suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("FitToLabel", &BasicPluginsTest::testFitToLabel));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("MetricSizeMapping", &BasicPluginsTest::testMetricSizeMapping));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("EqualValueClustering", &BasicPluginsTest::testEqualValueClustering));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("HierarchicalClustering", &BasicPluginsTest::testHierarchicalClustering));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("QuotientClustering", &BasicPluginsTest::testQuotientClustering));
  suiteOfTests->addTest(new CppUnit::TestCaller<BasicPluginsTest>("StrengthClustering", &BasicPluginsTest::testStrengthClustering));

  return suiteOfTests;
}
