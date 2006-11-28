#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include "PluginsTest.h"
#include <tulip/TlpTools.h>
#include <tulip/BooleanProperty.h>
#include <tulip/PluginLoaderTxt.h>

using namespace std;
using namespace tlp;

#include <cppunit/extensions/HelperMacros.h>
CPPUNIT_TEST_SUITE_REGISTRATION( PluginsTest );

//==========================================================
void PluginsTest::setUp() {
  graph = tlp::newGraph();
}
//==========================================================
void PluginsTest::tearDown() {
  delete graph;
}
//==========================================================
void PluginsTest::testloadPlugin() {
  // plugin does not exist yet
  CPPUNIT_ASSERT(!tlp::BooleanProperty::factory->pluginExists("Test"));
  PluginLoaderTxt loader;
  tlp::loadPlugin("./testPlugin.so", &loader);
  // plugin should exist now
  CPPUNIT_ASSERT(tlp::BooleanProperty::factory->pluginExists("Test"));
  vector<pair<string, string> > deps = tlp::BooleanProperty::factory->getPluginDependencies("Test");
  // only one dependency (see testPlugin.cpp)
  CPPUNIT_ASSERT(deps.size() == 1);
  CPPUNIT_ASSERT(deps[0].first == "BooleanAlgorithm");
  CPPUNIT_ASSERT(deps[0].second == "Test");  
}
//==========================================================
void PluginsTest::testCircularPlugin() {
  string name = "Test";
  string err = "Error";
  tlp::BooleanProperty sel(graph);
  CPPUNIT_ASSERT(graph->computeProperty(name, &sel, err));
}
//==========================================================
void PluginsTest::testAncestorGraph() {
  string name = "Test";
  string err = "Error";
  tlp::loadPlugin("testPlugin2.so");
  Graph *child = graph->addSubGraph();
  Graph *child2 = graph->addSubGraph();
  Graph *child3 = child->addSubGraph();
  BooleanProperty sel(child);
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
