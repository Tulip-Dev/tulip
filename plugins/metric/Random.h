/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#ifndef RANDOMMETRIC_H
#define RANDOMMETRIC_H

#include <tulip/DoubleProperty.h>

/** \addtogroup metric */

/** This plugin assigns random values to nodes or edges or both.
 * The values are between 0 and 1.
 */
class RandomMetric : public tlp::DoubleAlgorithm {

public:
  PLUGININFORMATION("Random metric", "David Auber", "04/10/2001",
                    "Assigns random values to nodes and edges.", "1.1", "Misc")
  RandomMetric(const tlp::PluginContext *context);
  bool run() override;
};
#endif // RANDOMMETRIC_H
