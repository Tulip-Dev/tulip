//-*-c++-*-
#ifndef Tulip_GeneralGraph3D_H
#define Tulip_GeneralGraph3D_H
#include <tulip/TulipPlugin.h>

class SelfLoops
{
public:
  SelfLoops(node n1,node n2,edge ee1, edge ee2,edge ee3,edge old)
  {
    ghostNode1=n1;
    ghostNode2=n2;
    e1=ee1;
    e2=ee2;
    e3=ee3;
    oldEdge=old;
  }
  ~SelfLoops(){}
  node ghostNode1,ghostNode2;
  edge e1,e2,e3,oldEdge;  
};


struct LR
{
  int L;
  int R;
};

/** \addtogroup layout */
/*@{*/
///
class GeneralGraph3D:public Layout { 

public:
  GeneralGraph3D(const PropertyContext &);
  ~GeneralGraph3D();  
  bool run();

private:
  void DagLevelSpanningTree(SuperGraph* ,node n);
  void makeAcyclic(SuperGraph*,std::set<edge> &,std::list<SelfLoops> &);
  node makeSimpleSource(SuperGraph* superGraph);
  void makeProperDag(SuperGraph* superGraph, std::list<node> &, stdext::hash_map<edge,edge> &);
};
/*@}*/
#endif










