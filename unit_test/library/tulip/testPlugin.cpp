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
#include <stdio.h>
#include <string>
#include <iostream>
#include <cppunit/TestCase.h>
#include <tulip/TulipPlugin.h>

using namespace std;

class Test : public tlp::BooleanAlgorithm { 
public:
  Test(const tlp::PropertyContext &context) : tlp::BooleanAlgorithm(context){
    addDependency<tlp::BooleanAlgorithm>("Test", "1.0");
  }
  ~Test(){}
  bool run() { 
    std::string name = "Test";
    std::string err = "Error";
    return graph->computeProperty(name, _result, err);
  }
};
BOOLEANPLUGIN(Test,"Test","Jezequel","03/11/2004","0","1.0");
