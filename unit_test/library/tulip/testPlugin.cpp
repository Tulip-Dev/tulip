//-*-c++-*-
#include <stdio.h>
#include <string>
#include <cppunit/TestCase.h>
#include <tulip/TulipPlugin.h>
#include <iostream>
using namespace std;

class Test:public Selection { 
public:
  Test(const PropertyContext &context):Selection(context){}
  ~Test(){}
  bool run() { 
    std::string name = "Test";
    std::string err = "Error";
    CPPUNIT_ASSERT(!superGraph->computeProperty(name, selectionProxy, err));
    return true;}
};
SELECTIONPLUGIN(Test,"Test","Jezequel","03/11/2004","0","0","1");
