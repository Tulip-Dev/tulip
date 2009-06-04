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

#include <tulip/Curves.h>

#include <sstream>

#include "tulip/GlCatmullRomCurve.h"

using namespace std;

namespace tlp {

GlCatmullRomCurve::GlCatmullRomCurve(const vector<Coord> &curvePassPoints, const Color &beginColor, const Color &endColor,
									 const float beginSize, const float endSize, const string &texture, const bool closedCurve) :
			beginColor(beginColor), endColor(endColor), beginSize(beginSize), endSize(endSize), texture(texture), closedCurve(closedCurve) {

	computeBezierSegments(curvePassPoints);
}

GlCatmullRomCurve::~GlCatmullRomCurve() {
	reset(true);
}


void GlCatmullRomCurve::computeBezierSegmentControlPoints(const Coord &pBefore, const Coord &pStart, const Coord &pEnd, const Coord &pAfter, vector<Coord> &bezierSegmentControlPoints) {
	bezierSegmentControlPoints.push_back(pStart);
	Coord d0, d1;
	d0 = (pEnd - pBefore) / 2.;
	bezierSegmentControlPoints.push_back(pStart + d0 / 3.);
	d1 = (pAfter - pStart) / 2.;
	bezierSegmentControlPoints.push_back(pEnd - d1 / 3.);
	bezierSegmentControlPoints.push_back(pEnd);
}

void GlCatmullRomCurve::computeBezierSegments(const vector<Coord> &curvePassPoints) {

	ostringstream oss;

	if (curvePassPoints.size() == 2) {
		GlBezierCurve *curve = new GlBezierCurve(curvePassPoints, beginColor, endColor, beginSize, endSize, texture);
		oss << curve;
		addGlEntity(curve, oss.str());
		return;
	}

	vector<Color> colors = getColors(curvePassPoints, beginColor, endColor);
	vector<float> sizes = getSizes(curvePassPoints, beginSize, endSize);

	for (unsigned int i = 0 ; i < curvePassPoints.size() ; ++i) {
		vector<Coord> bezierSegmentControlPoints;
		if (i == 0 && !closedCurve) {
			computeBezierSegmentControlPoints(curvePassPoints[i], curvePassPoints[i], curvePassPoints[i+1], curvePassPoints[i+2], bezierSegmentControlPoints);
		} else if (i ==0 && closedCurve) {
			computeBezierSegmentControlPoints(curvePassPoints[curvePassPoints.size() - 1], curvePassPoints[i], curvePassPoints[i+1], curvePassPoints[i+2], bezierSegmentControlPoints);
		} else if (i == curvePassPoints.size() - 2 && !closedCurve) {
			computeBezierSegmentControlPoints(curvePassPoints[i-1], curvePassPoints[i], curvePassPoints[i+1], curvePassPoints[i+1], bezierSegmentControlPoints);
		} else if (i == curvePassPoints.size() - 2 && closedCurve) {
			computeBezierSegmentControlPoints(curvePassPoints[i-1], curvePassPoints[i], curvePassPoints[i+1], curvePassPoints[0], bezierSegmentControlPoints);
		} else if (i == curvePassPoints.size() -1 && closedCurve) {
			computeBezierSegmentControlPoints(curvePassPoints[i-1], curvePassPoints[i], curvePassPoints[0], curvePassPoints[1], bezierSegmentControlPoints);
		} else if (i != curvePassPoints.size() - 1) {
			computeBezierSegmentControlPoints(curvePassPoints[i-1], curvePassPoints[i], curvePassPoints[i+1], curvePassPoints[i+2], bezierSegmentControlPoints);
		}
		if (bezierSegmentControlPoints.size() > 0) {
			float beginSize, endSize;
			Color beginColor, endColor;
			if (i != curvePassPoints.size() - 1) {
				beginSize = sizes[i];
				endSize = sizes[i+1];
				beginColor = colors[i];
				endColor = colors[i+1];
			} else {
				beginSize = sizes[i];
				endSize = sizes[0];
				beginColor = colors[i];
				endColor = colors[0];
			}
			GlBezierCurve *curve = new GlBezierCurve(bezierSegmentControlPoints, beginColor, endColor, beginSize, endSize, texture);
			oss.str("");
			oss << curve;
			addGlEntity(curve, oss.str());
		}
	}
}

void GlCatmullRomCurve::draw(float lod, Camera *camera) {
	map<string, GlSimpleEntity*> *glEntities = getDisplays();
	map<string, GlSimpleEntity*>::iterator it;
	for (it = glEntities->begin(); it != glEntities->end() ; ++it) {
		GlSimpleEntity *entity = it->second;
		entity->draw(lod, camera);
	}
}

}
