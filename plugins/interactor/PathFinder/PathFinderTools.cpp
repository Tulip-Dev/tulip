/*
 * PathFinderTools.cpp
 *
 *  Created on: Apr 14, 2010
 *      Author: Ludwig Fiolka
 */

#include "PathFinderTools.h"

#include <tulip/GlGraphInputData.h>
#include <tulip/BoundingBox.h>
#include <tulip/DrawingTools.h>
#include <tulip/BooleanProperty.h>

using namespace tlp;
using namespace std;

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
