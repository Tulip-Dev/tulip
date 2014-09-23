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
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>

extern "C" {
#include <jpeglib.h>
}

#include "JPEGWriter.h"
#define JPEG_IOBUFFER_SIZE (16 * 1024)

using namespace std;

struct jpeg_destination_handler {
  JOCTET *buffer;
  std::ostream *os_ptr;
};

static  void init_destination (j_compress_ptr);
static  boolean empty_output_buffer (j_compress_ptr);
static  void term_destination (j_compress_ptr);

namespace tlprender {
JPEGWriter::JPEGWriter(const GLubyte *imagebuffer, const int width, const int height) :
  ImageWriter(imagebuffer, width, height) {}

JPEGWriter::~JPEGWriter() {}

bool JPEGWriter::writeImage(std::ostream &os) const {
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;

  jpeg_create_compress(&cinfo);
  cinfo.err = jpeg_std_error(&jerr);
  cinfo.image_width = width;
  cinfo.image_height = height;
  cinfo.input_components = 3;
  cinfo.in_color_space = JCS_RGB;

  struct jpeg_destination_mgr dest_mgr;
  dest_mgr.init_destination = &init_destination;
  dest_mgr.empty_output_buffer = &empty_output_buffer;
  dest_mgr.term_destination = &term_destination;

  cinfo.dest = &dest_mgr;
  cinfo.client_data = &os;

  jpeg_set_defaults(&cinfo);
  jpeg_set_quality(&cinfo, 255, FALSE);

  jpeg_start_compress(&cinfo, TRUE);

  stringstream ss;

  if (titleState) ss << "Title: " << title << endl;

  if (softwareState) ss << "Software: " << software << endl;

  if (sourceState) ss << "Source: " << source << endl;

  if (layoutState) ss << "Layout: " << layout << endl;

  if (commentState) ss << "Comment: " << comment << endl;

  jpeg_write_marker(&cinfo, JPEG_COM, (const JOCTET *) ss.str().c_str(), ss.str().length());

  JSAMPROW row_pointer = new JSAMPLE[width * 3];

  while (cinfo.next_scanline < cinfo.image_height) {
    const GLubyte *offset = & buffer[width * (cinfo.image_height - cinfo.next_scanline) * 4 * sizeof(GLubyte)];

    for (int i=0; i < width; ++i) {
      memcpy(row_pointer+(i*3), offset+(i*4), 3);
    }

    jpeg_write_scanlines(&cinfo, &row_pointer, 1);
  }

  delete [] row_pointer;

  jpeg_finish_compress(&cinfo);
  jpeg_destroy_compress(&cinfo);
  return true;
}
}

static void init_destination (j_compress_ptr cinfo) {
  struct jpeg_destination_handler *dest_handler = new struct jpeg_destination_handler;
  dest_handler->buffer = new JOCTET[JPEG_IOBUFFER_SIZE];
  dest_handler->os_ptr = (std::ostream *) cinfo->client_data;
  cinfo->client_data = dest_handler;
  cinfo->dest->next_output_byte = dest_handler->buffer;
  cinfo->dest->free_in_buffer = JPEG_IOBUFFER_SIZE;
}

static boolean empty_output_buffer (j_compress_ptr cinfo) {
  struct jpeg_destination_handler *dest_handler = (struct jpeg_destination_handler *) cinfo->client_data;

  for (int i=0; i < JPEG_IOBUFFER_SIZE; ++i) {
    *(dest_handler->os_ptr) << (dest_handler->buffer)[i];
  }

  cinfo->dest->next_output_byte = dest_handler->buffer;
  cinfo->dest->free_in_buffer = JPEG_IOBUFFER_SIZE;
  return TRUE;
}

static void term_destination (j_compress_ptr cinfo) {
  struct jpeg_destination_handler *dest_handler = (struct jpeg_destination_handler *) cinfo->client_data;

  for (size_t i=0; i < (JPEG_IOBUFFER_SIZE - cinfo->dest->free_in_buffer); ++i) {
    *(dest_handler->os_ptr) << (dest_handler->buffer)[i];
  }

  dest_handler->os_ptr->flush();
  delete dest_handler->buffer;
  delete dest_handler;
}
