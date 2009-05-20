#include <sstream>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include "PluginsTest.h">
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
CPPUNIT_TEST_SUITE_REGISTRATION( PluginsTest );

//==========================================================
void PluginsTest::setUp() {
  graph = tlp::newGraph();
}
//==========================================================
void PluginsTest::tearDown() {
  delete graph;
}
//==========================================================
void PluginsTest::testloadPlugin() {
  // plugin does not exist yet
  CPPUNIT_ASSERT(!tlp::BooleanProperty::factory->pluginExists("Test"));
  PluginLoaderTxt loader;
  tlp::loadPlugin("./testPlugin.so", &loader);
  // plugin should exist now
  CPPUNIT_ASSERT(tlp::BooleanProperty::factory->pluginExists("Test"));
  list<Dependency> deps = tlp::BooleanProperty::factory->getPluginDependencies("Test");
  // only one dependency (see testPlugin.cpp)
  CPPUNIT_ASSERT(deps.size() == 1);
  CPPUNIT_ASSERT(deps.front().factoryName == tlp::demangleTlpClassName(typeid(BooleanAlgorithm).name()));
  CPPUNIT_ASSERT(deps.front().pluginName == "Test");
}
//==========================================================
void PluginsTest::testCircularPlugin() {
  string name = "Test";
  string err = "Error";
  // ensure graph is not empty
  graph->addNode();
  tlp::BooleanProperty sel(graph);
  CPPUNIT_ASSERT(graph->computeProperty(name, &sel, err));
}
//==========================================================
void PluginsTest::testAncestorGraph() {
  string name = "Test";
  string err = "Error";
  tlp::loadPlugin("testPlugin2.so");
  Graph *child = graph->addSubGraph();
  Graph *child2 = graph->addSubGraph();
  // ensure child2 is not empty
  child2->addNode();
  Graph *child3 = child->addSubGraph();
  // ensure child3 is not empty
  child3->addNode();
  BooleanProperty sel(child);
  CPPUNIT_ASSERT(!graph->computeProperty(name, &sel, err));  
  CPPUNIT_ASSERT(!child2->computeProperty(name, &sel, err));  
  CPPUNIT_ASSERT(child3->computeProperty(name, &sel, err));
}
//==========================================================
void PluginsTest::initializeGraph(const string& type) {
  DataSet ds;
  Graph* g = importGraph(type, ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
template<typename PropType>
  bool PluginsTest::computeProperty(const std::string &algorithm,
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
void PluginsTest::testImportCompleteGraph() {
  DataSet ds;
  Graph* g = importGraph("Complete General Graph", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void PluginsTest::testImportCompleteTree() {
  DataSet ds;
  Graph* g = importGraph("Complete Tree", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void PluginsTest::testImportRandomGraph() {
  DataSet ds;
  Graph* g = importGraph("Random General Graph", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void PluginsTest::testImportRandomTree() {
  DataSet ds;
  Graph* g = importGraph("Uniform Random Binary Tree", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void PluginsTest::testImportRandomTreeGeneral() {
  DataSet ds;
  Graph* g = importGraph("Random General Tree", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void PluginsTest::testImportRandomSimpleGraph() {
  DataSet ds;
  Graph* g = importGraph("Random Simple Graph", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void PluginsTest::testImportPlanarGraph() {
  DataSet ds;
  Graph* g = importGraph("Planar Graph", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void PluginsTest::testImportGrid() {
  DataSet ds;
  Graph* g = importGraph("Grid", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void PluginsTest::testImportGridApproximation() {
  DataSet ds;
  Graph* g = importGraph("Grid Approximation", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
}
//==========================================================
void PluginsTest::testImportDot() {
  DataSet ds;
  ds.set("file::filename", string("toto.dot"));
  Graph* g = importGraph("dot (graphviz)", ds, NULL, graph);
  CPPUNIT_ASSERT(g == NULL);  
  ds.set("file::filename", string("graph.dot"));
  g = importGraph("dot (graphviz)", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);  
}
//==========================================================
void PluginsTest::testImportGml() {
  DataSet ds;
  ds.set("file::filename", string("toto.gml"));
  Graph* g = importGraph("GML", ds, NULL, graph);
  CPPUNIT_ASSERT(g == NULL);  
  ds.set("file::filename", string("CMPb.gml"));
  g = importGraph("GML", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);  
}
//==========================================================
void PluginsTest::testExportGml() {
  DataSet ds;
  Graph* g = importGraph("Planar Graph", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);
  std::stringstream os;
  CPPUNIT_ASSERT(exportGraph(graph, os, "GML", ds, NULL));
}
//==========================================================
void PluginsTest::testImportFileSystem() {
  DataSet ds;
  ds.set("dir::directory", string("toto"));
  Graph* g = importGraph("File System Directory", ds, NULL, graph);
  CPPUNIT_ASSERT(g == NULL);  
  ds.set("dir::directory", string(".."));
  g = importGraph("File System Directory", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);  
}
//==========================================================
void PluginsTest::testImportAdjacencyMatrix() {
  DataSet ds;
  ds.set("file::name", string("toto.txt"));
  Graph* g = importGraph("Adjacency Matrix", ds, NULL, graph);
  CPPUNIT_ASSERT(g == NULL);  
  ds.set("file::name", string("adj_mat.txt"));
  g = importGraph("Adjacency Matrix", ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);  
}
//==========================================================
void PluginsTest::testArityMetric() {
  bool result = computeProperty<DoubleProperty>("Degree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testBetweennessCentrality() {
  bool result = computeProperty<DoubleProperty>("Betweenness Centrality");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testBiconnectedComponent() {
  bool result = computeProperty<DoubleProperty>("Biconnected Component");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testClusterMetric() {
  bool result = computeProperty<DoubleProperty>("Cluster");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testConnectedComponent() {
  bool result = computeProperty<DoubleProperty>("Connected Component");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testDagLevelMetric() {
  bool result = computeProperty<DoubleProperty>("Dag Level");
  CPPUNIT_ASSERT(result == false);
  graph->clear();
  result = computeProperty<DoubleProperty>("Dag Level", "Random General Tree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testDepthMetric() {
  bool result = computeProperty<DoubleProperty>("Depth");
  CPPUNIT_ASSERT(result == false);
  graph->clear();
  result = computeProperty<DoubleProperty>("Depth", "Random General Tree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testEccentricity() {
  bool result = computeProperty<DoubleProperty>("Eccentricity");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testIdMetric() {
  bool result = computeProperty<DoubleProperty>("Id");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testLeafMetric() {
  bool result = computeProperty<DoubleProperty>("Leaf");
  CPPUNIT_ASSERT(result == false);
  graph->clear();
  result = computeProperty<DoubleProperty>("Leaf", "Random General Tree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testNodeMetric() {
  bool result = computeProperty<DoubleProperty>("Node");
  CPPUNIT_ASSERT(result == false);
  graph->clear();
  result = computeProperty<DoubleProperty>("Node", "Random General Tree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testPathLengthMetric() {
  bool result = computeProperty<DoubleProperty>("Path Length");
  CPPUNIT_ASSERT(result == false);
  graph->clear();
  result = computeProperty<DoubleProperty>("Path Length",
					   "Random General Tree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testRandomMetric() {
  bool result = computeProperty<DoubleProperty>("Random");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testStrahlerMetric() {
  bool result = computeProperty<DoubleProperty>("Strahler");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testStrengthMetric() {
  bool result = computeProperty<DoubleProperty>("Strength");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testStrongComponent() {
  bool result = computeProperty<DoubleProperty>("Strongly Connected Component");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testMetricColorMapping() {
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
void PluginsTest::testEnumeratedColorValuesMapping() {
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
void PluginsTest::testBubbleTree() {
  bool result = computeProperty<LayoutProperty>("Bubble Tree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testCircular() {
  bool result = computeProperty<LayoutProperty>("Circular");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testConeTreeExtended() {
  bool result = computeProperty<LayoutProperty>("Cone Tree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testConnectedComponentPacking() {
  bool result = computeProperty<LayoutProperty>("Connected Component Packing");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testDendrogram() {
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
void PluginsTest::testEmbedder() {
  bool result = computeProperty<LayoutProperty>("Embedder (HDE)");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testGEMLayout() {
  bool result = computeProperty<LayoutProperty>("GEM (Frick)");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testHierarchicalGraph() {
  bool result = computeProperty<LayoutProperty>("Hierarchical Graph");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testImprovedWalker() {
  bool result = computeProperty<LayoutProperty>("Improved Walker");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testMixedModel() {
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
void PluginsTest::testRandomLayout() {
  bool result = computeProperty<LayoutProperty>("Random");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testSquarifiedTreeMap() {
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
void PluginsTest::testTreeLeaf() {
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
void PluginsTest::testTreeMap() {
}
//==========================================================
void PluginsTest::testTreeRadial() {
  bool result = computeProperty<LayoutProperty>("Tree Radial");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testTreeReingoldAndTilfordExtended() {
  bool result = computeProperty<LayoutProperty>("Hierarchical Tree (R-T Extended)");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testTutte() {
  bool result = computeProperty<LayoutProperty>("3-Connected (Tutte)",
						"Complete General Graph");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testInducedSubGraphSelection() {
  bool result = computeProperty<BooleanProperty>("Induced Sub-Graph");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testKruskal() {
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
void PluginsTest::testLoopSelection() {
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
void PluginsTest::testMultipleEdgeSelection() {
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
void PluginsTest::testReachableSubGraphSelection() {
  bool result = computeProperty<BooleanProperty>("Reachable Sub-Graph");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testSpanningDagSelection() {
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
void PluginsTest::testSpanningTreeSelection() {
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
void PluginsTest::testAutoSize() {
  bool result = computeProperty<SizeProperty>("Auto Sizing");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testFitToLabel() {
  StringProperty* label = graph->getProperty<StringProperty>("viewLabel");
  label->setAllNodeValue("toto");
  bool result = computeProperty<SizeProperty>("Fit to label");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testMetricSizeMapping() {
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
void PluginsTest::testEqualValueClustering() {
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
void PluginsTest::testHierarchicalClustering() {
  initializeGraph("Planar Graph");
  DoubleProperty* metric = graph->getProperty<DoubleProperty>("viewMetric");
  string errorMsg;
  bool result = graph->computeProperty("Degree", metric, errorMsg);
  CPPUNIT_ASSERT(result);
  result = tlp::applyAlgorithm(graph, errorMsg, NULL, "Hierarchical", NULL); 
  CPPUNIT_ASSERT(result);
}
//==========================================================
void PluginsTest::testQuotientClustering() {
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
void PluginsTest::testStrengthClustering() {
  initializeGraph("Planar Graph");
  string errorMsg;
  bool result = tlp::applyAlgorithm(graph, errorMsg, NULL,
				       "Strength Clustering", NULL); 
  CPPUNIT_ASSERT(result);
}

CppUnit::Test * PluginsTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite("Tulip lib : Plugins mechanism" );
  suiteOfTests->addTest( new CppUnit::TestCaller<PluginsTest>("loadPlugin", 
							      &PluginsTest::testloadPlugin ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PluginsTest>("Circular call", 
							      &PluginsTest::testCircularPlugin ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PluginsTest>("Graph validity", 
							      &PluginsTest::testAncestorGraph) );
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("Import CompleteGraph", &PluginsTest::testImportCompleteGraph));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("Import CompleteTree", &PluginsTest::testImportCompleteTree));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("Import RandomGraph", &PluginsTest::testImportRandomGraph));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("Import RandomTree", &PluginsTest::testImportRandomTree));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("Import RandomTreeGeneral", &PluginsTest::testImportRandomTreeGeneral));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("Import RandomSimpleGraph", &PluginsTest::testImportRandomSimpleGraph));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("Import PlanarGraph", &PluginsTest::testImportPlanarGraph));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("Import Grid", &PluginsTest::testImportGrid));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("Import GridApproximation", &PluginsTest::testImportGridApproximation));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("Import Dot", &PluginsTest::testImportDot));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("Import Gml", &PluginsTest::testImportGml));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("Import FileSystem", &PluginsTest::testImportFileSystem));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("Import AdjacencyMatrix", &PluginsTest::testImportAdjacencyMatrix));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("ArityMetric", &PluginsTest::testArityMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("BetweennessCentrality", &PluginsTest::testBetweennessCentrality));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("BiconnectedComponent", &PluginsTest::testBiconnectedComponent));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("ClusterMetric", &PluginsTest::testClusterMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("ConnectedComponent", &PluginsTest::testConnectedComponent));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("DagLevelMetric", &PluginsTest::testDagLevelMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("DepthMetric", &PluginsTest::testDepthMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("Eccentricity", &PluginsTest::testEccentricity));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("IdMetric", &PluginsTest::testIdMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("LeafMetric", &PluginsTest::testLeafMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("NodeMetric", &PluginsTest::testNodeMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("PathLengthMetric", &PluginsTest::testPathLengthMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("Random Metric", &PluginsTest::testRandomMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("StrahlerMetric", &PluginsTest::testStrahlerMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("StrengthMetric", &PluginsTest::testStrengthMetric));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("StrongComponent", &PluginsTest::testStrongComponent));;
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("MetricColorMapping", &PluginsTest::testMetricColorMapping));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("EnumeratedColorValuesMapping", &PluginsTest::testEnumeratedColorValuesMapping));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("BubbleTree", &PluginsTest::testBubbleTree));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("Circular", &PluginsTest::testCircular));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("ConeTreeExtended", &PluginsTest::testConeTreeExtended));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("ConnectedComponentPacking", &PluginsTest::testConnectedComponentPacking));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("Dendrogram", &PluginsTest::testDendrogram));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("Embedder", &PluginsTest::testEmbedder));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("GEMLayout", &PluginsTest::testGEMLayout));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("HierarchicalGraph", &PluginsTest::testHierarchicalGraph));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("ImprovedWalker", &PluginsTest::testImprovedWalker));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("MixedModel", &PluginsTest::testMixedModel));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("Random Layout", &PluginsTest::testRandomLayout));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("SquarifiedTreeMap", &PluginsTest::testSquarifiedTreeMap));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("TreeLeaf", &PluginsTest::testTreeLeaf));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("TreeMap", &PluginsTest::testTreeMap));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("TreeRadial", &PluginsTest::testTreeRadial));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("TreeReingoldAndTilfordExtended", &PluginsTest::testTreeReingoldAndTilfordExtended));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("Tutte", &PluginsTest::testTutte));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("InducedSubGraphSelection", &PluginsTest::testInducedSubGraphSelection));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("Kruskal", &PluginsTest::testKruskal));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("LoopSelection", &PluginsTest::testLoopSelection));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("MultipleEdgeSelection", &PluginsTest::testMultipleEdgeSelection));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("ReachableSubGraphSelection", &PluginsTest::testReachableSubGraphSelection));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("SpanningDagSelection", &PluginsTest::testSpanningDagSelection));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("SpanningTreeSelection", &PluginsTest::testSpanningTreeSelection));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("AutoSize", &PluginsTest::testAutoSize));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("FitToLabel", &PluginsTest::testFitToLabel));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("MetricSizeMapping", &PluginsTest::testMetricSizeMapping));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("EqualValueClustering", &PluginsTest::testEqualValueClustering));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("HierarchicalClustering", &PluginsTest::testHierarchicalClustering));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("QuotientClustering", &PluginsTest::testQuotientClustering));
  suiteOfTests->addTest(new CppUnit::TestCaller<PluginsTest>("StrengthClustering", &PluginsTest::testStrengthClustering));

  return suiteOfTests;
}
