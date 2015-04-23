/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include <string>
#include <tulip/Graph.h>
#include <tulip/DataSet.h>
#include <tulip/StringCollection.h>
#include <tulip/Coord.h>
#include <tulip/Size.h>
#include <tulip/Color.h>

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

// Test all DataSet supported serializable types
void DataSetTest::testDataSetSerialization() {

  // DataSet to serialize
  DataSet dataSet;

  // std::string
  const string str = "tulip";
  dataSet.set("string", str);

  // bool
  const bool b = true;
  dataSet.set("bool", b);

  // double
  const double d = 3.14;
  dataSet.set("double", d);

  // float
  const float f = 0.2f;
  dataSet.set("float", f);

  // int
  const int i = 33;
  dataSet.set("int", i);

  // unsigned int
  const unsigned int ui = 256;
  dataSet.set("unsigned int", ui);

  // long
  const long l = 56845725;
  dataSet.set("long", l);

  // tlp::Coord
  const Coord c(1,2,3);
  dataSet.set("Coord", c);

  // tlp::Color
  const Color col = Color::Azure;
  dataSet.set("Color", col);

  // tlp::Size
  const Size s(4,5,6);
  dataSet.set("Size", s);

  // std::vector<std::string>
  vector<string> vs;
  vs.push_back("foo");
  vs.push_back("bar");
  dataSet.set("vector<string>", vs);

  // std::vector<double>
  vector<double> vd;
  vd.push_back(12.56);
  vd.push_back(45.85);
  dataSet.set("vector<double>", vd);

  // std::vector<int>
  vector<int> vi;
  vi.push_back(45);
  vi.push_back(-120);
  dataSet.set("vector<int>", vi);

  // std::vector<bool>
  vector<bool> vb;
  vb.push_back(true);
  vb.push_back(false);
  dataSet.set("vector<bool>", vb);

  // std::vector<tlp::Coord>
  vector<Coord> vc;
  vc.push_back(c);
  vc.push_back(Coord(10,20,30));
  dataSet.set("vector<Coord>", vc);

  // std::set<tlp::edge>, std::vector<tlp::edge>, std::vector<tlp::node>
  set<edge> se;
  vector<edge> ve;
  vector<node> vn;
  for (unsigned int i = 0 ; i < 100 ; ++i) {
    vn.push_back(node(i));
    ve.push_back(edge(i));
    se.insert(edge(i));
  }
  dataSet.set("vector<node>", vn);
  dataSet.set("vector<edge>", ve);
  dataSet.set("set<edge>", se);

  // tlp::node
  node n = node(5);
  dataSet.set("node", n);

  // tlp::edge
  edge e = edge(6);
  dataSet.set("edge", e);

  // tlp::StringCollection
  StringCollection sc;
  sc.push_back("foo");
  sc.push_back("bar");
  dataSet.set("StringCollection", sc);

  // serialize the DataSet
  ostringstream oss;
  DataSet::write(oss, dataSet);

  // import the serialized DataSet in a new one
  DataSet dataSet2;
  istringstream iss(oss.str());
  CPPUNIT_ASSERT(DataSet::read(iss, dataSet2));

  // check that imported values are equal to serialized ones

  string str2;
  CPPUNIT_ASSERT(dataSet2.get("string", str2));
  CPPUNIT_ASSERT_EQUAL(str, str2);

  bool b2 = false;
  CPPUNIT_ASSERT(dataSet2.get("bool", b2));
  CPPUNIT_ASSERT_EQUAL(b, b2);

  double d2 = 0;
  CPPUNIT_ASSERT(dataSet2.get("double", d2));
  CPPUNIT_ASSERT_EQUAL(d, d2);

  float f2 = 0;
  CPPUNIT_ASSERT(dataSet2.get("float", f2));
  CPPUNIT_ASSERT_EQUAL(f, f2);

  int i2 = 0;
  CPPUNIT_ASSERT(dataSet2.get("int", i2));
  CPPUNIT_ASSERT_EQUAL(i, i2);

  unsigned int ui2 = 0;
  CPPUNIT_ASSERT(dataSet2.get("unsigned int", ui2));
  CPPUNIT_ASSERT_EQUAL(ui, ui2);

  long l2;
  CPPUNIT_ASSERT(dataSet2.get("long", l2));
  CPPUNIT_ASSERT_EQUAL(l, l2);

  Coord c2;
  CPPUNIT_ASSERT(dataSet2.get("Coord", c2));
  CPPUNIT_ASSERT_EQUAL(c, c2);

  Size s2;
  CPPUNIT_ASSERT(dataSet2.get("Size", s2));
  CPPUNIT_ASSERT_EQUAL(s, s2);

  node n2;
  CPPUNIT_ASSERT(dataSet2.get("node", n2));
  CPPUNIT_ASSERT_EQUAL(n, n2);

  edge e2;
  CPPUNIT_ASSERT(dataSet2.get("edge", e2));
  CPPUNIT_ASSERT_EQUAL(e, e2);

  vector<string> vs2;
  CPPUNIT_ASSERT(dataSet2.get("vector<string>", vs2));
  CPPUNIT_ASSERT(vs == vs2);

  vector<double> vd2;
  CPPUNIT_ASSERT(dataSet2.get("vector<double>", vd2));
  CPPUNIT_ASSERT(vd == vd2);

  vector<int> vi2;
  CPPUNIT_ASSERT(dataSet2.get("vector<int>", vi2));
  CPPUNIT_ASSERT(vi == vi2);

  vector<bool> vb2;
  CPPUNIT_ASSERT(dataSet2.get("vector<bool>", vb2));
  CPPUNIT_ASSERT(vb == vb2);

  vector<Coord> vc2;
  CPPUNIT_ASSERT(dataSet2.get("vector<Coord>", vc2));
  CPPUNIT_ASSERT(vc == vc2);

  vector<node> vn2;
  CPPUNIT_ASSERT(dataSet2.get("vector<node>", vn2));
  CPPUNIT_ASSERT(vn == vn2);

  vector<edge> ve2;
  CPPUNIT_ASSERT(dataSet2.get("vector<edge>", ve2));
  CPPUNIT_ASSERT(ve == ve2);

  set<edge> se2;
  CPPUNIT_ASSERT(dataSet2.get("set<edge>", se2));
  CPPUNIT_ASSERT(se == se2);

  StringCollection sc2;
  CPPUNIT_ASSERT(dataSet2.get("StringCollection", sc2));
  CPPUNIT_ASSERT(sc.getValues() == sc2.getValues());

}
