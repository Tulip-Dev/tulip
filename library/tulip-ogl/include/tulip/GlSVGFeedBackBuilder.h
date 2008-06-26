//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 25/10/2007
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_GLSVGFEEDBACKBUILDER_H
#define Tulip_GLSVGFEEDBACKBUILDER_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <sstream>

#include "tulip/GlTLPFeedBackBuilder.h"

namespace tlp {
  
  class TLP_GL_SCOPE GlSVGFeedBackBuilder : public GlTLPFeedBackBuilder {
  
  public:
  
    GlSVGFeedBackBuilder() 
      :stream_out(std::stringstream::in | std::stringstream::out) {}

    virtual void begin(const Vector<int, 4> &viewport,GLfloat *clearColor,GLfloat pointSize,GLfloat lineWidth);
    virtual void colorInfo(GLfloat *data);
    virtual void beginGlEntity(GLfloat data);
    virtual void endGlEntity();
    virtual void beginGlGraph(GLfloat data);
    virtual void endGlGraph();
    virtual void beginNode(GLfloat data);
    virtual void endNode();
    virtual void beginEdge(GLfloat data);
    virtual void endEdge();
    virtual void pointToken(GLfloat *data);
    virtual void lineToken(GLfloat *data);
    virtual void lineResetToken(GLfloat *data);
    virtual void polygonToken(GLfloat *data);
    virtual void bitmapToken(GLfloat *data);
    virtual void drawPixelToken(GLfloat *data);
    virtual void copyPixelToken(GLfloat *data);
    virtual void end();
    
    virtual void getResult(std::string* str);
    
  private:

    std::stringstream stream_out;

    GLfloat clearColor[4];
    GLfloat pointSize;
    GLfloat lineWidth;

    Color fillColor;
    Color strokeColor;
    Color textColor;

    int width;
    int height;

  };
  
}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLFEEDBACKBUILDER_H
