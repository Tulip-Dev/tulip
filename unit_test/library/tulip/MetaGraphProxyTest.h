#ifndef TLPMETAGRAPHPROXYTEST
#define TLPMETAGRAPHPROXYTEST

#include <string>
#include <tulip/GraphProperty.h>
#include <tulip/SuperGraph.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

// Warning MetaGraphProxy has been renamed in MetaGraph

class MetaGraphProxyTest : public CppUnit::TestFixture {
private:
  SuperGraph *graph;
  MetaGraph *metaGraph;

public:
  void setUp();
  void tearDown();
  void testDestroyGraph();
  void testSetGet();
  void testSetAll();
  static CppUnit::Test *suite();
};

#endif
