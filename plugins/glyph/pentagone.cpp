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
#include <tulip/GlPentagon.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/Graph.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

namespace tlp {

static GlPentagon *pentagon = NULL;
static void drawPentagon(const Color &fillColor,const Color &borderColor,float borderWidth,const std::string &textureName, float lod) {

  pentagon->setFillColor(fillColor);

  if (borderWidth > 0) {
    pentagon->setOutlineMode(true);
    pentagon->setOutlineColor(borderColor);
    pentagon->setOutlineSize(borderWidth);
  }
  else {
    pentagon->setOutlineMode(false);
  }

  pentagon->setTextureName(textureName);
  pentagon->draw(lod,NULL);
}

/** \addtogroup glyph */

/// A 2D glyph
/**
 * This glyph draws a textured pentagon using the "viewTexture"
 * node property value. If this property has no value, the pentagon
 * is then colored using the "viewColor" node property value.
 */
class Pentagon: public Glyph {
public:
  GLYPHINFORMATION("2D - Pentagon", "David Auber", "09/07/2002", "Textured Pentagon", "1.0", NodeShape::Pentagon)
  Pentagon(const tlp::PluginContext *context = NULL);
  virtual ~Pentagon();
  virtual void getIncludeBoundingBox(BoundingBox &boundingBox,node);
  virtual void draw(node n, float lod);
};
PLUGIN(Pentagon)
Pentagon::Pentagon(const tlp::PluginContext* context) :
  Glyph(context) {
  if(!pentagon)
    pentagon=new GlPentagon(Coord(0,0,0),Size(.5,.5,0));
}
Pentagon::~Pentagon() {
}
void Pentagon::getIncludeBoundingBox(BoundingBox &boundingBox,node) {
  boundingBox[0] = Coord(-0.3f, -0.35f, 0);
  boundingBox[1] = Coord(0.3f, 0.35f, 0);
}
void Pentagon::draw(node n, float lod) {
  pentagon->setLightingMode(true);
  string textureName=glGraphInputData->getElementTexture()->getNodeValue(n);

  if(textureName!="")
    textureName=glGraphInputData->parameters->getTexturePath()+textureName;

  drawPentagon(glGraphInputData->getElementColor()->getNodeValue(n),
               glGraphInputData->getElementBorderColor()->getNodeValue(n),
               glGraphInputData->getElementBorderWidth()->getNodeValue(n),
               textureName,
               lod);
}

class EEPentagon: public EdgeExtremityGlyph {
public:
  GLYPHINFORMATION("2D - Pentagon extremity", "David Auber", "09/07/2002", "Textured Pentagon for edge extremities", "1.0", EdgeExtremityShape::Pentagon)

  EEPentagon(const tlp::PluginContext* context): EdgeExtremityGlyph(context) {
    if(!pentagon)
      pentagon=new GlPentagon(Coord(0,0,0),Size(.5,.5,0));
  }

  void draw(edge e, node, const Color& glyphColor, const Color &borderColor, float lod) {
    pentagon->setLightingMode(false);
    string textureName=edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e);

    if(textureName!="")
      textureName=edgeExtGlGraphInputData->parameters->getTexturePath()+textureName;

    drawPentagon(glyphColor,
                 borderColor,
                 edgeExtGlGraphInputData->getElementBorderWidth()->getEdgeValue(e),
                 textureName,
                 lod);
  }
};
PLUGIN(EEPentagon)

} // end of namespace tlp
