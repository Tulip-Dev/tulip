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

#include <tulip/BoundingBox.h>
#include <tulip/Coord.h>
#include <tulip/BooleanProperty.h>

#include <tulip/ZoomAndPanAnimation.h>
#include <tulip/GlRect2D.h>
#include <tulip/GlScene.h>
#include <tulip/GlLayer.h>

#include "../TimeMeasurer.h"
#include "RectangleZoomInteractor.h"

using namespace tlp;

static ZoomAndPanAnimation *zoomAndPanAnimation = NULL;
static const unsigned int baseAnimDuration = 1000;
static bool animating = false;
static double animDuration = 0;
static TimeMeasurer tm;
static const unsigned int delay = 40;

static void animate(void *value) {
  AnimateParams *animParams = reinterpret_cast<AnimateParams *>(value);
  double t = tm.getElapsedTime() / animDuration;
  if (t < 1.0) {
    zoomAndPanAnimation->zoomAndPanAnimationStep(t);
    animParams->scene->setBackupBackBuffer(false);
    animParams->scene->requestDraw();
    timerFunc(delay, animate, animParams);
  } else {
    zoomAndPanAnimation->zoomAndPanAnimationStep(1.0);
    animParams->scene->setBackupBackBuffer(true);
    animParams->scene->requestDraw();
    delete zoomAndPanAnimation;
    zoomAndPanAnimation = NULL;
    animating = false;
  }
}

RectangleZoomInteractor::RectangleZoomInteractor(GlScene *glScene) : _firstX(-1), _firstY(-1), _curX(-1), _curY(-1), _dragStarted(false) {
  setScene(glScene);
  _animParams.scene = glScene;
}

bool RectangleZoomInteractor::mouseCallback(const MouseButton &button, const MouseButtonState &state, int x, int y, const int & /*modifiers*/) {
  if (!_glScene || animating)
    return false;
  Camera *camera = _glScene->getMainLayer()->getCamera();
  tlp::Vec4i viewport = camera->getViewport();
  if (x < viewport[0] || x > viewport[2] || y < viewport[1] || y > viewport[3] || animating)
    return false;
  _mouseButton = button;
  if (button == LEFT_BUTTON) {
    if (state == DOWN) {
      _firstX = _curX = x;
      _firstY = _curY = y;
      // double click
      if (tm.getElapsedTime() < 150) {
        keyboardCallback("c", 0);
      } else {
        _dragStarted = true;
      }
      tm.reset();
      return true;
    } else if (state == UP && _dragStarted) {
      _dragStarted = false;
      tlp::Coord bbMin = camera->viewportTo3DWorld(tlp::Coord(viewport[2] - _firstX, _firstY));
      tlp::Coord bbMax = camera->viewportTo3DWorld(tlp::Coord(viewport[2] - _curX, _curY));
      tlp::BoundingBox bb;
      bb.expand(bbMin);
      bb.expand(bbMax);

      zoomAndPanAnimation = new ZoomAndPanAnimation(camera, bb, baseAnimDuration);
      if (zoomAndPanAnimation->canDoZoomAndPan()) {
        animDuration = zoomAndPanAnimation->getAnimationDuration();
        animating = true;
        _animParams.scene = _glScene;
        tm.reset();
        timerFunc(delay, animate, &_animParams);
      } else {
        delete zoomAndPanAnimation;
        zoomAndPanAnimation = NULL;
      }

      _firstX = _curX = -1;
      _firstY = _curY = -1;
      _glScene->requestDraw();
      return true;
    }
  }
  return false;
}

bool RectangleZoomInteractor::mouseMoveCallback(int x, int y, const int & /*modifiers*/) {
  if (!_glScene || animating)
    return false;
  tlp::Vec4i viewport = _glScene->getMainLayer()->getCamera()->getViewport();
  if (x < viewport[0] || x > viewport[2] || y < viewport[1] || y > viewport[3] || animating)
    return false;
  if (_mouseButton == LEFT_BUTTON && _dragStarted) {
    _curX = x;
    _curY = y;
    _glScene->requestDraw();
    return true;
  }
  return false;
}

bool RectangleZoomInteractor::keyboardCallback(const std::string &keyStr, const int & /*modifiers*/) {
  if (!_glScene || animating)
    return false;

  if (keyStr == "c") {
    Camera *camera = _glScene->getMainLayer()->getCamera();
    zoomAndPanAnimation = new ZoomAndPanAnimation(camera, camera->getSceneBoundingBox(), baseAnimDuration);
    animDuration = zoomAndPanAnimation->getAnimationDuration();
    animating = true;
    _animParams.scene = _glScene;
    tm.reset();
    timerFunc(delay, animate, &_animParams);
    return true;
  }
  return false;
}

void RectangleZoomInteractor::draw() {
  if (!_glScene || _firstX != -1) {
    Camera *camera = _glScene->getMainLayer()->getCamera();
    Vec4i viewport = camera->getViewport();
    camera->initGl2D();
    tlp::Vec2f bl(std::min(_firstX, _curX), std::min(viewport[3] - _firstY, viewport[3] - _curY));
    tlp::Vec2f tr(std::max(_firstX, _curX), std::max(viewport[3] - _firstY, viewport[3] - _curY));
    GlRect2D rect(bl, tr, tlp::Color(0, 0, 255, 100), tlp::Color::Black);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    rect.draw(*camera);
    glDisable(GL_BLEND);
    camera->initGl();
  }
}
