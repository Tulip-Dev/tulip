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

#include "PathFinderTools.h"

#include <tulip/GlGraphInputData.h>
#include <tulip/BoundingBox.h>
#include <tulip/DrawingTools.h>
#include <tulip/BooleanProperty.h>

using namespace std;

namespace tlp {
// Computes the enclosing circle of the elements contained in a boolean property.
Circlef getEnclosingCircle(GlGraphInputData *inputData, BooleanProperty *selection) {
  BoundingBox bbox(computeBoundingBox(inputData->getGraph(), inputData->getElementLayout(), inputData->getElementSize(), inputData->getElementRotation(), selection));
  Coord center(bbox.center());
  float norm = (bbox[1] - bbox[0]).norm();
  Circlef result;
  result[0] = center.getX();
  result[1] = center.getY();
  result.radius = norm;
  return result;
}

bool getNodeEnclosingCircle(Circlef &circle, GlGraphInputData *inputData, node n) {
  BooleanProperty *selection = new BooleanProperty(inputData->getGraph());
  selection->setAllNodeValue(false);
  selection->setNodeValue(n, true);
  circle = getEnclosingCircle(inputData, selection);
  return true;
}

bool getEdgeEnclosingCircle(Circlef &circle, GlGraphInputData *inputData, edge e) {
  BooleanProperty *selection = new BooleanProperty(inputData->getGraph());
  selection->setAllEdgeValue(false);
  selection->setEdgeValue(e, true);

  if (inputData->getElementLayout()->getEdgeValue(e).size() == 0)
    return false;

  circle = getEnclosingCircle(inputData, selection);
  return true;
}
}
