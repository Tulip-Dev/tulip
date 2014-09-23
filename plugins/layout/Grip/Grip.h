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
#ifndef GRIP_H
#define GRIP_H

#include <tulip/TulipPluginHeaders.h>
#include <tulip/tuliphash.h>
#include "MISFiltering.h"

/*@{*/
/** \file
 *  \brief  This plugin is an implementation of the GRIP layout
 *
 *  An implementation of a force directed graph drawing
 *  algorithm first published as:
 *
 *  P. Gajer and S.G. Kobourov,
 *  "GRIP: Graph dRawing with Intelligent Placement",
 *  in Journal Graph Algorithm and Applications, vol. 6, no. 3,
 *  p. 203-224, 2002
 *
 *  <b>HISTORY</b>
 *
 *  - 2007 Version 1.0: Initial release
 *  by Romain Bourqui, LaBRI, University Bordeaux I, France
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
*/

class Grip : public tlp::LayoutAlgorithm {

public:

  PLUGININFORMATION("GRIP","Romain Bourqui","01/11/2010","The Graph dRawing with Intelligent Placement layout algorithm.","1.1", "Force Directed")

  Grip(const tlp::PluginContext *);
  ~Grip();

  bool run();

private :
  void computeOrdering();
  void firstNodesPlacement();
  void placement();
  void initialPlacement(unsigned int, unsigned int);
  void kk_local_reffinement(tlp::node);
  void kk_reffinement(unsigned int, unsigned int);
  void fr_reffinement(unsigned int, unsigned int);
  void displace(tlp::node);
  void updateLocalTemp(tlp::node);
  void init();
  void init_heat(unsigned int);

  void seeLayout(unsigned int);

  unsigned int rounds(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
  void set_nbr_size();
  float sched( int, int, int, int, int);


  MISFiltering * misf;
  float edgeLength;
  int level;
  TLP_HASH_MAP<tlp::node, std::vector<unsigned int> > neighbors_dist;
  TLP_HASH_MAP<tlp::node, std::vector<tlp::node> > neighbors;
  TLP_HASH_MAP<unsigned int, unsigned int> levelToNbNeighbors;
  TLP_HASH_MAP<tlp::node, tlp::Coord> disp;
  TLP_HASH_MAP<tlp::node, tlp::Coord> oldDisp;
  TLP_HASH_MAP<tlp::node, double> heat;
  TLP_HASH_MAP<tlp::node, double> oldCos;

  tlp::Graph * currentGraph;
  int _dim;
};
/*@}*/
#endif
