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
  
  SizesProxy * sizeOfNodes;
  LayoutProxy* originalLayout;
  
  bool useEdgeLength;
  MetricProxy* edgeLength;
  
  bool forbidMoveOfSelectedNodes;
  SelectionProxy* selectedNodes;
  
  bool removeOverlaps;
};

#endif









