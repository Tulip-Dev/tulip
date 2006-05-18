//-*-c++-*-
#ifndef _SPRINGELECTRICAL_H
#define _SPRINGELECTRICAL_H
#include <cmath>
#include <tulip/TulipPlugin.h>

class SpringElectrical:public Layout { 
public:
  SpringElectrical(const PropertyContext &);
  bool run();

private:
  MutableContainer<double> sizeNorm;
  bool checkEdgeIntersection(const node n, const Coord& move);
  SelectionProxy* inputSelection;
  SizesProxy* inputSize;
  LayoutProxy* inputLayout;
  MutableContainer<Coord> prevMove;
  double k,k2; 
  Coord t;
};

#endif









