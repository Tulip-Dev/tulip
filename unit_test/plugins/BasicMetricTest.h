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
#ifndef BASICMETRICTEST_H
#define BASICMETRICTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace tlp {
class Graph;
}

class BasicMetricTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(BasicMetricTest);
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
  CPPUNIT_TEST_SUITE_END();
private:
  tlp::Graph *graph;
  template<typename PropType>
  bool computeProperty(const std::string &algorithm, const std::string& graphType = "Planar Graph", PropType* prop = NULL);
public:
  void setUp();
  void tearDown();

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
};

#endif // BASICMETRICTEST_H
