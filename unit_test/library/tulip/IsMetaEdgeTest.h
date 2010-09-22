#include <string>
#include <tulip/Graph.h>
#include <tulip/Matrix.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

class IsMetaEdgeTest : public CppUnit::TestFixture {

public:

  void setUp();
  void tearDown();
  void testIsMetaEdge();
  static CppUnit::Test *suite();

private:
	
  tlp::Graph *graph;
  tlp::Graph *quotientGraph;	

};
