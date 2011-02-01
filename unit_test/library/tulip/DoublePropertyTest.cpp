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

#include <string>
#include <iostream>

#include <cppunit/TestCaller.h>
#include "DoublePropertyTest.h"

using namespace tlp;
using namespace std;

const string doublePropertyName = "double property test";

const double originalMin = 5;
const double originalMax = 10;

const double newMin = 1;
const double newMax = 15;

#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION( DoublePropertyTest );

void DoublePropertyTest::setUp() {
  node n2, n3;
  graph = newGraph();
  graph->getLocalProperty<DoubleProperty>(doublePropertyName);
  n1 = graph->addNode();
  graph->getLocalProperty<DoubleProperty>(doublePropertyName)->setNodeValue(n1, originalMin);

  n2 = graph->addNode();
  graph->getLocalProperty<DoubleProperty>(doublePropertyName)->setNodeValue(n2, 6);

  n3 = graph->addNode();
  graph->getLocalProperty<DoubleProperty>(doublePropertyName)->setNodeValue(n3, 7);

  n4 = graph->addNode();
  graph->getLocalProperty<DoubleProperty>(doublePropertyName)->setNodeValue(n4, originalMax);
}

void DoublePropertyTest::tearDown() {
  delete graph;
}

void DoublePropertyTest::testDoublePropertyMinUpdate() {
  double minNode;

  minNode = graph->getLocalProperty<DoubleProperty>(doublePropertyName)->getNodeMin();
  CPPUNIT_ASSERT_MESSAGE("test DoubleProperty min value before update", minNode == originalMin);

  graph->getLocalProperty<DoubleProperty>(doublePropertyName)->setNodeValue(n1, newMin);
  minNode = graph->getLocalProperty<DoubleProperty>(doublePropertyName)->getNodeMin();
  CPPUNIT_ASSERT_MESSAGE("test DoubleProperty min value after update", minNode == newMin);
}

void DoublePropertyTest::testDoublePropertyMaxUpdate() {
  double maxNode;

  maxNode = graph->getLocalProperty<DoubleProperty>(doublePropertyName)->getNodeMax();
  CPPUNIT_ASSERT_MESSAGE("test DoubleProperty max value before update", maxNode == originalMax);

  graph->getLocalProperty<DoubleProperty>(doublePropertyName)->setNodeValue(n4, newMax);
  maxNode = graph->getLocalProperty<DoubleProperty>(doublePropertyName)->getNodeMax();
  CPPUNIT_ASSERT_MESSAGE("test DoubleProperty max value after update", maxNode == newMax);
}

//==========================================================
CppUnit::Test * DoublePropertyTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : Double Property" );

  suiteOfTests->addTest(new CppUnit::TestCaller<DoublePropertyTest>(" Min update", &DoublePropertyTest::testDoublePropertyMinUpdate));
  suiteOfTests->addTest(new CppUnit::TestCaller<DoublePropertyTest>(" Max update", &DoublePropertyTest::testDoublePropertyMaxUpdate));

  return suiteOfTests;
}
