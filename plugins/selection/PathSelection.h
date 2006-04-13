//-*-c++-*-
#ifndef Tulip_PathSelection_H
#define Tulip_PathSelection_H

#include <tulip/TulipPlugin.h>
///
class PathSelection:public SelectionAlgorithm { 
public:
  ///
  PathSelection(const PropertyContext &);
  ///
  ~PathSelection();
  ///
  bool run();
private:
  void reconnect(node n, Int *values);
};

#endif




