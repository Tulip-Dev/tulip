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
#include <sstream>
#include <cppunit/TestCase.h>
#include <cppunit/TestCaller.h>
#include "PluginsTest.h"

#include <tulip/TulipRelease.h>
#include <tulip/ForEach.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/PluginLoaderTxt.h>

using namespace std;
using namespace tlp;

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
  CPPUNIT_ASSERT(!tlp::PropertyPluginLister<tlp::TemplateAlgorithm<tlp::BooleanProperty> >::pluginExists("Test"));
  PluginLoaderTxt loader;
  PluginLibraryLoader::loadPluginLibrary("./testPlugin.so", &loader);
  // plugin should exist now
  CPPUNIT_ASSERT(tlp::PropertyPluginLister<tlp::TemplateAlgorithm<tlp::BooleanProperty> >::pluginExists("Test"));
  list<Dependency> deps = tlp::PropertyPluginLister<tlp::TemplateAlgorithm<tlp::BooleanProperty> >::getPluginDependencies("Test");
  // only one dependency (see testPlugin.cpp)
  CPPUNIT_ASSERT_EQUAL(size_t(1), deps.size());
  CPPUNIT_ASSERT_EQUAL(tlp::demangleTlpClassName(typeid(BooleanAlgorithm).name()), deps.front().factoryName);
  CPPUNIT_ASSERT_EQUAL(string("Test"), deps.front().pluginName);
}
//==========================================================
void PluginsTest::testCircularPlugin() {
  string name = "Test";
  string err = "Error";
  // ensure graph is not empty
  graph->addNode();
  tlp::BooleanProperty sel(graph);
  CPPUNIT_ASSERT(graph->computeProperty(name, &sel, err));
}
//==========================================================
void PluginsTest::testAncestorGraph() {
  PluginLibraryLoader::loadPluginLibrary("./testPlugin2.so");
  string simpleAlgorithm = "Test2";
  string invalidAlgorithm = "Test3";
  string err;

  /**
   * The graph Hierarchy is as follows
   * graph --------- child1* ------- grandchild
   *          \_____ child2
   *
   * The property belongs to child1, so only him and grandchild can use it
   **/
  Graph *child1 = graph->addSubGraph();
  Graph *grandchild = child1->addSubGraph();
  BooleanProperty sel(child1);
  
  Graph *child2 = graph->addSubGraph();
  
  //since the property belongs to a descendant graph, this fails
  bool result = graph->computeProperty(simpleAlgorithm, &sel, err);
  CPPUNIT_ASSERT_MESSAGE(err, !result);
  
  //since the property belongs to a descendant of a sibling graph, this fails
  result = child2->computeProperty(simpleAlgorithm, &sel, err);
  CPPUNIT_ASSERT_MESSAGE(err, !result);

  //These will fail because the graph is empty
  result = child1->computeProperty(simpleAlgorithm, &sel, err);
  CPPUNIT_ASSERT_MESSAGE(err, !result);

  result = grandchild->computeProperty(simpleAlgorithm, &sel, err);
  CPPUNIT_ASSERT_MESSAGE(err, !result);

  grandchild->addNode();

  //now the graph is not empty they will pass
  result = child1->computeProperty(simpleAlgorithm, &sel, err);
  CPPUNIT_ASSERT_MESSAGE(err, result);
  
  result = grandchild->computeProperty(simpleAlgorithm, &sel, err);
  CPPUNIT_ASSERT_MESSAGE(err, result);

  //now testing with an algorithm that does not exists
  result = child1->computeProperty(invalidAlgorithm, &sel, err);
  CPPUNIT_ASSERT_MESSAGE(err, !result);
  
  result = grandchild->computeProperty(invalidAlgorithm, &sel, err);
  CPPUNIT_ASSERT_MESSAGE(err, !result);
}

void PluginsTest::availablePlugins() {
  std::set<std::string> testBooleanPlugins;
  testBooleanPlugins.insert("Test");
  testBooleanPlugins.insert("Test2");
  
  std::string pluginName;
  forEach(pluginName, BooleanPluginLister::availablePlugins()) {
    if(testBooleanPlugins.find(pluginName) != testBooleanPlugins.end()) {
      testBooleanPlugins.erase(pluginName);
    }
  }

  CPPUNIT_ASSERT_MESSAGE("The test plugins were not listed by the PluginLister", testBooleanPlugins.empty());
}

void PluginsTest::pluginInformations() {
  CPPUNIT_ASSERT_MESSAGE("'Test' plugin must be loaded", BooleanPluginLister::pluginExists("Test"));

  std::list<Dependency> dependencies = BooleanPluginLister::getPluginDependencies("Test");
  CPPUNIT_ASSERT_EQUAL(size_t(1), dependencies.size());
  CPPUNIT_ASSERT_EQUAL(string("Test"), dependencies.begin()->pluginName);
  CPPUNIT_ASSERT_EQUAL(string("TemplateAlgorithm<tlp::BooleanProperty>"), dependencies.begin()->factoryName);
  CPPUNIT_ASSERT_EQUAL(string("1.0"), dependencies.begin()->pluginRelease);

  tlp::ParameterList parameters = BooleanPluginLister::getPluginParameters("Test");

  Iterator<string>* it = parameters.getParametersNames();
  CPPUNIT_ASSERT_MESSAGE("Test plugin has no parameters", it->hasNext());
  string parameterName = it->next();
  CPPUNIT_ASSERT_EQUAL(string("testParameter"), parameterName);
  CPPUNIT_ASSERT_MESSAGE("test parameter should not be mandatory", !parameters.isMandatory(parameterName));
  CPPUNIT_ASSERT_EQUAL(string("0"), parameters.getDefaultValue(parameterName));
  CPPUNIT_ASSERT_EQUAL(string("i"), parameters.getTypeName(parameterName));

  const AbstractPluginInfo& factory(BooleanPluginLister::pluginInformations("Test"));
  CPPUNIT_ASSERT_EQUAL(string("Jezequel"), factory.getAuthor());
  CPPUNIT_ASSERT_EQUAL(string("03/11/2004"), factory.getDate());
  CPPUNIT_ASSERT_EQUAL(string(""), factory.getGroup());
  CPPUNIT_ASSERT_EQUAL(0, factory.getId());
  CPPUNIT_ASSERT_EQUAL(string("1"), factory.getMajor());
  CPPUNIT_ASSERT_EQUAL(string("0"), factory.getMinor());
  CPPUNIT_ASSERT_EQUAL(string("Test"), factory.getName());
  CPPUNIT_ASSERT_EQUAL(string("1.0"), factory.getRelease());
  CPPUNIT_ASSERT_EQUAL(tlp::getMajor(TULIP_RELEASE), factory.getTulipMajor());
  CPPUNIT_ASSERT_EQUAL(tlp::getMinor(TULIP_RELEASE), factory.getTulipMinor());
  CPPUNIT_ASSERT_EQUAL(string(TULIP_RELEASE), factory.getTulipRelease());
}
