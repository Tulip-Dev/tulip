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
 *  - 06/11/2002 Verson 0.0.2: new version with improvements
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
 *  \author Romain Bourqui LaBRI, University Bordeaux I France, Email : bourqui@labri.fr
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 *
 */
class MixedModel : public Layout {
 public:
  MixedModel(const PropertyContext &context);
  ~MixedModel();
  bool run();
  bool check(std::string &);

 private:
  
  std::vector<edge> getPlanarSubGraph(SuperGraphConMap *graph, std::vector<edge> unplanar_edges);
  void initPartition();
  void assignInOutPoints();
  void computeCoords();
  void placeNodesEdges();

  edge existEdge(node n, node v);
  
  node rightV(unsigned int k);
  node leftV(unsigned int k);
  int next_right(unsigned int k, const node v);
  int next_left(unsigned int k, const node v);
  
  SuperGraphMap* carte;
  std::vector< std::vector<node> > V;
  std::map<node, Coord> NodeCoords;
  
  std::map<node, int> outl;
  std::map<node, int> outr;
  std::map<node, int> inl;
  std::map<node, int> inr;
  
  std::map<node, unsigned int> rank;
  std::map<node, std::vector<edge> > EdgesIN;
  std::map<node, std::vector<edge> > EdgesOUT;

  std::map<edge, std::vector<Coord> > InPoints;
  std::map<edge, Coord> OutPoints;

  SuperGraph * Pere;
  SuperGraphConMap * graphMap;
  SuperGraph * currentGraph;
  std::vector<edge> dummy;
  std::map<node, std::vector<Coord> > out_points;
  MutableContainer<Coord>::MutableContainer nodeSize;
  std::vector<edge> unplanar_edges;
  bool planar;
  SizesProxy *sizeProxy;

};
/*@}*/
#endif
