//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 23/10/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_GLFEEDBACKBUILDER_H
#define Tulip_GLFEEDBACKBUILDER_H

#include <tulip/tulipconf.h>

#include <string>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <tulip/Vector.h>

namespace tlp {

  //====================================================
  typedef struct _FeedBack3Dcolor {
    GLfloat x;
    GLfloat y;
    GLfloat z;
    GLfloat red;
    GLfloat green;
    GLfloat blue;
    GLfloat alpha;
  } Feedback3Dcolor;
  
  //====================================================
  typedef struct _DepthIndex {
    GLfloat *ptr;
    GLfloat depth;
  } DepthIndex;
  
  //====================================================  
  /**
   * Abstract class used to build a object with a OpenGl feedback buffer
   */
  class TLP_GL_SCOPE GlFeedBackBuilder {
    
  public:
    
    virtual ~GlFeedBackBuilder() {}
    
    virtual void begin(const Vector<int, 4> &viewport) {}
    virtual void passThroughToken(GLfloat *data) {}
    virtual void pointToken(GLfloat *data) {}
    virtual void lineToken(GLfloat *data) {}
    virtual void lineResetToken(GLfloat *data) {}
    virtual void polygonToken(GLfloat *data) {}
    virtual void bitmapToken(GLfloat *data) {}
    virtual void drawPixelToken(GLfloat *data) {}
    virtual void copyPixelToken(GLfloat *data) {}
    virtual void end() {}
    
    virtual void getResult(std::string* str) = 0;
    
  };
 
}

#endif // Tulip_GLFEEDBACKBUILDER_H
