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

#include <GL/glew.h>

#include <tulip/OpenGlConfigManager.h>

#include <iostream>
#include <cstdlib>

//====================================================
tlp::OpenGlConfigManager* tlp::OpenGlConfigManager::inst=NULL;

using namespace std;

namespace tlp {

OpenGlConfigManager& OpenGlConfigManager::getInst() {
  if(!inst)
    inst=new OpenGlConfigManager();

  return *inst;
}

OpenGlConfigManager::OpenGlConfigManager():
  glewIsInit(false),
  driversAreChecked(false), antialiased(true) {
}

void OpenGlConfigManager::initExtensions() {
  if (!glewIsInit) {
    glewExperimental=true;
    glewIsInit = (glewInit() == GLEW_OK);
  }
}

double OpenGlConfigManager::getOpenGLVersion() {
  return atof(reinterpret_cast<const char *>(glGetString(GL_VERSION)));
}

string OpenGlConfigManager::getOpenGLVendor() {
  return string(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
}

bool OpenGlConfigManager::isExtensionSupported(const string &extensionName) {
  if (!glewIsInit)
    return false;

  if (checkedExtensions.find(extensionName) == checkedExtensions.end()) {
    checkedExtensions[extensionName] = (glewIsSupported(extensionName.c_str()) == GL_TRUE);
  }

  return checkedExtensions[extensionName];
}

bool OpenGlConfigManager::hasVertexBufferObject() {
  return isExtensionSupported("GL_ARB_vertex_buffer_object");
}

void OpenGlConfigManager::activateAntiAliasing() {
  if (antialiased) {
    glEnable(GL_MULTISAMPLE);
  }
}

void OpenGlConfigManager::desactivateAntiAliasing() {
  glDisable(GL_MULTISAMPLE);
}

void OpenGlConfigManager::activateLineAndPointAntiAliasing() {
  if (antialiased) {
    glDisable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
  }
}

void OpenGlConfigManager::desactivateLineAndPointAntiAliasing() {
  if (antialiased) {
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POINT_SMOOTH);
  }
}

void OpenGlConfigManager::activatePolygonAntiAliasing() {
  if (antialiased) {
    glDisable(GL_LINE_SMOOTH);
    glDisable(GL_POINT_SMOOTH);
    glEnable(GL_MULTISAMPLE);
  }
}

void OpenGlConfigManager::desactivatePolygonAntiAliasing() {
  if (antialiased) {
    glDisable(GL_MULTISAMPLE);
  }
}

}


