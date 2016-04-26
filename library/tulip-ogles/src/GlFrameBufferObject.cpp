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

#include <GL/glew.h>

#include <tulip/GlFrameBufferObject.h>

using namespace tlp;

GlFrameBufferObject::GlFrameBufferObject(int width, int height, Attachment attachement,
                                         GLint textureMagFilter, GLint textureMinFilter,
                                         GLint textureWrap, bool generateMipmap) :
  _width(width), _height(height), _fboHandle(0), _texture(0), _attachmentRbo(0), _isValid(false) {

  glGenFramebuffers(1, &_fboHandle);
  glBindFramebuffer(GL_FRAMEBUFFER, _fboHandle);

  glGenTextures(1, &_texture);
  glBindTexture(GL_TEXTURE_2D, _texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureMagFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureMinFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrap);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrap);
  if (generateMipmap) {
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
  }
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);

  glGenRenderbuffers(1, &_attachmentRbo);
  glBindRenderbuffer(GL_RENDERBUFFER, _attachmentRbo);

  if (attachement == Depth) {
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _attachmentRbo);
  }

  if (attachement == Stencil) {
    glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _attachmentRbo);
  }

  if (attachement == CombinedDepthStencil) {
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _attachmentRbo);
  }

  GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  _isValid = (status == GL_FRAMEBUFFER_COMPLETE);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GlFrameBufferObject::~GlFrameBufferObject() {
  if (_texture != 0) {
    glDeleteTextures(1, &_texture);
  }
  if (_attachmentRbo != 0) {
    glDeleteRenderbuffers(1, &_attachmentRbo);
  }
  if (_fboHandle != 0) {
    glDeleteFramebuffers(1, &_fboHandle);
  }
}

void GlFrameBufferObject::bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, _fboHandle);
}


void GlFrameBufferObject::release() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool GlFrameBufferObject::bufferBound() {
  int boundFB = 0;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &boundFB);
  return boundFB != 0;
}
