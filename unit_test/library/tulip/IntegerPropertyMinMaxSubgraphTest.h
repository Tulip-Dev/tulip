#ifndef INTEGERPROPERTYMINMAXSUBGRAPHTEST_H_
#define INTEGERPROPERTYMINMAXSUBGRAPHTEST_H_

#include <tulip/Graph.h>
#include <tulip/IntegerProperty.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

class IntegerPropertyMinMaxSubgraphTest : public CppUnit::TestFixture {
 public:
    
  void setUp();
  void tearDown();
  void testIntegerPropertyRootGraphMin();
  void testIntegerPropertyRootGraphMax();
  void testIntegerPropertySubgraphMin();
  void testIntegerPropertySubgraphMax();

  static CppUnit::Test *suite();

 private :

  tlp::Graph *graph, *subGraph;

};

#endif /* INTEGERPROPERTYMINMAXSUBGRAPHTEST_H_ */
