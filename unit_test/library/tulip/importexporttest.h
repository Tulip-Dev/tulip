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
#ifndef IMPORTEXPORTTEST_H
#define IMPORTEXPORTTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace tlp {
class Graph;
}

class ImportExportTest : public CppUnit::TestFixture {

public:
  ImportExportTest(const std::string &importAlgorithm, const std::string &exportAlgorithm);
  void setUp();
  void testgridImportExport();
  void testSubGraphsImportExport();
  void testAttributes();

private:
  tlp::Graph* createSimpleGraph() const;
  void importExportGraph(tlp::Graph* original);

  void testGraphsTopologiesAreEqual(tlp::Graph* first, tlp::Graph* second);
  void testGraphPropertiesAreEqual(tlp::Graph* first, tlp::Graph* second);
  void testGraphsAreEqual(tlp::Graph* first, tlp::Graph* second);
  void testGraphAttributesAreEqual(tlp::Graph* first, tlp::Graph* second);
  const std::string importAlgorithm;
  const std::string exportAlgorithm;
};

class TlpImportExportTest : public ImportExportTest {
  CPPUNIT_TEST_SUITE(TlpImportExportTest);
  CPPUNIT_TEST(testgridImportExport);
  CPPUNIT_TEST(testAttributes);
  CPPUNIT_TEST(testSubGraphsImportExport);
  CPPUNIT_TEST_SUITE_END();
public:
  TlpImportExportTest();
};

class JsonImportExportTest : public ImportExportTest {
  CPPUNIT_TEST_SUITE(JsonImportExportTest);
  CPPUNIT_TEST(testgridImportExport);
  CPPUNIT_TEST(testAttributes);
  CPPUNIT_TEST(testSubGraphsImportExport);
  CPPUNIT_TEST_SUITE_END();
public:
  JsonImportExportTest();
};


#endif // IMPORTEXPORTTEST_H
