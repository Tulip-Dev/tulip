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

#include <tulip/Curves.h>
#include <tulip/GlCurve.h>
#include <tulip/GlTextureManager.h>
#include <tulip/GlXMLTools.h>

using namespace std;

namespace tlp {
GlCurve::GlCurve(const vector<Coord> &points,
                 const Color &beginFColor,
                 const Color &endFColor ,
                 const float &beginSize,
                 const float &endSize):
  _points(points),
  _beginFillColor(beginFColor),
  _endFillColor(endFColor),
  _beginSize(beginSize),
  _endSize(endSize),
  texture("") {

  assert(points.size() >= 3);

  for(vector<Coord>::iterator it= _points.begin(); it!=_points.end(); ++it)
    boundingBox.expand(*it);
}
//=====================================================
GlCurve::GlCurve(const unsigned int nbPoints):
  _points(nbPoints),
  texture("") {
}
//=====================================================
GlCurve::~GlCurve() {
}
//=====================================================
void GlCurve::resizePoints(const unsigned int nbPoints) {
  assert(nbPoints >= 3);
  _points.resize(nbPoints);
}
//=====================================================
const tlp::Coord& GlCurve::point(const unsigned int i) const {
  return _points[i];
}
//=====================================================
tlp::Coord& GlCurve::point(const unsigned int i) {
  return _points[i];
}
//=====================================================
void GlCurve::draw(float, Camera *) {
  //    tlp::warning() << ".";

  glDisable(GL_CULL_FACE);
  glDisable(GL_LIGHTING);
  //glEnable(GL_COLOR_MATERIAL);
  vector<Coord> newPoints(_points.size());

  for(unsigned int i=0; i < _points.size(); ++i) {
    newPoints[i] = _points[i];
  }

  glLineWidth(2);
  glPushAttrib(GL_ALL_ATTRIB_BITS);
  tlp::splineLine(newPoints,
                  _beginFillColor,
                  _endFillColor);
  glPopAttrib();

  if(texture!="") {
    GlTextureManager::getInst().activateTexture(texture);
  }

  tlp::splineQuad(newPoints,
                  _beginFillColor,
                  _endFillColor,
                  _beginSize,
                  _endSize,
                  newPoints[0] - Coord(1.  , 0.0, 0.),
                  newPoints[3] + Coord(1.  , 0.0, 0.));

  GlTextureManager::getInst().desactivateTexture();
  glEnable(GL_LIGHTING);
  glEnable(GL_CULL_FACE);
}
//=====================================================
void GlCurve::setTexture(const std::string &texture) {
  this->texture=texture;
}
//=====================================================
void GlCurve::translate(const Coord& mouvement) {
  boundingBox.translate(mouvement);

  for(vector<Coord>::iterator it=_points.begin(); it!=_points.end(); ++it) {
    (*it)+=mouvement;
  }
}
//=====================================================
void GlCurve::getXML(string &outString) {

  GlXMLTools::createProperty(outString,"type","GlCurve","GlEntity");

  GlXMLTools::getXML(outString,"points",_points);
  GlXMLTools::getXML(outString,"beginFillColor",_beginFillColor);
  GlXMLTools::getXML(outString,"endFillColor",_endFillColor);
  GlXMLTools::getXML(outString,"beginSize",_beginSize);
  GlXMLTools::getXML(outString,"endSize",_endSize);
}
//============================================================
void GlCurve::setWithXML(const string &inString, unsigned int &currentPosition) {

  GlXMLTools::setWithXML(inString, currentPosition,"points",_points);
  GlXMLTools::setWithXML(inString, currentPosition,"beginFillColor",_beginFillColor);
  GlXMLTools::setWithXML(inString, currentPosition,"endFillColor",_endFillColor);
  GlXMLTools::setWithXML(inString, currentPosition,"beginSize",_beginSize);
  GlXMLTools::setWithXML(inString, currentPosition,"endSize",_endSize);

  for(vector<Coord>::iterator it= _points.begin(); it!=_points.end(); ++it)
    boundingBox.expand(*it);
}
}
