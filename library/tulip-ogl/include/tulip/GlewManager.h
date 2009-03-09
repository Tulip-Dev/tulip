//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 17/2/2009
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/
#ifndef Tulip_GLEWMANAGER_H
#define Tulip_GLEWMANAGER_H

#include <cassert>
#include <iostream>
#include <GL/glew.h>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#define BUFFER_OFFSET(bytes) ((GLubyte*) NULL + (bytes))

namespace tlp {

  /** \brief Singleton used to manage Glew
   *
   * Singleton used to manage Glew.
   */
  class TLP_GL_SCOPE GlewManager {

  public:

    /**
     * Return the current instance. If instance doesn't exist, create it.
     */
    static GlewManager &getInst() {
      if(!inst)
        inst=new GlewManager();
      return *inst;
    }

    /**
     * Init Glew
     */
    void initGlew() {
      if(_glewIsInit)
        return;
      GLenum err = glewInit();
      if (GLEW_OK != err) {
        glewIsOk=false;
        std::cout << "Glew init error: " << glewGetErrorString(err) << std::endl;
      }else{
        std::string glewVersion=(const char*)glewGetString(GLEW_VERSION);
        if(glewVersion=="1.5.0" || glewVersion=="1.5.1"){
          glewIsOk=true;
          std::cout << "ok" << std::endl;
        }else{
          glewIsOk=false;
          std::cout << "version : " << glewVersion << std::endl;
        }
      }
      _glewIsInit=true;
    }

    /**
     * Glew is init
     */
    bool glewIsInit() {
     return _glewIsInit;
    }

    /**
     * Return if glew can be used
     */
    bool canUseGlew() {
      if(!_glewIsInit)
        return false;
      return glewIsOk;
    }

  private:

    /**
     * Private constructor for singleton
     */
    GlewManager():_glewIsInit(false),glewIsOk(false) {
    }

    static GlewManager* inst;

    bool _glewIsInit;
    bool glewIsOk;

  };

}

#endif // Tulip_GLEWMANAGER_H
