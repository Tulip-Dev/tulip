#ifndef TLPPUSHPOPTEST
#define TLPPUSHPOPTEST

#include <tulip/Graph.h>
#include <tulip/TlpTools.h>
#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestSuite.h>

class PushPopTest : public CppUnit::TestFixture {
private:
  tlp::Graph *graph;

public:
  void setUp();
  void tearDown();

  void testAddDel();
  void testSetValue();
  void testSubgraph();
  void testTests();
  void testAddDelProps();
  /*void testOrderEdgeAndSwap();
  void testSubgraphId();
  void testDeleteSubgraph();
  void testInheritance();
  void testIterators();
  void testPropertiesIteration();
  void testDegree(); */

  static CppUnit::Test *suite();
 private:
  void build(unsigned int, unsigned int);
};

#endif
