/*
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
///@cond DOXYGEN_HIDDEN

#ifndef Tulip_OPENGLCONFIGMANAGER_H
#define Tulip_OPENGLCONFIGMANAGER_H

#include <tulip/tulipconf.h>

#include <string>
#include <tulip/tuliphash.h>

#define BUFFER_OFFSET(bytes) (reinterpret_cast<GLubyte *>(bytes))

namespace tlp {

/**
 *
 * Used to manage OpenGl configuration
 */
class TLP_GL_SCOPE OpenGlConfigManager {

public:
  /**
   * Returns the OpenGL version number supported by the host system as a string.
   *
   * \since Tulip 5.0
   */
  static std::string getOpenGLVersionString();

  /**
   * Returns the OpenGL version number supported by the host system as a number.
   */
  static double getOpenGLVersion();

  /**
   * Return the vendor name of the OpenGL driver installed on the host system.
   */
  static std::string getOpenGLVendor();

  static void initExtensions();

  /**
   * Checks if an OpenGL extension is supported by the driver installed on the host system.
   * \param extensionName the name of the OpenGL extension to check in the form "GL_.*" (for
   * instance "GL_ARB_vertex_buffer_object")
   */
  static bool isExtensionSupported(const std::string &extensionName);

  /**
   * Returns if vertex buffer objects can be used on the host system.
   */
  static bool hasVertexBufferObject();

  /**
   * Enables / disables anti-aliasing rendering.
   */
  static void setAntiAliasing(const bool antialiasing) {
    _antialiased = antialiasing;
  }

  /**
   * Returns the anti-aliasing state
   */
  static bool antiAliasing() {
    return _antialiased;
  }

  /**
   * Activates anti-aliasing
   * This method has no effect if anti-aliasing has been disabled by a call to
   * setAntiAliasing(false).
   */
  static void activateAntiAliasing();

  /**
   * Deactivates anti-aliasing
   * This method has no effect if anti-aliasing has been disabled by a call to
   * setAntiAliasing(false).
   */
  static void deactivateAntiAliasing();

  /**
   * Returns a maximum number of samples for anti-aliasing based on graphics hardware capability
   *
   */
  static int maxNumberOfSamples();

private:
  static bool _glewIsInit;
  static bool _antialiased;
  static tlp_hash_map<std::string, bool> _checkedExtensions;
};
} // namespace tlp

#endif
///@endcond
