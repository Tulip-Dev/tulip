//-*-c++-*-
#ifndef Tulip_GeneralGraph3D_H
#define Tulip_GeneralGraph3D_H
#include <tulip/TulipPlugin.h>
#include <tulip/AcyclicTest.h>

struct LR
{
  int L;
  int R;
};

/** \addtogroup layout */
/*@{*/
/// GeneralGraph3D.h - An implementation of a hierarchical graph drawing algorithm in 3D.
/** This plugin is an implementation of a hierarchical layout in three dimensions.
 *
 *  <b>HISTORY</b>
 *
 *  - 01/12/1999 Verson 0.0.1: Initial release
 *
 *  \author David Auber University Bordeaux I France, Email : auber@tulip-software.org
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 *
*/
class GeneralGraph3D:public LayoutAlgorithm { 

public:
  GeneralGraph3D(const PropertyContext &);
  ~GeneralGraph3D();  
  bool run();

private:
  void DagLevelSpanningTree(Graph* ,node n);
  void makeAcyclic(Graph*,std::set<edge> &,std::list<SelfLoops> &);
  node makeSimpleSource(Graph* graph);
  void makeProperDag(Graph* graph, std::list<node> &, stdext::hash_map<edge,edge> &);
};
/*@}*/
#endif










