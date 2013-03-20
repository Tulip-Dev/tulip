/*
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
///@cond DOXYGEN_HIDDEN

#ifndef Tulip_OPENGLCONFIGMANAGER_H
#define Tulip_OPENGLCONFIGMANAGER_H

#include <cassert>
#include <iostream>
#include <map>

#if defined(_MSC_VER)
#include <Windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
# if defined(__WIN32__)
# include <GL/glext.h>
# endif
#endif

#include <tulip/tulipconf.h>

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

namespace tlp {

/** \brief Singleton used to manage OpenGl configuration
 *
 * Singleton used to manage OpenGl configuration
 */
class TLP_GL_SCOPE OpenGlConfigManager {

public:

  /**
   * Return the current instance. If instance doesn't exist, create it.
   */
  static OpenGlConfigManager &getInst();

  /**
   * Check if system has good graphics card drivers.
   */
  void checkDrivers();

  /**
   * Returns the OpenGL version number supported by the host system.
   */
  double getOpenGLVersion();

  /**
   * Return the vendor name of the OpenGL driver installed on the host system.
   */
  std::string getOpenGLVendor();

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
    antialiased = antialiasing;
  }

  /**
   * Activates the anti-aliasing of lines and points primitives.
   * This method has no effect if anti-aliasing has been disabled by a call to setAntiAliasing(false).
   */
  void activateLineAndPointAntiAliasing();

  /**
   * Desactivates the anti-aliasing of lines and points primitives.
   * This method has no effect if anti-aliasing has been disabled by a call to setAntiAliasing(false).
   */
  void desactivateLineAndPointAntiAliasing();

  /**
   * Activates the anti-aliasing of polygons primitives.
   * This method has no effect if anti-aliasing has been disabled by a call to setAntiAliasing(false).
   */
  void activatePolygonAntiAliasing();
  /**
   * Desactivates the anti-aliasing of polygons primitives.
   * This method has no effect if anti-aliasing has been disabled by a call to setAntiAliasing(false).
   */
  void desactivatePolygonAntiAliasing();

  void initGlew();

private:



  /**
   * Private constructor for singleton
   */
  OpenGlConfigManager();

  static OpenGlConfigManager* inst;

  bool glewIsInit;
  bool driversAreChecked;
  bool antialiased;

  std::map<std::string, bool> checkedExtensions;

};

}

#endif
///@endcond
