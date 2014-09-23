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

#include "stringpropertytest.h"

#include <tulip/Graph.h>
#include <tulip/StringProperty.h>

using namespace std;
using namespace tlp;

CPPUNIT_TEST_SUITE_REGISTRATION(StringPropertyTest);

void StringPropertyTest::setUp() {
  graph = tlp::newGraph();
}

void StringPropertyTest::tearDown() {
  delete graph;
}

void StringPropertyTest::simpleVectorTest() {
  node n = graph->addNode();
  StringVectorProperty *vectorProperty = graph->getProperty<StringVectorProperty>("data_1");
  vector<string> tmp;

  for (unsigned int i=0; i<5; ++i) {
    stringstream tmpstr;
    tmpstr << "\"test string #\"  : " << i;
    tmp.push_back(tmpstr.str());
  }

  vectorProperty->setNodeValue(n, tmp);

  const vector<string>& value = vectorProperty->getNodeValue(n);
  CPPUNIT_ASSERT_EQUAL(size_t(5), value.size());

  for(unsigned int i=0; i< value.size(); ++i) {
    CPPUNIT_ASSERT_EQUAL(tmp[i], value[i]);
  }
}

void StringPropertyTest::complexVectorTest() {
  node n = graph->addNode();
  StringVectorProperty *vectorProperty = graph->getProperty<StringVectorProperty>("stp");
  vector<string> tmp;

  for (unsigned int i=0; i<5000; ++i) {
    stringstream tmpstr;
    tmpstr << "\"test string #\"  : " << i;
    tmp = vectorProperty->getNodeValue(n);
    CPPUNIT_ASSERT(tmp.size() == i);
    tmp.push_back(tmpstr.str());
    vectorProperty->setNodeValue(n, tmp);
  }

  const vector<string>& value = vectorProperty->getNodeValue(n);

  CPPUNIT_ASSERT_EQUAL(size_t(5000), value.size());

  for(unsigned int i=0; i< value.size(); ++i) {
    CPPUNIT_ASSERT_EQUAL(tmp[i], value[i]);
  }
}
