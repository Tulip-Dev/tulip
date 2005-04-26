//-*-c++-*-
#ifndef _SPRINGELECTRICAL_H
#define _SPRINGELECTRICAL_H
#include <cmath>
#include <tulip/TulipPlugin.h>

class SpringElectrical:public Layout { 
public:
  SpringElectrical(const PropertyContext &);
  ~SpringElectrical();
  bool run();
  bool check(std::string &);
  void reset();

private:
  MutableContainer<double> sizeNorm;
  bool checkEdgeIntersection(const node n, const Coord& move);
  Coord repulsiveForces(node n);
  Coord attractiveForces(node n);
  SelectionProxy* inputSelection;
  SizesProxy* inputSize;
  LayoutProxy* inputLayout;
  double k,k2; 
  Coord t;
};

#endif









