//-*-c++-*-
/**
 Author: Morgan Mathiaut
 Email : mathiaut@labri.fr
 Last modification : 27/03/2008
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef Tulip_GLHTMLFEEDBACKBUILDER_H
#define Tulip_GLHTMLFEEDBACKBUILDER_H

#include <sstream>
#include <queue>

#include <tulip/GlTLPFeedBackBuilder.h>

#include <tulip/StringProperty.h>

#include "Shape.h"

namespace tlprender {
  
  class GlHTMLFeedBackBuilder : public tlp::GlTLPFeedBackBuilder {
  
  public:
  
    GlHTMLFeedBackBuilder(bool outputBody,const std::string &filename,tlp::StringProperty *hrefp,tlp::StringProperty *altp);

    virtual void begin(const tlp::Vector<int, 4> &viewport,GLfloat *clearColor,GLfloat pointSize,GLfloat lineWidth);
    virtual void beginNode(GLfloat data);
    virtual void endNode();
    virtual void polygonToken(GLfloat *data);
    virtual void end();
    
    virtual void getResult(std::string* str);
    
  private:

    std::stringstream stream_out;
    std::priority_queue<Shape *> shapeQueue;

    bool outputBody;
    std::string filename;
    tlp::StringProperty *hrefp;
    tlp::StringProperty *altp;
    Shape *poly;
    int nodeId;

    int height;

  };
  
}

#endif // Tulip_GLHTMLFEEDBACKBUILDER_H
