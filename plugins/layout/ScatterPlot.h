//-*-c++-*-
#ifndef _SCATTERPLOTLAYOUT_H
#define _SCATTERPlOTLAYOUT_H

#include <tulip/TulipPlugin.h>
#include <tulip/MetricProxy.h>

class ScatterPlot : public Layout 
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
  MetricProxy* metricProxy[3]; 
};
#endif










