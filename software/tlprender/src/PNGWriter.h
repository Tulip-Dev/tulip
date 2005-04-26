#ifndef PNGWRITER_H // -*-c++-*-
#define PNGWRITER_H

#include <iostream>

#include "ImageWriter.h"

namespace tlprender {
  class PNGWriter : public ImageWriter {
  public:
    PNGWriter(const GLubyte *imagebuffer, const int width, const int height);
    ~PNGWriter() {}
    bool writeImage(std::ostream &) const;
  };

}
#endif //PNGWRITER_H
