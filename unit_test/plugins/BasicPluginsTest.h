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
#ifndef TLPPLUGINSTEST
#define TLPPLUGINSTEST

#include <string>

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

#include <tulip/Graph.h>

class BasicPluginsTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(BasicPluginsTest);
  CPPUNIT_TEST(testImportCompleteGraph);
  CPPUNIT_TEST(testImportCompleteTree);
  CPPUNIT_TEST(testImportRandomGraph);
  CPPUNIT_TEST(testImportRandomTree);
  CPPUNIT_TEST(testImportRandomTreeGeneral);
  CPPUNIT_TEST(testImportRandomSimpleGraph);
  CPPUNIT_TEST(testImportPlanarGraph);
  CPPUNIT_TEST(testImportGrid);
  CPPUNIT_TEST(testImportGridApproximation);
  CPPUNIT_TEST(testImportDot);
  CPPUNIT_TEST(testImportGml);
  CPPUNIT_TEST(testExportGml);
  CPPUNIT_TEST(testImportFileSystem);
  CPPUNIT_TEST(testImportAdjacencyMatrix);
  CPPUNIT_TEST(testArityMetric);
  CPPUNIT_TEST(testBetweennessCentrality);
  CPPUNIT_TEST(testBiconnectedComponent);
  CPPUNIT_TEST(testClusterMetric);
  CPPUNIT_TEST(testConnectedComponent);
  CPPUNIT_TEST(testDagLevelMetric);
  CPPUNIT_TEST(testDepthMetric);
  CPPUNIT_TEST(testEccentricity);
  CPPUNIT_TEST(testIdMetric);
  CPPUNIT_TEST(testLeafMetric);
  CPPUNIT_TEST(testNodeMetric);
  CPPUNIT_TEST(testPathLengthMetric);
  CPPUNIT_TEST(testRandomMetric);
  CPPUNIT_TEST(testStrahlerMetric);
  CPPUNIT_TEST(testStrengthMetric);
  CPPUNIT_TEST(testStrongComponent);
  CPPUNIT_TEST(testMetricColorMapping);
  CPPUNIT_TEST(testBubbleTree);
  CPPUNIT_TEST(testCircular);
  CPPUNIT_TEST(testConeTreeExtended);
  CPPUNIT_TEST(testConnectedComponentPacking);
  CPPUNIT_TEST(testDendrogram);
  CPPUNIT_TEST(testGEMLayout);
  CPPUNIT_TEST(testHierarchicalGraph);
  CPPUNIT_TEST(testImprovedWalker);
  CPPUNIT_TEST(testMixedModel);
  CPPUNIT_TEST(testRandomLayout);
  CPPUNIT_TEST(testSquarifiedTreeMap);
  CPPUNIT_TEST(testTreeLeaf);
  CPPUNIT_TEST(testTreeMap);
  CPPUNIT_TEST(testTreeRadial);
  CPPUNIT_TEST(testTreeReingoldAndTilfordExtended);
  CPPUNIT_TEST(testTutte);
  CPPUNIT_TEST(testInducedSubGraphSelection);
  CPPUNIT_TEST(testKruskal);
  CPPUNIT_TEST(testLoopSelection);
  CPPUNIT_TEST(testMultipleEdgeSelection);
  CPPUNIT_TEST(testReachableSubGraphSelection);
  CPPUNIT_TEST(testSpanningDagSelection);
  CPPUNIT_TEST(testSpanningTreeSelection);
  CPPUNIT_TEST(testAutoSize);
  CPPUNIT_TEST(testFitToLabel);
  CPPUNIT_TEST(testMetricSizeMapping);
  CPPUNIT_TEST(testEqualValueClustering);
  CPPUNIT_TEST(testHierarchicalClustering);
  CPPUNIT_TEST(testQuotientClustering);
  CPPUNIT_TEST(testStrengthClustering);
  CPPUNIT_TEST_SUITE_END();
private:
  tlp::Graph *graph;

public:
  void setUp();
  void tearDown();
  void initializeGraph(const std::string& type);
  template<typename PropType>
  bool computeProperty(const std::string &algorithm,
                       const std::string& graphType = "Planar Graph",
                       PropType* prop = NULL);
  void testImportCompleteGraph();
  void testImportCompleteTree();
  void testImportRandomGraph();
  void testImportRandomTree();
  void testImportRandomTreeGeneral();
  void testImportRandomSimpleGraph();
  void testImportPlanarGraph();
  void testImportGrid();
  void testImportGridApproximation();
  void testImportDot();
  void testImportGml();
  void testExportGml();
  void testImportFileSystem();
  void testImportAdjacencyMatrix();
  void testArityMetric();
  void testBetweennessCentrality();
  void testBiconnectedComponent();
  void testClusterMetric();
  void testConnectedComponent();
  void testDagLevelMetric();
  void testDepthMetric();
  void testEccentricity();
  void testIdMetric();
  void testLeafMetric();
  void testNodeMetric();
  void testPathLengthMetric();
  void testRandomMetric();
  void testStrahlerMetric();
  void testStrengthMetric();
  void testStrongComponent();
  void testMetricColorMapping();
  void testBubbleTree();
  void testCircular();
  void testConeTreeExtended();
  void testConnectedComponentPacking();
  void testDendrogram();
  void testGEMLayout();
  void testHierarchicalGraph();
  void testImprovedWalker();
  void testMixedModel();
  void testRandomLayout();
  void testSquarifiedTreeMap();
  void testTreeLeaf();
  void testTreeMap();
  void testTreeRadial();
  void testTreeReingoldAndTilfordExtended();
  void testTutte();
  void testInducedSubGraphSelection();
  void testKruskal();
  void testLoopSelection();
  void testMultipleEdgeSelection();
  void testReachableSubGraphSelection();
  void testSpanningDagSelection();
  void testSpanningTreeSelection();
  void testAutoSize();
  void testFitToLabel();
  void testMetricSizeMapping();
  void testEqualValueClustering();
  void testHierarchicalClustering();
  void testQuotientClustering();
  void testStrengthClustering();
};

#endif
