/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

  inst->initGlew();
  return *inst;
}

OpenGlConfigManager::OpenGlConfigManager():
  glewIsInit(false),
  driversAreChecked(false), antialiased(true),
  graphicsCardWarningDisplayer(NULL) {
}

void OpenGlConfigManager::initGlew() {
  if (!glewIsInit) {
    glewExperimental=true;
    glewIsInit = (glewInit() == GLEW_OK);
  }
}

void OpenGlConfigManager::checkDrivers() {
  if(driversAreChecked)
    return;

  driversAreChecked=true;

  bool nvidia=false;
  bool ati=false;
  string vendor(getOpenGLVendor());

  if(vendor.find("NVIDIA")!=string::npos)
    nvidia=true;

  if(vendor.find("ATI")!=string::npos)
    ati=true;

  if(!nvidia && !ati) {
    std::string message("Your graphics card is not powerful enough,\nor it is not configured with the proper drivers.\nFor optimal performances, make sure to install\nthe proprietary drivers corresponding to your\ngraphics card model.");
    if (graphicsCardWarningDisplayer)
      graphicsCardWarningDisplayer->displayWarning(message);

    tlp::warning() << message.c_str() << std::endl;
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

void OpenGlConfigManager::setGraphicsCardWarningDisplayer(OpenGlConfigManager::GraphicsCardWarningDisplayer* displayer) {
    delete getInst().graphicsCardWarningDisplayer;
    getInst().graphicsCardWarningDisplayer = displayer;
}
}


