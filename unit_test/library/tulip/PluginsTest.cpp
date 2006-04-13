#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <tulip/TlpTools.h>

#include "tulip/TemplateFactory.h"
#include "PluginsTest.h"
using namespace std;

#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION( PluginsTest );

//==========================================================
void PluginsTest::setUp() {
  graph = tlp::newSuperGraph();
}
//==========================================================
void PluginsTest::tearDown() {
  delete graph;
}
//==========================================================
void PluginsTest::testloadPlugin() {
  CPPUNIT_ASSERT( Selection::factory->objMap.find("Test") == Selection::factory->objMap.end());
  tlp::loadPlugin("testPlugin.so");
  CPPUNIT_ASSERT( Selection::factory->objMap.find("Test") != Selection::factory->objMap.end());
}
//==========================================================
void PluginsTest::testCircularPlugin() {
  string name = "Test";
  string err = "Error";
  Selection sel(graph);
  CPPUNIT_ASSERT(graph->computeProperty(name, &sel, err));
}
//==========================================================
void PluginsTest::testAncestorGraph() {
  string name = "Test";
  string err = "Error";
  tlp::loadPlugin("testPlugin2.so");
  SuperGraph *child = graph->addSubGraph();
  SuperGraph *child2 = graph->addSubGraph();
  SuperGraph *child3 = child->addSubGraph();
  Selection sel(child);
  CPPUNIT_ASSERT(!graph->computeProperty(name, &sel, err));  
  CPPUNIT_ASSERT(!child2->computeProperty(name, &sel, err));  
  CPPUNIT_ASSERT(child3->computeProperty(name, &sel, err));
}
//==========================================================
CppUnit::Test * PluginsTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip lib : Plugins mechanism" );
  suiteOfTests->addTest( new CppUnit::TestCaller<PluginsTest>( 
							      "loadPlugin", 
							      &PluginsTest::testloadPlugin ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PluginsTest>( 
							      "Circular call", 
							      &PluginsTest::testCircularPlugin ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PluginsTest>( 
							      "Graph validity", 
							      &PluginsTest::testAncestorGraph) );
  return suiteOfTests;
}
