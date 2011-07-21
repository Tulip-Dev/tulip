/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include <cassert>
#include <iomanip>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include "DataSetTest.h"

using namespace std;
using namespace tlp;

CPPUNIT_TEST_SUITE_REGISTRATION( DataSetTest );

//==========================================================
void DataSetTest::testSetGetBool() {
  DataSet dSet;
  CPPUNIT_ASSERT(!dSet.exist("bool"));

  bool v1,v2;
  v1 = true;
  CPPUNIT_ASSERT(!dSet.get("bool", v1));

  dSet.set("bool", v1);
  CPPUNIT_ASSERT(dSet.get("bool", v2));

  CPPUNIT_ASSERT_EQUAL(v1, v2);
}

//==========================================================
void DataSetTest::testSetGetDouble() {
  DataSet dSet;
  CPPUNIT_ASSERT(!dSet.exist("double"));

  double v1,v2;
  v1 = 1.1;
  CPPUNIT_ASSERT(!dSet.get("double", v1));

  dSet.set("double", v1);
  CPPUNIT_ASSERT(dSet.get("double", v2));

  CPPUNIT_ASSERT_EQUAL(v1, v2);
}

//==========================================================
void DataSetTest::testSetGetInt() {
  DataSet dSet;
  CPPUNIT_ASSERT(!dSet.exist("int"));

  int v1,v2;
  v1 = 1;
  CPPUNIT_ASSERT(!dSet.get("int", v1));

  dSet.set("int", v1);
  CPPUNIT_ASSERT(dSet.get("int", v2));

  CPPUNIT_ASSERT_EQUAL(v1, v2);
}

//==========================================================
void DataSetTest::testSetGetString() {
  DataSet dSet;
  CPPUNIT_ASSERT(!dSet.exist("string"));

  std::string v1,v2;
  v1 = "test";
  CPPUNIT_ASSERT(!dSet.get("string", v1));

  dSet.set("string", v1);
  CPPUNIT_ASSERT(dSet.get("string", v2));

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
  CPPUNIT_ASSERT(!dSet.exist("struct"));

  Struct v1("test"), v2;

  unsigned int nbInstances = 2;
  CPPUNIT_ASSERT_EQUAL(nbInstances, Struct::nbInstances);
  CPPUNIT_ASSERT(!v1.value.empty());
  CPPUNIT_ASSERT(v2.value.empty());

  CPPUNIT_ASSERT(!dSet.get("struct", v1));

  dSet.set("struct", v1);
  ++nbInstances;
  CPPUNIT_ASSERT_EQUAL(nbInstances, Struct::nbInstances);

  CPPUNIT_ASSERT(dSet.get("struct", v2));
  CPPUNIT_ASSERT_EQUAL(v1.value, v2.value);
}
