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
#ifndef Tulip_OPENGLCONFIGMANAGER_H
#define Tulip_OPENGLCONFIGMANAGER_H

#include <cassert>
#include <iostream>
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

  class OpenGlErrorViewer;

  /** \brief Singleton used to manage OpenGl configuration
   *
   * Singleton used to manage OpenGl configuration
   */
  class TLP_GL_SCOPE OpenGlConfigManager {

  public:

    /**
     * Return the current instance. If instance doesn't exist, create it.
     */
    static OpenGlConfigManager &getInst() {
      if(!inst)
        inst=new OpenGlConfigManager();
      return *inst;
    }

    /**
     * Change the error viewer and return the old one
     */
    OpenGlErrorViewer *setErrorViewer(OpenGlErrorViewer *errorViewer);

    /**
     * Check if system has good graphics card drivers
     */
    void checkDrivers();

    /**
     * Init Glew
     */
    void initGlew();

    /**
     * Glew is init
     */
    bool glewIsInit() {
     return glewIsChecked;
    }

    /**
     * Return if glew can be used
     */
    bool canUseGlew() {
      if(!glewIsChecked)
        return false;
      return glewIsOk;
    }

  private:

    /**
     * Private constructor for singleton
     */
    OpenGlConfigManager();

    static OpenGlConfigManager* inst;

    OpenGlErrorViewer *errorViewer;

    bool glewIsChecked;
    bool driversAreChecked;
    bool glewIsOk;

  };

}

#endif
