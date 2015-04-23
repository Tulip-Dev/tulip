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
#include <png.h>

#include "PNGWriter.h"

using namespace std;

//functions needed to use  c++ streams with libpng
static void stream_write_data(png_structp, png_bytep, png_size_t);
static void stream_flush_data(png_structp);

namespace tlprender {
PNGWriter::PNGWriter(const GLubyte *imagebuffer, const int width, const int height):
  ImageWriter(imagebuffer, width, height) {
}

bool PNGWriter::writeImage(std::ostream &os) const {
  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr) {
    return false;
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);

  if (!info_ptr) {
    png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
    return false;
  }

#ifdef PNG_SETJMP_SUPPORTED

  if (setjmp(png_jmpbuf(png_ptr)))
#endif
  {
    png_destroy_write_struct(&png_ptr, &info_ptr);
    return false;
  }

  png_set_write_fn(png_ptr, &os, &stream_write_data, &stream_flush_data);
  /* PNG Header */
  png_set_IHDR(png_ptr, info_ptr, width, height, 8,
               PNG_COLOR_TYPE_RGB_ALPHA, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

  int num_text = 0 + titleState + softwareState + sourceState + layoutState + commentState;
  png_text *text_ptr = new png_text[num_text];
  int i=0;

  if (titleState) {
    text_ptr[i].compression = PNG_TEXT_COMPRESSION_NONE;
    text_ptr[i].key = "Title";
    text_ptr[i].text = const_cast<char *>(title.c_str());
    ++i;
  }

  if (softwareState) {
    text_ptr[i].compression = PNG_TEXT_COMPRESSION_NONE;
    text_ptr[i].key = "Software";
    text_ptr[i].text = const_cast<char *>(software.c_str());
    ++i;
  }

  if (sourceState) {
    text_ptr[i].compression = PNG_TEXT_COMPRESSION_NONE;
    text_ptr[i].key = "Source";
    text_ptr[i].text = const_cast<char *>(source.c_str());
    ++i;
  }

  if (layoutState) {
    text_ptr[i].compression = PNG_TEXT_COMPRESSION_NONE;
    text_ptr[i].key = "Layout";
    text_ptr[i].text = const_cast<char *>(layout.c_str());
  }

  if (commentState) {
    text_ptr[i].compression = PNG_TEXT_COMPRESSION_NONE;
    text_ptr[i].key = "Comment";
    text_ptr[i].text = const_cast<char *>(comment.c_str());
    ++i;
  }

  png_set_text(png_ptr, info_ptr, text_ptr, num_text);

  png_write_info(png_ptr, info_ptr);
  delete text_ptr[];

  /* Image */
  for (int i=height-1; i>=0; --i) {
    png_bytep rowp = (png_bytep) buffer + (width * i * 4 * sizeof(GLubyte));
    png_write_row(png_ptr, rowp);
  }

  /* PNG EOF */
  png_write_end(png_ptr, info_ptr);
  png_destroy_write_struct(&png_ptr, &info_ptr);
  return true;
}
}

static void stream_write_data(png_structp png_ptr, png_bytep data, png_size_t length) {
  ostream *os = (ostream *)png_get_io_ptr(png_ptr);

  for(png_size_t i=0; i<length; ++i) {
    (*os) << data[i];
  }
}

static void stream_flush_data(png_structp png_ptr) {
  ostream *os = (ostream *)png_get_io_ptr(png_ptr);
  os->flush();
}
