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
const string newStringMin = "1";
const double newMax = 15;
const string newStringMax = "15";

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

void DoublePropertyTest::testDoublePropertyMinUpdateFromString() {
  double minNode;

  minNode = graph->getLocalProperty<DoubleProperty>(doublePropertyName)->getNodeMin();
  CPPUNIT_ASSERT_MESSAGE("test DoubleProperty min value before update", minNode == originalMin);

  graph->getLocalProperty<DoubleProperty>(doublePropertyName)->setNodeStringValue(n1, newStringMin);
  minNode = graph->getLocalProperty<DoubleProperty>(doublePropertyName)->getNodeMin();
  CPPUNIT_ASSERT_MESSAGE("test DoubleProperty min value after update", minNode == newMin);
}

void DoublePropertyTest::testDoublePropertyMaxUpdateFromString() {
  double maxNode;

  maxNode = graph->getLocalProperty<DoubleProperty>(doublePropertyName)->getNodeMax();
  CPPUNIT_ASSERT_MESSAGE("test DoubleProperty max value before update", maxNode == originalMax);

  graph->getLocalProperty<DoubleProperty>(doublePropertyName)->setNodeStringValue(n4, newStringMax);
  maxNode = graph->getLocalProperty<DoubleProperty>(doublePropertyName)->getNodeMax();
  CPPUNIT_ASSERT_MESSAGE("test DoubleProperty max value after update", maxNode == newMax);
}

void DoublePropertyTest::testDoublePropertySubGraphMin() {
  DoubleProperty *doubleProperty = graph->getProperty<DoubleProperty>(doublePropertyName);
  Graph* subGraph = graph->addSubGraph();
  node n2 = subGraph->addNode();
  doubleProperty->setNodeValue(n2, 6);
  node n3 = subGraph->addNode();
  doubleProperty->setNodeValue(n3, 9);
  CPPUNIT_ASSERT(doubleProperty->getNodeMin() == originalMin);
  CPPUNIT_ASSERT(doubleProperty->getNodeMin(subGraph) == 6);

  subGraph->delNode(n2);
  CPPUNIT_ASSERT(doubleProperty->getNodeMin(subGraph) == 9);
  CPPUNIT_ASSERT(doubleProperty->getNodeMin() == originalMin);
  graph->delNode(n1);
  CPPUNIT_ASSERT(doubleProperty->getNodeMin(subGraph) == 9);
  CPPUNIT_ASSERT(doubleProperty->getNodeMin() == 6);
}

void DoublePropertyTest::testDoublePropertySubGraphMax() {
  DoubleProperty *doubleProperty = graph->getProperty<DoubleProperty>(doublePropertyName);
  Graph* subGraph = graph->addSubGraph();
  node n2 = subGraph->addNode();
  doubleProperty->setNodeValue(n2, 6);
  node n3 = subGraph->addNode();
  doubleProperty->setNodeValue(n3, 9);
  CPPUNIT_ASSERT(doubleProperty->getNodeMax() == originalMax);
  CPPUNIT_ASSERT(doubleProperty->getNodeMax(subGraph) == 9);

  subGraph->delNode(n3);
  CPPUNIT_ASSERT(doubleProperty->getNodeMax() == originalMax);
  CPPUNIT_ASSERT(doubleProperty->getNodeMax(subGraph) == 6);
  graph->delNode(n4);
  CPPUNIT_ASSERT(doubleProperty->getNodeMax(subGraph) == 6);
  CPPUNIT_ASSERT(doubleProperty->getNodeMax() == 9);
}

//==========================================================
CppUnit::Test * DoublePropertyTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : Double Property" );

  suiteOfTests->addTest(new CppUnit::TestCaller<DoublePropertyTest>(" Min update", &DoublePropertyTest::testDoublePropertyMinUpdate));
  suiteOfTests->addTest(new CppUnit::TestCaller<DoublePropertyTest>(" Max update", &DoublePropertyTest::testDoublePropertyMaxUpdate));
  suiteOfTests->addTest(new CppUnit::TestCaller<DoublePropertyTest>(" Min update from string", &DoublePropertyTest::testDoublePropertyMinUpdateFromString));
  suiteOfTests->addTest(new CppUnit::TestCaller<DoublePropertyTest>(" Max update from string", &DoublePropertyTest::testDoublePropertyMaxUpdateFromString));
  suiteOfTests->addTest(new CppUnit::TestCaller<DoublePropertyTest>(" Min update on subgraph", &DoublePropertyTest::testDoublePropertySubGraphMin));
  suiteOfTests->addTest(new CppUnit::TestCaller<DoublePropertyTest>(" Max update on subgraph", &DoublePropertyTest::testDoublePropertySubGraphMax));

  return suiteOfTests;
}
