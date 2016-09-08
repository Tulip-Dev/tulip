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
 * Copyright (c) 2016 Antoine Lambert, Thales Services SAS
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#include <GL/glew.h>

#include <tulip/OpenGlConfigManager.h>

#include <iostream>
#include <cstdlib>

//====================================================
tlp::OpenGlConfigManager* tlp::OpenGlConfigManager::_instance=nullptr;

using namespace std;

namespace tlp {

OpenGlConfigManager& OpenGlConfigManager::instance() {
  if(!_instance)
    _instance=new OpenGlConfigManager();

  return *_instance;
}

OpenGlConfigManager::OpenGlConfigManager():
  _glewIsInit(false), _antialiased(true) {
}

void OpenGlConfigManager::initExtensions() {
  if (!_glewIsInit) {
    glewExperimental=true;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
      std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }
    _glewIsInit = (err == GLEW_OK);
  }
}

double OpenGlConfigManager::getOpenGLVersion() {
  return atof(reinterpret_cast<const char *>(glGetString(GL_VERSION)));
}

string OpenGlConfigManager::getOpenGLVendor() {
  return string(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
}

bool OpenGlConfigManager::isExtensionSupported(const string &extensionName) {
  if (!_glewIsInit)
    return false;

  if (_checkedExtensions.find(extensionName) == _checkedExtensions.end()) {
    _checkedExtensions[extensionName] = (glewIsSupported(extensionName.c_str()) == GL_TRUE);
  }

  return _checkedExtensions[extensionName];
}

bool OpenGlConfigManager::hasVertexBufferObject() {
  return isExtensionSupported("GL_ARB_vertex_buffer_object");
}

void OpenGlConfigManager::activateAntiAliasing() {
  if (_antialiased) {
    glEnable(GL_MULTISAMPLE);
  }
}

void OpenGlConfigManager::desactivateAntiAliasing() {
  glDisable(GL_MULTISAMPLE);
}

int OpenGlConfigManager::maxNumberOfSamples() const {
  static int maxSamples = -1;

  if (maxSamples < 0)
    glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);

  return maxSamples / 4;
}

}


