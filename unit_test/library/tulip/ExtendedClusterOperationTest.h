#ifndef TLPBIEXTCLUSTEROPERATION
#define TLPBIEXTCLUSTEROPERATION

#include <string>
#include <tulip/Graph.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

class ExtendedClusterOperationTest : public CppUnit::TestFixture {
private:
  tlp::Graph *graph,*quotient;
  tlp::node meta;
  std::vector<tlp::node> nodes;
  std::vector<tlp::edge> edges;
  std::set<tlp::node> group;
public:
  void setUp();
  void tearDown();
  void testCreateMetaNode();
  void testBugOpenInSubgraph();
  //  void testExtractNodeFromMetaNode();
  //  void testAddNodeToMetaNode();
  void testOpenMetaNode();
  void testOpenMetaNodes();
  static CppUnit::Test *suite();
};

#endif
