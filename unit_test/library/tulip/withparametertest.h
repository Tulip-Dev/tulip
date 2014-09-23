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

#ifndef WITHPARAMETERTEST_H
#define WITHPARAMETERTEST_H

#include <tulip/WithParameter.h>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class WithParameterTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(WithParameterTest);
  CPPUNIT_TEST(testBuildDefaultDataSet);
  CPPUNIT_TEST_SUITE_END();
public:
  void testBuildDefaultDataSet();
};

class ParameteredClass : public tlp::WithParameter {
public :
  ParameteredClass();
};

#endif // WITHPARAMETERTEST_H
