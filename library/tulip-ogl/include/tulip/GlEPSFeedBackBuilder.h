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

#ifndef Tulip_GLEPSFEEDBACKBUILDER_H
#define Tulip_GLEPSFEEDBACKBUILDER_H
#ifndef DOXYGEN_NOTFOR_DEVEL

#include <sstream>

#include <tulip/GlTLPFeedBackBuilder.h>
#include <tulip/tulipconf.h>
#include <tulip/Color.h>

namespace tlp {

/**
 * Class to build EPS file with OpenGL feedback buffer
 */
class TLP_GL_SCOPE GlEPSFeedBackBuilder : public GlTLPFeedBackBuilder {

public:
  GlEPSFeedBackBuilder() : stream_out(std::stringstream::in | std::stringstream::out) {}

  /**
   * Begin new EPS document with viewport, clearColor, pointSize and lineWidth information
   */
  void begin(const Vector<int, 4> &viewport, GLfloat *clearColor, GLfloat pointSize,
             GLfloat lineWidth) override;
  /**
   * Record a new color
   */
  void colorInfo(GLfloat *data) override;
  /**
   * Record a new GlEntity
   */
  void beginGlEntity(GLfloat data) override;
  /**
   * End of a GlEntity
   */
  void endGlEntity() override;
  /**
   * Record a new GlGraph
   */
  void beginGlGraph(GLfloat data) override;
  /**
   * End of a GlGraph
   */
  void endGlGraph() override;
  /**
   * Record a new Node
   */
  void beginNode(GLfloat data) override;
  /**
   * End of a Node
   */
  void endNode() override;
  /**
   * Record a new Edge
   */
  void beginEdge(GLfloat data) override;
  /**
   * End of a Edge
   */
  void endEdge() override;
  /**
   * Record a new pointToken
   */
  void pointToken(GLfloat *data) override;
  /**
   * Record a new lineToken
   */
  void lineToken(GLfloat *data) override;
  /**
   * Record a new lineResetToken
   */
  void lineResetToken(GLfloat *data) override;
  /**
   * Record a new polygonToken
   */
  void polygonToken(GLfloat *data) override;
  /**
   * End of the EPS document
   */
  void end() override;

  /**
   * Put in str the built EPS document
   */
  void getResult(std::string *str) override;

private:
  std::stringstream stream_out;

  GLfloat pointSize;

  Color fillColor;
  Color strokeColor;
  Color textColor;
};
}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // Tulip_GLEPSFEEDBACKBUILDER_H
///@endcond
