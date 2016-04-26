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
 * Copyright (c) 2016 Antoine Lambert, Thales Services
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#ifndef GLFRAMEBUFFEROBJECT_H
#define GLFRAMEBUFFEROBJECT_H

#if defined(_MSC_VER)
#include <windows.h>
#endif

#include <GL/gl.h>

#include <tulip/tulipconf.h>

namespace tlp {

class TLP_GLES_SCOPE GlFrameBufferObject {

public:

  enum Attachment {
    NoAttachment,
    Depth,
    Stencil,
    CombinedDepthStencil
  };

  GlFrameBufferObject(int width, int height, Attachment attachement = Depth,
                      GLint textureMagFilter = GL_NEAREST, GLint textureMinFilter = GL_NEAREST,
                      GLint textureWrap = GL_CLAMP_TO_EDGE, bool generateMipmap = false);

  ~GlFrameBufferObject();

  int width() const {
    return _width;
  }

  int height() const {
    return _height;
  }

  void bind();

  void release();

  bool isValid() const {
    return _isValid;
  }

  GLuint texture() {
    return _texture;
  }

  static bool bufferBound();

private:

  int _width, _height;
  GLuint _fboHandle;
  GLuint _texture;
  GLuint _attachmentRbo;
  bool _isValid;

};

}

#endif // GLFRAMEBUFFEROBJECT_H
