//-*-c++-*-
#ifndef _SPRINGELECTRICAL_H
#define _SPRINGELECTRICAL_H
#include <cmath>
#include <tulip/TulipPlugin.h>

class SpringElectrical:public LayoutAlgorithm { 
public:
  SpringElectrical(const PropertyContext &);
  bool run();

private:
  MutableContainer<double> sizeNorm;
  bool checkEdgeIntersection(const node n, const Coord& move);
  
  SizeProperty * sizeOfNodes;
  LayoutProperty* originalLayout;
  
  bool useEdgeLength;
  DoubleProperty* edgeLength;
  
  bool forbidMoveOfSelectedNodes;
  BooleanProperty* selectedNodes;
  
  bool removeOverlaps;
};

#endif









