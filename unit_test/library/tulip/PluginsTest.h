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
#ifndef TLPPLUGINSTEST
#define TLPPLUGINSTEST

#include <string>

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/extensions/HelperMacros.h>

#include <tulip/Graph.h>

class PluginsTest : public CppUnit::TestFixture {
  CPPUNIT_TEST_SUITE(PluginsTest);
  CPPUNIT_TEST(testloadPlugin);
  CPPUNIT_TEST(testCircularPlugin);
  CPPUNIT_TEST(testAncestorGraph);
  CPPUNIT_TEST(availablePlugins);
  CPPUNIT_TEST(pluginInformations);
  CPPUNIT_TEST_SUITE_END();
private:
  tlp::Graph *graph;

public:
  void setUp();
  void tearDown();
  void testloadPlugin();
  void testCircularPlugin();
  void testAncestorGraph();
  void availablePlugins();
  void pluginInformations();
};

#endif
