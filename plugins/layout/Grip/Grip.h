/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include <tulip/LayoutProperty.h>
#include <unordered_map>
#include "MISFiltering.h"

/*@{*/
/** \file
 *  \brief  This plugin is an implementation of the GRIP layout
 *
 *  An implementation of a force directed graph drawing
 *  algorithm first published as:
 *
 *  GRIP: Graph dRawing with Intelligent Placement</b>, P. Gajer and S.G.
 *  Kobourov, Graph Drawing (GD) 2000, Lecture Notes in Computer Science,
 *  vol 1984. Springer, Berlin, Heidelberg. doi: <a
 * href=\"https://doi.org/10.1007/3-540-44541-2_21\">10.1007/3-540-44541-2_21</a>
 *
 *  <b>HISTORY</b>
 *
 *  - 2007 Version 1.0: Initial release
 *  by Romain Bourqui, LaBRI, University Bordeaux I, France
 *
 *
 */

class Grip : public tlp::LayoutAlgorithm {

public:
  PLUGININFORMATION(
      "GRIP", "Romain Bourqui", "01/11/2010",
      "Implements a force directed graph drawing algorithm first published as:<br/>"
      "<b>GRIP: Graph dRawing with Intelligent Placement</b>, P. Gajer and S.G. "
      "Kobourov, Graph Drawing (GD) 2000, Lecture Notes in Computer Science, "
      "vol 1984. Springer, Berlin, Heidelberg. doi: <a "
      "href=\"https://doi.org/10.1007/3-540-44541-2_21\">10.1007/3-540-44541-2_21</a>",
      "1.1", "Force Directed")

  Grip(const tlp::PluginContext *);
  ~Grip() override;

  bool run() override;

private:
  void computeCurrentGraphLayout();
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
  float sched(int, int, int, int, int);

  MISFiltering *misf;
  float edgeLength;
  int level;
  std::unordered_map<tlp::node, std::vector<unsigned int>> neighbors_dist;
  std::unordered_map<tlp::node, std::vector<tlp::node>> neighbors;
  std::unordered_map<unsigned int, unsigned int> levelToNbNeighbors;
  std::unordered_map<tlp::node, tlp::Coord> disp;
  std::unordered_map<tlp::node, tlp::Coord> oldDisp;
  std::unordered_map<tlp::node, double> heat;
  std::unordered_map<tlp::node, double> oldCos;

  tlp::Graph *currentGraph;
  int _dim;
};
/*@}*/
#endif
