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

///@cond DOXYGEN_HIDDEN

#ifndef OPENGLCONFIGMANAGER_H
#define OPENGLCONFIGMANAGER_H

#include <tulip/tulipconf.h>

#include <map>
#include <string>

namespace tlp {

/** \brief Singleton used to manage OpenGl configuration
 *
 * Singleton used to manage OpenGl configuration
 */
class TLP_GLES_SCOPE OpenGlConfigManager {

public:

  /**
   * Return the current instance. If instance doesn't exist, create it.
   */
  static OpenGlConfigManager &instance();

  /**
   * Returns the OpenGL version number supported by the host system.
   */
  double getOpenGLVersion();

  /**
   * Return the vendor name of the OpenGL driver installed on the host system.
   */
  std::string getOpenGLVendor();

  void initExtensions();

  /**
   * Checks if an OpenGL extension is supported by the driver installed on the host system.
   * \param extensionName the name of the OpenGL extension to check in the form "GL_.*" (for instance "GL_ARB_vertex_buffer_object")
   */
  bool isExtensionSupported(const std::string &extensionName);

  /**
   * Returns if vertex buffer objects can be used on the host system.
   */
  bool hasVertexBufferObject();

  /**
   * Enables / disables anti-aliasing rendering.
   */
  void setAntiAliasing(const bool antialiasing) {
    _antialiased = antialiasing;
  }

  /**
   * Returns the anti-aliasing state
   */
  bool antiAliasing() const {
    return _antialiased;
  }

  /**
   * Activates anti-aliasing
   * This method has no effect if anti-aliasing has been disabled by a call to setAntiAliasing(false).
   */
  void activateAntiAliasing();

  /**
   * Desactivates anti-aliasing
   * This method has no effect if anti-aliasing has been disabled by a call to setAntiAliasing(false).
   */
  void desactivateAntiAliasing();

  /**
   * Returns a maximum number of samples for anti-aliasing based on graphics hardware capability
   *
   */
  int maxNumberOfSamples() const;

private:

  /**
   * Private constructor for singleton
   */
  OpenGlConfigManager();

  static OpenGlConfigManager* _instance;

  bool _glewIsInit;
  bool _antialiased;

  std::map<std::string, bool> _checkedExtensions;

};

}

#endif
///@endcond
