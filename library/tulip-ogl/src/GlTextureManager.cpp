/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#include <GL/glew.h>

#include <QImage>
#include <QGLWidget>

#include <tulip/GlTextureManager.h>

//====================================================
tlp::GlTextureManager* tlp::GlTextureManager::inst=0;

using namespace std;

namespace tlp {
GlTextureManager::GlTextureManager():animationFrame(0) {
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
bool GlTextureManager::existsTexture(const string& filename) {
  return (texturesMap[currentContext].find(filename) != texturesMap[currentContext].end());
}
//====================================================================
void loadTextureFromQImage(QImage &image,GlTexture &glTexture) {

  unsigned int width=image.width();
  unsigned int height=image.height();

  bool isSprite=false;

  if(width!=height) {
    bool widthPowerOfTwo=false;
    bool heightPowerOfTwo=false;

    for(unsigned int i=1; i<=width; i*=2) {
      if(i==width)
        widthPowerOfTwo=true;
    }

    for(unsigned int i=1; i<=height; i*=2) {
      if(i==height)
        heightPowerOfTwo=true;
    }

    if(widthPowerOfTwo && heightPowerOfTwo) {
      isSprite=true;
    }
  }

  int spriteNumber=1;

  if(isSprite) {
    if(width>height) {
      spriteNumber=width/height;
    }
    else {
      spriteNumber=height/width;
    }
  }

  GLuint* textureNum = new GLuint[spriteNumber];

  image = QGLWidget::convertToGLFormat(image);

  glTexture.width=width;
  glTexture.height=height;
  glTexture.spriteNumber=spriteNumber;
  glTexture.id=new GLuint[spriteNumber];

  glGenTextures(spriteNumber, textureNum);  //FIXME: handle case where no memory is available to load texture

  if(!isSprite) {
    glBindTexture(GL_TEXTURE_2D, textureNum[0]);

    glTexture.id[0]=textureNum[0];

    int GLFmt = image.hasAlphaChannel() ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, GLFmt, width, height, 0, GLFmt, GL_UNSIGNED_BYTE, image.bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  }
  else {
    QImage *images=new QImage[spriteNumber];

    if(width>height) {
      QRect rect(0,0,height,height);

      for(int i=0; i<spriteNumber; i++) {
        images[i]=image.copy(rect);
        rect.translate(height,0);
      }
    }
    else {
      QRect rect(0,0,width,width);

      for(int i=0; i<spriteNumber; i++) {
        images[i]=image.copy(rect);
        rect.translate(0,width);
      }
    }

    width=images[0].width();
    height=images[0].height();

    for(int i=0; i<spriteNumber; i++) {
      glBindTexture(GL_TEXTURE_2D, textureNum[i]);

      glTexture.id[i]=textureNum[i];

      int GLFmt = images[i].hasAlphaChannel() ? GL_RGBA : GL_RGB;
      glTexImage2D(GL_TEXTURE_2D, 0, GLFmt, width, height, 0, GLFmt, GL_UNSIGNED_BYTE, images[i].bits());

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    delete[] images;
  }
}
//====================================================================
bool GlTextureManager::loadTexture(const string& filename) {
  glEnable(GL_TEXTURE_2D);

  if (texturesMap[currentContext].find(filename) != texturesMap[currentContext].end())
    return true;

  QImage image(QString::fromUtf8(filename.c_str()));

  if (image.isNull()) {
    tlp::error() << "Error when loading texture from file " << filename.c_str() << std::endl;
    return false;
  }

  GlTexture texture;

  loadTextureFromQImage(image,texture);

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
