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
#ifndef Tulip_GLTEXTUREMANAGER_H
#define Tulip_GLTEXTUREMANAGER_H

#include <map>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <tulip/tulipconf.h>

#include <cassert>
#include "tulip/OpenGlConfigManager.h"
#include <string>
#include <set>
#include <iostream>

namespace tlp {

  class OpenGlErrorViewer;

  struct GlTexture {
    GLuint *id;
    int height;
    int width;
    unsigned int spriteNumber;
  };

  struct TextureInfo{
    bool hasAlpha;
    unsigned int  width;
    unsigned int  height;
    unsigned char *data;
  };

  /** \brief Class to manage textures
   * Singleton class to load/store textures need by OpenGL rendering
   */
  class TLP_GL_SCOPE GlTextureManager {

    typedef std::map<std::string,GlTexture> TextureUnit;
    typedef std::map<unsigned long,TextureUnit> ContextAndTextureMap;

  public:

    /**
     * Create the texture manager singleton
     */
    static void createInst();
    /**
     * Return the texture manager singleton, il singleton doesn't exist this function create it
     */
    static GlTextureManager &getInst() {
      if(!inst)
        inst=new GlTextureManager();
      return *inst;
    }

    /**
     * Change the error viewer and return the old one
     */
    OpenGlErrorViewer *setErrorViewer(OpenGlErrorViewer *errorViewer){
      OpenGlErrorViewer *oldErrorViewer=this->errorViewer;
      this->errorViewer=errorViewer;
      return oldErrorViewer;
    }

    /**
     * Change the current OpenGl context (each OpenGl window have a different OpenGl context)
     */
    void changeContext(unsigned long context);
    /**
     * Remove all textures of an OpenGl context and remove this context
     */
    void removeContext(unsigned long context);

    /**
     * Return texture info (id, width and height) for the given name
     */
    GlTexture getTextureInfo(const std::string&);
    /**
     * Load texture with textureName from Raw data
     */
    bool loadTextureFromRawData(const std::string &textureName, int width, int height, bool hasAlpha, unsigned char *data);
    /**
     * Load texture with given name
     */
    bool loadTexture(const std::string&);
    /**
     * Remove texture with given name
     */
    void deleteTexture(const std::string &);
    /**
     * Begin a new texture with given name
     */
    void beginNewTexture(const std::string&);
    /**
     * Activate a texture with given name
     */
    bool activateTexture(const std::string&,unsigned int);
    /**
     * Activate a texture with given name
     */
    bool activateTexture(const std::string&);
    /**
     * Disable texture with given name
     */
    void desactivateTexture();
    /**
     * Set animationStep for next textures (for next activateTexture)
     */
    void setAnimationFrame(unsigned int id) {animationFrame=id;}
    /**
     * Get animationStep of next textures
     */
    unsigned int getAnimationFrame() {return animationFrame;}
    /**
     * Clear vector of textures with error
     */
    void clearErrorVector() {texturesWithError.clear();}
    /**
     * Remove an entry of vector of textures with error
     */
    void removeEntryOfErrorVector(const std::string &name) {texturesWithError.erase(name);}


    /**
     * Register an external texture is GlTextureManager
     */
    void registerExternalTexture(const std::string &textureName, const GLuint textureId);

  private:

    /**
     * empty private constructor for singleton
     */
    GlTextureManager();

    bool loadTexture(const std::string&,const TextureInfo &,GlTexture &);

    static GlTextureManager* inst;

    OpenGlErrorViewer *errorViewer;

    unsigned long currentContext;

    ContextAndTextureMap texturesMap;
    std::set<std::string> texturesWithError;

    unsigned int animationFrame;

  };

}

#endif // Tulip_GLTEXTUREMANAGER_H
