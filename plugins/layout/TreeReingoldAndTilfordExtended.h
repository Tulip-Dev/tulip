//-*-c++-*-
#ifndef _TreeReingoldAndTilfordExtended_H
#define _TreeReingoldAndTilfordExtended_H
#include <string>
#include <math.h> 
#include <tulip/tuliphash.h>
#include <tulip/TulipPlugin.h>

struct LR {
  double L;
  double R;
  int size;
};
/** \addtogroup layout */
/*@{*/
/// TreeReingoldAndTilfordExtended.h - An implementation of a hierarchical tree layout.
/** This plugin is an implementation of the hierarchical tree layout
 *  algorithm first published as:
 *
 *  E.M. Reingold and J.S. Tilford, \n
 *  "Tidier Drawings of Trees", \n
 *  "IEEE Transactions on Software Engineering" \n
 *  pages "223--228" \n
 *  1981 \n
 *
 *  <b>HISTORY</b>
 *
 *  - 21/02/2000 Verson 0.0.1: Initial release
 *  - 06/11/2002 Verson 0.0.2: Documentation and code clean up
 *  - 06/01/2005 Verson 0.0.3: Code refactoring, management of : orienetation, parameters
 *
 *  \note This algorithm only works on trees.\n
 *  Let n be the number of nodes, the algorithm complexity is in O(n).\n
 *  It extends the original Reingold and Tilford by providing managment
 *  of different node size and different edge length. \n \n
 *  The algorithm use the (Size)"viewSize" property for element size and the
 *  (int)"treeEdgeLength" for the edge length.
 *
 *  \author David Auber LaBRI, University Bordeaux I France, Email : auber@tulip-software.org
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 *
 */
class TreeReingoldAndTilfordExtended:public tlp::LayoutAlgorithm { 
public:
  TreeReingoldAndTilfordExtended(const tlp::PropertyContext &);
  ~TreeReingoldAndTilfordExtended();
  bool run();

private:
  void calcLayout(tlp::node, TLP_HASH_MAP<tlp::node,double> *,double, double, int ,std::map<int,double> &);
  double  calcDecal(const std::list<LR>& , const std::list<LR>&);
  std::list<LR>* mergeLRList(std::list<LR>*,std::list<LR>*,double decal);
  std::list<LR>* TreePlace(tlp::node, TLP_HASH_MAP<tlp::node,double>*);
  void TreeLevelSizing(tlp::node ,std::map<int,double> &,int , std::map<tlp::node,int> &levels);

  tlp::Graph *tree;
  tlp::SizeProperty *sizes;
  tlp::IntegerProperty *lengthMetric;
  bool ortho;
  bool useLength;
  std::string orientation;
  float spacing;
  float nodeSpacing;
};
/*@}*/
#endif










