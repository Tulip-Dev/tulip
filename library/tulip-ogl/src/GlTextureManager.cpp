#include "tulip/OpenGlConfigManager.h"

#include "tulip/GlTextureManager.h"

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

typedef bool (TextureLoader_t)(const string &, TextureInfo *,string &);

//====================================================================
/* simple loader for 24bit bitmaps (data is in rgb-format) */
static bool loadBMP(const string &filename, TextureInfo *texture,string &errorMsg) {

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
      errorMsg = "File not found : " + filename;
      //cerr << __PRETTY_FUNCTION__ << ": File not found :" << filename << endl;
      return false;
    }
  if(!fread(&bfType, sizeof(short int), 1, file))
    {
      errorMsg = "Error reading : " + filename;
      //cerr << __PRETTY_FUNCTION__ << ": Error reading " << filename << endl;
      fclose(file);
      return false;
    }
  /* check if file is a bitmap */
  if (bfType != 19778)
    {
      errorMsg = "Not a Bitmap-File : " + filename;
      //cerr << __PRETTY_FUNCTION__ << ": Not a Bitmap-File: " << filename << endl;
      fclose(file);
      return false;
    }
  /* get the file size */
  /* skip file size and reserved fields of bitmap file header */
  fseek(file, 8, SEEK_CUR);
  /* get the position of the actual bitmap data */
  if (!fread(&bfOffBits, sizeof(long int), 1, file))
    {
      errorMsg = "Error reading : " + filename;
      //cerr << __PRETTY_FUNCTION__ << ": Error reading " << filename << endl;
      fclose(file);
      return false;
    }
  /* skip size of bitmap info header */
  fseek(file, 4, SEEK_CUR);
  if (!fread(&texture->width, sizeof(int), 1, file))
    {
      errorMsg = "Error reading : " + filename;
      //cerr << __PRETTY_FUNCTION__ << ": Error reading " << filename << endl;
      fclose(file);
      return false;
    }
   if (!fread(&texture->height, sizeof(int), 1, file))
    {
      errorMsg = "Error reading : " + filename;
      //cerr << __PRETTY_FUNCTION__ << ": Error reading " << filename << endl;
      fclose(file);
      return false;
    }

  /* get the number of planes (must be set to 1) */
  if (!fread(&biPlanes, sizeof(short int), 1, file))
    {
      errorMsg = "Error reading : " + filename;
      //cerr << __PRETTY_FUNCTION__ << ": Error reading " << filename << endl;
      fclose(file);
      return false;
    }
  if (biPlanes != 1)
    {
      errorMsg = "Error: number of Planes not 1 in : " + filename;
      //cerr << __PRETTY_FUNCTION__ << ": Error: number of Planes not 1: " << filename << endl;
      fclose(file);
      return false;
    }
  /* get the number of bits per pixel */
  if (!fread(&biBitCount, sizeof(short int), 1, file))
    {
      errorMsg = "Error reading file : " + filename;
      //cerr << __PRETTY_FUNCTION__ << ": Error reading file: " << filename << endl;
      fclose(file);
      return false;
    }
  if (biBitCount != 24)
    {
      errorMsg = "Error : Bits per Pixel not 24 : " + filename;
      //cerr << __PRETTY_FUNCTION__ << ": Bits per Pixel not 24: " << filename << endl;
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
      errorMsg = "Error loading file : " + filename;
      //cerr << __PRETTY_FUNCTION__ << ": Error loading file: " << filename << endl;
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
static bool loadJPEG(const string &filename, TextureInfo *texture,string &errorMsg) {
#ifndef NDEBUG
  cerr << __PRETTY_FUNCTION__ << ": filename=" << filename << endl;
#endif
  FILE *file;
  if ((file = fopen(filename.c_str(), "rb")) == NULL) {
    errorMsg = "File not found : " + filename;
    //cerr << __PRETTY_FUNCTION__ << ": File not found:" << filename << endl;
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
static bool loadPNG(const string &filename, TextureInfo *texture,string &errorMsg)
{
#ifndef NDEBUG
  cerr << __PRETTY_FUNCTION__ << ": filename=" << filename << endl;
#endif

  FILE *file;

  if ((file = fopen(filename.c_str(), "rb")) == NULL) {
    errorMsg = "File not found : " + filename;
    //cerr << __PRETTY_FUNCTION__ << ": File not found:" << filename << endl;
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
GlTextureManager::GlTextureManager():errorViewer(new OpenGlErrorViewer),animationFrame(0) {
}
//====================================================================
bool GlTextureManager::loadTextureFromRawData(const string &textureName, int width, int height, bool hasAlpha, unsigned char *data)
{
  glEnable(GL_TEXTURE_2D);
  if (texturesMap[currentContext].find(textureName) != texturesMap[currentContext].end())
      return true;

  TextureInfo texti;
  texti.data=data;
  texti.hasAlpha=hasAlpha;
  texti.width=width;
  texti.height=height;

  GlTexture texture;
  if(!loadTexture(textureName,texti,texture))
    return false;

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
    errorViewer->displayError(filename,"Warning : don't know extension "+extension+" for file : "+filename);
    //cerr << "Warning: don't know extension \"" << extension << "\"" << endl;
  }

  TextureInfo texti;
  string errorMsg;
  if ((loader == NULL) || !(*loader)(filename, &texti,errorMsg)) {
    if(errorMsg!="")
      errorViewer->displayError(filename,errorMsg);
    glDisable(GL_TEXTURE_2D);
    return false;
  }

  GlTexture texture;
  if(!loadTexture(filename,texti,texture)){
    delete [] texti.data;
    return false;
  }

  delete [] texti.data;

  (texturesMap[currentContext])[filename] = texture;

  return true;
}

bool GlTextureManager::loadTexture(const std::string &filename,const TextureInfo &texti,GlTexture &glTexture){
  int GLFmt = texti.hasAlpha ? GL_RGBA : GL_RGB;

  bool spriteOnHeight=false;
  bool spriteOnWidth=false;
  unsigned int spriteNumber=1;
  int width=texti.width;
  int height=texti.height;

  if((texti.height-(texti.height/texti.width)*texti.width)!=0 && (texti.width-(texti.width/texti.height)*texti.height)!=0){
    errorViewer->displayError(filename,"Texture size is not valid\nTexture size should be of the form :\n - width=height or\n - height=N*width (for animated textures)\nfor file :"+filename);
    return false;
  }else{
    if(texti.width!=texti.height){
      if(texti.height>texti.width){
        spriteOnHeight=true;
        spriteNumber=texti.height/texti.width;
        height=width;
      }else{
        spriteOnWidth=true;
        spriteNumber=texti.width/texti.height;
        width=height;
      }
    }

    bool formatOk=false;
    for(unsigned int i=1;i<=width;i*=2){
      if(i==width)
        formatOk=true;
    }
    if(!formatOk){
      errorViewer->displayError(filename,"Texture size is not valid\nTexture width should be a power of two\nfor file :"+filename);
      return false;
    }

    formatOk=false;
    for(unsigned int i=1;i<=height;i*=2){
      if(i==height)
        formatOk=true;
    }
    if(!formatOk){
      errorViewer->displayError(filename,"Texture size is not valid\nTexture height should be a power of two\nfor file :"+filename);
      return false;
    }
  }

  GLuint textureNum[spriteNumber];

  glTexture.width=width;
  glTexture.height=height;
  glTexture.spriteNumber=spriteNumber;
  glTexture.id=new GLuint[spriteNumber];

  unsigned char **dataForWidthSpriteTexture;
  dataForWidthSpriteTexture=new unsigned char*[spriteNumber];
  if(spriteOnWidth){
    for(unsigned int i=0;i<spriteNumber;i++){
      if(texti.hasAlpha)
        dataForWidthSpriteTexture[i]=new unsigned char[4*width*height];
      else
        dataForWidthSpriteTexture[i]=new unsigned char[3*width*height];
    }

    for(unsigned int i=0;i<texti.height;i++){
      for(unsigned int j=0;j<texti.width;++j){
        if(texti.hasAlpha){
          dataForWidthSpriteTexture[j/width][i*width*4+(j-(j/width)*width)*4]=texti.data[i*texti.width*4+j*4];
          dataForWidthSpriteTexture[j/width][i*width*4+(j-(j/width)*width)*4+1]=texti.data[i*texti.width*4+j*4+1];
          dataForWidthSpriteTexture[j/width][i*width*4+(j-(j/width)*width)*4+2]=texti.data[i*texti.width*4+j*4+2];
          dataForWidthSpriteTexture[j/width][i*width*4+(j-(j/width)*width)*4+3]=texti.data[i*texti.width*4+j*4+3];
        }else{
          dataForWidthSpriteTexture[j/width][i*width*3+(j-(j/width)*width)*3]=texti.data[i*texti.width*3+j*3];
          dataForWidthSpriteTexture[j/width][i*width*3+(j-(j/width)*width)*3+1]=texti.data[i*texti.width*3+j*3+1];
          dataForWidthSpriteTexture[j/width][i*width*3+(j-(j/width)*width)*3+2]=texti.data[i*texti.width*3+j*3+2];
        }
      }
    }
  }

  glGenTextures(spriteNumber, textureNum);	//FIXME: handle case where no memory is available to load texture
  for(unsigned int i=0;i<spriteNumber;++i){
    glBindTexture(GL_TEXTURE_2D, textureNum[i]);

    glTexture.id[i]=textureNum[i];

    if(!spriteOnWidth){
      if(texti.hasAlpha)
        glTexImage2D(GL_TEXTURE_2D, 0, GLFmt, width, height, 0, GLFmt, GL_UNSIGNED_BYTE, texti.data + (width*height*4*i));
      else
        glTexImage2D(GL_TEXTURE_2D, 0, GLFmt, width, height, 0, GLFmt, GL_UNSIGNED_BYTE, texti.data + (width*height*3*i));
    }else{
      if(texti.hasAlpha)
        glTexImage2D(GL_TEXTURE_2D, 0, GLFmt, width, height, 0, GLFmt, GL_UNSIGNED_BYTE, dataForWidthSpriteTexture[i]);
      else
        glTexImage2D(GL_TEXTURE_2D, 0, GLFmt, width, height, 0, GLFmt, GL_UNSIGNED_BYTE, dataForWidthSpriteTexture[i]);
    }
    /* use no filtering */
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  }

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
void GlTextureManager::deleteTexture(const string& name){
  for(ContextAndTextureMap::iterator it=texturesMap.begin();it!=texturesMap.end();++it){
    TextureUnit::iterator it2=(*it).second.find(name);
    if(it2!=(*it).second.end()){
      for(unsigned int i=0;i<(*it2).second.spriteNumber;++i)
        glDeleteTextures(1,&((*it2).second.id[i]));
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
  activateTexture(filename,animationFrame);
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

  if(!loadOk){
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
