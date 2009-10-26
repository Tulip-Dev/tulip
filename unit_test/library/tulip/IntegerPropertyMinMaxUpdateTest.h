/*
 * IntegerPropertyMinMaxUpdateTest.h
 *
 *  Created on: 12 nov. 2008
 *      Author: antoine
 */

#ifndef INTEGERPROPERTYMINMAXUPDATETEST_H_
#define INTEGERPROPERTYMINMAXUPDATETEST_H_

#include <tulip/Graph.h>
#include <tulip/IntegerProperty.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>

class IntegerPropertyMinMaxUpdateTest : public CppUnit::TestFixture {
public:

	void setUp();
	void tearDown();
	void testIntegerPropertyMinUpdate();
	void testIntegerPropertyMaxUpdate();

	static CppUnit::Test *suite();

private :

	tlp::Graph *graph;
	tlp::node n1, n4;
};

#endif /* INTEGERPROPERTYMINMAXUPDATETEST_H_ */
