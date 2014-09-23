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
#include "basicmetrictest.h"
#include <tulip/Graph.h>
#include <tulip/DoubleProperty.h>

using namespace std;
using namespace tlp;

CPPUNIT_TEST_SUITE_REGISTRATION(BasicMetricTest);

template<typename PropType>
bool BasicMetricTest::computeProperty(const std::string &algorithm,
                                      const std::string & graphType,
                                      PropType* prop) {

  DataSet ds;
  tlp::Graph* g = tlp::importGraph(graphType, ds, NULL, graph);
  CPPUNIT_ASSERT(g == graph);

  bool deleteProp = prop == NULL;

  if (prop == NULL)
    prop = new PropType(graph);

  std::string errorMsg;
  bool result = graph->applyPropertyAlgorithm(algorithm, prop, errorMsg);

  if (deleteProp)
    delete prop;

  return result;
}

void BasicMetricTest::setUp() {
  graph = tlp::newGraph();
}

void BasicMetricTest::tearDown() {
  delete graph;
}

void BasicMetricTest::testArityMetric() {
  bool result = computeProperty<DoubleProperty>("Degree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicMetricTest::testBetweennessCentrality() {
  bool result = computeProperty<DoubleProperty>("Betweenness Centrality");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicMetricTest::testBiconnectedComponent() {
  bool result = computeProperty<DoubleProperty>("Biconnected Component");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicMetricTest::testClusterMetric() {
  bool result = computeProperty<DoubleProperty>("Cluster");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicMetricTest::testConnectedComponent() {
  bool result = computeProperty<DoubleProperty>("Connected Component");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicMetricTest::testDagLevelMetric() {
  bool result = computeProperty<DoubleProperty>("Dag Level");
  CPPUNIT_ASSERT(result == false);
  graph->clear();
  result = computeProperty<DoubleProperty>("Dag Level", "Random General Tree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicMetricTest::testDepthMetric() {
  bool result = computeProperty<DoubleProperty>("Depth");
  CPPUNIT_ASSERT(result == false);
  graph->clear();
  result = computeProperty<DoubleProperty>("Depth", "Random General Tree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicMetricTest::testEccentricity() {
  bool result = computeProperty<DoubleProperty>("Eccentricity");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicMetricTest::testIdMetric() {
  bool result = computeProperty<DoubleProperty>("Id");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicMetricTest::testLeafMetric() {
  bool result = computeProperty<DoubleProperty>("Leaf");
  CPPUNIT_ASSERT(result == false);
  graph->clear();
  result = computeProperty<DoubleProperty>("Leaf", "Random General Tree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicMetricTest::testNodeMetric() {
  bool result = computeProperty<DoubleProperty>("Node");
  CPPUNIT_ASSERT(result == false);
  graph->clear();
  result = computeProperty<DoubleProperty>("Node", "Random General Tree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicMetricTest::testPathLengthMetric() {
  bool result = computeProperty<DoubleProperty>("Path Length");
  CPPUNIT_ASSERT(result == false);
  graph->clear();
  result = computeProperty<DoubleProperty>("Path Length",
           "Random General Tree");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicMetricTest::testRandomMetric() {
  bool result = computeProperty<DoubleProperty>("Random metric");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicMetricTest::testStrahlerMetric() {
  bool result = computeProperty<DoubleProperty>("Strahler");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicMetricTest::testStrengthMetric() {
  bool result = computeProperty<DoubleProperty>("Strength");
  CPPUNIT_ASSERT(result);
}
//==========================================================
void BasicMetricTest::testStrongComponent() {
  bool result = computeProperty<DoubleProperty>("Strongly Connected Component");
  CPPUNIT_ASSERT(result);
}
//==========================================================

