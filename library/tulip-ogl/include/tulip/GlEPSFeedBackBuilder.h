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
#ifndef Tulip_GLEPSFEEDBACKBUILDER_H
#define Tulip_GLEPSFEEDBACKBUILDER_H

#include <tulip/tulipconf.h>

#include <sstream>

#include "tulip/GlTLPFeedBackBuilder.h"

namespace tlp {
  
  class TLP_GL_SCOPE GlEPSFeedBackBuilder : public GlTLPFeedBackBuilder {
    
  public:
  
    GlEPSFeedBackBuilder() 
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

  };
 
}

#endif // Tulip_GLEPSFEEDBACKBUILDER_H
