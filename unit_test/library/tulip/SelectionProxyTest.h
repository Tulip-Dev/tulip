#ifndef TLPSELECTIONPROXYTEST
#define TLPSELECTIONPROXYTEST

#include <string>
#include <tulip/BooleanProperty.h>
#include <tulip/SuperGraph.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

// Warning SelectionProxy has been renamed in Selection

class SelectionProxyTest : public CppUnit::TestFixture {
private:
  SuperGraph *graph;
  Selection *selection;

public:
  void setUp();
  void tearDown();
  void testSetAll();
  void testSetAll(bool);
  void testSetGet();
  void testSetGet(bool);
  void testCopy();
  void testIterators();
  static CppUnit::Test *suite();
};

#endif
