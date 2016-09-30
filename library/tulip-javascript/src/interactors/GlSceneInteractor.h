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

#ifndef GLUTINTERACTOR_H
#define GLUTINTERACTOR_H

#include <string>

#include <tulip/GlScene.h>

extern void timerFunc(unsigned int msecs, void (*func)(void *value), void *value);

enum SpecialKey {
  KEY_F1,
  KEY_F2,
  KEY_F3,
  KEY_F4,
  KEY_F5,
  KEY_F6,
  KEY_F7,
  KEY_F8,
  KEY_F9,
  KEY_F10,
  KEY_F11,
  KEY_F12,
  KEY_LEFT,
  KEY_UP,
  KEY_RIGHT,
  KEY_DOWN,
  KEY_PAGE_UP,
  KEY_PAGE_DOWN,
  KEY_HOME,
  KEY_END,
  KEY_INSERT
};

enum MouseButton { LEFT_BUTTON, MIDDLE_BUTTON, RIGHT_BUTTON, WHEEL };

enum MouseButtonState { DOWN, UP };

enum MouseEntryState { LEFT, ENTERED };

enum KeyboardModifiers { ACTIVE_SHIFT = 1, ACTIVE_CTRL = 2, ACTIVE_ALT = 4 };

class GlSceneInteractor {

public:
  virtual void activate() {
  }

  virtual void desactivate() {
  }

  virtual bool mouseCallback(const MouseButton & /*button*/, const MouseButtonState & /*state*/, int /*x*/, int /*y*/, const int & /*modifiers*/) {
    return false;
  }

  virtual bool mouseMoveCallback(int /*x*/, int /*y*/, const int & /*modifiers*/) {
    return false;
  }

  virtual bool keyboardCallback(const std::string & /*keyStr*/, const int & /*modifiers*/) {
    return false;
  }

  virtual void draw() {
  }

  virtual void setScene(tlp::GlScene *glScene) {
    _glScene = glScene;
  }

  tlp::GlScene *getScene() const {
    return _glScene;
  }

protected:
  tlp::GlScene *_glScene;
};

#endif // GLUTINTERACTOR_H
