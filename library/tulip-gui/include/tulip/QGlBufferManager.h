/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#ifndef Tulip_QGLBUFFERMANAGER_H
#define Tulip_QGLBUFFERMANAGER_H

#include <map>
#include <unordered_map>

#include <tulip/tulipconf.h>
#include <QOpenGLFramebufferObject>

namespace tlp {

/** \brief Class to manage QGlFramebufferObject
 */
class TLP_QT_SCOPE QGlBufferManager {

public:
  static void clearBuffers();

  /**
   * Return if QGlFramebufferObject can be used
   */
  static bool canUseFramebufferObject() {
    return QOpenGLFramebufferObject::hasOpenGLFramebufferObjects();
  }

  /**
   * Return a QOpenGLFramebufferObject with given size
   */
  static QOpenGLFramebufferObject *getFramebufferObject(int width, int height);

private:
  static std::map<std::pair<int, int>, QOpenGLFramebufferObject *> widthHeightToFramebuffer;
  static std::unordered_map<QOpenGLFramebufferObject *, std::pair<int, int>> framebufferToWidthHeight;
};
} // namespace tlp

#endif // Tulip_QGLBUFFERMANAGER_H
///@endcond
