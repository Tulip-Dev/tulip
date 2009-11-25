#include <cppunit/extensions/HelperMacros.h>

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
  CPPUNIT_ASSERT_MESSAGE("test IntegerProperty min value on root graph", intProperty->getNodeMin() == rootGraphMin);
}

void IntegerPropertyMinMaxSubgraphTest::testIntegerPropertyRootGraphMax() {
  IntegerProperty *intProperty = graph->getProperty<IntegerProperty>(integerPropertyName);
  CPPUNIT_ASSERT_MESSAGE("test IntegerProperty max value on root graph", intProperty->getNodeMax() == rootGraphMax);
}

void IntegerPropertyMinMaxSubgraphTest::testIntegerPropertySubgraphMin() {
  IntegerProperty *intProperty = subGraph->getProperty<IntegerProperty>(integerPropertyName);
  CPPUNIT_ASSERT_MESSAGE("test IntegerProperty min value on subgraph graph", intProperty->getNodeMin(subGraph) == subgraphMin);
}

void IntegerPropertyMinMaxSubgraphTest::testIntegerPropertySubgraphMax() {
  IntegerProperty *intProperty = subGraph->getProperty<IntegerProperty>(integerPropertyName);
  CPPUNIT_ASSERT_MESSAGE("test IntegerProperty max value on subgraph", intProperty->getNodeMax(subGraph) == subgraphMax);
}

CppUnit::Test *IntegerPropertyMinMaxSubgraphTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : Integer Property min max on subgraph" );

  suiteOfTests->addTest(new CppUnit::TestCaller<IntegerPropertyMinMaxSubgraphTest>("Integer Property min on root graph", &IntegerPropertyMinMaxSubgraphTest::testIntegerPropertyRootGraphMin));
  suiteOfTests->addTest(new CppUnit::TestCaller<IntegerPropertyMinMaxSubgraphTest>("Integer Property max on root graph", &IntegerPropertyMinMaxSubgraphTest::testIntegerPropertyRootGraphMax));
  suiteOfTests->addTest(new CppUnit::TestCaller<IntegerPropertyMinMaxSubgraphTest>("Integer Property min on subgraph", &IntegerPropertyMinMaxSubgraphTest::testIntegerPropertySubgraphMin));
  suiteOfTests->addTest(new CppUnit::TestCaller<IntegerPropertyMinMaxSubgraphTest>("Integer Property max on subgraph", &IntegerPropertyMinMaxSubgraphTest::testIntegerPropertySubgraphMax));

  return suiteOfTests;
}
