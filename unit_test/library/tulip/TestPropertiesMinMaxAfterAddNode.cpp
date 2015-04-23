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
#include "TestPropertiesMinMaxAfterAddNode.h"

#include <tulip/DoubleProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>

CPPUNIT_TEST_SUITE_REGISTRATION( TestPropertiesMinMaxAfterAddNode );

using namespace tlp;

void TestPropertiesMinMaxAfterAddNode::setUp() {
  graph = tlp::newGraph();
}

void TestPropertiesMinMaxAfterAddNode::tearDown() {
  delete graph;
}

void TestPropertiesMinMaxAfterAddNode::testDoublePropertyMinMaxAfterAddNode() {
  DoubleProperty *doubleProp = graph->getProperty<DoubleProperty>("doubleProp");

  // add two nodes
  node n1 = graph->addNode();
  node n2 = graph->addNode();

  const double d1 = 3.5;
  const double d2 = 89.6;

  // set values to doubleProp
  doubleProp->setNodeValue(n1, d1);
  doubleProp->setNodeValue(n2, d2);
  CPPUNIT_ASSERT_EQUAL(d1, doubleProp->getNodeMin(graph));
  CPPUNIT_ASSERT_EQUAL(d2, doubleProp->getNodeMax(graph));

  // add a new node, the value associated to doubleProp property is the default one 0
  graph->addNode();

  // min should be 0
  CPPUNIT_ASSERT_EQUAL(0.0, doubleProp->getNodeMin(graph));
  CPPUNIT_ASSERT_EQUAL(d2, doubleProp->getNodeMax(graph));
}

void TestPropertiesMinMaxAfterAddNode::testIntegerPropertyMinMaxAfterAddNode() {
  IntegerProperty *intProp = graph->getProperty<IntegerProperty>("intProp");

  // add two nodes
  node n1 = graph->addNode();
  node n2 = graph->addNode();

  const int i1 = 3;
  const int i2 = 56;

  // set values to doubleProp
  intProp->setNodeValue(n1, i1);
  intProp->setNodeValue(n2, i2);
  CPPUNIT_ASSERT_EQUAL(i1, intProp->getNodeMin(graph));
  CPPUNIT_ASSERT_EQUAL(i2, intProp->getNodeMax(graph));

  // add a new node, the value associated to doubleProp property is the default one 0
  graph->addNode();

  // min should be 0
  CPPUNIT_ASSERT_EQUAL(0, intProp->getNodeMin(graph));
  CPPUNIT_ASSERT_EQUAL(i2, intProp->getNodeMax(graph));
}

void TestPropertiesMinMaxAfterAddNode::testLayoutPropertyMinMaxAfterAddNode() {
  LayoutProperty *property = graph->getProperty<LayoutProperty>("testLayout");

  // add two nodes
  node n1 = graph->addNode();
  node n2 = graph->addNode();

  const Coord firstNodePos(1.0f, 2.0f);
  const Coord secondNodePos(3.0f, 4.0f);

  property->setNodeValue(n1, firstNodePos);
  property->setNodeValue(n2, secondNodePos);
  CPPUNIT_ASSERT_EQUAL(firstNodePos, property->getMin(graph));
  CPPUNIT_ASSERT_EQUAL(secondNodePos, property->getMax(graph));

  // add a new node, the value associated to the layout property is the default one 0
  graph->addNode();

  // min should be 0
  CPPUNIT_ASSERT_EQUAL(Coord(), property->getMin(graph));
  CPPUNIT_ASSERT_EQUAL(secondNodePos, property->getMax(graph));
}
