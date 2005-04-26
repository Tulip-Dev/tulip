#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "ImageWriter.h"
#ifdef HAVE_LIBPNG
#  include "PNGWriter.h"
#endif
#ifdef HAVE_LIBJPEG
#  include "JPEGWriter.h"
#endif

using namespace std;

namespace tlprender
{
  const string SUPPORTED_IMAGE_FORMATS[] = {
#ifdef HAVE_LIBPNG
    "png",
#endif
#ifdef HAVE_LIBJPEG
    "jpeg",
#endif
    ""
  };

  //constructor
  ImageWriter::ImageWriter(const GLubyte *imagebuffer, const int width, const int height) :
    buffer(imagebuffer),
    width(width), height(height),
    title(""), software(""), source(""), layout(""), comment(""),
    titleState(false), softwareState(false), sourceState(false), layoutState(false), commentState(false)
  {}

  ImageWriter *ImageWriter::getImageWriter(string &type, const GLubyte *buffer, const int width, const int height)
  {
#ifdef HAVE_LIBPNG
    if (type=="png") return new tlprender::PNGWriter(buffer, width, height);
    else
#endif
#ifdef HAVE_LIBJPEG
      if (type=="jpeg") return new tlprender::JPEGWriter(buffer, width, height);
      else
#endif
        {
          return NULL;
        }
  }

  void ImageWriter::setTitle(const string &s) {title = s; titleState = (s!="");}
  void ImageWriter::setSoftware(const string &s) {software = s; softwareState = (s!="");}
  void ImageWriter::setSource(const string &s) {source = s; sourceState = (s!="");}
  void ImageWriter::setLayout(const string &s) {layout = s; layoutState = (s!="");}
  void ImageWriter::setComment(const string &s) {comment = s; commentState = (s!="");}
  
} //namespace tlprender
