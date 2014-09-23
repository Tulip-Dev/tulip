/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#ifndef _MixedModel_H
#define _MixedModel_H


/** \addtogroup layout */

/** This plugin is an implementation of the planar polyline graph
 *  drawing algorithm, the mixed model algorithm, first published as:
 *
 *  C. Gutwenger and P. Mutzel, \n
 *  "Planar Polyline Drawings with Good Angular Resolution", \n
 *  "Lecture Notes In Computer Science, Vol. 1547" \n
 *  "Proceedings of the 6th International Symposium on Graph Drawing," \n
 *  pages "167--182" \n
 *  1998 \n
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
 */
class MixedModel : public tlp::LayoutAlgorithm {
public:
  PLUGININFORMATION("Mixed Model","Romain BOURQUI ","09/11/2005","Ok","1.0","Planar")
  MixedModel(const tlp::PluginContext* context);
  ~MixedModel();
  bool run();
  bool check(std::string &);

private:

  std::vector<tlp::edge> getPlanarSubGraph(tlp::PlanarConMap *graph, std::vector<tlp::edge> unplanar_edges);
  void initPartition();
  void assignInOutPoints();
  void computeCoords();
  void placeNodesEdges();

  tlp::edge existEdge(tlp::node n, tlp::node v) {
    return carte->existEdge(n , v, false);
  }

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
  tlp::MutableContainer<tlp::Coord> nodeSize;
  std::vector<tlp::edge> unplanar_edges;
  bool planar;
  tlp::SizeProperty *sizeResult;
  tlp::IntegerProperty *glyphResult;
};

#endif
