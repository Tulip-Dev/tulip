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
#ifndef _CLUSTERMETRIC_H
#define _CLUSTERMETRIC_H

#include <tulip/DoubleProperty.h>

/** \addtogroup metric */

/** This plugin computes the Cluster metric described in :
 *
 *  Y. Chiricota. F. Jourdan, an G. Melançon \n
 *  "Software component capture using graph clustering",\n
 *  "Proceedings of the 11th IEEE International Workshop on Program Comprehension, 2003", \n
 *  "doi: 10.1109/WPC.2003.1199205"\n
 *  Extended to unbounded depth (for the neighbors):
 *
 *  \note This algorithm works on general graphs. The algorithm use the parameter
 *  depth in order to determine the depth of the neighbors.
 *
 */
class ClusterMetric : public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION(
      "Cluster", "David Auber", "26/02/2003",
      "Computes the Cluster metric as described in:<br>"
      "<b>Software component capture using graph clustering</b>, Y. Chiricota. "
      "F. Jourdan, an G. Melancon, Proceedings of the 11th IEEE International Workshop on Program "
      "Comprehension, 2003."
      "doi: <a href=\"https://doi.org/10.1109/WPC.2003.1199205\">10.1109/WPC.2003.1199205</a>",
      "1.0", "Graph")
  ClusterMetric(const tlp::PluginContext *context);
  bool run() override;
};

#endif
