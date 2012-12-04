#ifndef REVERSEEDGES_H
#define REVERSEEDGES_H

#include <tulip/Algorithm.h>

class ReverseEdges:public tlp::Algorithm {
public:
  PLUGININFORMATIONS("Reverse edges", "Ludwig Fiolka", "10/10/2012", "Reverse selected/all edges on the current graph", "0.1", "Topology Update")
  
  ReverseEdges(tlp::PluginContext* context);
  virtual ~ReverseEdges();
  
  bool run();
};

#endif // REVERSEEDGES_H



