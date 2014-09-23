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
#ifndef TLPMUTABLECONTAINERTEST
#define TLPMUTABLECONTAINERTEST

#include <string>
#include <tulip/MutableContainer.h>
// needed by MutableContainer<std::string>
#include <tulip/PropertyTypes.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

namespace tlp {
class MutableContainerTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(MutableContainerTest);
  CPPUNIT_TEST(testSetAll);
  CPPUNIT_TEST(testSetGet);
  CPPUNIT_TEST(testFindAll);
  CPPUNIT_TEST(testCompression);
  CPPUNIT_TEST_SUITE_END();
private:
  tlp::MutableContainer<bool> *mutBool;
  tlp::MutableContainer<double> *mutDouble;
  tlp::MutableContainer<std::string> *mutString;
  static const unsigned int NBTEST=1000;

public:
  void setUp();
  void tearDown();
  void testSetAll();
  void testSetGet();
  void testFindAll();
  void testCompression();
};
}
#endif
