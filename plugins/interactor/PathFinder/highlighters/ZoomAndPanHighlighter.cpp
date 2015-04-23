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

#include "ZoomAndPanHighlighter.h"

#include <tulip/Graph.h>
#include <tulip/GlMainView.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/LayoutProperty.h>
#include <tulip/Coord.h>
#include <tulip/BoundingBox.h>
#include <tulip/DrawingTools.h>
#include <tulip/GlGraphComposite.h>
#include <tulip/GlMainWidget.h>
#include <tulip/QtGlSceneZoomAndPanAnimator.h>

using namespace std;
using namespace tlp;

void ZoomAndPanHighlighter::highlight(const PathFinder*, GlMainWidget *glMainWidget, BooleanProperty *selection, node, node) {
  GlGraphInputData *inputData(getInputData(glMainWidget));
  Graph *graph = glMainWidget->getScene()->getGlGraphComposite()->getGraph();
  LayoutProperty *layout = inputData->getElementLayout();
  BoundingBox bbox(computeBoundingBox(graph, layout, inputData->getElementSize(), inputData->getElementRotation(), selection));

  QtGlSceneZoomAndPanAnimator animator(glMainWidget, bbox);
  animator.animateZoomAndPan();
}

void ZoomAndPanHighlighter::draw(GlMainWidget*) {
}

bool ZoomAndPanHighlighter::isConfigurable() const {
  return false;
}

QWidget *ZoomAndPanHighlighter::getConfigurationWidget() {
  return 0;
}
