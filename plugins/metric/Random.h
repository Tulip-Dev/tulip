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
#ifndef _RANDOMMETRIC_H
#define _RANDOMMETRIC_H

#include <tulip/TulipPluginHeaders.h>

/** \addtogroup metric */

/** This plugins assigns random values to nodes and edges. the values are between 0 and 1.
 *
 */
class RandomMetric:public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION("Random metric","David Auber","04/10/2001","Assigns random values to nodes and edges.","1.0","Misc")
  RandomMetric(const tlp::PluginContext* context);
  bool run();
};

#endif











