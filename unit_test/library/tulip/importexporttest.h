#ifndef IMPORTEXPORTTEST_H
#define IMPORTEXPORTTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

namespace tlp {
  class Graph;
}

class ImportExportTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(ImportExportTest);
//   CPPUNIT_TEST(testgridImportExport);
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
