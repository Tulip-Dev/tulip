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

#include <tulip/GlCatmullRomCurve.h>
#include <tulip/GlBezierCurve.h>

using namespace std;

static string catmullRomSpecificShaderCode =
		"uniform bool closedCurve;"

		"vec3 bezierControlPoints[4] = vec3[4](vec3(0.0), vec3(0.0), vec3(0.0), vec3(0.0));"

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
		"	if (t == 0.0) {"
		"		return 0;"
		"	} else if (t == 1.0)   {"
		"		return nbControlPoints - 1;"
		"	} else {"
		"		int i = 0;"
		"		while (t >= controlPoints[i+1].w) {"
		"			++i;"
		"		}"
		"		return i;"
		"	}"
		"}"

		"vec3 computeCurvePoint(float t) {"
		"	int i = computeSegmentIndex(t);"
		"	float localT = 0.0;"
		"	if (t == 1.0) {"
		"		localT = 1.0;"
		"	} else if (t != 0.0) {"
		"		localT = (t - controlPoints[i].w) / (controlPoints[i+1].w - controlPoints[i].w);"
		"	}"
		"	if (i == 0) {"
		"		computeBezierSegmentControlPoints(closedCurve ? controlPoints[nbControlPoints - 1].xyz : controlPoints[i].xyz, controlPoints[i].xyz, controlPoints[i+1].xyz, controlPoints[i+2].xyz);"
		"	} else if (i == nbControlPoints - 2) {"
		"		computeBezierSegmentControlPoints(controlPoints[i-1].xyz, controlPoints[i].xyz, controlPoints[i+1].xyz, closedCurve ? controlPoints[0].xyz : controlPoints[i+1].xyz);"
		"	} else if (i == nbControlPoints - 1) {"
		"		computeBezierSegmentControlPoints(controlPoints[i-2].xyz, controlPoints[i-1].xyz, controlPoints[i].xyz, closedCurve ? controlPoints[0].xyz : controlPoints[i].xyz);"
		"	} else {"
		"		computeBezierSegmentControlPoints(controlPoints[i-1].xyz, controlPoints[i].xyz, controlPoints[i+1].xyz, controlPoints[i+2].xyz);"
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

static vector<Coord> getCamullControlPoints(const vector<Coord> &controlPoints, bool closedCurve) {
	vector<Coord> controlPointsCp(controlPoints);
	if (closedCurve) {
		controlPointsCp.push_back(controlPoints[0]);
	}
	return controlPointsCp;
}

GlCatmullRomCurve::GlCatmullRomCurve(const vector<Coord> &controlPoints, const Color &startColor, const Color &endColor,
		const float startSize, const float endSize, const bool closedCurve, const unsigned int nbCurvePoints)
: AbstractGlCurve("catmull rom vertex shader", catmullRomSpecificShaderCode, controlPoints, startColor, endColor, startSize, endSize, nbCurvePoints), closedCurve(closedCurve) {

	float globalParameter[nbControlPoints];
	globalParameter[0] = 0.0f;
	globalParameter[nbControlPoints - 1] = 1.0f;
	float cumLength[nbControlPoints - 1];
	cumLength[0] = 0.0f;
	float totalLength = 0;
	for (unsigned int i = 1 ; i < nbControlPoints ; ++i) {
		float dist = controlPoints[i-1].dist(controlPoints[i]);
		cumLength[i] = cumLength[i-1] + dist;
		totalLength += dist;
	}
	for (unsigned int i = 1 ; i < nbControlPoints - 1 ; ++i) {
		globalParameter[i] = cumLength[i] / totalLength;
	}

	for (unsigned int i = 0 ; i < nbControlPoints ; ++i) {
		glControlPoints[4*i+3] = globalParameter[i];
	}
}

GlCatmullRomCurve::~GlCatmullRomCurve() {}

int GlCatmullRomCurve::computeSegmentIndex(float t) {
	if (t == 0.0) {
		return 0;
	} else if (t == 1.0)  {
		return nbControlPoints - 1;
	} else {
		int i = 0;
		while (t >= glControlPoints[4*(i+1)+3]) {
			++i;
		}
		return i;
	}
}

Coord GlCatmullRomCurve::computeCurvePointOnCPU(float t) {
	int i = computeSegmentIndex(t);
	float localT = 0.0;
	if (t == 1.0) {
		localT = 1.0;
	} else if (t != 0.0) {
		localT = (t - glControlPoints[4*i+3]) / (glControlPoints[4*(i+1)+3] - glControlPoints[4*i+3]);
	}
	vector<Coord> bezierControlPoints;
	if (i == 0) {
		computeBezierSegmentControlPoints(closedCurve ? controlPoints[controlPoints.size() - 1] : controlPoints[i], controlPoints[i], controlPoints[i+1], controlPoints[i+2], bezierControlPoints);
	} else if (i == nbControlPoints - 2) {
		computeBezierSegmentControlPoints(controlPoints[i-1], controlPoints[i], controlPoints[i+1], closedCurve ? controlPoints[0] : controlPoints[i+1], bezierControlPoints);
	} else if (i == nbControlPoints - 1) {
		computeBezierSegmentControlPoints(controlPoints[i-2], controlPoints[i-1], controlPoints[i], closedCurve ? controlPoints[0] : controlPoints[i], bezierControlPoints);
	} else if (i != nbControlPoints - 1) {
		computeBezierSegmentControlPoints(controlPoints[i-1], controlPoints[i], controlPoints[i+1], controlPoints[i+2], bezierControlPoints);
	}
	float t2 = localT * localT;
	float t3 = t2 * localT;
	float s = 1.0 - localT;
	float s2 = s * s;
	float s3 = s2	* s;
	return (bezierControlPoints[0] * s3 + bezierControlPoints[1] * 3.0 * localT * s2 + bezierControlPoints[2] * 3.0 * t2 * s + bezierControlPoints[3] * t3);
}

void GlCatmullRomCurve::computeBezierSegmentControlPoints(const Coord &pBefore, const Coord &pStart, const Coord &pEnd, const Coord &pAfter, vector<Coord> &bezierSegmentControlPoints) {
	bezierSegmentControlPoints.push_back(pStart);
	Coord d0, d1;
	d0 = (pEnd - pBefore) / 2.f;
	bezierSegmentControlPoints.push_back(pStart + d0 / 3.f);
	d1 = (pAfter - pStart) / 2.f;
	bezierSegmentControlPoints.push_back(pEnd - d1 / 3.f);
	bezierSegmentControlPoints.push_back(pEnd);
}

void GlCatmullRomCurve::setCurveVertexShaderRenderingSpecificParameters() {
	curveShaderProgram->setUniformBool("closedCurve", closedCurve);
}


void GlCatmullRomCurve::draw(float lod, Camera *camera) {
	if (controlPoints.size() == 2) {
		GlBezierCurve curve(controlPoints, startColor, endColor, startSize, endSize, nbCurvePoints);
		curve.setOutlined(outlined);
		curve.setOutlineColor(outlineColor);
		curve.setTexture(texture);
		curve.setBillboardCurve(billboardCurve);
		curve.setLookDir(lookDir);
		curve.draw(lod, camera);
	} else {
		AbstractGlCurve::draw(lod, camera);
	}

	if (closedCurve) {
		vector<Coord> lastCurveSegmentControlPoints;
		computeBezierSegmentControlPoints(controlPoints[controlPoints.size() - 2], controlPoints[controlPoints.size() - 1], controlPoints[0], controlPoints[1], lastCurveSegmentControlPoints);
		GlBezierCurve bezierSegment(lastCurveSegmentControlPoints, startColor, endColor, startSize, endSize, nbCurvePoints);
		bezierSegment.setOutlined(outlined);
		bezierSegment.setOutlineColor(outlineColor);
		bezierSegment.setTexture(texture);
		bezierSegment.setBillboardCurve(billboardCurve);
		bezierSegment.setLookDir(lookDir);
		bezierSegment.draw(lod, camera);
	}
}

}
