//-*-c++-*-
#ifndef _SEGMENTMETRIC_H
#define _SEGMENTMETRIC_H

#include <tulip/TulipPlugin.h>

// Class RandomMetric  implemente l'interface Metric
// Permet d'obtenir un placement aleatorie des sommets du graphe
// Autheur Auber David
// Date 01/12/1999
///
class SegmentMetric:public Metric
{ 
public:
  SegmentMetric(const PropertyContext &);
  ~SegmentMetric();
  double getNodeValue(const node n);
  int segNode(node,int &,int &);
  bool check(std::string &);
};

#endif
