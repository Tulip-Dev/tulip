#ifndef TLPBICONNEXTESTTEST
#define TLPBICONNEXTESTTEST


#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

class SuperGraph;

class TestAlgorithmTest : public CppUnit::TestFixture {
private:
  SuperGraph *graph;

public:
  void setUp();
  void tearDown();
  void testSimple();
  void testTree();
  void testAcyclic();
  void testConnected();
  void testBiconnected();
  void testTriconnected();
  static CppUnit::Test *suite();

};

#endif
