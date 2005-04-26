#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include <tulip/TlpTools.h>

#include "tulip/TemplateFactory.h"
#include "PluginsTest.h"
using namespace std;


//==========================================================
void PluginsTest::setUp() {
  graph = tlp::newSuperGraph();
  //.superGraph = graph;
  //.propertyProxy = new SelectionProxy(graph);
}
//==========================================================
void PluginsTest::tearDown() {
  delete graph;
}
//==========================================================
void PluginsTest::testloadPlugin() {
  CPPUNIT_ASSERT( SelectionProxy::factory.objMap.find("Test") == SelectionProxy::factory.objMap.end());
  tlp::loadPlugin("testPlugin.so");
  CPPUNIT_ASSERT( SelectionProxy::factory.objMap.find("Test") != SelectionProxy::factory.objMap.end());
}
//==========================================================
void PluginsTest::testCircularPlugin() {
  string name = "Test";
  string err = "Error";
  SelectionProxy *sel = new SelectionProxy(graph);
  graph->computeProperty(name, sel, err);  
}
//==========================================================
void PluginsTest::testAncestorGraph() {
  string name = "Test";
  string err = "Error";
  tlp::loadPlugin("testPlugin2.so");
  SuperGraph *child = graph->addSubGraph();
  SuperGraph *child2 = graph->addSubGraph();
  SuperGraph *child3 = child->addSubGraph();
  SelectionProxy *sel = new SelectionProxy(child);
  CPPUNIT_ASSERT(!graph->computeProperty(name, sel, err));  
  CPPUNIT_ASSERT(!child2->computeProperty(name, sel, err));  
  CPPUNIT_ASSERT(child3->computeProperty(name, sel, err));  
}
//==========================================================
CppUnit::Test * PluginsTest::suite() {
  CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "Tulip Plugins Test Suite" );
  suiteOfTests->addTest( new CppUnit::TestCaller<PluginsTest>( 
								       "test loadPlugin", 
								       &PluginsTest::testloadPlugin ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PluginsTest>( 
								       "test loadCircularPlugin", 
								       &PluginsTest::testCircularPlugin ) );
  suiteOfTests->addTest( new CppUnit::TestCaller<PluginsTest>( 
								       "test AcestorGraph", 
								       &PluginsTest::testAncestorGraph) );
  //  suiteOfTests->addTest( new CppUnit::TestCaller<PluginsTest>( 
  //							       "test set/get", 
  //							       &SelectionProxyTest::testSetGet ) );
  return suiteOfTests;
}
