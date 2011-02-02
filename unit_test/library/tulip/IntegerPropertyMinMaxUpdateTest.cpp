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

#include <string>
#include <iostream>

#include <cppunit/TestCaller.h>
#include "IntegerPropertyMinMaxUpdateTest.h"

using namespace tlp;
using namespace std;

const string integerPropertyName = "integer property test";

const int originalMin = 5;
const int originalMax = 10;

const int newMin = 1;
const string newStringMin = "1";
const int newMax = 15;
const string newStringMax = "15";

#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION( IntegerPropertyMinMaxUpdateTest );

void IntegerPropertyMinMaxUpdateTest::setUp() {
	node n2, n3;
	graph = newGraph();
	graph->getLocalProperty<IntegerProperty>(integerPropertyName);
	n1 = graph->addNode();
	graph->getLocalProperty<IntegerProperty>(integerPropertyName)->setNodeValue(n1, originalMin);

	n2 = graph->addNode();
	graph->getLocalProperty<IntegerProperty>(integerPropertyName)->setNodeValue(n2, 6);

	n3 = graph->addNode();
	graph->getLocalProperty<IntegerProperty>(integerPropertyName)->setNodeValue(n3, 7);

	n4 = graph->addNode();
	graph->getLocalProperty<IntegerProperty>(integerPropertyName)->setNodeValue(n4, originalMax);
}

void IntegerPropertyMinMaxUpdateTest::tearDown() {
	delete graph;
}

void IntegerPropertyMinMaxUpdateTest::testIntegerPropertyMinUpdate() {
	int minNode;

	minNode = graph->getLocalProperty<IntegerProperty>(integerPropertyName)->getNodeMin();
	CPPUNIT_ASSERT_MESSAGE("test IntegerProperty min value before update", minNode == originalMin);

	graph->getLocalProperty<IntegerProperty>(integerPropertyName)->setNodeValue(n1, newMin);
	minNode = graph->getLocalProperty<IntegerProperty>(integerPropertyName)->getNodeMin();
	CPPUNIT_ASSERT_MESSAGE("test IntegerProperty min value after update", minNode == newMin);
}

void IntegerPropertyMinMaxUpdateTest::testIntegerPropertyMaxUpdate() {
	int maxNode;

	maxNode = graph->getLocalProperty<IntegerProperty>(integerPropertyName)->getNodeMax();
	CPPUNIT_ASSERT_MESSAGE("test IntegerProperty max value before update", maxNode == originalMax);

	graph->getLocalProperty<IntegerProperty>(integerPropertyName)->setNodeValue(n4, newMax);
	maxNode = graph->getLocalProperty<IntegerProperty>(integerPropertyName)->getNodeMax();
	CPPUNIT_ASSERT_MESSAGE("test IntegerProperty max value after update", maxNode == newMax);
}

void IntegerPropertyMinMaxUpdateTest::testIntegerPropertyMinUpdateFromString() {
	int minNode;

	minNode = graph->getLocalProperty<IntegerProperty>(integerPropertyName)->getNodeMin();
	CPPUNIT_ASSERT_MESSAGE("test IntegerProperty min value before update", minNode == originalMin);

	graph->getLocalProperty<IntegerProperty>(integerPropertyName)->setNodeStringValue(n1, newStringMin);
	minNode = graph->getLocalProperty<IntegerProperty>(integerPropertyName)->getNodeMin();
	CPPUNIT_ASSERT_MESSAGE("test IntegerProperty min value after update", minNode == newMin);
}

void IntegerPropertyMinMaxUpdateTest::testIntegerPropertyMaxUpdateFromString() {
	int maxNode;

	maxNode = graph->getLocalProperty<IntegerProperty>(integerPropertyName)->getNodeMax();
	CPPUNIT_ASSERT_MESSAGE("test IntegerProperty max value before update", maxNode == originalMax);

	graph->getLocalProperty<IntegerProperty>(integerPropertyName)->setNodeStringValue(n4, newStringMax);
	maxNode = graph->getLocalProperty<IntegerProperty>(integerPropertyName)->getNodeMax();
	CPPUNIT_ASSERT_MESSAGE("test IntegerProperty max value after update", maxNode == newMax);
}

//==========================================================
CppUnit::Test * IntegerPropertyMinMaxUpdateTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : Integer Property" );

  suiteOfTests->addTest(new CppUnit::TestCaller<IntegerPropertyMinMaxUpdateTest>(" Min update", &IntegerPropertyMinMaxUpdateTest::testIntegerPropertyMinUpdate));
  suiteOfTests->addTest(new CppUnit::TestCaller<IntegerPropertyMinMaxUpdateTest>(" Max update", &IntegerPropertyMinMaxUpdateTest::testIntegerPropertyMaxUpdate));
  suiteOfTests->addTest(new CppUnit::TestCaller<IntegerPropertyMinMaxUpdateTest>(" Min update from string", &IntegerPropertyMinMaxUpdateTest::testIntegerPropertyMinUpdateFromString));
  suiteOfTests->addTest(new CppUnit::TestCaller<IntegerPropertyMinMaxUpdateTest>(" Max update from string", &IntegerPropertyMinMaxUpdateTest::testIntegerPropertyMaxUpdateFromString));

  return suiteOfTests;
}
