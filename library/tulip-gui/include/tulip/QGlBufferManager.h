/*
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
///@cond DOXYGEN_HIDDEN

#ifndef Tulip_QGLBUFFERMANAGER_H
#define Tulip_QGLBUFFERMANAGER_H

#include <map>

#include <tulip/tulipconf.h>

class QGLPixelBuffer;
class QGLFramebufferObject;

namespace tlp {

/** \brief Class to manage QGlPixelBuffer and QGlFramebufferObject
 * Singleton class to manager QGlPixelBuffer and QGlFramebufferObject
 */
class TLP_QT_SCOPE QGlBufferManager {

public:

  /**
   * Return the QGlBuffer manager singleton, il singleton doesn't exist this function create it
   */
  static QGlBufferManager &getInst() {
    if(!inst)
      inst=new QGlBufferManager();

    return *inst;
  }

  static void clearBuffers();

  /**
   * Return if QGlPixelBuffer can be used
   */
  bool canUsePixelBuffer() {
    return pixelBufferWork;
  }

  /**
   * Return if QGlFramebufferObject can be used
   */
  bool canUseFramebufferObject() {
    return framebufferObjectWork;
  }

  /**
   * Return a QGlPixelBuffer with given size
   */
  QGLPixelBuffer *getPixelBuffer(int width, int height);

  /**
   * Return a QGLFramebufferObject with given size
   */
  QGLFramebufferObject *getFramebufferObject(int width, int height);

private:

  /**
   * empty private constructor for singleton
   */
  QGlBufferManager();

  static QGlBufferManager* inst;

  std::map<std::pair<int,int>,QGLPixelBuffer*> widthHeightToBuffer;
  std::map<QGLPixelBuffer*,std::pair<int,int> > bufferToWidthHeight;
  std::map<std::pair<int,int>,QGLFramebufferObject*> widthHeightToFramebuffer;
  std::map<QGLFramebufferObject*,std::pair<int,int> > framebufferToWidthHeight;

  bool pixelBufferWork;
  bool framebufferObjectWork;

};

}

#endif // Tulip_QGLBUFFERMANAGER_H
///@endcond
