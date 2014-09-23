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
#include <cmath>

#include <string>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/Graph.h>
#include <tulip/GlCircle.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

namespace tlp {

static GlCircle *circle = NULL;

static void drawCircle(const Color &fillColor,const Color &borderColor,float borderWidth,const std::string &textureName, float lod) {
  circle->setFillColor(fillColor);

  if (borderWidth > 0) {
    circle->setOutlineMode(true);
    circle->setOutlineColor(borderColor);
    circle->setOutlineSize(borderWidth);
  }
  else {
    circle->setOutlineMode(false);
  }

  circle->setTextureName(textureName);
  circle->draw(lod,NULL);
}

/** \addtogroup glyph */

/// A 2D glyph.
/**
 * This glyph draws a textured disc using the "viewTexture" node
 * property value. If this property has no value, the disc is then colored
 * using the "viewColor" node property value.
 */
class Circle: public Glyph {
public:
  GLYPHINFORMATION("2D - Circle", "David Auber", "09/07/2002", "Textured Circle", "1.1", NodeShape::Circle)
  Circle(const tlp::PluginContext* context = NULL);
  virtual ~Circle();
  virtual void getIncludeBoundingBox(BoundingBox &boundingBox,node);
  virtual void draw(node n, float lod);
};
PLUGIN(Circle)
Circle::Circle(const tlp::PluginContext* context) :
  Glyph(context) {
  if(!circle)
    circle=new GlCircle(Coord(0,0,0),0.5,Color(0,0,0,255),Color(0,0,0,255),true,true,0.,30);
}
Circle::~Circle() {
}
void Circle::getIncludeBoundingBox(BoundingBox &boundingBox,node) {
  boundingBox[0] = Coord(-0.35f, -0.35f, 0);
  boundingBox[1] = Coord(0.35f, 0.35f, 0);
}
void Circle::draw(node n, float lod) {
  circle->setLightingMode(true);
  string textureName=glGraphInputData->getElementTexture()->getNodeValue(n);

  if(textureName!="")
    textureName=glGraphInputData->parameters->getTexturePath()+textureName;

  drawCircle(Glyph::glGraphInputData->getElementColor()->getNodeValue(n),
             Glyph::glGraphInputData->getElementBorderColor()->getNodeValue(n),
             Glyph::glGraphInputData->getElementBorderWidth()->getNodeValue(n),
             textureName,
             lod);
}

static GlCircle *eecircle = NULL;
class EECircle: public EdgeExtremityGlyph {
public:
  GLYPHINFORMATION("2D - Circle extremity", "David Auber", "09/07/2002", "Textured Circle for edge extremities", "1.1", EdgeExtremityShape::Circle)
  EECircle(const tlp::PluginContext* context): EdgeExtremityGlyph(context) {
    if(!eecircle)
      eecircle=new GlCircle(Coord(0,0,0),0.5,Color(0,0,0,255),Color(0,0,0,255),true,true,0.,30);
  }
  void draw(edge e, node, const Color& glyphColor,const Color &borderColor, float lod) {
    eecircle->setLightingMode(false);
    string textureName=edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e);

    if(textureName!="")
      textureName=edgeExtGlGraphInputData->parameters->getTexturePath()+textureName;

    drawCircle(glyphColor,
               borderColor,
               edgeExtGlGraphInputData->getElementBorderWidth()->getEdgeValue(e),
               textureName,
               lod);
  }
};

PLUGIN(EECircle)

} // end of namespace tlp
