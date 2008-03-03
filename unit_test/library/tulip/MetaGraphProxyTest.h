#ifndef TLPMETAGRAPHPROXYTEST
#define TLPMETAGRAPHPROXYTEST

#include <string>
#include <tulip/GraphProperty.h>
#include <tulip/Graph.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

// Warning MetaGraphProxy has been renamed in GraphAlgorithm

class MetaGraphProxyTest : public CppUnit::TestFixture {
private:
  tlp::Graph *graph;
  tlp::GraphProperty *metaGraph;

public:
  void setUp();
  void tearDown();
  void testDestroyGraph();
  void testSetGet();
  void testSetAll();
  static CppUnit::Test *suite();
};

#endif
