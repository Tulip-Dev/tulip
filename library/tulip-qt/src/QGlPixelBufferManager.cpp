#include "tulip/QGlPixelBufferManager.h"

#include <QtOpenGL/QGLPixelBuffer>

#include "tulip/GlMainWidget.h"

#include <iostream>

//====================================================
#ifdef _WIN32
#ifdef DLL_EXPORT
tlp::QGlPixelBufferManager* tlp::QGlPixelBufferManager::inst=0;
#endif
#else
tlp::QGlPixelBufferManager* tlp::QGlPixelBufferManager::inst=0;
#endif

using namespace std;

namespace tlp
{

  QGLPixelBuffer *QGlPixelBufferManager::getPixelBuffer(int width, int height){
    map<pair<int,int>,QGLPixelBuffer *>::iterator it=widthHeightToBuffer.find(pair<int,int>(width,height));

    if(it!=widthHeightToBuffer.end())
      return (*it).second;

    QGLFormat format=QGLFormat::defaultFormat();
    format.setAlpha(true);
    QGLPixelBuffer *glPixelBuffer=new QGLPixelBuffer(width,height,format,GlMainWidget::getFirstQGLWidget());
    if(!glPixelBuffer->isValid()){
      while(!glPixelBuffer->isValid() && bufferToWidthHeight.size()>0) {
	int widthToRemove=0;
	int heightToRemove=0;
	QGLPixelBuffer *bufferToRemove;

	for(it=widthHeightToBuffer.begin();it!=widthHeightToBuffer.end();++it){
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
	glPixelBuffer=new QGLPixelBuffer(width,height,QGLFormat::defaultFormat(),GlMainWidget::getFirstQGLWidget());
      }

      while(!glPixelBuffer->isValid() && width>0 && height>0) {
	width=width/2;
	height=height/2;

	delete glPixelBuffer;
	glPixelBuffer=new QGLPixelBuffer(width,height,QGLFormat::defaultFormat(),GlMainWidget::getFirstQGLWidget());
      }
    }

    widthHeightToBuffer[pair<int,int>(width,height)]=glPixelBuffer;
    bufferToWidthHeight[glPixelBuffer]=pair<int,int>(width,height);

    return glPixelBuffer;
  }

}
