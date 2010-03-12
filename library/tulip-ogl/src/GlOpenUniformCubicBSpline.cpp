#include <GL/glew.h>

#include <sstream>

#include <tulip/GlTextureManager.h>
#include <tulip/GlOpenUniformCubicBSpline.h>
#include <tulip/GlTools.h>
#include <tulip/Curves.h>
#include <tulip/GlBezierCurve.h>

using namespace std;

namespace tlp {

static string bSplineSpecificShaderCode =
		"uniform float stepKnots;"

		"const int curveDegree = 3;"
		"float coeffs[curveDegree + 1];"

		"vec3 computeCurvePoint(float t) {"
		"	if (t == 0.0) {"
		"		return controlPoints[0].xyz;"
		"	} else if (t >= 1.0) {"
		"		return controlPoints[nbControlPoints - 1].xyz;"
		"	} else {"
		"		int k = curveDegree;"
		"		float cpt = 0.0;"
		"		while (t > (cpt * stepKnots) && t >= ((cpt+1.0) * stepKnots)) {"
		"			++k;"
		"			++cpt;"
		"		}"
		"		float knotVal = float(cpt) * stepKnots;"
		"		for (int i = 0 ; i < coeffs.length ; ++i) {"
		"			coeffs[i] = 0.0;"
		"		}"
		"		coeffs[curveDegree] = 1.0;"
		"		for (int i = 1 ; i <= curveDegree ; ++i) {"
		"			coeffs[curveDegree-i] = (clamp(knotVal + stepKnots, 0.0, 1.0) - t) / (clamp(knotVal + stepKnots, 0.0, 1.0) - clamp(knotVal + (-i+1) * stepKnots, 0.0, 1.0)) * coeffs[curveDegree-i+1];"
		"			int tabIdx = curveDegree-i+1;"
		"			for (int j = -i+1 ; j <= -1 ; ++j) {"
		"				coeffs[tabIdx] = ((t - clamp(knotVal + j * stepKnots, 0.0, 1.0)) / (clamp(knotVal + (j+i) * stepKnots, 0.0, 1.0) - clamp(knotVal + j * stepKnots, 0.0, 1.0))) * coeffs[tabIdx] + ((clamp(knotVal + (j+i+1) * stepKnots, 0.0, 1.0) - t) / (clamp(knotVal + (j+i+1) * stepKnots, 0.0, 1.0) - clamp(knotVal + (j+1) * stepKnots, 0.0, 1.0))) * coeffs[tabIdx+1];"
		"				++tabIdx;"
		"			}"
		"			coeffs[curveDegree] = ((t - knotVal) / (clamp(knotVal + i * stepKnots, 0.0, 1.0) - knotVal)) * coeffs[curveDegree];"
		"		}"
		"		int startIdx = k - curveDegree;"
		"		vec3 curvePoint = vec3(0.0);"
		"		for (int i = 0 ; i <= curveDegree ; ++i) {"
		"			curvePoint += coeffs[i] * controlPoints[startIdx + i].xyz;"
		"		}"
		"		return curvePoint;"
		"	}"
		"}"
		;


const unsigned int curveDegree = 3;

GlOpenUniformCubicBSpline::GlOpenUniformCubicBSpline(const vector<Coord> &controlPoints, const Color &startColor, const Color &endColor,
		const float startSize, const float endSize, const unsigned int nbCurvePoints)
:  AbstractGlCurve("open uniform cubic bspline vertex shader", bSplineSpecificShaderCode, controlPoints, startColor, endColor, startSize, endSize, nbCurvePoints) {
	nbKnots = nbControlPoints + curveDegree + 1;
	stepKnots = 1.0f / ((static_cast<float>(nbKnots) - 2.0f * (static_cast<float>(curveDegree) + 1.0f)) + 2.0f - 1.0f);
}

GlOpenUniformCubicBSpline::~GlOpenUniformCubicBSpline() {}

void GlOpenUniformCubicBSpline::setCurveVertexShaderRenderingSpecificParameters() {
	curveShaderProgram->setUniformFloat("stepKnots", stepKnots);
}

void GlOpenUniformCubicBSpline::draw(float lod,Camera *camera) {
	if (nbControlPoints < (curveDegree + 1)) {
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
}

static float clamp(float f, float minVal, float maxVal) {
	return min(max(f, minVal), maxVal);
}

Coord GlOpenUniformCubicBSpline::computeCurvePointOnCPU(float t) {
	if (t == 0.0) {
		return Coord(glControlPoints[0], glControlPoints[1], glControlPoints[2]);
	} else if (t >= 1.0) {
		return Coord(glControlPoints[4*(nbControlPoints-1)], glControlPoints[4*(nbControlPoints-1)+1], glControlPoints[4*(nbControlPoints-1)+2]);
	} else {
		float coeffs[curveDegree + 1];
		memset(coeffs, 0, (curveDegree + 1) * sizeof(float));
		int k = curveDegree;
		int cpt = 0;
		while (t > (cpt*stepKnots) && t >= ((cpt+1)*stepKnots)) {
			++k;
			++cpt;
		}
		float knotVal = (cpt * stepKnots);
		coeffs[curveDegree] = 1.0;
		for (int i = 1 ; i <= curveDegree ; ++i) {
			coeffs[curveDegree-i] = (clamp(knotVal + stepKnots, 0.0, 1.0) - t) / (clamp(knotVal + stepKnots, 0.0, 1.0) - clamp(knotVal + (-i+1) * stepKnots, 0.0, 1.0)) * coeffs[curveDegree-i+1];
			int tabIdx = curveDegree-i+1;
			for (int j = -i+1 ; j <= -1 ; ++j) {
				coeffs[tabIdx] = ((t - clamp(knotVal + j * stepKnots, 0.0, 1.0)) / (clamp(knotVal + (j+i) * stepKnots, 0.0, 1.0) - clamp(knotVal + j * stepKnots, 0.0, 1.0))) * coeffs[tabIdx] + ((clamp(knotVal + (j+i+1) * stepKnots, 0.0, 1.0) - t) / (clamp(knotVal + (j+i+1) * stepKnots, 0.0, 1.0) - clamp(knotVal + (j+1) * stepKnots, 0.0, 1.0))) * coeffs[tabIdx+1];
				++tabIdx;
			}
			coeffs[curveDegree] = ((t - knotVal) / (clamp(knotVal + i * stepKnots, 0.0, 1.0) - knotVal)) * coeffs[curveDegree];
		}
		Coord curvePoint(0.0f, 0.0f, 0.0f);
		int startIdx = k - curveDegree;
		for (unsigned int i = 0 ; i <= curveDegree ; ++i) {
			curvePoint += coeffs[i] * Coord(glControlPoints[4*(startIdx + i)], glControlPoints[4*(startIdx + i)+1], glControlPoints[4*(startIdx + i)+2]);
		}
		return curvePoint;
	}
}

}
