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
#ifndef Tulip_QGLPIXELBUFFERMANAGER_H
#define Tulip_QGLPIXELBUFFERMANAGER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <map>

#include <tulip/tulipconf.h>

class QGLPixelBuffer;

namespace tlp {

  /** \brief Class to manage QGlPixelBuffer
   * Singleton class to manager QGlPixelBuffer
   */
  class TLP_QT_SCOPE QGlPixelBufferManager {

  public:

    /**
     * Create the QGlPixelBuffer manager singleton
     */
    static void createInst();
    /**
     * Return the QGlPixelBuffer manager singleton, il singleton doesn't exist this function create it
     */
    static QGlPixelBufferManager &getInst() {
      if(!inst)
        inst=new QGlPixelBufferManager();
      return *inst;
    }
    /**
     * Return a QGlPixelBuffer with given size
     */
    QGLPixelBuffer *getPixelBuffer(int width, int height);

  private:

    /**
     * empty private constructor for singleton
     */
    QGlPixelBufferManager() {}

    static QGlPixelBufferManager* inst;

    std::map<std::pair<int,int>,QGLPixelBuffer*> widthHeightToBuffer;
    std::map<QGLPixelBuffer*,std::pair<int,int> > bufferToWidthHeight;

  };

}

#endif // Tulip_QGLPIXELBUFFERMANAGER_H
