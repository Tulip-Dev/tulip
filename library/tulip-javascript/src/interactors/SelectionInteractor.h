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

#ifndef SELECTIONINTERACTOR_H
#define SELECTIONINTERACTOR_H

#include "GlSceneInteractor.h"

#include <vector>

class ZoomAndPanInteractor;

class SelectionInteractor : public GlSceneInteractor {

public:
  SelectionInteractor(tlp::GlScene *scene = NULL);

  void setScene(tlp::GlScene *glScene);

  virtual bool mouseCallback(const MouseButton &button, const MouseButtonState &state, int x, int y, const int &modifiers);

  virtual bool mouseMoveCallback(int x, int y, const int &modifiers);

  virtual void draw();

  void setSelectOnlyEdgesConnectedToSelectedNodes(bool state) {
    _selectOnlyEdgesConnectedToSelectedNodes = state;
  }

private:
  MouseButton _mouseButton;
  int _firstX, _firstY;
  int _curX, _curY;
  bool _dragStarted;

  ZoomAndPanInteractor *_znpInteractor;
  tlp::SelectedEntity _selectedEntity;
  std::vector<tlp::SelectedEntity> _selectedEntities;

  bool _selectOnlyEdgesConnectedToSelectedNodes;
};

#endif // SELECTIONINTERACTOR_H
