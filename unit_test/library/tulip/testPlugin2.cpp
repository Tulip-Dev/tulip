//-*-c++-*-
#include <stdio.h>
#include <string>
#include <iostream>
#include <cppunit/TestCase.h>
#include <tulip/TulipPlugin.h>

using namespace std;

class Test2 : public tlp::BooleanAlgorithm { 
public:
  Test2(const tlp::PropertyContext &context) : tlp::BooleanAlgorithm(context){}
  ~Test2(){}
  bool run() { 
    return true;
  }
};
BOOLEANPLUGIN(Test2,"Test2","Jezequel","03/11/2004","0","0","1");
