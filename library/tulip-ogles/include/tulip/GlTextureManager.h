/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

/**
 *
 * tulip-ogles library : rewrite of old tulip-ogl library using OpenGL ES API
 * Copyright (c) 2016 Antoine Lambert, Thales Services SAS
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#ifndef GLTEXTUREMANAGER_H
#define GLTEXTUREMANAGER_H

#if defined(_MSC_VER)
#include <windows.h>
#endif

#include <GL/gl.h>

#include <tulip/Vector.h>
#include <tulip/GlUtils.h>
#include <tulip/TextureAtlas.h>

#include <map>
#include <vector>

namespace tlp {

class TLP_GLES_SCOPE GlTextureManager {

public:
  static GlTextureManager *instance(const std::string &canvasId);

  static GlTextureManager *instance();

  static void setCurrentCanvasId(const std::string &canvasId) {
    _currentCanvasId = canvasId;
  }

  ~GlTextureManager();

  void addTextureInAtlasFromFile(const std::string &textureFile);
  void addTextureInAtlasFromData(const std::string &textureName, const unsigned char *textureData, unsigned int width, unsigned int height);
  void bindTexturesAtlas();
  void unbindTexturesAtlas();

  void addTextureFromFile(const std::string &textureFile, bool addAlsoInAtlas = false);
  void addTextureFromData(const std::string &textureName, const unsigned char *textureData, unsigned int width, unsigned int height,
                          bool addAlsoInAtlas = false);
  void addExternalTexture(const std::string &textureId, const GLuint glTextureId);
  void bindTexture(const std::string &textureId, const unsigned int textureUnit = 0);
  void unbindTexture(const std::string &textureId);

  GLint getSamplerIdForTexture(const std::string &texture, bool forceUseAtlas = false);
  tlp::Vec4f getCoordinatesOffsetsForTexture(const std::string &texture, bool forceUseAtlas = false);

  void setTextureLoader(GlTextureLoader *textureLoader);

private:
  void initMaxTextureSize();
  static std::map<std::string, GlTextureManager *> _instances;
  static std::string _currentCanvasId;

  GlTextureManager();

  TextureAtlas *_texturesAtlas[4];
  GLuint _currentUnit;

  std::map<std::string, GLuint> _textureUnit;
  std::map<std::string, GLuint> _textureAtlasUnit;
  std::map<std::string, tlp::Vec4f> _coordinatesOffsets;

  std::map<std::string, GLuint> _textures;

  GlTextureLoader *_textureLoader;
};
}

#endif // GLTEXTUREMANAGER_H
