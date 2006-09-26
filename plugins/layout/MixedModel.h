//-*-c++-*-
#ifndef _MixedModel_H
#define _MixedModel_H


/** \addtogroup layout */
/*@{*/
/// MixedModel.h- An implementation of a polyline graph drawing.
/** This plugin is an implementation of the planar polyline 
 *  drawing algorithm, the mixed model algorithm, first published as:
 *
 *  C. Gutwenger and P. Mutzel, \n
 *  "Planar Polyline Drawings with Good Angular Resolution", \n
 *  "Lecture Notes In Computer Science, Vol. 1547" \n
 *  "Proceedings of the 6th International Symposium on Graph Drawing," \n
 *  pages "167--182" \n
 *  1998 \n
 *
 *  <b>HISTORY</b>
 *
 *  - 19/06/2005 Verson 0.0.1: first version by S. BARDET and A. BURNETT
 *  - 06/11/2006 Version 0.0.2: new version with improvements
 *
 *  Let n be the number of nodes, the original algorithm complexity is in O(n).\n
 *  But the implementation of the canonical ordering has not been made in O(n).\n 
 *  This version of the algorithm considers each connected component of the graph, 
 *  tests if it is planar or not. If not, it computes a planar subgraphs, which is 
 *  a maximal planar "sub-map". Then an area aware version of Gutwenger and Mutzel 's 
 *  algorithm is used, and if the connected component was not planar, it adds the
 *  "unplanar" edges in 3D. Finally, it uses the Connected Component Packing plugin 
 *  of Tulip Software to pack the connected components.\n
 *
 *  \author S. Bardet, Romain Bourqui LaBRI, University Bordeaux I France, Email : bourqui@labri.fr
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 *
 */
class MixedModel : public tlp::LayoutAlgorithm {
 public:
  MixedModel(const tlp::PropertyContext &context);
  ~MixedModel();
  bool run();
  bool check(std::string &);

 private:
  
  std::vector<tlp::edge> getPlanarSubGraph(tlp::PlanarConMap *graph, std::vector<tlp::edge> unplanar_edges);
  void initPartition();
  void assignInOutPoints();
  void computeCoords();
  void placeNodesEdges();

  tlp::edge existEdge(tlp::node n, tlp::node v);
  
  tlp::node rightV(unsigned int k);
  tlp::node leftV(unsigned int k);
  int next_right(unsigned int k, const tlp::node v);
  int next_left(unsigned int k, const tlp::node v);
  
  tlp::PlanarConMap* carte;
  std::vector< std::vector<tlp::node> > V;
  std::map<tlp::node, tlp::Coord> NodeCoords;
  
  std::map<tlp::node, int> outl;
  std::map<tlp::node, int> outr;
  std::map<tlp::node, int> inl;
  std::map<tlp::node, int> inr;
  
  std::map<tlp::node, unsigned int> rank;
  std::map<tlp::node, std::vector<tlp::edge> > EdgesIN;
  std::map<tlp::node, std::vector<tlp::edge> > EdgesOUT;

  std::map<tlp::edge, std::vector<tlp::Coord> > InPoints;
  std::map<tlp::edge, tlp::Coord> OutPoints;

  tlp::Graph * Pere;
  tlp::PlanarConMap * graphMap;
  tlp::Graph * currentGraph;
  std::vector<tlp::edge> dummy;
  std::map<tlp::node, std::vector<tlp::Coord> > out_points;
  tlp::MutableContainer<tlp::Coord>::MutableContainer nodeSize;
  std::vector<tlp::edge> unplanar_edges;
  bool planar;
  tlp::SizeProperty *size;

};
/*@}*/
#endif
