#ifndef TLPTESTALGORITHMTEST
#define TLPTESTALGORITHMTEST


#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

namespace tlp {
class Graph;
}

class TestAlgorithmTest : public CppUnit::TestFixture {
private:
  tlp::Graph *graph;

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
