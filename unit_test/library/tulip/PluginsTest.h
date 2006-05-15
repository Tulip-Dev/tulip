#ifndef TLPPLUGINSTEST
#define TLPPLUGINSTEST

#include <string>
#include <tulip/Graph.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

class PluginsTest : public CppUnit::TestFixture {
private:
  Graph *graph;

public:
  void setUp();
  void tearDown();
  void testloadPlugin();
  void testCircularPlugin();
  void testAncestorGraph();
  static CppUnit::Test *suite();
};

#endif
