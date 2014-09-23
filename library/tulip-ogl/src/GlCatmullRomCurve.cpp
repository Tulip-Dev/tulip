/**
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

#include <GL/glew.h>

#include <sstream>

#include <tulip/GlCatmullRomCurve.h>
#include <tulip/GlBezierCurve.h>
#include <tulip/ParametricCurves.h>
#include <tulip/GlShaderProgram.h>

using namespace std;

static string catmullRomSpecificShaderCode=
  "uniform bool closedCurve;"
  "uniform float totalLength;"
  "uniform float alpha;"

  "vec3 bezierControlPoints[4];"

  "float parameter[2];"

  "void computeBezierSegmentControlPoints(vec3 pBefore, vec3 pStart, vec3 pEnd, vec3 pAfter) {"
  "	bezierControlPoints[0] = pStart;"
  "	float d1 = distance(pBefore, pStart);"
  "	float d2 = distance(pStart, pEnd);"
  "	float d3 = distance(pEnd, pAfter);"
  "	float d1alpha = pow(d1, alpha);"
  "	float d12alpha = pow(d1, 2*alpha);"
  "	float d2alpha = pow(d2, alpha);"
  "	float d22alpha = pow(d2, 2*alpha);"
  "	float d3alpha = pow(d3, alpha);"
  "	float d32alpha = pow(d3, 2*alpha);"
  "	bezierControlPoints[1] = (d12alpha*pEnd-d22alpha*pBefore+(2*d12alpha+3*d1alpha*d2alpha+d22alpha)*pStart)/(3*d1alpha*(d1alpha+d2alpha));"
  "	bezierControlPoints[2] = (d32alpha*pStart-d22alpha*pAfter+(2*d32alpha+3*d3alpha*d2alpha+d22alpha)*pEnd)/(3*d3alpha*(d3alpha+d2alpha));"
  "	bezierControlPoints[3] = pEnd;"
  "}"

  "int computeSegmentIndex(float t) {"
  "	float dist = pow(distance(getControlPoint(0), getControlPoint(1)), alpha);"
  "	parameter[0] = 0.0;"
  "	parameter[1] = dist / totalLength;"
  "	if (t == 0.0) {"
  "		return 0;"
  "	} else if (t == 1.0)   {"
  "		return nbControlPoints - 1;"
  "	} else {"
  "		int i = 0;"
  "		while (t >= (dist / totalLength)) {"
  "			++i;"
  "			parameter[0] = dist / totalLength;"
  "			dist += pow(distance(getControlPoint(i), getControlPoint(i+1)), alpha);"
  "		}"
  "		parameter[1] = dist / totalLength;"
  "		return i;"
  "	}"
  "}"

  "vec3 computeCurvePoint(float t) {"
  "	int i = computeSegmentIndex(t);"
  "	float localT = 0.0;"
  "	if (t == 1.0) {"
  "		localT = 1.0;"
  "	} else if (t != 0.0) {"
  "		localT = (t - parameter[0]) / (parameter[1] - parameter[0]);"
  "	}"
  "	if (i == 0) {"
  "		computeBezierSegmentControlPoints(closedCurve ? getControlPoint(nbControlPoints - 2) : getControlPoint(i) - (getControlPoint(i+1) - getControlPoint(i)), getControlPoint(i), getControlPoint(i+1), getControlPoint(i+2));"
  "	} else if (i == nbControlPoints - 2) {"
  "		computeBezierSegmentControlPoints(getControlPoint(i-1), getControlPoint(i), getControlPoint(i+1), closedCurve ? getControlPoint(1) : getControlPoint(i+1) + (getControlPoint(i+1) - getControlPoint(i)));"
  "	} else if (i == nbControlPoints - 1) {"
  "		computeBezierSegmentControlPoints(getControlPoint(i-2), getControlPoint(i-1), getControlPoint(i), closedCurve ? getControlPoint(1) : getControlPoint(i) + (getControlPoint(i) - getControlPoint(i-1)));"
  "	} else {"
  "		computeBezierSegmentControlPoints(getControlPoint(i-1), getControlPoint(i), getControlPoint(i+1), getControlPoint(i+2));"
  "	}"
  "	float t2 = localT * localT;"
  "	float t3 = t2 * localT;"
  "	float s = 1.0 - localT;"
  "	float s2 = s * s;"
  "	float s3 = s2	* s;"
  "	return (bezierControlPoints[0] * s3 + bezierControlPoints[1] * 3.0 * localT * s2 + bezierControlPoints[2] * 3.0 * t2 * s + bezierControlPoints[3] * t3);"
  "}"
  ;

namespace tlp {

GlCatmullRomCurve::GlCatmullRomCurve() : AbstractGlCurve("catmull rom vertex shader", catmullRomSpecificShaderCode), closedCurve(false), paramType(CENTRIPETAL) {}

GlCatmullRomCurve::GlCatmullRomCurve(const vector<Coord> &controlPoints, const Color &startColor, const Color &endColor,
                                     const float startSize, const float endSize, const bool closedCurve, const unsigned int nbCurvePoints, const ParameterizationType paramType)
  : AbstractGlCurve("catmull rom vertex shader", catmullRomSpecificShaderCode, controlPoints, startColor, endColor, startSize, endSize, nbCurvePoints), closedCurve(closedCurve), paramType(paramType) {}

GlCatmullRomCurve::~GlCatmullRomCurve() {}

Coord GlCatmullRomCurve::computeCurvePointOnCPU(const std::vector<Coord> &controlPoints, float t) {
  return computeCatmullRomPoint(controlPoints, t, closedCurve, alpha);
}

void GlCatmullRomCurve::computeCurvePointsOnCPU(const std::vector<Coord> &controlPoints, std::vector<Coord> &curvePoints, unsigned int nbCurvePoints) {
  computeCatmullRomPoints(controlPoints, curvePoints, closedCurve, nbCurvePoints, alpha);
}

void GlCatmullRomCurve::setCurveVertexShaderRenderingSpecificParameters() {
  curveShaderProgram->setUniformBool("closedCurve", closedCurve);
  curveShaderProgram->setUniformFloat("totalLength", totalLength);
  curveShaderProgram->setUniformFloat("alpha", alpha);
}

void GlCatmullRomCurve::drawCurve(vector<Coord> &controlPoints, const Color &startColor, const Color &endColor, const float startSize, const float endSize, const unsigned int nbCurvePoints) {

  GLint renderMode;
  glGetIntegerv(GL_RENDER_MODE, &renderMode);

  vector<Coord> controlPointsCp;
  vector<Coord> *controlPointsP = &controlPoints;

  if (paramType == UNIFORM) {
    alpha = 0.0f;
  }
  else if (paramType == CENTRIPETAL) {
    alpha = 0.5f;
  }
  else {
    alpha = 1.0f;
  }

  if (closedCurve && renderMode != GL_SELECT && curveShaderProgramNormal != NULL) {
    controlPointsCp = controlPoints;
    controlPointsCp.push_back(controlPointsCp[0]);
    controlPointsP = &controlPointsCp;
  }

  if (curveShaderProgramNormal != NULL) {
    totalLength = 0.0f;

    for (size_t i = 1 ; i < controlPointsP->size() ; ++i) {
      float dist = pow((float) (*controlPointsP)[i-1].dist((*controlPointsP)[i]), alpha);
      totalLength += dist;
    }
  }

  static GlBezierCurve curve;

  if (controlPoints.size() == 2) {
    curve.setOutlined(outlined);
    curve.setLineCurve(lineCurve);
    curve.setCurveLineWidth(curveLineWidth);
    curve.setCurveQuadBordersWidth(curveQuadBordersWidth);
    curve.setOutlineColorInterpolation(outlineColorInterpolation);
    curve.setOutlineColor(outlineColor);
    curve.setTexture(texture);
    curve.setBillboardCurve(billboardCurve);
    curve.setLookDir(lookDir);
    curve.drawCurve(controlPoints, startColor, endColor, startSize, endSize, nbCurvePoints);
  }
  else {
    AbstractGlCurve::drawCurve(*controlPointsP, startColor, endColor, startSize, endSize, nbCurvePoints);
  }

}

}
