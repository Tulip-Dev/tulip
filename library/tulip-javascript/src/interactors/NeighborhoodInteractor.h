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

/**
 *
 * tulip.js : port of the Tulip framework to JavaScript through emscripten
 * Copyright (c) 2016 Antoine Lambert, Thales Services SAS
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#ifndef NEIGHBORHOODINTERACTOR_H
#define NEIGHBORHOODINTERACTOR_H

#include <tulip/Graph.h>
#include <tulip/Coord.h>

#include "GlSceneInteractor.h"

namespace tlp {
class GlGraph;
class AdditionalGlSceneAnimation;
}

class ZoomAndPanInteractor;

class NeighborhoodInteractor : public GlSceneInteractor {

  friend void animate(void *value);

public:
  NeighborhoodInteractor(tlp::GlScene *scene = NULL);

  void setScene(tlp::GlScene *glScene);

  bool mouseCallback(const MouseButton &button, const MouseButtonState &state, int x, int y, const int &modifiers);

  bool mouseMoveCallback(int x, int y, const int &modifiers);

  void draw();

  void desactivate();

private:
  void buildNeighborhoodGraph(tlp::node centralNode);
  void destroyNeighborhoodGraph();
  tlp::node selectNodeInAugmentedDisplayGraph(int x, int y);
  void computeNeighborhoodGraphCircleLayout();
  float computeNeighborhoodGraphRadius(tlp::LayoutProperty *neighborhoodGraphLayoutProp);
  void computeNeighborhoodGraphBoundingBoxes();
  void performZoomAndPan(const tlp::BoundingBox &destBB, tlp::AdditionalGlSceneAnimation *additionalAnimation = nullptr);

  ZoomAndPanInteractor *_znpInteractor;

  tlp::GlGraph *_glGraph;
  tlp::node _centralNode;
  tlp::node _selectedNeighborNode;

  tlp::Graph *_neighborhoodGraph;
  tlp::GlGraph *_glNeighborhoodGraph;
  tlp::ColorProperty *_neighborhoodGraphColors;
  tlp::ColorProperty *_neighborhoodGraphBackupColors;
  tlp::LayoutProperty *_neighborhoodGraphLayout;
  tlp::LayoutProperty *_neighborhoodGraphCircleLayout;
  tlp::LayoutProperty *_neighborhoodGraphOriginalLayout;

  tlp::Coord _circleCenter;
  tlp::BoundingBox _neighborhoodGraphCircleLayoutBB;
  tlp::BoundingBox _neighborhoodGraphOriginalLayoutBB;
  bool _circleLayoutSet = false;
  bool _bringAndGoAnimation = false;
};

#endif // NEIGHBORHOODINTERACTOR_H
