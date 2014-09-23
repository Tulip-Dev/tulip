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

#include "ZoomUtils.h"

#include <tulip/QtGlSceneZoomAndPanAnimator.h>
#include <tulip/GlMainWidget.h>
#include <tulip/Camera.h>

using namespace std;

namespace tlp {
void zoomOnScreenRegion(GlMainWidget *glWidget, const BoundingBox &boundingBox, const bool optimalPath,
                        const double velocity, const double p) {
  QtGlSceneZoomAndPanAnimator animator(glWidget, boundingBox,1000,"Main", optimalPath, velocity, p);
  animator.animateZoomAndPan();
}

void zoomOnScreenRegionWithoutAnimation(GlMainWidget *glWidget, const BoundingBox &boundingBox) {
  Camera &camera = glWidget->getScene()->getGraphCamera();
  Coord bbScreenFirst = camera.worldTo2DScreen(Coord(boundingBox[0]));
  Coord bbScreenSecond = camera.worldTo2DScreen(Coord(boundingBox[1]));
  float bbWidthScreen = bbScreenSecond.getX() - bbScreenFirst.getX();
  float bbHeightScreen = bbScreenSecond.getY() - bbScreenFirst.getY();

  bbWidthScreen += bbWidthScreen * .1;
  bbHeightScreen += bbHeightScreen * .1;

  float startSize, endSize;

  if (bbHeightScreen < bbWidthScreen) {
    startSize = glWidget->width();
    endSize = bbWidthScreen;
  }
  else {
    startSize = glWidget->height();
    endSize = bbHeightScreen;
  }

  float zoomFactor = startSize / endSize;

  double zoomStart = camera.getZoomFactor();
  double zoomEnd = zoomStart * zoomFactor;
  bool withZoom = zoomFactor < 0.99 || zoomFactor > 1.01;

  Coord newCamCenter = (Coord(boundingBox[0]) + Coord(boundingBox[1])) / ((float)2.0);
  newCamCenter.setZ(boundingBox[0][2]);

  camera.setCenter(newCamCenter);
  camera.setEyes(Coord(0, 0, camera.getSceneRadius()));
  camera.setEyes(camera.getEyes() + camera.getCenter());
  camera.setUp(Coord(0, 1., 0));

  if (withZoom) {
    camera.setZoomFactor(zoomEnd);
  }
}
}
