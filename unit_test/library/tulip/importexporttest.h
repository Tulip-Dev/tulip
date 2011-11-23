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
#ifndef IMPORTEXPORTTEST_H
#define IMPORTEXPORTTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace tlp {
class Graph;
}

class ImportExportTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(ImportExportTest);
  CPPUNIT_TEST(testgridImportExport);
  CPPUNIT_TEST(testAttributes);
  CPPUNIT_TEST(testSubGraphsImportExport);
  CPPUNIT_TEST_SUITE_END();
public:
  ImportExportTest();
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

#endif // IMPORTEXPORTTEST_H
