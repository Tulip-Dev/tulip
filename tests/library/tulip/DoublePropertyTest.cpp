/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include "DoublePropertyTest.h"

#include <tulip/TlpTools.h>

using namespace tlp;
using namespace std;

const string doublePropertyName = "double property test";

const double originalMin = 5;
const double originalMax = 10;

const double newMin = 1;
const double newMax = 15;

CPPUNIT_TEST_SUITE_REGISTRATION( DoublePropertyTest );

void DoublePropertyTest::setUp() {
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

  e1 = graph->addEdge(n1, n3);
  e2 = graph->addEdge(n2, n4);
}

void DoublePropertyTest::tearDown() {
  delete graph;
}

void DoublePropertyTest::testAnonymousDoublePropertyMaxUpdate() {
  DoubleProperty prop(graph);
  double maxNode;

  maxNode = prop.getNodeMax();
  CPPUNIT_ASSERT(maxNode == 0.0);
  prop.setNodeValue(n1, newMax);
  maxNode = prop.getNodeMax();
  CPPUNIT_ASSERT(maxNode == newMax);
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

void DoublePropertyTest::testDoublePropertySetAllValue() {

  // create a subgraph
  Graph *sg = graph->addSubGraph();
  sg->addNode(graph->source(e1));
  sg->addNode(graph->target(e1));
  sg->addEdge(e1);

  const double v1 = tlp::randomDouble();
  const double v2 = tlp::randomDouble();

  // create a double property and set all values for nodes and edges
  DoubleProperty* prop = graph->getLocalProperty<DoubleProperty>(doublePropertyName);
  prop->setAllNodeValue(v1);
  prop->setAllEdgeValue(v2);

  // check that the default property value has been correctly modified
  CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getNodeDefaultValue(), v1, 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getEdgeDefaultValue(), v2, 1e-6);

  // check that each node has the correct value
  node n;
  forEach(n, graph->getNodes()) {
    CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getNodeValue(n), v1, 1e-6);
  }
  // check that the default node value has been changed
  CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getNodeDefaultValue(), v1, 1e-6);

  // check that each edge has the correct value
  edge e;
  forEach(e, graph->getEdges()) {
    CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getEdgeValue(e), v2, 1e-6);
  }
  // check that the default edge value has been changed
  CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getEdgeDefaultValue(), v2, 1e-6);

  // set different values for the nodes and edges of the subgraph
  prop->setValueToGraphNodes(v2, sg);
  prop->setValueToGraphEdges(v1, sg);

  // check that the default property value has not been modified
  CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getNodeDefaultValue(), v1, 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getEdgeDefaultValue(), v2, 1e-6);

  // check that the nodes have expected values
  forEach(n, graph->getNodes()) {
    if (sg->isElement(n)) {
      CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getNodeValue(n), v2, 1e-6);
    } else {
      CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getNodeValue(n), v1, 1e-6);
    }
  }
  // check that the default node value has not been modified
  CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getNodeDefaultValue(), v1, 1e-6);

  // check that the edges have expected values
  forEach(e, graph->getEdges()) {
    if (sg->isElement(e)) {
      CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getEdgeValue(e), v1, 1e-6);
    } else {
      CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getEdgeValue(e), v2, 1e-6);
    }
  }
  // check that the default edge value has not been modified
  CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getEdgeDefaultValue(), v2, 1e-6);

}

void DoublePropertyTest::testDoublePropertySetDefaultValue() {

  const double v1 = tlp::randomDouble();
  const double v2 = tlp::randomDouble();

  // create a double property and set all values for nodes and edges
  DoubleProperty* prop = graph->getLocalProperty<DoubleProperty>(doublePropertyName);
  prop->setAllNodeValue(v1);
  prop->setAllEdgeValue(v2);

  // check number of non default valuated edges
  CPPUNIT_ASSERT_EQUAL(prop->numberOfNonDefaultValuatedNodes(), 0u);
  CPPUNIT_ASSERT_EQUAL(prop->numberOfNonDefaultValuatedEdges(), 0u);

  // check that the default property value has been correctly modified
  CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getNodeDefaultValue(), v1, 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getEdgeDefaultValue(), v2, 1e-6);

  // change the default node value for future added nodes
  prop->setNodeDefaultValue(v2);
  // change the default edge value for future added edges
  prop->setEdgeDefaultValue(v1);

  // check number of non default valuated edges (not 0 anymore)
  CPPUNIT_ASSERT_EQUAL(prop->numberOfNonDefaultValuatedNodes(), graph->numberOfNodes());
  CPPUNIT_ASSERT_EQUAL(prop->numberOfNonDefaultValuatedEdges(), graph->numberOfEdges());

  // check that the default property value has been correctly modified
  CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getNodeDefaultValue(), v2, 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getEdgeDefaultValue(), v1, 1e-6);

  // add a new node
  node nNew = graph->addNode();
  // add a new edge
  edge eNew = graph->addEdge(graph->getRandomNode(), graph->getRandomNode());

  // check that the new default property value has been correctly set
  CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getNodeValue(nNew), v2, 1e-6);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getEdgeValue(eNew), v1, 1e-6);

  // check that original nodes and edges still have the same value
  // as before modifying the default property value
  node n;
  forEach(n, graph->getNodes()) {
    if (n != nNew) {
      CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getNodeValue(n), v1, 1e-6);
    }
  }
  edge e;
  forEach(e, graph->getEdges()) {
    if (e != eNew) {
      CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getEdgeValue(e), v2, 1e-6);
    }
  }

  // check if there is no graph push/pop side effect when setting the new default value
  // on a node that already has it
  graph->push();
  prop->setNodeValue(n1, v2);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getNodeValue(n1), v2, 1e-6);
  graph->pop();
  CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getNodeValue(n1), v1, 1e-6);
  graph->unpop();
  CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getNodeValue(n1), v2, 1e-6);

  // check that after pushing a graph, adding a new node and changing the default property value
  // the node property value gets restored to the default value of the property the time the node
  // was created
  double v3 = tlp::randomDouble();
  // push graph state
  graph->push();
  // add a node, its property value should be v2
  node newNode = graph->addNode();
  CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getNodeValue(newNode), v2, 1e-6);
  // change the default property value to v3
  prop->setNodeDefaultValue(v3);
  // pop graph state
  graph->pop();
  // unpop graph state
  graph->unpop();
  // node value should be v2
  CPPUNIT_ASSERT_DOUBLES_EQUAL(prop->getNodeValue(newNode), v2, 1e-6);
}
