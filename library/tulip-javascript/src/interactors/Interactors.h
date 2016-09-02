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

#ifndef INTERACTORS_H
#define INTERACTORS_H

#include "ZoomAndPanInteractor.h"
#include "RectangleZoomInteractor.h"
#include "SelectionInteractor.h"
#include "SelectionModifierInteractor.h"
#include "NeighborhoodInteractor.h"
#include "LassoSelectionInteractor.h"
#include "FisheyeInteractor.h"

static ZoomAndPanInteractor zoomAndPanInteractor;
static RectangleZoomInteractor rectangleZoomInteractor;
static SelectionInteractor selectionInteractor;
static SelectionModifierInteractor selectionModifierInteractor;
static NeighborhoodInteractor neighborhoodInteractor;
static LassoSelectionInteractor lassoSelectionInteractor;
static FisheyeInteractor fisheyeInteractor;

static std::map<std::string, GlSceneInteractor *> initInteractorsMap() {
  std::map<std::string, GlSceneInteractor *> ret;
  ret["ZoomAndPan"] = &zoomAndPanInteractor;
  ret["RectangleZoom"] = &rectangleZoomInteractor;
  ret["Selection"] = &selectionInteractor;
  ret["SelectionModifier"] = &selectionModifierInteractor;
  ret["Neighborhood"] = &neighborhoodInteractor;
  ret["LassoSelection"] = &lassoSelectionInteractor;
  ret["Fisheye"] = &fisheyeInteractor;
  return ret;
}

static std::map<std::string, GlSceneInteractor *> interactorsMap = initInteractorsMap();

#endif // INTERACTORS_H
