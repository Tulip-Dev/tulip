//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 04/06/2009
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
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
