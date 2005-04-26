//-*-c++-*-
#include <stdio.h>
#include <string>
#include <cppunit/TestCase.h>
#include <tulip/TulipPlugin.h>
#include <iostream>
using namespace std;

class Test2:public Selection { 
public:
  Test2(const PropertyContext &context):Selection(context){}
  ~Test2(){}
  bool run() { 
    return true;
  }
};
SELECTIONPLUGIN(Test2,"Test2","Jezequel","03/11/2004","0","0","1");
