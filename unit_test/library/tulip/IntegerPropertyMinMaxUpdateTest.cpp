/*
 * IntegerPropertyMinMaxUpdateTest.cpp
 *
 *  Created on: 12 nov. 2008
 *      Author: antoine
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
const int newMax = 15;

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

//==========================================================
CppUnit::Test * IntegerPropertyMinMaxUpdateTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : Integer Property" );

  suiteOfTests->addTest(new CppUnit::TestCaller<IntegerPropertyMinMaxUpdateTest>(" Min update", &IntegerPropertyMinMaxUpdateTest::testIntegerPropertyMinUpdate));
  suiteOfTests->addTest(new CppUnit::TestCaller<IntegerPropertyMinMaxUpdateTest>(" Max update", &IntegerPropertyMinMaxUpdateTest::testIntegerPropertyMaxUpdate));

  return suiteOfTests;
}
