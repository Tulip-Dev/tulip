#ifndef TLPOBSERVABLEPROPERTYTEST
#define TLPOSERVABLEPROPERTYTEST

#include <tulip/Graph.h>
#include <tulip/TlpTools.h>
#include <tulip/AbstractProperty.h>
#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/TestSuite.h>

class ObservablePropertyTest : public CppUnit::TestFixture {
 private:
  tlp::Graph *graph;
  tlp::PropertyInterface *props[7];

public:
  void setUp();
  void tearDown();

  void testAddObserver();
  void testSynchronousSetNodeValue();
  void testAsynchronousSetNodeValue();
  void testSynchronousSetAllNodeValue();
  void testAsynchronousSetAllNodeValue();
  void testSynchronousSetEdgeValue();
  void testAsynchronousSetEdgeValue();
  void testSynchronousSetAllEdgeValue();
  void testAsynchronousSetAllEdgeValue();
  void testSynchronousDelete();
  void testAsynchronousDelete();
  void testRemoveObserver();
  void testObserverWhenRemoveObservable();

  void setNodeValue(tlp::PropertyInterface*, char*, bool, bool, bool = true);
  void setEdgeValue(tlp::PropertyInterface*, char*, bool, bool, bool = true);
  void addObservers();

  static CppUnit::Test *suite();
};

#endif
