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
#ifndef TLPIMPORTEXPPORT
#define TLPIMPPORTEXPORT

#include <string>
#include <tulip/Graph.h>
#include <tulip/TlpTools.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

class TlpImportExportTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(TlpImportExportTest);
  CPPUNIT_TEST(testImport);
  CPPUNIT_TEST(testSave);
  CPPUNIT_TEST(testExport);
  CPPUNIT_TEST(testExportCluster);
  CPPUNIT_TEST(testExportAttributes);
  CPPUNIT_TEST_SUITE_END();
public:
  void testLoad();
  void testImport();
  void testSave();
  void testExport();
  void testExportCluster();
  void testExportAttributes();
};

#endif
