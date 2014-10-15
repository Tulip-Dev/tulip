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
#if defined(_MSC_VER)
// need to include that header with Visual Studio to fix a typedef conflict
// with latest version of jpeg lib (9a)
#include <basetsd.h>
#endif
#include <jpeglib.h>
#include <png.h>
#include <GL/glew.h>

#include <tulip/GlTextureManager.h>
#include <tulip/OpenGlConfigManager.h>

//====================================================
tlp::GlTextureManager* tlp::GlTextureManager::inst=NULL;
tlp::GlTextureLoader* tlp::GlTextureManager::loader=NULL;

using namespace std;

namespace tlp {

struct TextureInfo {
  bool hasAlpha;
  unsigned int  width;
  unsigned int  height;
  unsigned char *data;
};

typedef bool (TextureLoader_t)(const string &, TextureInfo *);

//====================================================================
/* simple loader for 24bit bitmaps (data is in rgb-format) */
static bool loadBMP(const string &filename, TextureInfo *texture) {
  FILE *file;
  unsigned short int bfType;
  long int bfOffBits;
  short int biPlanes;
  short int biBitCount;
  long int biSizeImage;
  int i;
  /* make sure the file is there and open it read-only (binary) */
#ifndef _MSC_VER
  file = fopen(filename.c_str(), "rb");
#else
  fopen_s(&file, filename.c_str(), "rb");
#endif

  if (file == NULL) {
    tlp::error() << "File not found:" << filename << std::endl;
    return false;
  }

  if(!fread(&bfType, sizeof(short int), 1, file)) {
    tlp::error() << "Error reading file: " << filename << endl;
    fclose(file);
    return false;
  }

  /* check if file is a bitmap */
  if (bfType != 19778) {
    tlp::error() << "Not a Bitmap-File: " << filename << std::endl;
    fclose(file);
    return false;
  }

  /* get the file size */
  /* skip file size and reserved fields of bitmap file header */
  fseek(file, 8, SEEK_CUR);

  /* get the position of the actual bitmap data */
  if (!fread(&bfOffBits, sizeof(int), 1, file)) {
    tlp::error() << "Error reading file: " << filename << std::endl;
    fclose(file);
    return false;
  }

  /* skip size of bitmap info header */
  fseek(file, 4, SEEK_CUR);

  if (!fread(&texture->width, sizeof(int), 1, file)) {
    tlp::error() << "Error reading file: " << filename << std::endl;
    fclose(file);
    return false;
  }

  if (!fread(&texture->height, sizeof(int), 1, file)) {
    tlp::error() << "Error reading file: " << filename << std::endl;
    fclose(file);
    return false;
  }

  /* get the number of planes (must be set to 1) */
  if (!fread(&biPlanes, sizeof(short int), 1, file)) {
    tlp::error() << "Error reading file: " << filename << std::endl;
    fclose(file);
    return false;
  }

  if (biPlanes != 1) {
    tlp::error() << "Error: number of Planes not 1 in: " << filename << std::endl;
    fclose(file);
    return false;
  }

  /* get the number of bits per pixel */
  if (!fread(&biBitCount, sizeof(short int), 1, file)) {
    tlp::error() << "Error reading file: " << filename << std::endl;
    fclose(file);
    return false;
  }

  if (biBitCount != 24) {
    tlp::error() << "Error: Bits per Pixel not 24: " << filename << std::endl;
    fclose(file);
    return false;
  }

  /* calculate the size of the image in bytes */
  biSizeImage = texture->width * texture->height * 3;
  texture->data = new unsigned char[biSizeImage];
  /* seek to the actual data */
  fseek(file, bfOffBits, SEEK_SET);

  if (!fread(texture->data, biSizeImage, 1, file)) {
    tlp::error() << "Error reading file: " << filename << std::endl;
    delete [] texture->data;
    texture->data=NULL;
    fclose(file);
    return false;
  }

  /* swap red and blue (bgr -> rgb) */
  for (i = 0; i < biSizeImage; i += 3) {
    unsigned char temp = texture->data[i];
    texture->data[i] = texture->data[i + 2];
    texture->data[i + 2] = temp;
  }

  fclose(file);
  texture->hasAlpha = false;
  return true;
}
//====================================================================
static bool loadJPEG(const string &filename, TextureInfo *texture) {
  FILE *file;
#ifndef _MSC_VER
  file = fopen(filename.c_str(), "rb");
#else
  fopen_s(&file, filename.c_str(), "rb");
#endif

  if (file == NULL) {
    tlp::error() << "File not found:" << filename << std::endl;
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
//====================================================================
static bool loadPNG(const string &filename, TextureInfo *texture) {
  FILE *file;
#ifndef _MSC_VER
  file = fopen(filename.c_str(), "rb");
#else
  fopen_s(&file, filename.c_str(), "rb");
#endif

  if (file == NULL) {
    tlp::error() << "File not found:" << filename << std::endl;
    return false;
  }

  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr) {
    tlp::error() << "Error reading file: " << filename << std::endl;
    fclose(file);
    return false;
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);

  if (!info_ptr) {
    tlp::error() << "Error reading file: " << filename << std::endl;
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    fclose(file);
    return false;
  }

  png_infop end_info = png_create_info_struct(png_ptr);

  if (!end_info) {
    tlp::error() << "Error reading file: " << filename << std::endl;
    png_destroy_read_struct(&png_ptr, &info_ptr,  (png_infopp)NULL);
    fclose(file);
    return false;
  }

#ifdef PNG_SETJMP_SUPPORTED

  if (setjmp(png_jmpbuf(png_ptr)))
#endif
  {
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    fclose(file);
    return file != NULL;
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
  png_bytep* row_pointers = new png_bytep[texture->height];

  for (unsigned int i=0; i < texture->height; ++i)
    row_pointers[i] = (png_bytep) &(texture->data[linestride*(texture->height-1-i)]);

  png_set_strip_16(png_ptr);  //force 8 bits/channel
  png_set_gray_to_rgb(png_ptr); //force RGB
  png_read_image(png_ptr, row_pointers);
  png_read_end(png_ptr, end_info);
  png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
  fclose(file);
  return true;
}

static bool generateTexture(const std::string &filename,
                            const TextureInfo &texti,
                            GlTexture &glTexture) {
  int GLFmt = texti.hasAlpha ? GL_RGBA : GL_RGB;

  bool spriteOnWidth=false;
  unsigned int spriteNumber=1;
  unsigned int width=texti.width;
  unsigned int height=texti.height;

  if((texti.height-(texti.height/texti.width)*texti.width)!=0 && (texti.width-(texti.width/texti.height)*texti.height)!=0) {
    tlp::error() << "Texture loader error: invalid size\ntexture size should be of the form:\n - width=height or\n - height=N*width (for animated textures)\nfor file: " << filename << std::endl;
    return false;
  }
  else {
    if(texti.width!=texti.height) {
      if(texti.height>texti.width) {
        //spriteOnHeight=true;
        spriteNumber=texti.height/texti.width;
        height=width;
      }
      else {
        spriteOnWidth=true;
        spriteNumber=texti.width/texti.height;
        width=height;
      }
    }

    bool canUseNpotTextures = OpenGlConfigManager::getInst().isExtensionSupported("GL_ARB_texture_non_power_of_two");

    if (!canUseNpotTextures) {
      bool formatOk=false;

      for(unsigned int i=1; i<=width; i*=2) {
        if(i==width)
          formatOk=true;
      }

      if(!formatOk) {
        tlp::error() << "Texture loader error: invalid size\ntexture width should be a power of 2\nfor file: " << filename << std::endl;
        return false;
      }

      formatOk=false;

      for(unsigned int i=1; i<=height; i*=2) {
        if(i==height)
          formatOk=true;
      }

      if(!formatOk) {
        tlp::error() << "Texture loader error: invalid size\ntexture height should be a power of 2\nfor file: " << filename << std::endl;
        return false;
      }
    }
  }

  bool canUseMipmaps = OpenGlConfigManager::getInst().isExtensionSupported("GL_ARB_framebuffer_object") ||
                       OpenGlConfigManager::getInst().isExtensionSupported("GL_EXT_framebuffer_object");

  GLuint* textureNum = new GLuint[spriteNumber];

  glTexture.width=width;
  glTexture.height=height;
  glTexture.spriteNumber=spriteNumber;
  glTexture.id=new GLuint[spriteNumber];

  unsigned char **dataForWidthSpriteTexture;
  dataForWidthSpriteTexture=new unsigned char*[spriteNumber];

  if(spriteOnWidth) {
    for(unsigned int i=0; i<spriteNumber; i++) {
      if(texti.hasAlpha)
        dataForWidthSpriteTexture[i]=new unsigned char[4*width*height];
      else
        dataForWidthSpriteTexture[i]=new unsigned char[3*width*height];
    }

    for(unsigned int i=0; i<texti.height; i++) {
      for(unsigned int j=0; j<texti.width; ++j) {
        if(texti.hasAlpha) {
          dataForWidthSpriteTexture[j/width][i*width*4+(j-(j/width)*width)*4]=texti.data[i*texti.width*4+j*4];
          dataForWidthSpriteTexture[j/width][i*width*4+(j-(j/width)*width)*4+1]=texti.data[i*texti.width*4+j*4+1];
          dataForWidthSpriteTexture[j/width][i*width*4+(j-(j/width)*width)*4+2]=texti.data[i*texti.width*4+j*4+2];
          dataForWidthSpriteTexture[j/width][i*width*4+(j-(j/width)*width)*4+3]=texti.data[i*texti.width*4+j*4+3];
        }
        else {
          dataForWidthSpriteTexture[j/width][i*width*3+(j-(j/width)*width)*3]=texti.data[i*texti.width*3+j*3];
          dataForWidthSpriteTexture[j/width][i*width*3+(j-(j/width)*width)*3+1]=texti.data[i*texti.width*3+j*3+1];
          dataForWidthSpriteTexture[j/width][i*width*3+(j-(j/width)*width)*3+2]=texti.data[i*texti.width*3+j*3+2];
        }
      }
    }
  }

  glGenTextures(spriteNumber, textureNum);  //FIXME: handle case where no memory is available to load texture

  glEnable(GL_TEXTURE_2D);

  for(unsigned int i=0; i<spriteNumber; ++i) {
    glBindTexture(GL_TEXTURE_2D, textureNum[i]);

    glTexture.id[i]=textureNum[i];

    if(!spriteOnWidth) {
      if(texti.hasAlpha)
        glTexImage2D(GL_TEXTURE_2D, 0, GLFmt, width, height, 0, GLFmt, GL_UNSIGNED_BYTE, texti.data + (width*height*4*i));
      else
        glTexImage2D(GL_TEXTURE_2D, 0, GLFmt, width, height, 0, GLFmt, GL_UNSIGNED_BYTE, texti.data + (width*height*3*i));
    }
    else {
      glTexImage2D(GL_TEXTURE_2D, 0, GLFmt, width, height, 0, GLFmt, GL_UNSIGNED_BYTE, dataForWidthSpriteTexture[i]);

      delete [] dataForWidthSpriteTexture[i];
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (canUseMipmaps) {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

  }

  glDisable(GL_TEXTURE_2D);

  delete [] textureNum;
  delete [] dataForWidthSpriteTexture;

  return true;
}
//====================================================================
bool GlTextureLoader::loadTexture(const string& filename,
                                  GlTexture &texture) {
  string extension = filename.substr(filename.find_last_of('.') + 1);

  for (int i=0; i < (int)extension.length(); ++i)
    extension[i] = (char) toupper(extension[i]);

  TextureLoader_t *loader = NULL;

  if (extension == "BMP") loader = &loadBMP;
  else if ((extension == "JPG") || (extension == "JPEG")) loader = &loadJPEG;
  else if (extension == "PNG") loader = &loadPNG;
  else {
    tlp::error() << "GlTextureLoader Error: no texture loader found for file extension \"" << extension << "\"" << std::endl;
  }

  TextureInfo texti;

  if ((loader == NULL) || !(*loader)(filename, &texti)) {
    return false;
  }

  bool result = generateTexture(filename, texti, texture);

  delete [] texti.data;

  return result;
}
//====================================================================
GlTextureManager::GlTextureManager():animationFrame(0) {
}
//====================================================================
void GlTextureManager::changeContext(uintptr_t context) {
  currentContext=context;
}
//====================================================================
void GlTextureManager::removeContext(uintptr_t context) {
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
bool GlTextureManager::existsTexture(const string& filename) {
  return (texturesMap[currentContext].find(filename) != texturesMap[currentContext].end());
}
//====================================================================
bool GlTextureManager::loadTexture(const string& filename) {
  glEnable(GL_TEXTURE_2D);

  if (texturesMap[currentContext].find(filename) != texturesMap[currentContext].end())
    return true;

  GlTexture texture;

  if (!getTextureLoader()->loadTexture(filename, texture))
    return false;

  (texturesMap[currentContext])[filename] = texture;
  return true;
}

void GlTextureManager::registerExternalTexture(const std::string &textureName, const GLuint textureId) {
  GlTexture texture;
  texture.spriteNumber=1;
  texture.id=new GLuint[1];
  texture.id[0] = textureId;
  (texturesMap[currentContext])[textureName] = texture;
}

//====================================================================
void GlTextureManager::deleteTexture(const string& name) {
  for(ContextAndTextureMap::iterator it=texturesMap.begin(); it!=texturesMap.end(); ++it) {
    TextureUnit::iterator it2=it->second.find(name);

    if(it2!=it->second.end()) {
      for(unsigned int i=0; i<it2->second.spriteNumber; ++i)
        glDeleteTextures(1,&(it2->second.id[i]));

      delete [] it2->second.id;
      it->second.erase(name);
    }
  }
}
//====================================================================
void GlTextureManager::beginNewTexture(const string&) {
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
  return activateTexture(filename,animationFrame);
}
//====================================================================
bool GlTextureManager::activateTexture(const string& filename,unsigned int frame) {
  if(texturesWithError.count(filename)!=0)
    return false;

  bool loadOk=true;

  if (texturesMap[currentContext].find(filename) == texturesMap[currentContext].end())
    loadOk=loadTexture(filename);
  else
    glEnable(GL_TEXTURE_2D);

  if(!loadOk) {
    texturesWithError.insert(filename);
    glDisable(GL_TEXTURE_2D);
    return false;
  }

  unsigned int spriteNumber=((texturesMap[currentContext])[filename]).spriteNumber;
  frame=frame-(frame/spriteNumber)*spriteNumber;
  glBindTexture(GL_TEXTURE_2D, (texturesMap[currentContext])[filename].id[frame]);
  return true;
}
//====================================================================
void GlTextureManager::desactivateTexture() {
  glDisable(GL_TEXTURE_2D);
}


}
