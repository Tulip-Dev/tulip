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
//*********************************************************************************
//** GlQuad.cpp : Source for a general quad display class
//**
//** author : Delorme Maxime
//** date   : 07/07/05
//*********************************************************************************
#include "tulip/GlQuad.h"

#include "tulip/GlTextureManager.h"
#include "tulip/GlXMLTools.h"
#include "tulip/GlTools.h"
#include "tulip/OpenGlConfigManager.h"

using namespace std;

namespace tlp {

GlQuad::GlQuad():_textureName("") {
  // No default constructor :)
}

void GlQuad::setColor(const Color& color) {
  for(int i=0; i < 4; ++i) {
    _colors[i] = color;
  }
}

GlQuad::GlQuad(const Coord positions[4], const Color &color) {
  for(int i=0; i < 4; ++i) {
      _positions[i] = positions[i];
      _colors[i]    = color;
      boundingBox.expand(_positions[i]);
    }
}

GlQuad::GlQuad(const Coord positions[4], const Color colors[4]){
  for(int i=0; i < 4; ++i) {
    _positions[i] = positions[i];
    _colors[i]    = colors[i];
    boundingBox.expand(_positions[i]);
  }
}

GlQuad::~GlQuad() {
}

void GlQuad::setPosition(const int idPosition, const Coord &position) {
  _positions[idPosition] = position;
  boundingBox = BoundingBox();
  for(int i=0; i<4; ++i)
    boundingBox.expand(_positions[i]);
}

void GlQuad::setColor(const int idColor, const Color &color) {
  _colors[idColor] = color;
}

const Coord& GlQuad::getPosition(const int idPosition) const {
  return  (*(const_cast< Array<Coord, 4> * > (&_positions)))[idPosition];
}

const Color& GlQuad::getColor(const int idColor) const {
  return (*(const_cast< Array<Color, 4> * > (&_colors)))[idColor];
}

void GlQuad::setTextureName(const string &name) {
  _textureName = name;
}

const string& GlQuad::getTextureName() const {
  return _textureName;
}

void GlQuad::draw(float, Camera *) {
  
  if(_textureName!="") {
    GlTextureManager::getInst().activateTexture(_textureName);
  }

  OpenGlConfigManager::getInst().activatePolygonAntiAliasing();
  glDisable(GL_CULL_FACE);
  glBegin(GL_QUADS);
  glNormal3f(0.0f, 0.0f, 1.0f);
  glTexCoord2f(0.0f, 0.0f);
  setMaterial(_colors[0]);
  glVertex3f(_positions[0][0], _positions[0][1], _positions[0][2]);
  glTexCoord2f(1.0f, 0.0f);
  setMaterial(_colors[1]);
  glVertex3f(_positions[1][0], _positions[1][1], _positions[1][2]);
  glTexCoord2f(1.0f, 1.0f);
  setMaterial(_colors[2]);
  glVertex3f(_positions[2][0], _positions[2][1], _positions[2][2]);
  glTexCoord2f(0.0f, 1.0f);
  setMaterial(_colors[3]);
  glVertex3f(_positions[3][0], _positions[3][1], _positions[3][2]);

  glEnd();
  glEnable(GL_CULL_FACE);

  OpenGlConfigManager::getInst().desactivatePolygonAntiAliasing();

  GlTextureManager::getInst().desactivateTexture();

}
//===========================================================
void GlQuad::translate(const Coord& mouvement) {
  boundingBox.translate(mouvement);
  for(int i=0; i < 4; ++i) {
    _positions[i] += mouvement;
  }
}
//===========================================================
void GlQuad::getXML(xmlNodePtr rootNode) {
  xmlNodePtr dataNode=NULL;

  GlXMLTools::createProperty(rootNode, "type", "GlQuad");

  GlXMLTools::getDataNode(rootNode,dataNode);

  GlXMLTools::getXML(dataNode,"position0",_positions[0]);
  GlXMLTools::getXML(dataNode,"position1",_positions[1]);
  GlXMLTools::getXML(dataNode,"position2",_positions[2]);
  GlXMLTools::getXML(dataNode,"position3",_positions[3]);
  GlXMLTools::getXML(dataNode,"color0",_colors[0]);
  GlXMLTools::getXML(dataNode,"color1",_colors[1]);
  GlXMLTools::getXML(dataNode,"color2",_colors[2]);
  GlXMLTools::getXML(dataNode,"color3",_colors[3]);


  }
  //============================================================
  void GlQuad::setWithXML(xmlNodePtr rootNode) {
    xmlNodePtr dataNode=NULL;

    GlXMLTools::getDataNode(rootNode,dataNode);

    // Parse Data
    if(dataNode) {
      GlXMLTools::setWithXML(dataNode,"position0",_positions[0]);
      GlXMLTools::setWithXML(dataNode,"position1",_positions[1]);
      GlXMLTools::setWithXML(dataNode,"position2",_positions[2]);
      GlXMLTools::setWithXML(dataNode,"position3",_positions[3]);
      GlXMLTools::setWithXML(dataNode,"color0",_colors[0]);
      GlXMLTools::setWithXML(dataNode,"color1",_colors[1]);
      GlXMLTools::setWithXML(dataNode,"color2",_colors[2]);
      GlXMLTools::setWithXML(dataNode,"color3",_colors[3]);

      for(int i=0; i < 4; ++i) {
        boundingBox.expand(_positions[i]);
      }
    }
  }

}
