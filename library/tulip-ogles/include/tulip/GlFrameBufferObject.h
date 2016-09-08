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

#ifndef GLFRAMEBUFFEROBJECT_H
#define GLFRAMEBUFFEROBJECT_H

#if defined(_MSC_VER)
#include <windows.h>
#endif

#include <GL/gl.h>

#include <tulip/tulipconf.h>
#include <tulip/Vector.h>

namespace tlp {

class TLP_GLES_SCOPE GlFrameBufferObject {

public:

  enum Attachment {
    NoAttachment,
    Depth,
    Stencil,
    CombinedDepthStencil
  };

  GlFrameBufferObject(int width, int height, Attachment attachement = Depth, unsigned int numberOfSamples = 0,
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

  static bool hasOpenGLFramebufferBlit();

  static void blitFramebuffer(GlFrameBufferObject *target, const Vec2i &targetX0Y0, const Vec2i &targetX1Y1,
                              GlFrameBufferObject *source, const Vec2i &sourceX0Y0, const Vec2i &sourceX1Y1,
                              GLbitfield mask = GL_COLOR_BUFFER_BIT, GLenum filter = GL_NEAREST);

private:

  GLuint handle() const {
    return _fboHandle;
  }


  int _width, _height;
  GLuint _fboHandle;
  GLuint _texture;
  GLuint _attachmentRbo;
  bool _isValid;

};

}

#endif // GLFRAMEBUFFEROBJECT_H
