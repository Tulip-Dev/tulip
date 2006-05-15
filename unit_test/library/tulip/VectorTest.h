#ifndef TLPBIEXTCLUSTEROPERATION
#define TLPBIEXTCLUSTEROPERATION

#include <string>
#include <tulip/Graph.h>
#include <tulip/Vector.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

class VectorTest : public CppUnit::TestFixture {

public:
  void setUp() {}
  void tearDown() {}
  void testExternalOperation();
  void testInternalOperation();
  static CppUnit::Test *suite();
};

#endif
