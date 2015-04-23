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

#include "IntegerPropertyMinMaxSubgraphTest.h"

const std::string integerPropertyName = "integer property for purpose testing";

const int rootGraphMin = 1;
const int rootGraphMax = 100;

const int subgraphMin = 2;
const int subgraphMax = 50;

using namespace tlp;

CPPUNIT_TEST_SUITE_REGISTRATION(IntegerPropertyMinMaxSubgraphTest);

void IntegerPropertyMinMaxSubgraphTest::setUp() {
  graph = newGraph();
  IntegerProperty *intProperty = graph->getProperty<IntegerProperty>(integerPropertyName);
  node n = graph->addNode();
  intProperty->setNodeValue(n, rootGraphMin);
  n = graph->addNode();
  intProperty->setNodeValue(n, rootGraphMax);
  subGraph = graph->addSubGraph();
  n = subGraph->addNode();
  intProperty->setNodeValue(n, subgraphMin);
  n = subGraph->addNode();
  intProperty->setNodeValue(n, subgraphMax);
}

void IntegerPropertyMinMaxSubgraphTest::tearDown() {
  delete graph;
}

void IntegerPropertyMinMaxSubgraphTest::testIntegerPropertyRootGraphMin() {
  IntegerProperty *intProperty = graph->getProperty<IntegerProperty>(integerPropertyName);
  CPPUNIT_ASSERT_EQUAL_MESSAGE("test IntegerProperty min value on root graph", rootGraphMin, intProperty->getNodeMin());
}

void IntegerPropertyMinMaxSubgraphTest::testIntegerPropertyRootGraphMax() {
  IntegerProperty *intProperty = graph->getProperty<IntegerProperty>(integerPropertyName);
  CPPUNIT_ASSERT_EQUAL_MESSAGE("test IntegerProperty max value on root graph", rootGraphMax, intProperty->getNodeMax());
}

void IntegerPropertyMinMaxSubgraphTest::testIntegerPropertySubgraphMin() {
  IntegerProperty *intProperty = subGraph->getProperty<IntegerProperty>(integerPropertyName);
  CPPUNIT_ASSERT_EQUAL_MESSAGE("test IntegerProperty min value on subgraph graph", subgraphMin, intProperty->getNodeMin(subGraph));
}

void IntegerPropertyMinMaxSubgraphTest::testIntegerPropertySubgraphMax() {
  IntegerProperty *intProperty = subGraph->getProperty<IntegerProperty>(integerPropertyName);
  CPPUNIT_ASSERT_EQUAL_MESSAGE("test IntegerProperty max value on subgraph", subgraphMax, intProperty->getNodeMax(subGraph));
}
