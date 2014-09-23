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
#ifndef MISFILTERING_H
#define MISFILTERING_H

#include <tulip/TulipPluginHeaders.h>
#include <tulip/MutableContainer.h>
#include <tulip/tuliphash.h>

class MISFiltering {

public :
  MISFiltering(tlp::Graph *);
  ~MISFiltering();

  void computeFiltering();
  void getNearest(tlp::node, std::vector<tlp::node> &,std::vector<unsigned int>&, unsigned int, unsigned int = 3);

  std::vector<tlp::node> ordering;
  std::vector<unsigned int> index;

private :
  unsigned int level;
  tlp::MutableContainer<bool> inLastVi;
  tlp::MutableContainer<bool> inCurVi;
  tlp::MutableContainer<bool> removed;
  std::vector<unsigned int> v_dist;

  TLP_HASH_MAP<unsigned int, TLP_HASH_SET<tlp::node> > levelToNodes;



  void bfsDepth(tlp::node, unsigned int);
  void updateVectors();

  tlp::Graph * g_copy;
  tlp::MutableContainer<bool> removedVisit;
  tlp::MutableContainer<bool> attendedVisit;
  tlp::MutableContainer<bool> visited;

  std::vector<tlp::node> toVisit;
};
#endif
