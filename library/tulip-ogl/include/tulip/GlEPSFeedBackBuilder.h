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

  GlEPSFeedBackBuilder()
    :stream_out(std::stringstream::in | std::stringstream::out) {}

  /**
   * Begin new EPS document with viewport, clearColor, pointSize and lineWidth informations
   */
  virtual void begin(const Vector<int, 4> &viewport,GLfloat *clearColor,GLfloat pointSize,GLfloat lineWidth);
  /**
   * Record a new color
   */
  virtual void colorInfo(GLfloat *data);
  /**
   * Record a new GlEntity
   */
  virtual void beginGlEntity(GLfloat data);
  /**
   * End of a GlEntity
   */
  virtual void endGlEntity();
  /**
   * Record a new GlGraph
   */
  virtual void beginGlGraph(GLfloat data);
  /**
   * End of a GlGraph
   */
  virtual void endGlGraph();
  /**
   * Record a new Node
   */
  virtual void beginNode(GLfloat data);
  /**
   * End of a Node
   */
  virtual void endNode();
  /**
   * Record a new Edge
   */
  virtual void beginEdge(GLfloat data);
  /**
   * End of a Edge
   */
  virtual void endEdge();
  /**
   * Record a new pointToken
   */
  virtual void pointToken(GLfloat *data);
  /**
   * Record a new lineToken
   */
  virtual void lineToken(GLfloat *data);
  /**
   * Record a new lineResetToken
   */
  virtual void lineResetToken(GLfloat *data);
  /**
   * Record a new polygonToken
   */
  virtual void polygonToken(GLfloat *data);
  /**
   * End of the EPS document
   */
  virtual void end();

  /**
   * Put in str the built EPS document
   */
  virtual void getResult(std::string* str);

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
