#ifndef REVERSEEDGES_H
#define REVERSEEDGES_H

#include <tulip/Algorithm.h>

class ReverseEdges:public tlp::Algorithm {
public:
  PLUGININFORMATIONS("Reverse edges", "Ludwig Fiolka", "10/10/2012", "Reverse all/selected edges on the current graph", "0.1", "")

  ReverseEdges(tlp::PluginContext* context);
  virtual ~ReverseEdges();

  bool run();
  bool check(std::string &);
};

#endif // REVERSEEDGES_H



