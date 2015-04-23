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
#ifndef IMAGE_WRITER_H // -*-c++-*-
#define IMAGE_WRITER_H
#include <iostream>
#include <string>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

namespace tlprender {
extern const std::string SUPPORTED_IMAGE_FORMATS[];

class ImageWriter {
protected:
  const GLubyte *buffer;
  int width, height;
  std::string title, software, source, layout, comment;
  bool titleState, softwareState, sourceState, layoutState, commentState;
  ImageWriter(const GLubyte *imagebuffer, const int width, const int height);

public:
  virtual ~ImageWriter() {}
  static ImageWriter *getImageWriter(std::string &type, const GLubyte *buffer, const int width, const int height);
  void setTitle(const std::string &);
  void setSoftware(const std::string &);
  void setSource(const std::string &);
  void setLayout(const std::string &);
  void setComment(const std::string &);

  //an image writer will define this method and write date to the provided stream
  virtual bool writeImage(std::ostream &) const = 0;
};
}

#endif //IMAGE_WRITER_H
