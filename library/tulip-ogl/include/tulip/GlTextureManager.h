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
#if defined(__APPLE__)
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include <string>
#include <set>
#include <iostream>

namespace tlp {

  struct GlTexture {
    GLuint *id;
    int height;
    int width;
    unsigned int spriteNumber;
  };

  struct TextureInfo{
    bool hasAlpha;
    int  width;
    int  height;
    unsigned char *data;
  };

  class TLP_GL_SCOPE GlTextureManagerErrorViewer {

  public :

    virtual void displayError(const std::string &fileName,const std::string &errorMsg){
      std::cerr << errorMsg << std::endl;
    }

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
    GlTextureManagerErrorViewer *setErrorViewer(GlTextureManagerErrorViewer *errorViewer){
      GlTextureManagerErrorViewer *oldErrorViewer=this->errorViewer;
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
     * Register an external texture is GlTextureManager
     */
    void registerExternalTexture(const std::string &textureName, const GLuint textureId);

  private:

    /**
     * empty private constructor for singleton
     */
    GlTextureManager():animationFrame(0),errorViewer(new GlTextureManagerErrorViewer) {}

    bool loadTexture(const std::string&,const TextureInfo &,GlTexture &);

    static GlTextureManager* inst;

    GlTextureManagerErrorViewer *errorViewer;

    unsigned long currentContext;

    ContextAndTextureMap texturesMap;

    unsigned int animationFrame;

  };

}

#endif // Tulip_GLTEXTUREMANAGER_H
