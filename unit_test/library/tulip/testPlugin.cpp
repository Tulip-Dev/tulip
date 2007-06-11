//-*-c++-*-
#include <stdio.h>
#include <string>
#include <iostream>
#include <cppunit/TestCase.h>
#include <tulip/TulipPlugin.h>

using namespace std;

class Test : public tlp::BooleanAlgorithm { 
public:
  Test(const tlp::PropertyContext &context) : tlp::BooleanAlgorithm(context){
    addDependency<BooleanAlgorithm>("Test", "1.0");
  }
  ~Test(){}
  bool run() { 
    std::string name = "Test";
    std::string err = "Error";
    return graph->computeProperty(name, booleanResult, err);
  }
};
BOOLEANPLUGIN(Test,"Test","Jezequel","03/11/2004","0","1.0");
