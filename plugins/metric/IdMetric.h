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

#ifndef _IDMETRIC_H
#define _IDMETRIC_H

#include <tulip/TulipPluginHeaders.h>

/** \addtogroup metric */

/** This plugins assigns id to nodes and edges. The ids are those used by tulip.
 *
 */
class IdMetric:public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION("Id","David Auber","06/04/2000","Assigns their Tulip id to nodes adn edges.","1.0","Misc")
  IdMetric(const tlp::PluginContext* context);
  bool run();
};



#endif






