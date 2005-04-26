#ifndef TLPPLUGINSTEST
#define TLPPLUGINSTEST

#include <string>
#include <tulip/SuperGraph.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

class PluginsTest : public CppUnit::TestFixture {
private:
  SuperGraph *graph;

public:
  void setUp();
  void tearDown();
  void testloadPlugin();
  void testCircularPlugin();
  void testAncestorGraph();
  static CppUnit::Test *suite();
};

#endif
