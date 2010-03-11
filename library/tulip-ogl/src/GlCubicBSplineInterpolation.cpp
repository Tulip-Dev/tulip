#include "tulip/GlCubicBSplineInterpolation.h"

using namespace std;

namespace tlp {

GlCubicBSplineInterpolation::GlCubicBSplineInterpolation(const vector<Coord> &pointsToInterpolate, const Color &startColor, const Color &endColor,
														 const float startSize, const float endSize, const unsigned int nbCurvePoints,
														 const bool outlined, const Color &outlineColor, const string &texture) : bspline(NULL) {
	vector<Coord> bsplineControlPoints;
	constructInterpolatingCubicBSpline(pointsToInterpolate, bsplineControlPoints);
	bspline = new GlOpenUniformCubicBSpline(bsplineControlPoints, startColor, endColor, startSize, endSize, nbCurvePoints, outlined, outlineColor, texture);
	boundingBox = bspline->getBoundingBox();
}

GlCubicBSplineInterpolation::~GlCubicBSplineInterpolation() {
	delete bspline;
}

void GlCubicBSplineInterpolation::setTexture(const std::string &texture) {
	bspline->setTexture(texture);
}

void GlCubicBSplineInterpolation::setOutlined(const bool outlined) {
	bspline->setOutlined(outlined);
}

void GlCubicBSplineInterpolation::setOutlineColor(const Color &outlineColor) {
	bspline->setOutlineColor(outlineColor);
}

void GlCubicBSplineInterpolation::translate(const Coord& mouvement) {
	bspline->translate(mouvement);
}

void GlCubicBSplineInterpolation::draw(float lod,Camera *camera) {
	bspline->draw(lod, camera);
}

void GlCubicBSplineInterpolation::constructInterpolatingCubicBSpline(const vector<Coord> &pointsToInterpolate, vector<Coord> &bSplineControlPoints) {
	Coord Ai[pointsToInterpolate.size()];
	float Bi[pointsToInterpolate.size()];
	Coord di[pointsToInterpolate.size()];
	di[0] = (pointsToInterpolate[1] - pointsToInterpolate[0]) / 3.0f;
	di[pointsToInterpolate.size() - 1] = (pointsToInterpolate[pointsToInterpolate.size()-1] - pointsToInterpolate[pointsToInterpolate.size()-2]) / 3.0f;
	Bi[1] = -0.25f;
	Ai[1] = (pointsToInterpolate[2] - pointsToInterpolate[0] - di[0]) / 4.0f;
	for (unsigned int i = 2; i < pointsToInterpolate.size() - 1; ++i) {
		Bi[i] = -1.0f /(4.0f + Bi[i-1]);
		Ai[i] = Coord(-(pointsToInterpolate[i+1] - pointsToInterpolate[i-1] - Ai[i-1])*Bi[i]);
	}
	for (unsigned int i = pointsToInterpolate.size() - 2; i > 0; --i) {
		di[i] = Ai[i] + di[i+1]*Bi[i];
	}
	bSplineControlPoints.push_back(pointsToInterpolate[0]);
	bSplineControlPoints.push_back(pointsToInterpolate[0]+di[0]);
	for (unsigned int i = 1 ; i < pointsToInterpolate.size() - 1 ; ++i) {
		bSplineControlPoints.push_back(pointsToInterpolate[i]-di[i]);
		bSplineControlPoints.push_back(pointsToInterpolate[i]);
		bSplineControlPoints.push_back(pointsToInterpolate[i]+di[i]);
	}
	bSplineControlPoints.push_back(pointsToInterpolate[pointsToInterpolate.size() - 1]-di[pointsToInterpolate.size() - 1]);
	bSplineControlPoints.push_back(pointsToInterpolate[pointsToInterpolate.size() - 1]);

}

}
