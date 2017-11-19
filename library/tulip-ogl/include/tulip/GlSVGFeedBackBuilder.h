/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#ifndef Tulip_GLSVGFEEDBACKBUILDER_H
#define Tulip_GLSVGFEEDBACKBUILDER_H

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <sstream>

#include <tulip/GlTLPFeedBackBuilder.h>
#include <tulip/Color.h>

namespace tlp {

class TLP_GL_SCOPE GlSVGFeedBackBuilder : public GlTLPFeedBackBuilder {

public:
  GlSVGFeedBackBuilder()
      : stream_out(std::stringstream::in | std::stringstream::out), inGlEntity(false),
        inGlGraph(false), inNode(false), inEdge(false) {}

  void begin(const Vector<int, 4> &viewport, GLfloat *clearColor, GLfloat pointSize,
             GLfloat lineWidth) override;
  void colorInfo(GLfloat *data) override;
  void beginGlEntity(GLfloat data) override;
  void endGlEntity() override;
  void beginGlGraph(GLfloat data) override;
  void endGlGraph() override;
  void beginNode(GLfloat data) override;
  void endNode() override;
  void beginEdge(GLfloat data) override;
  void endEdge() override;
  void pointToken(GLfloat *data) override;
  void lineToken(GLfloat *data) override;
  void lineResetToken(GLfloat *data) override;
  void polygonToken(GLfloat *data) override;
  void bitmapToken(GLfloat *data) override;
  void drawPixelToken(GLfloat *data) override;
  void copyPixelToken(GLfloat *data) override;
  void end() override;

  void getResult(std::string *str) override;

private:
  std::stringstream stream_out;

  GLfloat clearColor[4];
  GLfloat pointSize;
  GLfloat lineWidth;

  Color fillColor;
  Color strokeColor;
  Color textColor;

  bool inGlEntity;
  bool inGlGraph;
  bool inNode;
  bool inEdge;

  int width;
  int height;
};
}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLFEEDBACKBUILDER_H
///@endcond
