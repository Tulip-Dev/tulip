/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#ifndef Tulip_GLTEXTUREMANAGER_H
#define Tulip_GLTEXTUREMANAGER_H

#include <tulip/tulipconf.h>
#ifndef TULIP_BUILD_GL_TEX_LOADER
#include <tulip/TulipException.h>
#endif
#include <tulip/OpenGlIncludes.h>

#include <set>
#include <tulip/tuliphash.h>
#include <string>

namespace tlp {

struct GlTexture {
  GLuint *id;
  int height;
  int width;
  unsigned int spriteNumber;
};

/**
 *  \brief Class to load textures
 */
class TLP_GL_SCOPE GlTextureLoader {
public:
  /**
   * Load a texture from a file
   * in the default implementation only bmp, jpeg and png files
   * can be loaded.
   * Return false if an error occurs
   */
#ifdef TULIP_BUILD_GL_TEX_LOADER
  virtual bool loadTexture(const std::string &filename, GlTexture &texture);
#else
  virtual bool loadTexture(const std::string &filename, GlTexture &texture) = 0;
#endif
  virtual ~GlTextureLoader() {}
};

/**
 * class to load/store textures need by OpenGL rendering
 */
class TLP_GL_SCOPE GlTextureManager {

  typedef tlp_hash_map<std::string, GlTexture> TextureMap;

public:
  /**
   * Return texture info (id, width and height) for the given name
   */
  static GlTexture getTextureInfo(const std::string &);

  /**
   * Check if a texture for the given name exists in the current context
   */
  static bool existsTexture(const std::string &filename);
  /**
   * Load texture with given name
   */
  static bool loadTexture(const std::string &);
  /**
   * Remove texture with given name
   */
  static void deleteTexture(const std::string &);
  /**
   * Begin a new texture with given name
   */
  static void beginNewTexture(const std::string &);
  /**
   * Activate a texture with given name
   */
  static bool activateTexture(const std::string &, unsigned int);
  /**
   * Activate a texture with given name
   */
  static bool activateTexture(const std::string &);
  /**
   * Disable texture with given name
   */
  static void deactivateTexture();
  /**
   * Set animationStep for next textures (for next activateTexture)
   */
  static inline void setAnimationFrame(unsigned int id) {
    animationFrame = id;
  }
  /**
   * Get animationStep of next textures
   */
  static inline unsigned int getAnimationFrame() {
    return animationFrame;
  }
  /**
   * Clear vector of textures with error
   */
  static inline void clearErrorVector() {
    texturesWithError.clear();
  }
  /**
   * Remove an entry of vector of textures with error
   */
  static inline void removeEntryOfErrorVector(const std::string &name) {
    texturesWithError.erase(name);
  }

  /**
   * Register an external texture is GlTextureManager
   */
  static void registerExternalTexture(const std::string &textureName, const GLuint textureId);

  /**
   * Get Texture loader
   */
  static inline GlTextureLoader *getTextureLoader() {
#ifdef TULIP_BUILD_GL_TEX_LOADER
    return loader ? loader : (loader = new GlTextureLoader());
#else
    if (!loader)
      throw TulipException("GlTextureLoader Error: no texture loader found");
    return loader;
#endif
  }

  /**
   * Set Texture loader
   */
  static void setTextureLoader(GlTextureLoader *texLoader) {
    if (loader)
      delete loader;

    loader = texLoader;
  }

  static void deleteAllTextures();

private:
  static GlTextureLoader *loader;

  static TextureMap texturesMap;
  static std::set<std::string> texturesWithError;

  static unsigned int animationFrame;
};
} // namespace tlp

#endif // Tulip_GLTEXTUREMANAGER_H
///@endcond
