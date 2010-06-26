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

#include <tulip/Graph.h>

class BasicPluginsTest : public CppUnit::TestFixture {
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
  void testEnumeratedColorValuesMapping(); 
  void testBubbleTree();
  void testCircular();
  void testConeTreeExtended();
  void testConnectedComponentPacking();
  void testDendrogram();
  void testEmbedder();
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
  static CppUnit::Test *suite();
};

#endif
