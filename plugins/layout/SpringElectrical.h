//-*-c++-*-
#ifndef _SPRINGELECTRICAL_H
#define _SPRINGELECTRICAL_H
#include <cmath>
#include <tulip/TulipPlugin.h>

class SpringElectrical:public tlp::LayoutAlgorithm { 
public:
  SpringElectrical(const tlp::PropertyContext &);
  bool run();

private:
  tlp::MutableContainer<double> sizeNorm;
  bool checkEdgeIntersection(const tlp::node n, const tlp::Coord& move);
  
  tlp::SizeProperty * sizeOfNodes;
  tlp::LayoutProperty* originalLayout;
  
  bool useEdgeLength;
  tlp::DoubleProperty* edgeLength;
  
  bool forbidMoveOfSelectedNodes;
  tlp::BooleanProperty* selectedNodes;
  
  bool removeOverlaps;
};

#endif









