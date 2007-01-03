#include <cassert>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include "IdManagerTest.h"

using namespace std;
using namespace tlp;

#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION( IdManagerTest );

//==========================================================
void IdManagerTest::testFragmentation() {
  for (unsigned int i = 0; i <1000; ++i) {
    CPPUNIT_ASSERT_EQUAL(i, idManager->get());
  }
  for (unsigned int i = 1; i <100; ++i) {
    idManager->free(i);
  }
  CPPUNIT_ASSERT_EQUAL((size_t) 99, idManager->freeIds.size());
  idManager->free(0);
  CPPUNIT_ASSERT_EQUAL((size_t) 0, idManager->freeIds.size());

  for (unsigned int i = 900; i <999; ++i) {
    idManager->free(i);
  }
  CPPUNIT_ASSERT_EQUAL((size_t) 99, idManager->freeIds.size());
  idManager->free(999);
  CPPUNIT_ASSERT_EQUAL((size_t) 0, idManager->freeIds.size());
}
//==========================================================
void IdManagerTest::testGetFree() {
  for (unsigned int i = 0; i <1000; ++i) {
    CPPUNIT_ASSERT_EQUAL(i, idManager->get());
  }
  for (unsigned int i = 0; i <500; ++i) {
    idManager->free(i*2);
  }
  for (unsigned int i = 1; i < 500; ++i) {
    CPPUNIT_ASSERT_EQUAL(i * 2u, idManager->get());
  }
  CPPUNIT_ASSERT_EQUAL(0u, idManager->get());
  for (unsigned int i = 100; i <= 200; ++i) {
    idManager->free(i);
  }
  for (unsigned int i = 100; i <= 200; ++i) {
    CPPUNIT_ASSERT_EQUAL(i , idManager->get());
  }
}
//==========================================================
void IdManagerTest::testIsFree() {
  for (unsigned int i = 0; i <1000; ++i) {
     idManager->get();
  }
  for (unsigned int i = 0; i <500; ++i) {
    idManager->free(i*2);
  }
  for (unsigned int i = 0; i < 500; ++i) {
    CPPUNIT_ASSERT_EQUAL( true , idManager->is_free(i * 2));
    CPPUNIT_ASSERT_EQUAL( false, idManager->is_free(i * 2 + 1));
  }
  CPPUNIT_ASSERT_EQUAL( true , idManager->is_free(1200));
}
//==========================================================
CppUnit::Test * IdManagerTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : IdManager" );
  suiteOfTests->addTest( new CppUnit::TestCaller<IdManagerTest>( "Is Free test", 
							      &IdManagerTest::testIsFree ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<IdManagerTest>( "Get / Free Id", 
							      &IdManagerTest::testGetFree ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<IdManagerTest>( "Fragmentation", 
							      &IdManagerTest::testFragmentation ) );
  return suiteOfTests;
}
//==========================================================
