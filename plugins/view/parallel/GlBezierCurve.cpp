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

#include <tulip/GlTextureManager.h>
#include <tulip/GlPolyQuad.h>

#include "GlBezierCurve.h"
#include "CurveUtils.h"
#include "FastBezier.h"
#include "ParallelTools.h"

using namespace std;

namespace tlp {
//=============================================
GlBezierCurve::GlBezierCurve(const vector<Coord> &controlPoints,
		const Color &beginColor,
		const Color &endColor,
		const float &beginSize,
		const float &endSize,
		const string &texture,
		const unsigned int nbCurvePoints):
			controlPoints(controlPoints),
			beginColor(beginColor),
			endColor(endColor),
			beginSize(beginSize),
			endSize(endSize),
			texture(texture),
			nbCurvePoints(nbCurvePoints) {

	for (unsigned int i = 0 ; i < controlPoints.size() ; ++i) {
		boundingBox.check(controlPoints[i]);
	}
}
//=====================================================
GlBezierCurve::~GlBezierCurve() {}
//=====================================================
void GlBezierCurve::draw(float lod, Camera *camera) {
	vector<Coord> curvePoints;
	FastBezier::computeBezierPoints(controlPoints,curvePoints);
	drawCurve(curvePoints, beginColor, endColor, beginSize, endSize, curvePoints[0] - Coord(1,0,0), curvePoints[curvePoints.size() - 1] + Coord(1,0,0), texture);
}
//=====================================================
void GlBezierCurve::setTexture(const std::string &texture) {
	this->texture=texture;
}
//=====================================================
void GlBezierCurve::translate(const Coord& mouvement){
	boundingBox.first+=mouvement;
	boundingBox.second+=mouvement;
	for (unsigned int i = 0 ; i < controlPoints.size() ; ++i) {
		controlPoints[i] += mouvement;
	}
}
//=====================================================
void GlBezierCurve::getXML(xmlNodePtr rootNode) {
	xmlNodePtr dataNode=NULL;

	GlXMLTools::createProperty(rootNode, "type", "GlBezierCurve");

	GlXMLTools::getDataNode(rootNode,dataNode);

	GlXMLTools::getXML(dataNode,"beginSize",beginSize);
	GlXMLTools::getXML(dataNode,"endSize",endSize);
}
//============================================================
void GlBezierCurve::setWithXML(xmlNodePtr rootNode) {
	xmlNodePtr dataNode=NULL;

	GlXMLTools::getDataNode(rootNode,dataNode);

	// Parse Data
	if(dataNode) {
		GlXMLTools::setWithXML(dataNode,"beginSize",beginSize);
		GlXMLTools::setWithXML(dataNode,"endSize",endSize);
	}
}

}
