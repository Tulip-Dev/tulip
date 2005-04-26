#ifndef JPEGWRITER_H // -*-c++-*-
#define JPEGWRITER_H

#include <iostream>

#include "ImageWriter.h"

namespace tlprender {
  class JPEGWriter : public ImageWriter {
  public:
    JPEGWriter(const GLubyte *imagebuffer, const int width, const int height);
    ~JPEGWriter();
    bool writeImage(std::ostream &) const;
  };
}

#endif //JPEGWRITER_H
