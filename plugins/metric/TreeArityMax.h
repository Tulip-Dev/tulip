//-*-c++-*-
#ifndef _TREEARITYMAXMETRIC_H
#define _TREEARITYMAXMETRIC_H

#include <tulip/TulipPlugin.h>

class TreeArityMax:public Metric
{ 

public:
  TreeArityMax(const PropertyContext &);
  ~TreeArityMax();
  double getNodeValue(const node n);
  bool check(std::string &);
};

#endif
