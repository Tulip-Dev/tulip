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
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include "TlpImportExportTest.h"
#include <tulip/Graph.h>
#include <tulip/ForEach.h>
#include <tulip/DataSet.h>
#include <tulip/Color.h>
#include <tulip/Coord.h>
#include <tulip/Size.h>

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
  graph = tlp::loadGraph("DATA/filesystem.tlp.gz");
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
  dataSet.set("file::filename", std::string("DATA/filesystem.tlp.gz"));
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
  // for sf bug  #2999413
  graph->delNode(n1);
  n1 = graph->addNode();
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
    // - 1 is because of delNode
    CPPUNIT_ASSERT((n == node(n1.id - 1)) || (n == node(n2.id - 1)));
  }
  edge e;
  forEach(e, graph->getEdges()) {
    CPPUNIT_ASSERT(e == e1);
  }
  delete graph;
}
//==========================================================
void TlpImportExportTest::testExportCluster() {
  Graph *graph = newGraph();
  CPPUNIT_ASSERT(graph != 0);
  node n1 = graph->addNode();
  node n2 = graph->addNode();
  edge e1 = graph->addEdge(n1, n2);
  Graph* sg = newSubGraph(graph, "\"name with double quotes \"");
  sg->addNode(n1);
  sg->addNode(n2);
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
  Graph* g;
  forEach(g, graph->getSubGraphs()) {
    string name;
    g->getAttribute(string("name"), name);
    CPPUNIT_ASSERT(name == string("\"name with double quotes \""));
    forEach(n, g->getNodes()) {
      CPPUNIT_ASSERT((n == n1) || (n == n2));
    }
  }
  delete graph;
}
//==========================================================
void TlpImportExportTest::testExportAttributes() {
  Graph *graph = newGraph();
  ostream *os = new ofstream("export_attributes.tlp");
  bool b = true, sb = false;
  double d = 100., sd = 50.;
  Color color(1, 2, 3, 4), scolor(4, 3, 2, 1);
  Coord coord(5., 6., 7.), scoord(0., 8. ,9.);
  float f = 200., sf = 100.;
  int i = -10, si = -5;
  unsigned int ui = 10, sui = 5;
  Size size(8., 9., 10.), ssize (10., 9., 8.);
  string str("data"), sstr("atad");

  // set graph attributes of different types
  graph->setAttribute("type = bool", b);
  graph->setAttribute("type = double", d);
  graph->setAttribute("type = color", color);
  graph->setAttribute("type = coord", coord);
  graph->setAttribute("type = float", f);
  graph->setAttribute("type = int", i);
  graph->setAttribute("type = uint", ui);
  graph->setAttribute("type = size", size);
  graph->setAttribute("type = string", str);

  Graph* sg = newSubGraph(graph, "sg");
  CPPUNIT_ASSERT(sg->getId() == 1);

  // set sg attributes of different types
  sg->setAttribute("type = bool", sb);
  sg->setAttribute("type = double", sd);
  sg->setAttribute("type = color", scolor);
  sg->setAttribute("type = coord", scoord);
  sg->setAttribute("type = float", sf);
  sg->setAttribute("type = int", si);
  sg->setAttribute("type = uint", sui);
  sg->setAttribute("type = size", ssize);
  sg->setAttribute("type = string", sstr);

  DataSet dataSet;
  bool ok = exportGraph(graph, *os, "tlp", dataSet);
  delete graph;
  delete os;
  CPPUNIT_ASSERT(ok == true);
  graph = (Graph *) NULL;
  graph = loadGraph("export_attributes.tlp");
  CPPUNIT_ASSERT(graph);

  b = false;
  CPPUNIT_ASSERT(graph->getAttribute("type = bool", b));
  CPPUNIT_ASSERT(b);
  d = 0.;
  CPPUNIT_ASSERT(graph->getAttribute("type = double", d));
  CPPUNIT_ASSERT(d == 100.);
  color = Color(0, 0, 0, 0);
  CPPUNIT_ASSERT(graph->getAttribute("type = color", color));
  CPPUNIT_ASSERT(color == Color(1, 2, 3, 4));
  coord = Coord(0., 0., 0);
  CPPUNIT_ASSERT(graph->getAttribute("type = coord", coord));
  CPPUNIT_ASSERT(coord == Coord(5., 6., 7.));
  f = 0.;
  CPPUNIT_ASSERT(graph->getAttribute("type = float", f));
  CPPUNIT_ASSERT(f == 200.);
  i = 0;
  CPPUNIT_ASSERT(graph->getAttribute("type = int", i));
  CPPUNIT_ASSERT(i == -10);
  ui = 0;
  CPPUNIT_ASSERT(graph->getAttribute("type = uint", ui));
  CPPUNIT_ASSERT(ui == 10);
  size = Size(0., 0., 0.);
  CPPUNIT_ASSERT(graph->getAttribute("type = size", size));
  CPPUNIT_ASSERT(size == Size(8., 9., 10.));
  str = string("");
  CPPUNIT_ASSERT(graph->getAttribute("type = string", str));
  CPPUNIT_ASSERT(str == "data");

  sg = graph->getSubGraph(1);
  CPPUNIT_ASSERT(sg);

  b = false;
  CPPUNIT_ASSERT(sg->getAttribute("type = bool", b));
  CPPUNIT_ASSERT(b == sb);
  d = 0.;
  CPPUNIT_ASSERT(sg->getAttribute("type = double", d));
  CPPUNIT_ASSERT(d == sd);
  color = Color(0, 0, 0, 0);
  CPPUNIT_ASSERT(sg->getAttribute("type = color", color));
  CPPUNIT_ASSERT(color == scolor);
  coord = Coord(0., 0., 0);
  CPPUNIT_ASSERT(sg->getAttribute("type = coord", coord));
  CPPUNIT_ASSERT(coord == scoord);
  f = 0.;
  CPPUNIT_ASSERT(sg->getAttribute("type = float", f));
  CPPUNIT_ASSERT(f == sf);
  i = 0;
  CPPUNIT_ASSERT(sg->getAttribute("type = int", i));
  CPPUNIT_ASSERT(i == si);
  ui = 0;
  CPPUNIT_ASSERT(sg->getAttribute("type = uint", ui));
  CPPUNIT_ASSERT(ui == sui);
  size = Size(0., 0., 0.);
  CPPUNIT_ASSERT(sg->getAttribute("type = size", size));
  CPPUNIT_ASSERT(size == ssize);
  str = string("");
  CPPUNIT_ASSERT(sg->getAttribute("type = string", str));
  CPPUNIT_ASSERT(str == sstr);

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
  suiteOfTests->addTest( new CppUnit::TestCaller<TlpImportExportTest>( 
							      "Export Cluster test", 
							      &TlpImportExportTest::testExportCluster ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<TlpImportExportTest>( 
							      "Export Attributes test", 
							      &TlpImportExportTest::testExportAttributes ) );
  return suiteOfTests;
}
