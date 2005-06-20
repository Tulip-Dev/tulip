#ifndef TLPBIEXTCLUSTEROPERATION
#define TLPBIEXTCLUSTEROPERATION

#include <string>
#include <tulip/SuperGraph.h>
#include <tulip/IdManager.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

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

#endif
