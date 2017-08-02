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
#include <stdio.h>
#include <string>
#include <iostream>
#include <cppunit/TestCase.h>
#include <tulip/TulipPluginHeaders.h>

using namespace std;

class Test : public tlp::BooleanAlgorithm {
public:
  PLUGININFORMATION("Test","Jezequel","03/11/2004","0","1.0", "")
  Test(tlp::PluginContext* context) : tlp::BooleanAlgorithm(context) {
    addDependency("Test", "1.0");
    addInParameter<int>("testParameter", "no help for you :)", "0", false);
  }
  ~Test() {}
  bool run() {
    std::string name = "Test";
    std::string err = "Error";
    return graph->applyPropertyAlgorithm(name, result, err);
  }
};
PLUGIN(Test)

//this plugin is registered with the same identifier as the one above, it should not show up
class Test3 : public tlp::BooleanAlgorithm {
public:
  PLUGININFORMATION("Test","Jezequel","03/11/2004","0","1.0", "")
  Test3(tlp::PluginContext *context) : tlp::BooleanAlgorithm(context) {}
  bool run() {
    return false;
  }
};
PLUGIN(Test3)
