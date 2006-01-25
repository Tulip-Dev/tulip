//-*-c++-*-
#ifndef Tulip_HierarchicalGraph_H
#define Tulip_Hierarchical_H
#include <tulip/TulipPlugin.h>

class SelfLoops {
public:
  SelfLoops(node n1,node n2,edge ee1, edge ee2,edge ee3,edge old) {
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

class LessThanNode2 {
public:
  MetricProxy *metric;
  bool operator() (node n1,node n2){return (metric->getNodeValue(n1) < metric->getNodeValue(n2));}
};

/** \addtogroup layout */
/*@{*/
/// Hierarchical.h - An implementation of hierarchical graph drawing algorithm.
/** This plugin is an implementation of hierarchical layout
 *  algorithm first published as:
 *
 *  D. Auber, \n
 *  "Tulip - A Huge Graph Visualization Framework", \n
 *  "Book. Graph Drawing Software. (Ed. Michael Junger & Petra Mutzel", \n
 *  "2004", \n
 *  pages 105 - 126.
 *
 *  <b>HISTORY</b>
 *
 *  - 01/12/1999 Verson 0.0.1: Initial release
 *
 *  \note This algorithm implements a O(n+m) space hierarchical layout 
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
class HierarchicalGraph:public Layout { 

public:
  HierarchicalGraph(const PropertyContext &);
  ~HierarchicalGraph();
  bool run();

private:
  void DagLevelSpanningTree(SuperGraph* ,node n);
  std::vector< std::vector<node> > grid;
  MetricProxy *embedding;
  void twoLayerCrossReduction(SuperGraph*,unsigned int freeLayer,bool sense);
  void crossReduction(SuperGraph*);
  void computeEdgeBends(const SuperGraph *mySGraph, LayoutProxy &tmpLayout, 
			const stdext::hash_map<edge,edge> &replacedEdges, const std::vector<edge> &reversedEdges);
  void computeSelfLoops(SuperGraph *mySGraph, LayoutProxy &tmpLayout, std::vector<tlp::SelfLoops> &listSelfLoops);
  void buildGrid(SuperGraph*);
  unsigned int degree(SuperGraph*,node,bool);
  void initCross(SuperGraph*superGraph,node n, stdext::hash_map<node,bool> &visited,int &id);
  LessThanNode2 lessNode;

  SizesProxy *nodeSize;
  std::string orientation;
  
};
/*@}*/
#endif










