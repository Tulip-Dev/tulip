/*
	  GlCatmullRomCurve.cpp

   Created on: 21 avr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlPolyQuad.h>

#include "GlSmoothCurve.h"
#include "FastBezier.h"
#include "CurveUtils.h"

using namespace std;

namespace tlp {

GlSmoothCurve::GlSmoothCurve(const std::vector<Coord> &curvePassPoints, const Color &color, const float size,
									 const std::string &texture, const bool closedCurve) :
	curvePassPoints(curvePassPoints), color(color), size(size), texture(texture), closedCurve(closedCurve) {
	for (unsigned int i = 0 ; i < curvePassPoints.size() ; ++i) {
		boundingBox.check(curvePassPoints[i]);
	}
}

GlSmoothCurve::~GlSmoothCurve() {}

void GlSmoothCurve::draw(float lod,Camera *camera) {
	Coord startN, endN;
	startN = curvePassPoints[0] - Coord(1,0,0);
	if (closedCurve) {
		endN = curvePassPoints[0] + Coord(1,0,0);
	} else {
		endN = curvePassPoints[curvePassPoints.size() - 1] + Coord(1,0,0);
	}
	vector<Coord> splinePoints;
	FastBezier::computeSplinePoints(curvePassPoints, splinePoints, closedCurve);
	drawCurve(splinePoints, color, color, size, size, startN, endN, texture);
}

void GlSmoothCurve::translate(const Coord& mouvement) {
	boundingBox.first += mouvement;
	boundingBox.second += mouvement;
}

}
