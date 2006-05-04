//-*-c++-*-
#ifndef _SEGMENTMETRIC_H
#define _SEGMENTMETRIC_H

#include <tulip/TulipPlugin.h>

// Class RandomMetric  implemente l'interface Metric
// Autheur Auber David
// Date 01/12/1999
///
class SegmentMetric:public DoubleAlgorithm { 
public:
  SegmentMetric(const PropertyContext &);
  bool run();
  bool check(std::string &);

private:
  int segNode(node,int &,int &);
  double getNodeValue(const node n);
};

#endif
