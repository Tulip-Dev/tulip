#ifndef TLPOBSERVABLEGRAPHTEST
#define TLPOBSERVABLEGRAPHTEST

#include <tulip/Graph.h>
#include <tulip/TlpTools.h>
#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestSuite.h>

class ObservableGraphTest : public CppUnit::TestFixture {
private:
  tlp::Graph *graph;

public:
  void setUp();
  void tearDown();

  void testAddDel();
  void testClear();
  void testOrderEdgeAndSwap();
  void testSubgraph();
  void testDeleteSubgraph();
  void testReverse();
  void testObserverWhenRemoveObservable();

  static CppUnit::Test *suite();
};

#endif
