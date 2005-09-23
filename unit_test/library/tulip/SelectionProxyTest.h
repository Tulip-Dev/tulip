#ifndef TLPSELECTIONPROXYTEST
#define TLPSELECTIONPROXYTEST

#include <string>
#include <tulip/SelectionProxy.h>
#include <tulip/SuperGraph.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

class SelectionProxyTest : public CppUnit::TestFixture {
private:
  SuperGraph *graph;
  SelectionProxy *selection;

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
