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

#include <tulip/GlBuffer.h>

using namespace tlp;

std::map<GlBuffer::BufferType, GLuint> GlBuffer::_currentBoundBufferId;

GlBuffer::GlBuffer(BufferType type, UsagePattern usagePattern) :
  _type(type), _usagePattern(usagePattern), _bufferId(0), _allocated(false) {
  glGenBuffers(1, &_bufferId);
  if (type == VertexBuffer) {
    _target = GL_ARRAY_BUFFER;
  } else {
    _target = GL_ELEMENT_ARRAY_BUFFER;
  }
  setUsagePattern(usagePattern);
  if (_currentBoundBufferId.empty()) {
    _currentBoundBufferId[VertexBuffer] = 0;
    _currentBoundBufferId[IndexBuffer] = 0;
  }
}

GlBuffer::~GlBuffer() {
  if (isBound())
    release();
  glDeleteBuffers(1, &_bufferId);
}

void GlBuffer::setUsagePattern(UsagePattern usagePattern) {
  _usagePattern = usagePattern;
  if (usagePattern == StreamDraw) {
    _usage = GL_STREAM_DRAW;
  } else if (usagePattern == StaticDraw) {
    _usage = GL_STATIC_DRAW;
  } else {
    _usage = GL_DYNAMIC_DRAW;
  }
}

void GlBuffer::allocate(unsigned int bytesCount, const void *data) {
  if (bytesCount > 0) {
    bool bound = isBound();
    if (!bound) bind();
    glBufferData(_target, bytesCount, data, _usage);
    _allocated = true;
    if (!bound) release();
  }
}

void GlBuffer::allocate(unsigned int bytesCount) {
  allocate(bytesCount, nullptr);
}

void GlBuffer::allocate(const std::vector<float> &data) {
  allocate(data.size() * sizeof(float), &data[0]);
}

void GlBuffer::allocate(const std::vector<unsigned int> &data) {
  allocate(data.size() * sizeof(unsigned int), &data[0]);
}

void GlBuffer::allocate(const std::vector<unsigned short> &data) {
  allocate(data.size() * sizeof(unsigned short), &data[0]);
}

unsigned int GlBuffer::bytesCount() const {
  bool bound = isBound();
  if (!bound) bind();
  GLint bytesCount = 0;
  glGetBufferParameteriv(_target, GL_BUFFER_SIZE, &bytesCount);
  if (!bound) release();
  return static_cast<unsigned int>(bytesCount);
}

void GlBuffer::write(unsigned int offset, unsigned int bytesCount, const void *data) {
  if (!allocated()) {
    std::cerr << "GlBuffer error : can't write to buffer " << _bufferId << " as it has not been allocated" << std::endl;
    return;
  } else if ((offset + bytesCount) > this->bytesCount()) {
    std::cerr << "GlBuffer error : can't write " << bytesCount << " bytes in buffer " << _bufferId << " at offset "
              << offset << " as the data will not entirely fit in (the buffer can contain up to " << this->bytesCount() << " bytes)" << std::endl;
    return;
  }
  if (bytesCount > 0) {
    bool bound = isBound();
    if (!bound) bind();
    glBufferSubData(_target, offset, bytesCount, data);
    if (!bound) release();
  }
}

void GlBuffer::write(unsigned int bytesCount, const void *data) {
  write(0, bytesCount, data);
}

void GlBuffer::write(unsigned int offset, const std::vector<float> &data) {
  write(offset, data.size() * sizeof(float), &data[0]);
}

void GlBuffer::write(unsigned int offset, const std::vector<unsigned int> &data) {
  write(offset, data.size() * sizeof(unsigned int), &data[0]);
}

void GlBuffer::write(unsigned int offset, const std::vector<unsigned short> &data) {
  write(offset, data.size() * sizeof(unsigned short), &data[0]);
}

void GlBuffer::write(const std::vector<float> &data) {
  write(0, data);
}

void GlBuffer::write(const std::vector<unsigned int> &data) {
  write(0, data);
}

void GlBuffer::write(const std::vector<unsigned short> &data) {
  write(0, data);
}

void GlBuffer::bind() const {
  if (_currentBoundBufferId[_type] == _bufferId) return;
  glBindBuffer(_target, _bufferId);
  _currentBoundBufferId[_type] = _bufferId;
}

bool GlBuffer::isBound() const {
  return _currentBoundBufferId[_type] == _bufferId;
}

void GlBuffer::release() const {
  glBindBuffer(_target, 0);
  _currentBoundBufferId[_type] = 0;
}

void GlBuffer::release(BufferType type) {
  if (type == VertexBuffer) {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
  } else {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  }
  _currentBoundBufferId[type] = 0;
}

