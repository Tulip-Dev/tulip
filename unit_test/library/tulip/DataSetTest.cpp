#include <cassert>
#include <iomanip>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include "DataSetTest.h"

using namespace std;
using namespace tlp;

#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION( DataSetTest );

//==========================================================
void DataSetTest::testSetGetBool() {
  DataSet dSet;
  CPPUNIT_ASSERT_EQUAL(false, dSet.exist("bool"));

  bool v1,v2;
  v1 = true;
  CPPUNIT_ASSERT_EQUAL(false, dSet.get("bool", v1));

  dSet.set("bool", v1);
  CPPUNIT_ASSERT_EQUAL(true, dSet.get("bool", v2));

  CPPUNIT_ASSERT_EQUAL(v1, v2);
}

//==========================================================
void DataSetTest::testSetGetDouble() {
  DataSet dSet;
  CPPUNIT_ASSERT_EQUAL(false, dSet.exist("double"));

  double v1,v2;
  v1 = 1.1;
  CPPUNIT_ASSERT_EQUAL(false, dSet.get("double", v1));

  dSet.set("double", v1);
  CPPUNIT_ASSERT_EQUAL(true, dSet.get("double", v2));

  CPPUNIT_ASSERT_EQUAL(v1, v2);
}

//==========================================================
void DataSetTest::testSetGetInt() {
  DataSet dSet;
  CPPUNIT_ASSERT_EQUAL(false, dSet.exist("int"));

  int v1,v2;
  v1 = 1;
  CPPUNIT_ASSERT_EQUAL(false, dSet.get("int", v1));

  dSet.set("int", v1);
  CPPUNIT_ASSERT_EQUAL(true, dSet.get("int", v2));

  CPPUNIT_ASSERT_EQUAL(v1, v2);
}

//==========================================================
void DataSetTest::testSetGetString() {
  DataSet dSet;
  CPPUNIT_ASSERT_EQUAL(false, dSet.exist("string"));

  std::string v1,v2;
  v1 = "test";
  CPPUNIT_ASSERT_EQUAL(false, dSet.get("string", v1));

  dSet.set("string", v1);
  CPPUNIT_ASSERT_EQUAL(true, dSet.get("string", v2));

  CPPUNIT_ASSERT_EQUAL(v1, v2);
}

//==========================================================
struct Struct {
  static unsigned int nbInstances;

  std::string value;

  Struct& operator=(const Struct &st) {
    value = st.value;
    return *this;
  }
      
  Struct() {
    ++nbInstances;
  }

  Struct(const std::string& val) {
    ++nbInstances;
    value = val;
  }

  ~Struct() {
    --nbInstances;
  }

  Struct(const Struct &st) {
    ++nbInstances;
    value = st.value;
  }
};

unsigned int Struct::nbInstances = 0;

void DataSetTest::testSetGetStruct() {
  DataSet dSet;
  CPPUNIT_ASSERT_EQUAL(false, dSet.exist("struct"));

  Struct v1("test"), v2;

  unsigned int nbInstances = 2;
  CPPUNIT_ASSERT_EQUAL(Struct::nbInstances, nbInstances);
  CPPUNIT_ASSERT(!v1.value.empty());
  CPPUNIT_ASSERT(v2.value.empty());
  
  CPPUNIT_ASSERT_EQUAL(false, dSet.get("struct", v1));

  dSet.set("struct", v1);
  ++nbInstances;
  CPPUNIT_ASSERT_EQUAL(Struct::nbInstances, nbInstances);

  CPPUNIT_ASSERT_EQUAL(true, dSet.get("struct", v2));
  CPPUNIT_ASSERT(v1.value == v2.value);
}


//==========================================================
CppUnit::Test * DataSetTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : DataSet" );
  suiteOfTests->addTest( new CppUnit::TestCaller<DataSetTest>( "setGetBool", 
							      &DataSetTest::testSetGetBool ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<DataSetTest>( "setGetDouble", 
							      &DataSetTest::testSetGetDouble ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<DataSetTest>( "setGetInt", 
							      &DataSetTest::testSetGetInt ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<DataSetTest>( "setGetString", 
							      &DataSetTest::testSetGetString ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<DataSetTest>( "setGetStruct", 
							      &DataSetTest::testSetGetStruct ) );
  return suiteOfTests;
}
//==========================================================
