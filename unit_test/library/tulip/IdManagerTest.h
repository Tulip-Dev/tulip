#ifndef TLPBIEXTCLUSTEROPERATION
#define TLPBIEXTCLUSTEROPERATION

#include <string>
#include <tulip/Graph.h>
#include <tulip/IdManager.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

namespace tlp {

class IdManagerTest : public CppUnit::TestFixture {

public:
  void setUp() {idManager = new IdManager(); }
  void tearDown(){ delete idManager;}
  void testIsFree();
  void testFragmentation();
  void testGetFree();

  static CppUnit::Test *suite();

private:
  IdManager *idManager;
};

}
#endif
