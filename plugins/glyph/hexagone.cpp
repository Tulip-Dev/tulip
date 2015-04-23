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
#include <tulip/GlHexagon.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/Graph.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

namespace tlp {

static GlHexagon *hexagon = NULL;

static void drawHexagon(const Color &fillColor,const Color &borderColor,float borderWidth,const std::string &textureName, float lod) {

  hexagon->setFillColor(fillColor);

  if (borderWidth > 0) {
    hexagon->setOutlineMode(true);
    hexagon->setOutlineColor(borderColor);
    hexagon->setOutlineSize(borderWidth);
  }
  else {
    hexagon->setOutlineMode(false);
  }

  hexagon->setTextureName(textureName);
  hexagon->draw(lod,NULL);
}

/** \addtogroup glyph */

/// A 2D glyph
/**
 * This glyph draws a textured hexagon using the "viewTexture"
 * node property value. If this property has no value, the hexagon
 * is then colored using the "viewColor" node property value.
 */
class Hexagon: public Glyph {
public:
  GLYPHINFORMATION("2D - Hexagon", "David Auber", "09/07/2002", "Textured Hexagon", "1.0", NodeShape::Hexagon)
  Hexagon(const tlp::PluginContext *context = NULL);
  virtual ~Hexagon();
  virtual void getIncludeBoundingBox(BoundingBox &boundingBox,node);
  virtual void draw(node n, float lod);
};
PLUGIN(Hexagon)
Hexagon::Hexagon(const tlp::PluginContext* context) :
  Glyph(context) {
  if(!hexagon)
    hexagon=new GlHexagon(Coord(0,0,0),Size(.5,.5,0));
}
Hexagon::~Hexagon() {
}
void Hexagon::getIncludeBoundingBox(BoundingBox &boundingBox,node) {
  boundingBox[0] = Coord(-0.35f, -0.35f, 0);
  boundingBox[1] = Coord(0.35f, 0.35f, 0);
}
void Hexagon::draw(node n, float lod) {
  hexagon->setLightingMode(true);
  string textureName=glGraphInputData->getElementTexture()->getNodeValue(n);

  if(textureName!="")
    textureName=glGraphInputData->parameters->getTexturePath()+textureName;

  drawHexagon(glGraphInputData->getElementColor()->getNodeValue(n),
              glGraphInputData->getElementBorderColor()->getNodeValue(n),
              glGraphInputData->getElementBorderWidth()->getNodeValue(n),
              textureName,
              lod);
}

class EEHexagon: public EdgeExtremityGlyph {
public:
  GLYPHINFORMATION("2D - Hexagon extremity", "David Auber", "09/07/2002", "Textured Hexagon for edge extremities", "1.0", EdgeExtremityShape::Hexagon)

  EEHexagon(const tlp::PluginContext* context): EdgeExtremityGlyph(context) {
    if (hexagon == NULL)
      hexagon=new GlHexagon(Coord(0,0,0),Size(.5,.5,0));
  }

  void draw(edge e,
            node,
            const Color & glyphColor,
            const Color &borderColor,
            float lod) {
    hexagon->setLightingMode(false);
    string textureName=edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e);

    if(textureName!="")
      textureName=edgeExtGlGraphInputData->parameters->getTexturePath()+textureName;

    drawHexagon(glyphColor,
                borderColor,
                edgeExtGlGraphInputData->getElementBorderWidth()->getEdgeValue(e),
                textureName,
                lod);
  }
};
PLUGIN(EEHexagon)

} // end of namespace tlp
