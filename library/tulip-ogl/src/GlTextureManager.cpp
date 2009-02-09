#include "tulip/GlTextureManager.h"

#include <iostream>

extern "C" {
#include <stdio.h>

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#ifdef HAVE_LIBJPEG
#include <jpeglib.h>
#endif
#ifdef HAVE_LIBPNG
#include <png.h>
#endif
};

//====================================================
#ifdef _WIN32
#ifdef DLL_EXPORT
tlp::GlTextureManager* tlp::GlTextureManager::inst=0;
#endif
#else
tlp::GlTextureManager* tlp::GlTextureManager::inst=0;
#endif

using namespace std;

namespace tlp
{

typedef struct {
  bool hasAlpha;
  int  width;
  int  height;
  unsigned char *data;
} textureImage;

typedef bool (TextureLoader_t)(const string &, textureImage *);

//====================================================================
/* simple loader for 24bit bitmaps (data is in rgb-format) */
static bool loadBMP(const string &filename, textureImage *texture) {

#ifndef NDEBUG
  cerr << __PRETTY_FUNCTION__ << ": filename=" << filename << endl;
#endif

  FILE *file;
  unsigned short int bfType;
  long int bfOffBits;
  short int biPlanes;
  short int biBitCount;
  long int biSizeImage;
  int i;
  unsigned char temp;
  /* make sure the file is there and open it read-only (binary) */
  if ((file = fopen(filename.c_str(), "rb")) == NULL)
    {
      cerr << __PRETTY_FUNCTION__ << ": File not found :" << filename << endl;
      return false;
    }
  if(!fread(&bfType, sizeof(short int), 1, file))
    {
      cerr << __PRETTY_FUNCTION__ << ": Error reading " << filename << endl;
      fclose(file);
      return false;
    }
  /* check if file is a bitmap */
  if (bfType != 19778)
    {
      cerr << __PRETTY_FUNCTION__ << ": Not a Bitmap-File: " << filename << endl;
      fclose(file);
      return false;
    }
  /* get the file size */
  /* skip file size and reserved fields of bitmap file header */
  fseek(file, 8, SEEK_CUR);
  /* get the position of the actual bitmap data */
  if (!fread(&bfOffBits, sizeof(long int), 1, file))
    {
      cerr << __PRETTY_FUNCTION__ << ": Error reading " << filename << endl;
      fclose(file);
      return false;
    }
  /* skip size of bitmap info header */
  fseek(file, 4, SEEK_CUR);
  fread(&texture->width, sizeof(int), 1, file);
  fread(&texture->height, sizeof(int), 1, file);

  /* get the number of planes (must be set to 1) */
  fread(&biPlanes, sizeof(short int), 1, file);
  if (biPlanes != 1)
    {
      cerr << __PRETTY_FUNCTION__ << ": Error: number of Planes not 1: " << filename << endl;
      fclose(file);
      return false;
    }
  /* get the number of bits per pixel */
  if (!fread(&biBitCount, sizeof(short int), 1, file))
    {
      cerr << __PRETTY_FUNCTION__ << ": Error reading file: " << filename << endl;
      fclose(file);
      return false;
    }
  if (biBitCount != 24)
    {
      cerr << __PRETTY_FUNCTION__ << ": Bits per Pixel not 24: " << filename << endl;
      fclose(file);
      return false;
    }
  /* calculate the size of the image in bytes */
  biSizeImage = texture->width * texture->height * 3;
  texture->data = new unsigned char[biSizeImage];
  /* seek to the actual data */
  fseek(file, bfOffBits, SEEK_SET);
  if (!fread(texture->data, biSizeImage, 1, file))
    {
      cerr << __PRETTY_FUNCTION__ << ": Error loading file: " << filename << endl;
      delete [] texture->data;
      texture->data=NULL;
      fclose(file);
      return false;
    }
  /* swap red and blue (bgr -> rgb) */
  for (i = 0; i < biSizeImage; i += 3) {
    temp = texture->data[i];
    texture->data[i] = texture->data[i + 2];
    texture->data[i + 2] = temp;
  }
  fclose(file);
  texture->hasAlpha = false;
  return true;
}
//====================================================================
#ifdef HAVE_LIBJPEG
static bool loadJPEG(const string &filename, textureImage *texture) {
#ifndef NDEBUG
  cerr << __PRETTY_FUNCTION__ << ": filename=" << filename << endl;
#endif
  FILE *file;
  if ((file = fopen(filename.c_str(), "rb")) == NULL) {
    cerr << __PRETTY_FUNCTION__ << ": File not found:" << filename << endl;
    return false;
  }

  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);
  jpeg_stdio_src(&cinfo, file);
  jpeg_read_header(&cinfo, TRUE);

  cinfo.out_color_components = 3;
  cinfo.out_color_space = JCS_RGB;
  cinfo.dct_method = JDCT_FLOAT;
  cinfo.rec_outbuf_height = 1;

  jpeg_start_decompress(&cinfo);
  texture->hasAlpha = false;
  texture->width    = cinfo.output_width;
  texture->height   = cinfo.output_height;
  texture->data = new unsigned char[texture->width * texture->height * 3];

  JSAMPROW row_pointer = new JSAMPLE[texture->width * 3];
  while (cinfo.output_scanline < cinfo.output_height) {
    jpeg_read_scanlines(&cinfo, &row_pointer, 1);
    memcpy((void *) &(texture->data[(cinfo.output_height - cinfo.output_scanline) * 3 * cinfo.output_width]), row_pointer, (texture->width) * 3);
  }
  delete [] row_pointer;
  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);
  fclose(file);
  return true;
}
#endif
//====================================================================
#ifdef HAVE_LIBPNG
static bool loadPNG(const string &filename, textureImage *texture)
{
#ifndef NDEBUG
  cerr << __PRETTY_FUNCTION__ << ": filename=" << filename << endl;
#endif

  FILE *file;

  if ((file = fopen(filename.c_str(), "rb")) == NULL) {
    cerr << __PRETTY_FUNCTION__ << ": File not found:" << filename << endl;
    return false;
  }

  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr) {
    fclose(file);
    return false;
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr) {
      png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
      fclose(file);
      return false;
  }

  png_infop end_info = png_create_info_struct(png_ptr);
  if (!end_info) {
    png_destroy_read_struct(&png_ptr, &info_ptr,  (png_infopp)NULL);
    fclose(file);
    return (false);
  }

#ifdef PNG_SETJMP_SUPPORTED
  if (setjmp(png_jmpbuf(png_ptr)))
#endif
    {
      png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
      fclose(file);
      return file;
    }

  png_init_io(png_ptr, file);
  png_read_info(png_ptr, info_ptr);
/*
  png_uint_32 width, height;
  int bit_depth, color_type, interlace_method, compression_method, filter_method;
  png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth,
	       &color_type, &interlace_method, &compression_method, &filter_method);
*/

  int ctype = png_get_color_type( png_ptr, info_ptr );
  texture->hasAlpha = ( ctype == PNG_COLOR_TYPE_GRAY_ALPHA ) || ( ctype == PNG_COLOR_TYPE_RGB_ALPHA );
  texture->width    = png_get_image_width( png_ptr, info_ptr );
  texture->height   = png_get_image_height( png_ptr, info_ptr );
  int linestride = texture->width * (texture->hasAlpha ? 4 : 3);
  texture->data = new unsigned char[linestride * texture->height];
  png_bytep row_pointers[texture->height];
  for (int i=0; i < texture->height; ++i)
    row_pointers[i] = (png_bytep) &(texture->data[linestride*(texture->height-1-i)]);

  png_set_strip_16(png_ptr);	//force 8 bits/channel
  png_set_gray_to_rgb(png_ptr); //force RGB
  png_read_image(png_ptr, row_pointers);
  png_read_end(png_ptr, end_info);
  png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
  fclose(file);
  return true;
}
#endif
//====================================================================
bool GlTextureManager::loadTextureFromRawData(const string &textureName, int width, int height, bool hasAlpha, unsigned char *data)
{
  glEnable(GL_TEXTURE_2D);
  if (texturesMap[currentContext].find(textureName) != texturesMap[currentContext].end())
      return true;

  GLuint textureNum;
  textureImage texti;

  texti.width = width;
  texti.height = height;
  texti.hasAlpha = hasAlpha;
  texti.data = data;

  int GLFmt = texti.hasAlpha ? GL_RGBA : GL_RGB;

  GlTexture texture;
  texture.width=texti.width;
  texture.height=texti.height;

  glGenTextures(1, &textureNum);  //FIXME: handle case where no memory is available to load texture
  glBindTexture(GL_TEXTURE_2D, textureNum);

  texture.id=textureNum;

  glTexImage2D(GL_TEXTURE_2D, 0, GLFmt, texti.width, texti.height, 0, GLFmt, GL_UNSIGNED_BYTE, texti.data);
  /* use no filtering */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //delete [] texti.data;
  (texturesMap[currentContext])[textureName] = texture;

  return true;
}
//====================================================================
  void GlTextureManager::changeContext(unsigned long context) {
    currentContext=context;
  }
//====================================================================
  void GlTextureManager::removeContext(unsigned long context) {
    texturesMap.erase(context);
  }
//====================================================================
GlTexture GlTextureManager::getTextureInfo(const string& filename) {
  if(texturesMap[currentContext].find(filename) != texturesMap[currentContext].end())
    return (texturesMap[currentContext])[filename];
  else
    return GlTexture();
}

//====================================================================
bool GlTextureManager::loadTexture(const string& filename)
{
  glEnable(GL_TEXTURE_2D);
  if (texturesMap[currentContext].find(filename) != texturesMap[currentContext].end())
    return true;

  GLuint textureNum;
  textureImage texti;

  string extension = filename.substr(filename.find_last_of('.') + 1);
  for (int i=0; i < (int)extension.length(); ++i)
    extension[i] = (char) toupper(extension[i]);

  TextureLoader_t *loader = NULL;
  if (extension == "BMP") loader = &loadBMP;
#ifdef HAVE_LIBJPEG
  else if ((extension == "JPG") || (extension == "JPEG")) loader = &loadJPEG;
#endif
#ifdef HAVE_LIBPNG
  else if (extension == "PNG") loader = &loadPNG;
#endif
  else {
    cerr << "Warning: don't know extension \"" << extension << "\"" << endl;
  }

  if ((loader == NULL) || !(*loader)(filename, &texti)) {
    glDisable(GL_TEXTURE_2D);
    return false;
  }

  int GLFmt = texti.hasAlpha ? GL_RGBA : GL_RGB;

  GlTexture texture;
  texture.width=texti.width;
  texture.height=texti.height;

  glGenTextures(1, &textureNum);	//FIXME: handle case where no memory is available to load texture
  glBindTexture(GL_TEXTURE_2D, textureNum);

  texture.id=textureNum;

  glTexImage2D(GL_TEXTURE_2D, 0, GLFmt, texti.width, texti.height, 0, GLFmt, GL_UNSIGNED_BYTE, texti.data);
  /* use no filtering */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  delete [] texti.data;
  (texturesMap[currentContext])[filename] = texture;

  return true;
}
//====================================================================
void GlTextureManager::deleteTexture(const string& name){
  for(ContextAndTextureMap::iterator it=texturesMap.begin();it!=texturesMap.end();++it){
    TextureUnit::iterator it2=(*it).second.find(name);
    if(it2!=(*it).second.end()){
      glDeleteTextures(1,&((*it2).second.id));
      (*it).second.erase(name);
    }
  }
}
//====================================================================
void GlTextureManager::beginNewTexture(const string& name)
{
  GLuint textureNum;
  glGenTextures(1, &textureNum);
  glBindTexture(GL_TEXTURE_2D, textureNum);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
}
//====================================================================
bool GlTextureManager::activateTexture(const string& filename) {
  if (texturesMap[currentContext].find(filename) == texturesMap[currentContext].end())
    loadTexture(filename);
  else
    glEnable(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, (texturesMap[currentContext])[filename].id);
  return true;
}

//====================================================================
void GlTextureManager::desactivateTexture() {
  glDisable(GL_TEXTURE_2D);
}

}
