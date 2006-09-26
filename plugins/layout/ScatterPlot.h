//-*-c++-*-
#ifndef _SCATTERPLOTLAYOUT_H
#define _SCATTERPlOTLAYOUT_H

#include <tulip/TulipPlugin.h>
#include <tulip/DoubleProperty.h>

class ScatterPlot : public tlp::LayoutAlgorithm
{ 
 public:

  ScatterPlot(const tlp::PropertyContext &);
  ~ScatterPlot();
  
  bool run();
  bool check(std::string &);
  void reset();
 private:

  int nMetrics;
  bool shapeConversion;
  double disc[3];
  tlp::DoubleProperty* metric[3]; 
};
#endif










