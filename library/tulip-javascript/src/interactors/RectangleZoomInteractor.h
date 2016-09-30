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

#ifndef RECTANGLEZOOMINTERACTOR_H
#define RECTANGLEZOOMINTERACTOR_H

#include "GlSceneInteractor.h"

struct AnimateParams {

  AnimateParams() : scene(NULL) {
  }
  tlp::GlScene *scene;
};

class RectangleZoomInteractor : public GlSceneInteractor {

public:
  RectangleZoomInteractor(tlp::GlScene *glScene = NULL);

  virtual bool mouseCallback(const MouseButton &button, const MouseButtonState &state, int x, int y, const int &modifiers);

  virtual bool mouseMoveCallback(int x, int y, const int &modifiers);

  virtual bool keyboardCallback(const std::string &keyStr, const int &modifiers);

  virtual void draw();

private:
  MouseButton _mouseButton;
  int _firstX, _firstY;
  int _curX, _curY;
  bool _dragStarted;
  AnimateParams _animParams;
};

#endif // RECTANGLEZOOMINTERACTOR_H
