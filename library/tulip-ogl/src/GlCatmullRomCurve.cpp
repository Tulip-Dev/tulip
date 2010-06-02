/*
	  GlCatmullRomCurve.cpp

   Created on: 29 avr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#include <GL/glew.h>

#include <sstream>

#include "tulip/GlCatmullRomCurve.h"
#include "tulip/GlBezierCurve.h"
#include "tulip/ParametricCurves.h"

using namespace std;

static string catmullRomSpecificShaderCode=
		"uniform bool closedCurve;"
		"uniform float totalLength;"
		"vec3 bezierControlPoints[4] = vec3[4](vec3(0.0), vec3(0.0), vec3(0.0), vec3(0.0));"

		"float parameter[2];"

		"void computeBezierSegmentControlPoints(vec3 pBefore, vec3 pStart, vec3 pEnd, vec3 pAfter) {"
		"	vec3 d0, d1;"
		"	bezierControlPoints[0] = pStart;"
		"	d0 = (pEnd - pBefore) * 0.5;"
		"	bezierControlPoints[1] = pStart + d0 / 3.;"
		"	d1 = (pAfter - pStart) * 0.5;"
		"	bezierControlPoints[2] = pEnd - d1 / 3.;"
		"	bezierControlPoints[3] = pEnd;"
		"}"

		"int computeSegmentIndex(float t) {"
		"	float dist = distance(controlPoints[0], controlPoints[1]);"
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
		"			dist += distance(controlPoints[i], controlPoints[i+1]);"
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
		"		computeBezierSegmentControlPoints(closedCurve ? controlPoints[nbControlPoints - 2] : controlPoints[i], controlPoints[i], controlPoints[i+1], controlPoints[i+2]);"
		"	} else if (i == nbControlPoints - 2) {"
		"		computeBezierSegmentControlPoints(controlPoints[i-1], controlPoints[i], controlPoints[i+1], closedCurve ? controlPoints[1] : controlPoints[i+1]);"
		"	} else if (i == nbControlPoints - 1) {"
		"		computeBezierSegmentControlPoints(controlPoints[i-2], controlPoints[i-1], controlPoints[i], closedCurve ? controlPoints[1] : controlPoints[i]);"
		"	} else {"
		"		computeBezierSegmentControlPoints(controlPoints[i-1], controlPoints[i], controlPoints[i+1], controlPoints[i+2]);"
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

GlCatmullRomCurve::GlCatmullRomCurve() : AbstractGlCurve("catmull rom vertex shader", catmullRomSpecificShaderCode), closedCurve(false) {}

GlCatmullRomCurve::GlCatmullRomCurve(const vector<Coord> &controlPoints, const Color &startColor, const Color &endColor,
		const float startSize, const float endSize, const bool closedCurve, const unsigned int nbCurvePoints)
: AbstractGlCurve("catmull rom vertex shader", catmullRomSpecificShaderCode, controlPoints, startColor, endColor, startSize, endSize, nbCurvePoints), closedCurve(closedCurve) {}

GlCatmullRomCurve::~GlCatmullRomCurve() {}

Coord GlCatmullRomCurve::computeCurvePointOnCPU(const std::vector<Coord> &controlPoints, float t) {
	return computeCatmullRomPoint(controlPoints, t, closedCurve);
}

void GlCatmullRomCurve::computeCurvePointsOnCPU(const std::vector<Coord> &controlPoints, std::vector<Coord> &curvePoints, unsigned int nbCurvePoints) {
	computeCatmullRomPoints(controlPoints, curvePoints, closedCurve, nbCurvePoints);
}

void GlCatmullRomCurve::setCurveVertexShaderRenderingSpecificParameters() {
	curveShaderProgram->setUniformBool("closedCurve", closedCurve);
	curveShaderProgram->setUniformFloat("totalLength", totalLength);
}

void GlCatmullRomCurve::drawCurve(vector<Coord> *controlPoints, const Color &startColor, const Color &endColor, const float startSize, const float endSize, const unsigned int nbCurvePoints) {

	vector<Coord> controlPointsCp;
	vector<Coord> *controlPointsP = controlPoints;

	if (closedCurve && curveShaderProgramNormal != NULL) {
		controlPointsCp = *controlPoints;
		controlPointsCp.push_back(controlPointsCp[0]);
		controlPointsP = &controlPointsCp;
	}

	if (curveShaderProgramNormal != NULL) {
		totalLength = 0.0f;
		for (size_t i = 1 ; i < controlPointsP->size() ; ++i) {
			float dist = (*controlPointsP)[i-1].dist((*controlPointsP)[i]);
			totalLength += dist;
		}
	}

	static GlBezierCurve curve;
	if (controlPoints->size() == 2) {
		curve.setOutlined(outlined);
		curve.setOutlineColor(outlineColor);
		curve.setTexture(texture);
		curve.setBillboardCurve(billboardCurve);
		curve.setLookDir(lookDir);
		curve.drawCurve(controlPoints, startColor, endColor, startSize, endSize, nbCurvePoints);
	} else {
		AbstractGlCurve::drawCurve(controlPointsP, startColor, endColor, startSize, endSize, nbCurvePoints);
	}

}

}
