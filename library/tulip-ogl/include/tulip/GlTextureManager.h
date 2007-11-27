//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 10/10/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_GLTEXTUREMANAGER_H
#define Tulip_GLTEXTUREMANAGER_H

#include <map>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <cassert>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>

namespace tlp {

  struct GlTexture {
    GLuint id;
    int height;
    int width;
  };
  
  class TLP_GL_SCOPE GlTextureManager {

    typedef std::map<std::string,GlTexture> TextureUnit;
    typedef std::map<unsigned int,TextureUnit> ContextAndTextureMap;
  
  public:

    static void createInst();
    static GlTextureManager &getInst() {
      assert(inst);
      return *inst;
    }

    void changeContext(unsigned int context);

    GlTexture getTextureInfo(const std::string&);
    bool loadTexture(const std::string&);
    bool activateTexture(const std::string&);
    void desactivateTexture();

  private:

    GlTextureManager() {}

    static GlTextureManager* inst;

    unsigned int currentContext;

    ContextAndTextureMap texturesMap;

  };
 
}

#endif // Tulip_GLTEXTUREMANAGER_H
