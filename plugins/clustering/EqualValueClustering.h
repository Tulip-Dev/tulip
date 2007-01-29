//-*-c++-*-
#ifndef _EqualValueClustering_H
#define _EqualValueClustering_H

#include <tulip/TulipPlugin.h>

class EqualValueClustering:public tlp::Algorithm { 
public:
  EqualValueClustering(tlp::AlgorithmContext);
  bool run();
};

#endif





