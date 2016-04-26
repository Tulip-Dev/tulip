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
 * tulip-ogles library : rewrite of old tulip-ogl library using OpenGL ES API
 * Copyright (c) 2016 Antoine Lambert, Thales Services
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#include <GL/glew.h>

#include <tulip/NanoVGManager.h>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#endif

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

#define NANOVG_GLES2_IMPLEMENTATION
#include "nanovg_gl.h"
#include "nanovg_gl_utils.h"

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

using namespace tlp;

std::string NanoVGManager::_currentCanvasId;
std::map<std::string, NanoVGManager *> NanoVGManager::_instances;

NanoVGManager::NanoVGManager() {
  _vg = nvgCreateGLES2(NVG_ANTIALIAS);
}

NanoVGManager::~NanoVGManager() {
  nvgDeleteGLES2(_vg);
}

NVGcontext* NanoVGManager::getNanoVGContext() const {
  return _vg;
}

NanoVGManager *NanoVGManager::instance() {
  return instance(_currentCanvasId);
}

NanoVGManager *NanoVGManager::instance(const std::string &canvasId) {
  if (_instances.find(canvasId) == _instances.end()) {
    _instances[canvasId] = new NanoVGManager();
  }
  return _instances[canvasId];
}
