#ifndef TLPMETAGRAPHPROXYTEST
#define TLPMETAGRAPHPROXYTEST

#include <string>
#include <tulip/MetaGraphProxy.h>
#include <tulip/SuperGraph.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

class MetaGraphProxyTest : public CppUnit::TestFixture {
private:
  SuperGraph *graph;
  MetaGraphProxy *metaGraph;

public:
  void setUp();
  void tearDown();
  void testDestroyGraph();
  void testSetGet();
  void testSetAll();
  static CppUnit::Test *suite();
};

#endif
