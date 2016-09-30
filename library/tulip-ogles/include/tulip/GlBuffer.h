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

#ifndef GLBUFFER_H
#define GLBUFFER_H

#if defined(_MSC_VER)
#include <windows.h>
#endif

#include <GL/gl.h>

#include <tulip/Vector.h>

#include <map>
#include <vector>

namespace tlp {

class TLP_GLES_SCOPE GlBuffer {

public:
  enum BufferType { VertexBuffer, IndexBuffer };

  enum UsagePattern { StreamDraw, StaticDraw, DynamicDraw };

  GlBuffer(BufferType type, UsagePattern usagePattern = StaticDraw);
  ~GlBuffer();

  GLuint bufferId() const {
    return _bufferId;
  }

  BufferType type() const {
    return _type;
  }

  void setUsagePattern(UsagePattern usagePattern);

  UsagePattern usagePattern() const {
    return _usagePattern;
  }

  void allocate(unsigned int bytesCount, const void *data);
  void allocate(unsigned int bytesCount);

  void allocate(const std::vector<float> &data);
  void allocate(const std::vector<unsigned int> &data);
  void allocate(const std::vector<unsigned short> &data);

  template <unsigned int SIZE> void allocate(const std::vector<tlp::Vector<float, SIZE>> &data) {
    allocate(data.size() * SIZE * sizeof(float), &data[0]);
  }

  bool allocated() const {
    return _allocated;
  }

  void write(unsigned int offset, unsigned int bytesCount, const void *data);
  void write(unsigned int bytesCount, const void *data);

  void write(unsigned int offset, const std::vector<float> &data);
  void write(unsigned int offset, const std::vector<unsigned int> &data);
  void write(unsigned int offset, const std::vector<unsigned short> &data);

  void write(const std::vector<float> &data);
  void write(const std::vector<unsigned int> &data);
  void write(const std::vector<unsigned short> &data);

  template <unsigned int SIZE> void write(unsigned int offset, const std::vector<tlp::Vector<float, SIZE>> &data) {
    write(offset, data.size() * SIZE * sizeof(float), &data[0]);
  }

  template <unsigned int SIZE> void write(const std::vector<tlp::Vector<float, SIZE>> &data) {
    write(data.size() * SIZE * sizeof(float), &data[0]);
  }

  unsigned int bytesCount() const;

  void bind() const;
  bool isBound() const;
  void release() const;

  static void release(BufferType type);

private:
  BufferType _type;
  UsagePattern _usagePattern;
  GLenum _target;
  GLenum _usage;
  GLuint _bufferId;
  bool _allocated;

  static std::map<BufferType, GLuint> _currentBoundBufferId;
};
}

#endif // GLBUFFER_H
