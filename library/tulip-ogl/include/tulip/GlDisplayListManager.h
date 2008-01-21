//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 12/11/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_GLDISPLAYLISTMANAGER_H
#define Tulip_GLDISPLAYLISTMANAGER_H

#include <map>
#include <cassert>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>


namespace tlp {
  
  /** \brief Singleton used to manage OpenGl display list 
   *
   * Singleton used to manage OpenGl display list. 
   * First createInst must be call.
   */
  class TLP_GL_SCOPE GlDisplayListManager {
  
    typedef std::map<std::string,GLuint> DisplayListMap;
    typedef std::map<unsigned int, DisplayListMap> ContextAndDisplayListMap;

  public:
    
    /**
     * Return the current instance. If instance doesn't exist, create it.
     */
    static GlDisplayListManager &getInst() {
      if(!inst)
	inst=new GlDisplayListManager();
      return *inst;
    }

    /**
     * Change OpenGl context because display list can't be shared in different context
     */
    void changeContext(unsigned int context);

    /** 
     * Begin to record a new display list with name : name
     */
    bool beginNewDisplayList(const std::string& name);
    /**
     * End the current record of display list
     */
    void endNewDisplayList();

    /**
     * Call display list with name : name
     */
    bool callDisplayList(const std::string& name);

  private:

    /**
     * Private constructor for singleton
     */
    GlDisplayListManager() {}

    unsigned int currentContext;

    static GlDisplayListManager* inst;

    ContextAndDisplayListMap displayListMap;

  };
 
}

#endif // Tulip_GLDISPLAYLISTMANAGER_H
