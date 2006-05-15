#ifndef TLPBIEXTCLUSTEROPERATION
#define TLPBIEXTCLUSTEROPERATION

#include <string>
#include <tulip/Graph.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

class ExtendedClusterOperationTest : public CppUnit::TestFixture {
private:
  Graph *graph,*quotient;
  node meta;
  std::vector<node> nodes;
  std::vector<edge> edges;
  std::set<node> group;
public:
  void setUp();
  void tearDown();
  void testCreateMetaNode();
  //  void testExtractNodeFromMetaNode();
  //  void testAddNodeToMetaNode();
  void testOpenMetaNode();
  static CppUnit::Test *suite();
};

#endif
