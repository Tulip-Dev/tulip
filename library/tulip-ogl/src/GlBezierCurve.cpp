/*
	  GlBezierCurve.cpp

   Created on: 10 avr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#include <GL/glew.h>

#include "tulip/GlBezierCurve.h"
#include "tulip/GlCatmullRomCurve.h"

using namespace std;

vector<vector<double> > buildPascalTriangle(unsigned int n) {
	vector<vector<double> > pascalTriangle;
	pascalTriangle.resize(n);
	for (unsigned int i = 0; i < n; ++i) {
		pascalTriangle[i].resize(i+1);
	}

	pascalTriangle[0][0] = 1;
	for (unsigned int i = 1; i < n; i++) {
		pascalTriangle[i][0] = 1;
		pascalTriangle[i][i] = 1;
		for (unsigned int j = 1; j < i; ++j) {
			pascalTriangle[i][j] = pascalTriangle[i-1][j-1] + pascalTriangle[i-1][j];
		}
	}
	return pascalTriangle;
}

static const unsigned int CONTROL_POINTS_LIMIT = 120;



static string bezierSpecificVertexShaderSrc =
		"uniform sampler2D pascalTriangleTex;"
		"const int maxControlPoints = 120;"
		"const float pascalTriangleTexStep =  1.0 / float(maxControlPoints-1);"

		"vec3 computeCurvePoint(float t) {"
		"	if (t == 0.0) {"
		"		return controlPoints[0];"
		"	} else if (t == 1.0) {"
		"		return controlPoints[nbControlPoints - 1];"
		"	} else {"
		"		float s = (1.0 - t);"
		"		vec3 bezierPoint = vec3(0.0);"
		"		for (int i = 0 ; i < nbControlPoints ; ++i) { "
		"			vec2 pascalTriangleTexIdx = vec2(float(i) * pascalTriangleTexStep, float(nbControlPoints-1) * pascalTriangleTexStep);"
		"			bezierPoint += controlPoints[i].xyz * texture2D(pascalTriangleTex, pascalTriangleTexIdx).r * pow(t, float(i)) * pow(s, float(nbControlPoints - 1 - i));"
		"		}"
		"		return bezierPoint;"
		"	}"
		"}"

		;

namespace tlp {

float *GlBezierCurve::pascalTriangleTextureData(NULL);
GLuint GlBezierCurve::pascalTriangleTextureId(0);

GlBezierCurve::GlBezierCurve() : AbstractGlCurve("bezier vertex shader", bezierSpecificVertexShaderSrc) {}

GlBezierCurve::GlBezierCurve(const vector<Coord> &controlPoints, const Color &startColor, const Color &endColor,
		const float &startSize, const float &endSize, const unsigned int nbCurvePoints) :
		AbstractGlCurve("bezier vertex shader", bezierSpecificVertexShaderSrc, controlPoints, startColor, endColor, startSize, endSize, nbCurvePoints) {
}

GlBezierCurve::~GlBezierCurve() {}

void GlBezierCurve::buildPascalTriangleTexture() {
	vector<vector<double> > pascalTriangle = buildPascalTriangle(CONTROL_POINTS_LIMIT);
	pascalTriangleTextureData = new float [CONTROL_POINTS_LIMIT * CONTROL_POINTS_LIMIT];
	memset(pascalTriangleTextureData, 0, CONTROL_POINTS_LIMIT * CONTROL_POINTS_LIMIT * sizeof(float));
	for (unsigned int i = 0 ; i < CONTROL_POINTS_LIMIT ; ++i) {
		for (unsigned int j = 0 ; j <= i ; ++j) {
			pascalTriangleTextureData[i * CONTROL_POINTS_LIMIT + j] = pascalTriangle[i][j];
		}
	}
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &pascalTriangleTextureId);
	glBindTexture(GL_TEXTURE_2D, pascalTriangleTextureId);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D,0,GL_LUMINANCE32F_ARB,CONTROL_POINTS_LIMIT,CONTROL_POINTS_LIMIT,0,GL_LUMINANCE,GL_FLOAT,pascalTriangleTextureData);
	glDisable(GL_TEXTURE_2D);
}

void GlBezierCurve::setCurveVertexShaderRenderingSpecificParameters() {
	glActiveTexture(GL_TEXTURE2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, pascalTriangleTextureId);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	curveShaderProgram->setUniformTextureSampler("pascalTriangleTex", 2);
}

void GlBezierCurve::cleanupAfterCurveVertexShaderRendering() {
	glActiveTexture(GL_TEXTURE2);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
}

Coord GlBezierCurve::computeCurvePointOnCPU(const std::vector<Coord> &controlPoints, float t) {
	static vector<vector<double> > pascalTriangle = buildPascalTriangle(3*CONTROL_POINTS_LIMIT);
	tlp::Vector<double, 3> bezierPoint;
	bezierPoint.fill(0.0);
	for (size_t i = 0 ; i < controlPoints.size() ; ++i) {
		tlp::Vector<double, 3> controlPoint;
		controlPoint[0] = controlPoints[i][0];
		controlPoint[1] = controlPoints[i][1];
		controlPoint[2] = controlPoints[i][2];
		bezierPoint += controlPoint * pascalTriangle[controlPoints.size() - 1][i] * pow((double)t, (double)i) * pow((double)(1.0 - t), (double)(controlPoints.size() - 1 - i));
	}
	return Coord(bezierPoint[0], bezierPoint[1], bezierPoint[2]);
}

void GlBezierCurve::drawCurve(std::vector<Coord> *controlPoints, const Color &startColor, const Color &endColor, const float startSize, const float endSize, const unsigned int nbCurvePoints) {

	static bool floatTextureOk = glewIsSupported("GL_ARB_texture_float");
	if (pascalTriangleTextureId == 0 && floatTextureOk) {
		buildPascalTriangleTexture();
	}

	if (!floatTextureOk) {
		// float texture not supported, forcing CPU rendering
		curveShaderProgramNormal = NULL;
		curveShaderProgramBillboard = NULL;
	}

	if (controlPoints->size() <= CONTROL_POINTS_LIMIT) {
		AbstractGlCurve::drawCurve(controlPoints, startColor, endColor, startSize, endSize, controlPoints->size() > 2 ? nbCurvePoints : 2);
	} else {

		static GlCatmullRomCurve curve;

		const unsigned int nbApproximationPoints = 20;
		vector<Coord> curvePoints;
		for (unsigned int i = 0 ; i < nbApproximationPoints ; ++i) {
			curvePoints.push_back(computeCurvePointOnCPU(*controlPoints, i / static_cast<float>(nbApproximationPoints-1)));
		}
		curve.setClosedCurve(false);
		curve.setOutlined(outlined);
		curve.setOutlineColor(outlineColor);
		curve.setTexture(texture);
		curve.setBillboardCurve(billboardCurve);
		curve.setLookDir(lookDir);
		curve.drawCurve(&curvePoints, startColor, endColor, startSize, endSize, nbCurvePoints);
	}
}

}
