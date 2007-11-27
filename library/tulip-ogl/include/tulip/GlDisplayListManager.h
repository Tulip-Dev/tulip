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
  
  class TLP_GL_SCOPE GlDisplayListManager {
  
    typedef std::map<std::string,GLuint> DisplayListMap;
    typedef std::map<unsigned int, DisplayListMap> ContextAndDisplayListMap;

  public:

    static void createInst();
    static GlDisplayListManager &getInst() {
      assert(inst);
      return *inst;
    }

    void changeContext(unsigned int context);

    bool beginNewDisplayList(const std::string& name);
    void endNewDisplayList();

    bool callDisplayList(const std::string& name);

  private:

    GlDisplayListManager() {}

    unsigned int currentContext;

    static GlDisplayListManager* inst;

    ContextAndDisplayListMap displayListMap;

  };
 
}

#endif // Tulip_GLTEXTUREMANAGER_H
