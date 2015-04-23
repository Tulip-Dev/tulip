/*
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
///@cond DOXYGEN_HIDDEN

#ifndef Tulip_GLTLPFEEDBACKBUILDER_H
#define Tulip_GLTLPFEEDBACKBUILDER_H

#if defined(_MSC_VER)
#include <Windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <vector>

#include <tulip/GlFeedBackBuilder.h>

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
     needData(false) {}

  virtual ~GlTLPFeedBackBuilder() {}

  virtual void colorInfo(GLfloat *) {}
  virtual void beginGlEntity(GLfloat) {}
  virtual void endGlEntity() {}
  virtual void beginGlGraph(GLfloat) {}
  virtual void endGlGraph() {}
  virtual void beginNode(GLfloat) {}
  virtual void endNode() {}
  virtual void beginEdge(GLfloat) {}
  virtual void endEdge() {}
  virtual void passThroughToken(GLfloat *);
  virtual void pointToken(GLfloat *) {}
  virtual void lineToken(GLfloat *) {}
  virtual void lineResetToken(GLfloat *) {}
  virtual void polygonToken(GLfloat *) {}
  virtual void bitmapToken(GLfloat *) {}
  virtual void drawPixelToken(GLfloat *) {}
  virtual void copyPixelToken(GLfloat *) {}
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
///@endcond
