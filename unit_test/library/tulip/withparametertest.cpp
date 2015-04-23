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

#include "withparametertest.h"

#include <tulip/ForEach.h>
#include <tulip/WithParameter.h>
#include <tulip/Coord.h>
#include <tulip/Size.h>
#include <tulip/Color.h>
#include <tulip/DataSet.h>

using namespace tlp;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(WithParameterTest);

const std::string intParameter = "integerParameter";
const std::string stringParameter = "stringParameter";
const std::string longParameter = "longParameter";
const std::string uintParameter = "uintegerParameter";
const std::string doubleParameter = "doubleParameter";
const std::string floatParameter = "floatParameter";
const std::string boolParameter = "booleanParameter";
const std::string CoordParameter = "coordParameter";
const std::string SizeParameter = "sizeParameter";
const std::string ColorParameter = "colorParameter";

typedef unsigned int uint;

#define EXPECTED_VALUE(TYPE, VALUE) \
TYPE TYPE##Value = TYPE(); \
set.get<TYPE>(TYPE##Parameter, TYPE##Value); \
CPPUNIT_ASSERT_EQUAL(VALUE, TYPE##Value)

void WithParameterTest::testBuildDefaultDataSet() {
  WithParameter* parametrizedStuff = new ParameteredClass();

  DataSet set;
  parametrizedStuff->getParameters().buildDefaultDataSet(set);

  CPPUNIT_ASSERT(set.exist(intParameter));
  CPPUNIT_ASSERT(set.exist(longParameter));
  CPPUNIT_ASSERT(set.exist(stringParameter));
  CPPUNIT_ASSERT(set.exist(uintParameter));
  CPPUNIT_ASSERT(set.exist(doubleParameter));
  CPPUNIT_ASSERT(set.exist(floatParameter));
  CPPUNIT_ASSERT(set.exist(boolParameter));
  CPPUNIT_ASSERT(set.exist(SizeParameter));
  CPPUNIT_ASSERT(set.exist(ColorParameter));

  EXPECTED_VALUE(int, 42);
  EXPECTED_VALUE(long, 420000000l);
  EXPECTED_VALUE(string, string("forty two"));
  EXPECTED_VALUE(uint, 24u);
  EXPECTED_VALUE(double, 4.2);
  EXPECTED_VALUE(float, 2.4f);
  EXPECTED_VALUE(bool, true);
  EXPECTED_VALUE(Coord, Coord(4.4, 2, 1.1));
  EXPECTED_VALUE(Size, Size(2.2, 1.1, 4));
  EXPECTED_VALUE(Color, Color(1, 4, 255, 9));
}

ParameteredClass::ParameteredClass() {
  addInParameter<int>(intParameter, "this is a test integer parameter", "42");
  addInParameter<long>(longParameter, "this is a test long parameter", "420000000");
  addInParameter<string>(stringParameter, "this is a test string parameter", "forty two");
  addInParameter<uint>(uintParameter, "this is a test unsigned integer parameter", "24");
  addInParameter<double>(doubleParameter, "this is a test double parameter", "4.2");
  addInParameter<float>(floatParameter, "this is a test float parameter", "2.4");
  addInParameter<bool>(boolParameter, "this is a test boolean parameter", "true");
  addInParameter<Coord>(CoordParameter, "this is a test Coord parameter", "(4.4, 2, 1.1)");
  addInParameter<Size>(SizeParameter, "this is a test Size parameter", "(2.2, 1.1, 4)");
  addInParameter<Color>(ColorParameter, "this is a test Color parameter", "(1, 4, 255, 9)");
}

