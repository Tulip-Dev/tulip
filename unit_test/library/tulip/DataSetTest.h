#ifndef DATASETTEST
#define DATASETTEST

#include <string>
#include <tulip/Graph.h>
#include "../../../library/tulip/include/tulip/Reflect.h"
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

namespace tlp {

class DataSetTest : public CppUnit::TestFixture {

public:
  void setUp() {}
  void tearDown() {}
  void testSetGetBool();
  void testSetGetDouble();
  void testSetGetInt();
  void testSetGetString();
  void testSetGetStruct();
  static CppUnit::Test *suite();
};

}
#endif
