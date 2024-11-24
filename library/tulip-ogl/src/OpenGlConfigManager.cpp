/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <tulip/ParallelTools.h>

//====================================================

using namespace std;

namespace tlp {

bool OpenGlConfigManager::_glewIsInit = false;
bool OpenGlConfigManager::_antialiased = true;
tlp_hash_map<std::string, bool> OpenGlConfigManager::_checkedExtensions;

void OpenGlConfigManager::initExtensions() {
  if (!_glewIsInit) {
    glewExperimental = true;
    _glewIsInit = (glewInit() == GLEW_OK);
  }
}

string OpenGlConfigManager::getOpenGLVersionString() {
  auto glstring = reinterpret_cast<const char *>(glGetString(GL_VERSION));
  return glstring ? glstring : "??.??";
}

double OpenGlConfigManager::getOpenGLVersion() {
  auto glstring = reinterpret_cast<const char *>(glGetString(GL_VERSION));
  return glstring ? std::stod(glstring) : 0.0;
}

string OpenGlConfigManager::getOpenGLVendor() {
  auto glstring = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
  return glstring ? glstring : "unknown GL vendor";
}

bool OpenGlConfigManager::isExtensionSupported(const string &extensionName) {
  if (!_glewIsInit)
    return false;

  bool supported = false;
  TLP_LOCK_SECTION(OpenGlConfigManagerExtensionSupported) {
    auto it = _checkedExtensions.find(extensionName);
    if (it == _checkedExtensions.end()) {
      supported = _checkedExtensions[extensionName] =
          (glewIsSupported(extensionName.c_str()) == GL_TRUE);
    } else
      supported = it->second;
  }
  TLP_UNLOCK_SECTION(OpenGlConfigManagerExtensionSupported);
  return supported;
}

bool OpenGlConfigManager::hasVertexBufferObject() {
  return isExtensionSupported("GL_ARB_vertex_buffer_object");
}

void OpenGlConfigManager::activateAntiAliasing() {
  if (_antialiased) {
    glEnable(GL_MULTISAMPLE);
  }
}

void OpenGlConfigManager::deactivateAntiAliasing() {
  glDisable(GL_MULTISAMPLE);
}

int OpenGlConfigManager::maxNumberOfSamples() {
  static int maxSamples = -1;

  if (maxSamples < 0)
    glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);

  return maxSamples / 4;
}
} // namespace tlp
