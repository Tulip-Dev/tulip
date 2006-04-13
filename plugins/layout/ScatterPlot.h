//-*-c++-*-
#ifndef _SCATTERPLOTLAYOUT_H
#define _SCATTERPlOTLAYOUT_H

#include <tulip/TulipPlugin.h>
#include <tulip/Metric.h>

class ScatterPlot : public LayoutAlgorithm
{ 
 public:

  ScatterPlot(const PropertyContext &);
  ~ScatterPlot();
  
  bool run();
  bool check(std::string &);
  void reset();
 private:

  int nMetrics;
  bool shapeConversion;
  double disc[3];
  Metric* metric[3]; 
};
#endif










