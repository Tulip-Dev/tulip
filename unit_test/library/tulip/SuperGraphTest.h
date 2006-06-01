#ifndef TLPSUPERGRAPHTEST
#define TLPSUPERGRAPHTEST

#include <tulip/Graph.h>
#include <tulip/TlpTools.h>
#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestSuite.h>

class SuperGraphTest : public CppUnit::TestFixture {
  /*
  CPPUNIT_TEST_SUITE( SuperGraphTest );
  CPPUNIT_TEST( testAddDel );
  CPPUNIT_TEST( testClear );
  CPPUNIT_TEST( testOrderEdgeAndSwap );
  CPPUNIT_TEST( testSubgraph);
  CPPUNIT_TEST( testDeleteSubgraph);
  CPPUNIT_TEST( testInheritance);
  CPPUNIT_TEST( testPropertiesIteration);
  CPPUNIT_TEST( testDegree);
  CPPUNIT_TEST_SUITE_END();
  */
private:
  Graph *graph;

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

  static CppUnit::Test *suite();
 private:
  void build(unsigned int, unsigned int);
};

#endif
