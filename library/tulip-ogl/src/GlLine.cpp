/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#include "tulip/GlLine.h"
#include "tulip/GlTools.h"
#include "tulip/OpenGlConfigManager.h"


using namespace std;

namespace tlp {
GlLine::GlLine(const vector<Coord> &points, const vector<Color> &colors):
    		_points(points),_colors(colors),width(1.0),factor(1),pattern(0){

	for(vector<Coord>::iterator it= _points.begin();it!=_points.end();++it)
		boundingBox.expand(*it);
}
//=====================================================
GlLine::~GlLine() {
}
//=====================================================
void GlLine::resizePoints(const unsigned int nbPoints) {
	_points.resize(nbPoints);
}
//=====================================================
void GlLine::resizeColors(const unsigned int nbColors) {
	assert(nbColors >= 1);
	_points.resize(nbColors);
}
//=====================================================
const tlp::Coord& GlLine::point(const unsigned int i) const {
	return _points[i];
}
//=====================================================
tlp::Coord& GlLine::point(const unsigned int i) {
	return _points[i];
}
//=====================================================
void GlLine::addPoint(const Coord& point,const Color& color) {
	_points.push_back(point);
	_colors.push_back(color);
	boundingBox.expand(point);
}
//=====================================================
const tlp::Color& GlLine::color(const unsigned int i) const {
	return _colors[i];
}
//=====================================================
tlp::Color& GlLine::color(const unsigned int i) {
	return _colors[i];
}
//=====================================================
void GlLine::draw(float,Camera *) {

	OpenGlConfigManager::getInst().activateLineAndPointAntiAliasing();

	glDisable(GL_LIGHTING);
	glLineWidth(width);
	if(pattern!=0){
		glLineStipple(factor,pattern);
		glEnable(GL_LINE_STIPPLE);
	}
	glBegin(GL_LINE_STRIP);

	for(unsigned int i=0; i < _points.size(); ++i) {
		if (i < _colors.size()) {
			setColor(_colors[i]);
		}
		glVertex3fv((float *)&_points[i]);
	}
	glEnd();

	OpenGlConfigManager::getInst().desactivateLineAndPointAntiAliasing();

	if(pattern!=0)
		glDisable(GL_LINE_STIPPLE);

	glLineWidth(1.0);
	glEnable(GL_LIGHTING);
	glTest(__PRETTY_FUNCTION__);
}
//=====================================================
void GlLine::setLineWidth(float width){
	this->width=width;
}
//=====================================================
void GlLine::setLineStipple(unsigned char factor,unsigned int pattern){
	this->factor=factor;
	this->pattern=pattern;
}
//=====================================================
void GlLine::translate(const Coord& mouvement){
	boundingBox.translate(mouvement);

	for(vector<Coord>::iterator it=_points.begin();it!=_points.end();++it) {
		(*it)+=mouvement;
	}
}
//=====================================================
void GlLine::getXML(xmlNodePtr rootNode) {
	xmlNodePtr dataNode=NULL;

	GlXMLTools::createProperty(rootNode, "type", "GlLine");

	GlXMLTools::getDataNode(rootNode,dataNode);

	GlXMLTools::getXML(dataNode,"points",_points);
	GlXMLTools::getXML(dataNode,"colors",_colors);
	GlXMLTools::getXML(dataNode,"width",width);
	GlXMLTools::getXML(dataNode,"factor",factor);
	GlXMLTools::getXML(dataNode,"pattern",pattern);

}
//============================================================
void GlLine::setWithXML(xmlNodePtr rootNode) {
	xmlNodePtr dataNode=NULL;

	GlXMLTools::getDataNode(rootNode,dataNode);

	// Parse Data
	if(dataNode) {

		GlXMLTools::setWithXML(dataNode, "points", _points);
		GlXMLTools::setWithXML(dataNode, "colors", _colors);
		GlXMLTools::setWithXML(dataNode,"width",width);
		GlXMLTools::setWithXML(dataNode,"factor",factor);
		GlXMLTools::setWithXML(dataNode,"pattern",pattern);

		for(vector<Coord>::iterator it= _points.begin();it!=_points.end();++it)
			boundingBox.expand(*it);
	}
}
}
