//-*-c++-*-
#include <stdio.h>
#include <string>
#include <cppunit/TestCase.h>
#include <tulip/TulipPlugin.h>
#include <iostream>
using namespace std;

class Test2:public BooleanAlgorithm { 
public:
  Test2(const PropertyContext &context):BooleanAlgorithm(context){}
  ~Test2(){}
  bool run() { 
    return true;
  }
};
BOOLEANPLUGIN(Test2,"Test2","Jezequel","03/11/2004","0","0","1");
