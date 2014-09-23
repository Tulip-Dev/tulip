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

#include "TulipNodeColorMapping.h"

#include <tulip/Color.h>
#include <tulip/ColorProperty.h>
#include <tulip/BooleanProperty.h>

using namespace tlp;
using namespace std;

const Color COLOR_SELECT(255, 102, 255, 255);

namespace pocore {

TulipNodeColorMapping::TulipNodeColorMapping(Graph *graph) : graph(graph) {}

RGBA TulipNodeColorMapping::getColor(const double&, const unsigned int itemId) const {
  RGBA ret;

  if (graph->getProperty<BooleanProperty>("viewSelection")->getNodeValue(node(itemId))) {
    ret[0] = COLOR_SELECT.getR();
    ret[1] = COLOR_SELECT.getG();
    ret[2] = COLOR_SELECT.getB();
    ret[3] = COLOR_SELECT.getA();
  }
  else {
    Color nodeColor = graph->getProperty<ColorProperty>("viewColor")->getNodeValue(node(itemId));
    ret[0] = nodeColor.getR();
    ret[1] = nodeColor.getG();
    ret[2] = nodeColor.getB();
    ret[3] = nodeColor.getA();
  }

  return ret;
}

}
