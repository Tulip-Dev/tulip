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
#include "tulip/QGlBufferManager.h"

#include <QGLPixelBuffer>
#include <QGLFramebufferObject>

#include <tulip/GlMainWidget.h>

#include <iostream>

//====================================================
tlp::QGlBufferManager* tlp::QGlBufferManager::inst=0;

using namespace std;

namespace tlp {

QGlBufferManager::QGlBufferManager() {
  QGLFramebufferObject *glFramebufferObject=new QGLFramebufferObject(2,2);
  framebufferObjectWork=glFramebufferObject->isValid();
  delete glFramebufferObject;

  QGLFormat format=QGLFormat::defaultFormat();
  QGLPixelBuffer *glPixelBuffer=new QGLPixelBuffer(2,2,format,GlMainWidget::getFirstQGLWidget());
  pixelBufferWork=glPixelBuffer->isValid();
  delete glPixelBuffer;
}

void QGlBufferManager::clearBuffers() {
  if(!inst)
    return;

  for(std::map<std::pair<int,int>,QGLPixelBuffer*>::iterator it=inst->widthHeightToBuffer.begin(); it!=inst->widthHeightToBuffer.end(); ++it)
    delete (*it).second;

  for(std::map<std::pair<int,int>,QGLFramebufferObject*>::iterator it=inst->widthHeightToFramebuffer.begin(); it!=inst->widthHeightToFramebuffer.end(); ++it)
    delete (*it).second;

  inst->widthHeightToBuffer.clear();
  inst->bufferToWidthHeight.clear();
  inst->widthHeightToFramebuffer.clear();
  inst->framebufferToWidthHeight.clear();
}

QGLPixelBuffer *QGlBufferManager::getPixelBuffer(int width, int height) {
  map<pair<int,int>,QGLPixelBuffer *>::iterator it=widthHeightToBuffer.find(pair<int,int>(width,height));

  if(it!=widthHeightToBuffer.end())
    return (*it).second;

  QGLFormat format=QGLFormat::defaultFormat();
  format.setAlpha(true);
  format.setSampleBuffers(true);
  format.setSamples(8);
  QGLPixelBuffer *glPixelBuffer=new QGLPixelBuffer(width,height,format,GlMainWidget::getFirstQGLWidget());

  if(!glPixelBuffer->isValid()) {
    // We can't create a buffer with requested size so :
    //   - We search the bigest buffer and remove it
    //   - We try to allocate smaller buffer

    // Code to search the bigest buffer and delete it
    while(!glPixelBuffer->isValid() && !bufferToWidthHeight.empty()) {
      int widthToRemove=0;
      int heightToRemove=0;
      QGLPixelBuffer *bufferToRemove = NULL;

      for(it=widthHeightToBuffer.begin(); it!=widthHeightToBuffer.end(); ++it) {
        if((((*it).first.first)*((*it).first.second))>widthToRemove*heightToRemove) {
          widthToRemove=(*it).first.first;
          heightToRemove=(*it).first.second;
          bufferToRemove=(*it).second;
        }
      }

      delete bufferToRemove;
      widthHeightToBuffer.erase(pair<int,int>(widthToRemove,heightToRemove));
      bufferToWidthHeight.erase(bufferToRemove);

      delete glPixelBuffer;
      glPixelBuffer=new QGLPixelBuffer(width,height,format,GlMainWidget::getFirstQGLWidget());
    }

    // Code to allocate smaller buffer
    while(!glPixelBuffer->isValid() && width>0 && height>0) {
      width=width/2;
      height=height/2;

      delete glPixelBuffer;
      glPixelBuffer=new QGLPixelBuffer(width,height,format,GlMainWidget::getFirstQGLWidget());
    }
  }

  widthHeightToBuffer[pair<int,int>(width,height)]=glPixelBuffer;
  bufferToWidthHeight[glPixelBuffer]=pair<int,int>(width,height);

  return glPixelBuffer;
}

QGLFramebufferObject *QGlBufferManager::getFramebufferObject(int width, int height) {
  map<pair<int,int>,QGLFramebufferObject *>::iterator it=widthHeightToFramebuffer.find(pair<int,int>(width,height));

  if(it!=widthHeightToFramebuffer.end())
    return (*it).second;

  QGLFramebufferObject *glFramebufferObject=new QGLFramebufferObject(width,height/*,QGLFramebufferObject::NoAttachment, GL_TEXTURE_2D, GL_RGBA32F_ARB*/);

  if(!glFramebufferObject->isValid()) {
    while(!glFramebufferObject->isValid() && !framebufferToWidthHeight.empty()) {
      int widthToRemove=0;
      int heightToRemove=0;
      QGLFramebufferObject *bufferToRemove = NULL;

      for(it=widthHeightToFramebuffer.begin(); it!=widthHeightToFramebuffer.end(); ++it) {
        if((((*it).first.first)*((*it).first.second))>widthToRemove*heightToRemove) {
          widthToRemove=(*it).first.first;
          heightToRemove=(*it).first.second;
          bufferToRemove=(*it).second;
        }
      }

      delete bufferToRemove;
      widthHeightToFramebuffer.erase(pair<int,int>(widthToRemove,heightToRemove));
      framebufferToWidthHeight.erase(bufferToRemove);

      delete glFramebufferObject;
      glFramebufferObject=new QGLFramebufferObject(width,height/*,QGLFramebufferObject::NoAttachment, GL_TEXTURE_2D, GL_RGBA32F_ARB*/);
    }

    while(!glFramebufferObject->isValid() && width>0 && height>0) {
      width=width/2;
      height=height/2;

      delete glFramebufferObject;
      glFramebufferObject=new QGLFramebufferObject(width,height/*,QGLFramebufferObject::NoAttachment, GL_TEXTURE_2D, GL_RGBA32F_ARB*/);
    }
  }

  widthHeightToFramebuffer[pair<int,int>(width,height)]=glFramebufferObject;
  framebufferToWidthHeight[glFramebufferObject]=pair<int,int>(width,height);

  return glFramebufferObject;
}

}
