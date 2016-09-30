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

#ifndef SELECTIONMODIFIERINTERACTOR_H
#define SELECTIONMODIFIERINTERACTOR_H

#include "GlSceneInteractor.h"

#include <tulip/BoundingBox.h>
#include <tulip/Node.h>

#include <map>
#include <set>

class SelectionInteractor;

namespace tlp {
class Graph;
class BooleanProperty;
class LayoutProperty;
class SizeProperty;
class DoubleProperty;
class GlGraph;
}

class SelectionModifierInteractor : public GlSceneInteractor {

public:
  SelectionModifierInteractor(tlp::GlScene *glScene = NULL);

  void setScene(tlp::GlScene *glScene);

  virtual bool mouseCallback(const MouseButton &button, const MouseButtonState &state, int x, int y, const int &modifiers);

  virtual bool mouseMoveCallback(int x, int y, const int &modifiers);

  virtual void draw();

  void activate();

  void desactivate();

private:
  bool hasSelection();
  void updateSelectionBoundingBox();
  void translateSelection(tlp::Graph *, int x, int y);

  MouseButton _mouseButton;
  int _lastX, _lastY;

  std::map<tlp::Graph *, tlp::Graph *> _selectionSg;
  std::map<tlp::Graph *, tlp::Graph *> _displayGraph;
  std::map<tlp::Graph *, tlp::GlGraph *> _displayGlGraph;
  std::map<tlp::Graph *, tlp::BoundingBox> _selectionBoundingBox;
  SelectionInteractor *_selectionInteractor;

  std::map<tlp::Graph *, tlp::BooleanProperty *> _viewSelection;
  std::map<tlp::Graph *, tlp::LayoutProperty *> _viewLayout;
  std::map<tlp::Graph *, tlp::SizeProperty *> _viewSize;
  std::map<tlp::Graph *, tlp::DoubleProperty *> _viewRotation;

  bool _dragStarted;
  tlp::Graph *_selectedGraph;
  std::set<tlp::node> _previousSelection;
};

#endif // SELECTIONMODIFIERINTERACTOR_H
