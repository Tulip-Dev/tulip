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
#ifndef TLPOBSERVABLEGRAPHTEST
#define TLPOBSERVABLEGRAPHTEST

#include <tulip/Graph.h>
#include <tulip/TlpTools.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

class ObservableGraphTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(ObservableGraphTest);
  CPPUNIT_TEST(testAddDel);
  CPPUNIT_TEST(testAddDelProperties);
  CPPUNIT_TEST(testClear);
  CPPUNIT_TEST(testSubgraph);
  CPPUNIT_TEST(testDeleteSubgraph);
  CPPUNIT_TEST(testReverse);
  CPPUNIT_TEST(testObserverWhenRemoveObservable);
  CPPUNIT_TEST(testDelInheritedPropertyExistWhenDelInheritedPropertyIsSend);
  CPPUNIT_TEST(testNotifyDelInheritedPropertyIsSendWhenLocalPropertyIsDeleted);
  CPPUNIT_TEST(testDeleteBug747);
  CPPUNIT_TEST_SUITE_END();
private:
  static tlp::Graph *graph;

public:
  static void setGraph(tlp::Graph* g) {
    graph = g;
  }
  void setUp();
  void tearDown();

  void testAddDel();
  void testAddDelProperties();
  void testClear();
  void testSubgraph();
  void testDeleteSubgraph();
  void testReverse();
  void testObserverWhenRemoveObservable();
  void testDelInheritedPropertyExistWhenDelInheritedPropertyIsSend();
  void testNotifyDelInheritedPropertyIsSendWhenLocalPropertyIsDeleted();
  void testDeleteBug747();
};

#endif
