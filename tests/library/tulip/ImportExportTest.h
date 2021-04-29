/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include "CppUnitIncludes.h"

namespace tlp {
class Graph;
}

class ImportExportTest : public CppUnit::TestFixture {

public:
  ImportExportTest(const std::string &importAlgorithm, const std::string &exportAlgorithm);
  void setUp() override;
  void testgridImportExport();
  void testSubGraphsImportExport();
  void testAttributes();
  void testNanInfValuesImportExport();
  void testMetaGraphImportExport();

protected:
  void updateIdProperty(tlp::Graph *graph) const;
  tlp::Graph *createSimpleGraph() const;
  void importExportGraph(tlp::Graph *original);
  void exportGraph(tlp::Graph *graph, const std::string &exportPluginName,
                   const std::string &filename);
  tlp::Graph *importGraph(const std::string &importPluginName, const std::string &filename);

  void testGraphsTopologiesAreEqual(tlp::Graph *first, tlp::Graph *second);
  void testGraphPropertiesAreEqual(tlp::Graph *first, tlp::Graph *second);
  void testGraphsAreEqual(tlp::Graph *first, tlp::Graph *second);
  void testGraphAttributesAreEqual(tlp::Graph *first, tlp::Graph *second);
  const std::string importAlgorithm;
  const std::string exportAlgorithm;
};

class TlpImportExportTest : public ImportExportTest {
  CPPUNIT_TEST_SUITE(TlpImportExportTest);
  CPPUNIT_TEST(testgridImportExport);
  CPPUNIT_TEST(testAttributes);
  CPPUNIT_TEST(testSubGraphsImportExport);
  CPPUNIT_TEST(testNanInfValuesImportExport);
  CPPUNIT_TEST(testMetaGraphImportExport);
  CPPUNIT_TEST_SUITE_END();

public:
  TlpImportExportTest();
};

class TlpBImportExportTest : public ImportExportTest {
  CPPUNIT_TEST_SUITE(TlpBImportExportTest);
  CPPUNIT_TEST(testgridImportExport);
  CPPUNIT_TEST(testAttributes);
  CPPUNIT_TEST(testSubGraphsImportExport);
  CPPUNIT_TEST(testNanInfValuesImportExport);
  CPPUNIT_TEST(testMetaGraphImportExport);
  CPPUNIT_TEST_SUITE_END();

public:
  TlpBImportExportTest();
};

class JsonImportExportTest : public ImportExportTest {
  CPPUNIT_TEST_SUITE(JsonImportExportTest);
  CPPUNIT_TEST(testgridImportExport);
  CPPUNIT_TEST(testAttributes);
  CPPUNIT_TEST(testSubGraphsImportExport);
  CPPUNIT_TEST(testNanInfValuesImportExport);
  CPPUNIT_TEST(testMetaGraphImportExport);
  CPPUNIT_TEST_SUITE_END();

public:
  JsonImportExportTest();
};

class TulipSaveLoadGraphFunctionsTest : public ImportExportTest {
  CPPUNIT_TEST_SUITE(TulipSaveLoadGraphFunctionsTest);
  CPPUNIT_TEST(testTulipSaveLoadGraphFunctions);
  CPPUNIT_TEST_SUITE_END();

public:
  TulipSaveLoadGraphFunctionsTest();

  void setUp() override;
  void testTulipSaveLoadGraphFunctions();
};

#endif // IMPORTEXPORTTEST_H
