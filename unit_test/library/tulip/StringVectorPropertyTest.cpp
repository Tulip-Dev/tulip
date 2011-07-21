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
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <tulip/Graph.h>
#include <tulip/TlpTools.h>
#include <tulip/StringProperty.h>

using namespace std;
using namespace tlp;

#include <cppunit/extensions/HelperMacros.h>

class StringVectorPropertyTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(StringVectorPropertyTest);
  CPPUNIT_TEST(simpleTest);
  CPPUNIT_TEST(simpleDavidTest);
  CPPUNIT_TEST(complexTest);
  CPPUNIT_TEST(complexDavidTest);
  CPPUNIT_TEST_SUITE_END();
private:
  tlp::Graph *graph;

public:
  void setUp();
  void tearDown();

  void simpleTest();
  void simpleDavidTest();
  void complexTest();
  void complexDavidTest();
};

CPPUNIT_TEST_SUITE_REGISTRATION( StringVectorPropertyTest );

//==========================================================
void StringVectorPropertyTest::setUp() {
  graph = tlp::newGraph();
}
//==========================================================
void StringVectorPropertyTest::tearDown() {
  //graph->pop();
  delete graph;
}

//==========================================================
void StringVectorPropertyTest::simpleTest() {
  node n = graph->addNode();
  StringVectorProperty *stP = graph->getProperty<StringVectorProperty>("data_1");
  vector<string> tmp;

  for (unsigned int i=0; i<5; ++i) {
    stringstream tmpstr;
    tmpstr << "fdjlksjflksjl : " << i;
    tmp.push_back(tmpstr.str());
  }

  stP->setNodeValue(n, tmp);

  const vector<string>& value = stP->getNodeValue(n);
  CPPUNIT_ASSERT_EQUAL(size_t(5), value.size());

  for(unsigned int i=0; i< value.size(); ++i) {
    CPPUNIT_ASSERT_EQUAL(tmp[i], value[i]);
  }
}

//==========================================================
void StringVectorPropertyTest::simpleDavidTest() {
  node n = graph->addNode();
  StringVectorProperty *stP = graph->getProperty<StringVectorProperty>("stp");
  vector<string> tmp;

  for (unsigned int i=0; i<5; ++i) {
    stringstream tmpstr;
    tmpstr << "fdjlksjflks\"jl : " << i;
    tmp.push_back(tmpstr.str());
  }

  stP->setNodeValue(n, tmp);

  const vector<string>& value = stP->getNodeValue(n);
  CPPUNIT_ASSERT_EQUAL(size_t(5), value.size());

  for(unsigned int i=0; i< value.size(); ++i) {
    CPPUNIT_ASSERT_EQUAL(tmp[i], value[i]);
  }
}

//==========================================================
void StringVectorPropertyTest::complexTest() {
  node n = graph->addNode();
  StringVectorProperty *stP = graph->getProperty<StringVectorProperty>("stp");
  vector<string> tmp;

  for (unsigned int i=0; i<5000; ++i) {
    stringstream tmpstr;
    tmpstr << "fdjlksjflksjl : " << i;
    tmp = stP->getNodeValue(n);
    CPPUNIT_ASSERT(tmp.size() == i);
    tmp.push_back(tmpstr.str());
    stP->setNodeValue(n, tmp);
  }

  const vector<string>& value = stP->getNodeValue(n);

  CPPUNIT_ASSERT_EQUAL(size_t(5000), value.size());

  for(unsigned int i=0; i< value.size(); ++i) {
    CPPUNIT_ASSERT_EQUAL(tmp[i], value[i]);
  }
}
//==========================================================
void StringVectorPropertyTest::complexDavidTest() {
  node n = graph->addNode();
  StringVectorProperty *stP = graph->getProperty<StringVectorProperty>("stp");
  vector<string> expectedValues;
  vector<string> tmp;

  for (unsigned int i=0; i<5000; ++i) {
    stringstream tmpstr;
    tmpstr << "fdjlksjflks\"jl : " << i;
    tmp = stP->getNodeValue(n);
    CPPUNIT_ASSERT_EQUAL(size_t(i), tmp.size());
    tmp.push_back(tmpstr.str());
    expectedValues.push_back(tmpstr.str());
    stP->setNodeValue(n, tmp);
  }

  const vector<string>& value = stP->getNodeValue(n);

  CPPUNIT_ASSERT_EQUAL(expectedValues.size(), value.size());

  for(unsigned int i=0; i< value.size(); ++i) {
    CPPUNIT_ASSERT_EQUAL(value[i], expectedValues[i]);
  }
}
