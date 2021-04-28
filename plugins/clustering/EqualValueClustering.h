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
#ifndef _EqualValueClustering_H
#define _EqualValueClustering_H

#include <tulip/TulipPluginHeaders.h>

class EqualValueClustering : public tlp::Algorithm {
public:
  PLUGININFORMATION(
      "Equal Value", "Patrick Mary", "16/04/2014",
      "Performs a graph clusterization grouping in the same cluster the nodes or "
      "edges having the same value for a given property.",
      "1.1", "Clustering")
  EqualValueClustering(tlp::PluginContext *context);
  bool run() override;
  bool computeClusters(tlp::NumericProperty *prop, bool onNodes,
                       bool connected);
  bool computeClusters(tlp::PropertyInterface *prop, bool onNodes,
                       bool connected);
};

#endif
