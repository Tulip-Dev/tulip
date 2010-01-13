#ifndef TLPSUPERGRAPHTEST
#define TLPSUPERGRAPHTEST

#include <tulip/Graph.h>
#include <tulip/TlpTools.h>
#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestSuite.h>

class SuperGraphTest : public CppUnit::TestFixture {
private:
  tlp::Graph *graph;

public:
  void setUp();
  void tearDown();

  void testAddDel();
  void testClear();
  void testOrderEdgeAndSwap();
  void testSubgraph();
  void testSubgraphId();
  void testDeleteSubgraph();
  void testInheritance();
  void testIterators();
  void testPropertiesIteration();
  void testDegree();
  void testAttributes();
  void testGetNodesEqualTo();

  static CppUnit::Test *suite();
 private:
  void build(unsigned int, unsigned int);
};

#endif
