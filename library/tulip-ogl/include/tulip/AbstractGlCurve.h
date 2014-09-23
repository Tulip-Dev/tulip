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

#ifndef ABSTRACTGLCURVE_H
#define ABSTRACTGLCURVE_H

#if defined(_MSC_VER)
#include <Windows.h>
#endif

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
//# if defined(__WIN32__)
//# include <GL/glext.h>
//# endif
#endif

#include <map>
#include <tulip/GlSimpleEntity.h>
#include <tulip/Color.h>

namespace tlp {

class GlShaderProgram;
class GlShader;

class TLP_GL_SCOPE AbstractGlCurve : public GlSimpleEntity {

public :

  AbstractGlCurve(const std::string &shaderProgramName, const std::string &curveSpecificShaderCode);

  AbstractGlCurve(const std::string &shaderProgramName, const std::string &curveSpecificShaderCode, const std::vector<Coord> &controlPoints,
                  const Color &startColor, const Color &endColor, const float startSize, const float endSize, const unsigned int nbCurvePoints);

  virtual ~AbstractGlCurve();

  void draw(float lod, Camera *camera);

  void translate(const Coord &move);

  virtual void setTexture(const std::string &texture) {
    this->texture = texture;
  }

  virtual void setOutlined(const bool outlined) {
    this->outlined = outlined;
  }

  virtual void setOutlineColor(const Color &outlineColor) {
    this->outlineColor = outlineColor;
  }

  /**
   * If set to true, the curve quad outlines will have the same colors
   * than the curve quad
   */
  virtual void setOutlineColorInterpolation(const bool outlineColorInterpolation) {
    this->outlineColorInterpolation = outlineColorInterpolation;
  }

  /**
   * If set to true, the curve is drawn as a line and not as a thick quad
   */
  void setLineCurve(const bool lineCurve) {
    this->lineCurve = lineCurve;
  }

  void setCurveLineWidth(const float curveLineWidth) {
    this->curveLineWidth = curveLineWidth;
  }

  void setCurveQuadBordersWidth(const float curveQuadBorderWidth) {
    this->curveQuadBordersWidth = curveQuadBorderWidth;
  }

  virtual void setBillboardCurve(const bool billboardCurve) {
    this->billboardCurve = billboardCurve;
  }

  virtual void setLookDir(const Coord &lookDir) {
    this->lookDir = lookDir;
  }

  void getXML(std::string &);

  void setWithXML(const std::string &,unsigned int &);

  virtual void drawCurve(std::vector<Coord> &controlPoints, const Color &startColor, const Color &endColor, const float startSize, const float endSize, const unsigned int nbCurvePoints=100);

protected:

  virtual void setCurveVertexShaderRenderingSpecificParameters() {}

  virtual void cleanupAfterCurveVertexShaderRendering() {}

  virtual Coord computeCurvePointOnCPU(const std::vector<Coord> &controlPoints, float t) = 0;

  virtual void computeCurvePointsOnCPU(const std::vector<Coord> &controlPoints, std::vector<Coord> &curvePoints, unsigned int nbCurvePoints) = 0;

  static void buildCurveVertexBuffers(const unsigned int nbCurvePoints, bool vboOk);

  void initShader(const std::string &shaderProgramName, const std::string &curveSpecificShaderCode);

  static std::map<unsigned int, GLfloat *> curveVertexBuffersData;
  static std::map<unsigned int, std::vector<GLushort *> > curveVertexBuffersIndices;
  static std::map<unsigned int, GLuint* > curveVertexBuffersObject;
  static std::map<std::string, GlShaderProgram *> curvesShadersMap;
  static std::map<std::string, GlShaderProgram *> curvesBillboardShadersMap;
  static GlShader *curveVertexShaderNormalMain;
  static GlShader *curveVertexShaderBillboardMain;
  static GlShader *fisheyeDistortionVertexShader;
  static bool canUseGeometryShader;
  static std::map<std::string, std::pair<GlShaderProgram *, GlShaderProgram *> > curvesGeometryShadersMap;
  static GlShader *curveVertexGeometryShaderNormalMain;
  static std::map<std::string, std::pair<GlShaderProgram *, GlShaderProgram *> > curvesBillboardGeometryShadersMap;

  std::string shaderProgramName;
  GlShaderProgram *curveShaderProgramNormal;
  GlShaderProgram *curveShaderProgramBillboard;
  GlShaderProgram *curveShaderProgram;


  std::vector<Coord> controlPoints;
  Color startColor;
  Color endColor;
  float startSize;
  float endSize;
  unsigned int nbCurvePoints;
  bool outlined;
  Color outlineColor;
  std::string texture;
  float texCoordFactor;
  bool billboardCurve;
  Coord lookDir;
  bool lineCurve;
  float curveLineWidth;
  float curveQuadBordersWidth;
  bool outlineColorInterpolation;
};

}

#endif

///@endcond
