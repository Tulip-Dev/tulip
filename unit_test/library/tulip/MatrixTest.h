#ifndef TLPBIEXTCLUSTEROPERATION
#define TLPBIEXTCLUSTEROPERATION

#include <string>
#include <tulip/SuperGraph.h>
#include <tulip/Matrix.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

class MatrixTest : public CppUnit::TestFixture {

public:
  void setUp() {}
  void tearDown() {}
  void testExternalOperation();
  void testInternalOperation();
  static CppUnit::Test *suite();
};

#endif
