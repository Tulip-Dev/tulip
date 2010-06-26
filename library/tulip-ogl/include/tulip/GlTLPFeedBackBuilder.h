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
#ifndef Tulip_GLTLPFEEDBACKBUILDER_H
#define Tulip_GLTLPFEEDBACKBUILDER_H

#include <vector>

#include <tulip/Color.h>

#include "tulip/GlFeedBackBuilder.h"

namespace tlp {

  enum TLP_FB_TROUGHTOKENTYPE {
    TLP_FB_COLOR_INFO,
    TLP_FB_BEGIN_ENTITY,
    TLP_FB_END_ENTITY,
    TLP_FB_BEGIN_GRAPH,
    TLP_FB_END_GRAPH,
    TLP_FB_BEGIN_NODE,
    TLP_FB_END_NODE,
    TLP_FB_BEGIN_EDGE,
    TLP_FB_END_EDGE
  };
  
  class TLP_GL_SCOPE GlTLPFeedBackBuilder : public GlFeedBackBuilder {
  
  public:
  
    GlTLPFeedBackBuilder()
      :inGlEntity(false),
      inGlGraph(false),
      inNode(false),
      inEdge(false),
      inColorInfo(false),
      needData(false){}

    virtual ~GlTLPFeedBackBuilder() {}

    virtual void begin(Vector<float,4>& viewport) {}
    virtual void colorInfo(GLfloat *data) {}
    virtual void beginGlEntity(GLfloat data) {}
    virtual void endGlEntity() {}
    virtual void beginGlGraph(GLfloat data) {}
    virtual void endGlGraph() {}
    virtual void beginNode(GLfloat data) {}
    virtual void endNode() {}
    virtual void beginEdge(GLfloat data) {}
    virtual void endEdge() {}
    virtual void passThroughToken(GLfloat *data);
    virtual void pointToken(GLfloat *data) {}
    virtual void lineToken(GLfloat *data) {}
    virtual void lineResetToken(GLfloat *data) {}
    virtual void polygonToken(GLfloat *data) {}
    virtual void bitmapToken(GLfloat *data) {}
    virtual void drawPixelToken(GLfloat *data) {}
    virtual void copyPixelToken(GLfloat *data) {}
    virtual void end() {}

  private:
    
    bool inGlEntity;
    bool inGlGraph;
    bool inNode;
    bool inEdge;
    bool inColorInfo;
    bool needData;

    std::vector<GLfloat> dataBuffer; 

  };
  
}

#endif // Tulip_GLFEEDBACKBUILDER_H
