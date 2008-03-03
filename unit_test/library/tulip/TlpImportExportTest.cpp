#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include "TlpImportExportTest.h"
#include <tulip/Graph.h>
#include <tulip/ForEach.h>

#include <string>

using namespace std; 

using namespace tlp;

#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION( TlpImportExportTest );

//==========================================================
void TlpImportExportTest::testLoad() {
  Graph *graph = tlp::loadGraph("cette erreur fait partie de testLoad");
  CPPUNIT_ASSERT(graph == 0);
  graph = tlp::loadGraph("TlpImportExportTest.cpp");
  CPPUNIT_ASSERT(graph == 0);
  graph = tlp::loadGraph("../../../samples/tlp/filesystem.tlp.gz");
  CPPUNIT_ASSERT(graph != 0);
  delete graph;
}
//==========================================================
void TlpImportExportTest::testImport() {
  DataSet dataSet;
  dataSet.set("file::filename", std::string("cette erreur fait partie de testImport"));
  Graph *graph = tlp::importGraph("tlp", dataSet, NULL);
  CPPUNIT_ASSERT(graph == 0);
  dataSet.set("file::filename", std::string("TlpImportExportTest.cpp"));
  graph = tlp::importGraph("tlp", dataSet, NULL);
  CPPUNIT_ASSERT(graph == 0);
  dataSet.set("file::filename", std::string("../../../samples/tlp/filesystem.tlp.gz"));
  graph = tlp::importGraph("tlp", dataSet, NULL);
  CPPUNIT_ASSERT(graph != 0);
  delete graph;
}
//==========================================================
void TlpImportExportTest::testSave() {
  Graph *graph = newGraph();
  CPPUNIT_ASSERT(graph != 0);
  node n1 = graph->addNode();
  node n2 = graph->addNode();
  edge e1 = graph->addEdge(n1, n2);
  bool ok = saveGraph(graph, "save_test.tlp");
  delete graph;
  CPPUNIT_ASSERT(ok == true);
  graph = (Graph *) NULL;
  graph = loadGraph("save_test.tlp");
  CPPUNIT_ASSERT(graph != 0);
  node n;
  forEach(n, graph->getNodes()) {
    CPPUNIT_ASSERT((n == n1) || (n == n2));
  }
  edge e;
  forEach(e, graph->getEdges()) {
    CPPUNIT_ASSERT(e == e1);
  }
  delete graph;
}
//==========================================================
void TlpImportExportTest::testExport() {
  Graph *graph = newGraph();
  CPPUNIT_ASSERT(graph != 0);
  node n1 = graph->addNode();
  node n2 = graph->addNode();
  edge e1 = graph->addEdge(n1, n2);
  ostream *os = new ofstream("export_test.tlp");
  DataSet dataSet;
  bool ok = exportGraph(graph, *os, "tlp", dataSet);
  delete graph;
  delete os;
  CPPUNIT_ASSERT(ok == true);
  graph = (Graph *) NULL;
  graph = loadGraph("export_test.tlp");
  CPPUNIT_ASSERT(graph != 0);
  node n;
  forEach(n, graph->getNodes()) {
    CPPUNIT_ASSERT((n == n1) || (n == n2));
  }
  edge e;
  forEach(e, graph->getEdges()) {
    CPPUNIT_ASSERT(e == e1);
  }
  delete graph;
}
//==========================================================
CppUnit::Test * TlpImportExportTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : Tlp Import/Export mechanism" );
  suiteOfTests->addTest( new CppUnit::TestCaller<TlpImportExportTest>( 
							      "Load test", 
							      &TlpImportExportTest::testLoad ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<TlpImportExportTest>( 
							      "Import test", 
							      &TlpImportExportTest::testImport ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<TlpImportExportTest>( 
							      "Save test", 
							      &TlpImportExportTest::testSave ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<TlpImportExportTest>( 
							      "Export test", 
							      &TlpImportExportTest::testExport ) );
  return suiteOfTests;
}
