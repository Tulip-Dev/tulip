/*
 * IntegerPropertyMinMaxUpdateTest.h
 *
 *  Created on: 12 nov. 2008
 *      Author: antoine
 */

#ifndef INTEGERPROPERTYMINMAXUPDATETEST_H_
#define INTEGERPROPERTYMINMAXUPDATETEST_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include <tulip/Graph.h>
#include <tulip/IntegerProperty.h>

class IntegerPropertyMinMaxUpdateTest : public CppUnit::TestFixture {

	CPPUNIT_TEST_SUITE(IntegerPropertyMinMaxUpdateTest);
	CPPUNIT_TEST(testIntegerPropertyMinUpdate);
	CPPUNIT_TEST(testIntegerPropertyMaxUpdate);
	CPPUNIT_TEST_SUITE_END();

public:

	void setUp();
	void tearDown();
	void testIntegerPropertyMinUpdate();
	void testIntegerPropertyMaxUpdate();

private :

	tlp::Graph *graph;
	tlp::node n1, n4;
};

#endif /* INTEGERPROPERTYMINMAXUPDATETEST_H_ */
