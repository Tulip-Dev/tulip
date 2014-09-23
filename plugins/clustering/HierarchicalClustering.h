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
#ifndef _HierarchicalClustering_H
#define _HierarchicalClustering_H

#include <list>
#include <string>

#include <tulip/TulipPluginHeaders.h>

/** \addtogroup clustering */

class HierarchicalClustering:public tlp::Algorithm {
public:
  PLUGININFORMATION("Hierarchical","David Auber","27/01/2000","Alpha","1.0", "Clustering")
  HierarchicalClustering(tlp::PluginContext* context);
  ~HierarchicalClustering();
  bool run();
private :
  bool split (tlp::DoubleProperty*, std::list<tlp::node> &);
};


#endif





