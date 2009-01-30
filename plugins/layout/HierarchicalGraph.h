//-*-c++-*-
#ifndef Tulip_HierarchicalGraph_H
#define Tulip_Hierarchical_H
#include <tulip/TulipPlugin.h>

class LessThanNode2 {
public:
  tlp::DoubleProperty *metric;
  bool operator() (tlp::node n1, tlp::node n2){return (metric->getNodeValue(n1) < metric->getNodeValue(n2));}
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
class HierarchicalGraph:public tlp::LayoutAlgorithm { 

public:
  HierarchicalGraph(const tlp::PropertyContext &);
  ~HierarchicalGraph();
  bool run();

private:
  void DagLevelSpanningTree(tlp::Graph* ,tlp::DoubleProperty *);
  std::vector< std::vector<tlp::node> > grid;
  tlp::DoubleProperty *embedding;
  void twoLayerCrossReduction(tlp::Graph*,unsigned int freeLayer,bool sense);
  void crossReduction(tlp::Graph*);
  void computeEdgeBends(const tlp::Graph *mySGraph, tlp::LayoutProperty &tmpLayout, 
			const stdext::hash_map<tlp::edge,tlp::edge> &replacedEdges, const std::vector<tlp::edge> &reversedEdges);
  void computeSelfLoops(tlp::Graph *mySGraph, tlp::LayoutProperty &tmpLayout, std::vector<tlp::SelfLoops> &listSelfLoops);
  void buildGrid(tlp::Graph*);
  unsigned int degree(tlp::Graph*, tlp::node, bool);
  void initCross(tlp::Graph*graph, tlp::node n, tlp::MutableContainer<bool> &visited,int id);

  LessThanNode2 lessNode;
  tlp::SizeProperty *nodeSize;
  std::string orientation;
  float spacing;
  float nodeSpacing;
};
/*@}*/
#endif










