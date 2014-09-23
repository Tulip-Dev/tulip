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

#include <cstring>

#include <tulip/GlBezierCurve.h>
#include <tulip/GlCatmullRomCurve.h>
#include <tulip/ParametricCurves.h>

using namespace std;

static const unsigned int CONTROL_POINTS_LIMIT = 120;

static string bezierSpecificVertexShaderSrc =

  "vec3 computeCurvePoint(float t) {"
  "	if (t == 0.0) {"
  "		return getControlPoint(0);"
  "	} else if (t == 1.0) {"
  "		return getControlPoint(nbControlPoints - 1);"
  "	} else {"
  "		float s = (1.0 - t);"
  "     float r = float(nbControlPoints);"
  "     float curCoeff = 1.0;"
  "     float t2 = 1.0;"
  "		vec3 bezierPoint = vec3(0.0);"
  "		for (int i = 0 ; i < nbControlPoints ; ++i) { "
  "			bezierPoint += getControlPoint(i).xyz * curCoeff * t2 * pow(s, float(nbControlPoints - 1 - i));"
  "         float c = float(i+1);"
  "         curCoeff *= (r-c)/c;"
  "         t2 *= t;"
  "		}"
  "		return bezierPoint;"
  "	}"
  "}"

  ;

namespace tlp {

GlBezierCurve::GlBezierCurve() : AbstractGlCurve("bezier vertex shader", bezierSpecificVertexShaderSrc) {}

GlBezierCurve::GlBezierCurve(const vector<Coord> &controlPoints, const Color &startColor, const Color &endColor,
                             const float &startSize, const float &endSize, const unsigned int nbCurvePoints) :
  AbstractGlCurve("bezier vertex shader", bezierSpecificVertexShaderSrc, controlPoints, startColor, endColor, startSize, endSize, nbCurvePoints) {
}

GlBezierCurve::~GlBezierCurve() {}

void GlBezierCurve::computeCurvePointsOnCPU(const std::vector<Coord> &controlPoints, std::vector<Coord> &curvePoints, unsigned int nbCurvePoints) {
  computeBezierPoints(controlPoints, curvePoints, nbCurvePoints);
}

Coord GlBezierCurve::computeCurvePointOnCPU(const std::vector<Coord> &controlPoints, float t) {
  return computeBezierPoint(controlPoints, t);
}

void GlBezierCurve::drawCurve(std::vector<Coord> &controlPoints, const Color &startColor, const Color &endColor, const float startSize, const float endSize, const unsigned int nbCurvePoints) {

  if (controlPoints.size() <= CONTROL_POINTS_LIMIT) {
    AbstractGlCurve::drawCurve(controlPoints, startColor, endColor, startSize, endSize, nbCurvePoints);
  }
  else {

    static GlCatmullRomCurve curve;

    const unsigned int nbApproximationPoints = 40;
    vector<Coord> curvePoints;
    computeBezierPoints(controlPoints, curvePoints, nbApproximationPoints);
    curve.setClosedCurve(false);
    curve.setOutlined(outlined);
    curve.setOutlineColor(outlineColor);
    curve.setTexture(texture);
    curve.setBillboardCurve(billboardCurve);
    curve.setLookDir(lookDir);
    curve.drawCurve(curvePoints, startColor, endColor, startSize, endSize, nbCurvePoints);
  }
}

}
