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
  CPPUNIT_ASSERT_EQUAL_MESSAGE("test DoubleProperty min value before update", originalMin, minNode);

  graph->getLocalProperty<DoubleProperty>(doublePropertyName)->setNodeValue(n1, newMin);
  minNode = graph->getLocalProperty<DoubleProperty>(doublePropertyName)->getNodeMin();
  CPPUNIT_ASSERT_EQUAL_MESSAGE("test DoubleProperty min value after update", newMin, minNode);
}

void DoublePropertyTest::testDoublePropertyMaxUpdate() {
  double maxNode;

  maxNode = graph->getLocalProperty<DoubleProperty>(doublePropertyName)->getNodeMax();
  CPPUNIT_ASSERT_EQUAL_MESSAGE("test DoubleProperty max value before update", originalMax, maxNode);

  graph->getLocalProperty<DoubleProperty>(doublePropertyName)->setNodeValue(n4, newMax);
  maxNode = graph->getLocalProperty<DoubleProperty>(doublePropertyName)->getNodeMax();
  CPPUNIT_ASSERT_EQUAL_MESSAGE("test DoubleProperty max value after update", newMax, maxNode);
}

void DoublePropertyTest::testDoublePropertyMinUpdateFromString() {
  double minNode;

  minNode = graph->getLocalProperty<DoubleProperty>(doublePropertyName)->getNodeMin();
  CPPUNIT_ASSERT_EQUAL_MESSAGE("test DoubleProperty min value before update", originalMin, minNode);

  const string newStringMin = "1";

  graph->getLocalProperty<DoubleProperty>(doublePropertyName)->setNodeStringValue(n1, newStringMin);
  minNode = graph->getLocalProperty<DoubleProperty>(doublePropertyName)->getNodeMin();
  CPPUNIT_ASSERT_EQUAL_MESSAGE("test DoubleProperty min value after update", newMin, minNode);
}

void DoublePropertyTest::testDoublePropertyMaxUpdateFromString() {
  double maxNode;

  maxNode = graph->getLocalProperty<DoubleProperty>(doublePropertyName)->getNodeMax();
  CPPUNIT_ASSERT_EQUAL_MESSAGE("test DoubleProperty max value before update", originalMax, maxNode);

  const string newStringMax = "15";

  graph->getLocalProperty<DoubleProperty>(doublePropertyName)->setNodeStringValue(n4, newStringMax);
  maxNode = graph->getLocalProperty<DoubleProperty>(doublePropertyName)->getNodeMax();
  CPPUNIT_ASSERT_EQUAL_MESSAGE("test DoubleProperty max value after update", newMax, maxNode);
}

void DoublePropertyTest::testDoublePropertySubGraphMin() {
  DoubleProperty *doubleProperty = graph->getProperty<DoubleProperty>(doublePropertyName);
  Graph* subGraph = graph->addSubGraph();
  node n2 = subGraph->addNode();
  doubleProperty->setNodeValue(n2, 6);
  node n3 = subGraph->addNode();
  doubleProperty->setNodeValue(n3, 9);
  CPPUNIT_ASSERT_EQUAL(originalMin, doubleProperty->getNodeMin());
  CPPUNIT_ASSERT_EQUAL(6.0, doubleProperty->getNodeMin(subGraph));

  subGraph->delNode(n2);
  CPPUNIT_ASSERT_EQUAL(9.0, doubleProperty->getNodeMin(subGraph));
  CPPUNIT_ASSERT_EQUAL(originalMin, doubleProperty->getNodeMin());
  graph->delNode(n1);
  CPPUNIT_ASSERT_EQUAL(9.0, doubleProperty->getNodeMin(subGraph));
  CPPUNIT_ASSERT_EQUAL(6.0, doubleProperty->getNodeMin());
}

void DoublePropertyTest::testDoublePropertySubGraphMax() {
  DoubleProperty *doubleProperty = graph->getProperty<DoubleProperty>(doublePropertyName);
  Graph* subGraph = graph->addSubGraph();
  node n2 = subGraph->addNode();
  doubleProperty->setNodeValue(n2, 6.0);
  node n3 = subGraph->addNode();
  doubleProperty->setNodeValue(n3, 9.0);
  CPPUNIT_ASSERT_EQUAL(doubleProperty->getNodeMax(), originalMax);
  CPPUNIT_ASSERT_EQUAL(9.0, doubleProperty->getNodeMax(subGraph));

  subGraph->delNode(n3);
  CPPUNIT_ASSERT_EQUAL(doubleProperty->getNodeMax(), originalMax);
  CPPUNIT_ASSERT_EQUAL(6.0, doubleProperty->getNodeMax(subGraph));
  graph->delNode(n4);
  CPPUNIT_ASSERT(doubleProperty->getNodeMax(subGraph) == 6);
  CPPUNIT_ASSERT(doubleProperty->getNodeMax() == 9);
}

void DoublePropertyTest::testDoublePropertyInfValue() { 
  double zero = 0.0;
  double infValue = 1.0/zero;

  CPPUNIT_ASSERT(infValue == std::numeric_limits<double>::infinity());
  CPPUNIT_ASSERT(-infValue == -std::numeric_limits<double>::infinity());

  node n = graph->addNode();

  DoubleProperty* prop =
    graph->getLocalProperty<DoubleProperty>(doublePropertyName);
  CPPUNIT_ASSERT(prop->getNodeValue(n) == 0.0);

  prop->setNodeValue(n, infValue);
  CPPUNIT_ASSERT(prop->getNodeValue(n) == infValue);

  prop->setNodeValue(n, 1.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n) == 1.0);

  prop->setNodeValue(n, -infValue);
  CPPUNIT_ASSERT(prop->getNodeValue(n) == -infValue);

  prop->setNodeValue(n, 1.0);
  CPPUNIT_ASSERT(prop->getNodeValue(n) == 1.0);

  prop->setNodeStringValue(n , "inf");
  CPPUNIT_ASSERT(prop->getNodeValue(n) == infValue);

  prop->setNodeStringValue(n , "-inf");
  CPPUNIT_ASSERT(prop->getNodeValue(n) == -infValue);
}
