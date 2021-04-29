/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <tulip/Color.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlRegularPolygon.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

namespace tlp {

static void drawDiamond(const Color &fillColor, const Color &borderColor, float borderWidth,
                        const std::string &textureName, float lod, bool mode) {
  static GlRegularPolygon diamond(Coord(0, 0, 0), Size(.5, .5, 0), 4);
  diamond.setLightingMode(mode);
  diamond.setFillColor(fillColor);

  if (borderWidth > 0) {
    diamond.setOutlineMode(true);
    diamond.setOutlineColor(borderColor);
    diamond.setOutlineSize(borderWidth);
  } else {
    diamond.setOutlineMode(false);
  }

  diamond.setTextureName(textureName);
  diamond.draw(lod, nullptr);
}

/** \addtogroup glyph */

/// A 2D glyph
/**
 * This glyph draws a textured diamond using the "viewTexture"
 * node property value. If this property has no value, the diamond
 * is then colored using the "viewColor" node property value.
 */
class Diamond : public Glyph {
public:
  GLYPHINFORMATION("2D - Diamond", "Patrick Mary", "23/06/2011", "Textured Diamond", "1.0",
                   NodeShape::Diamond)
  Diamond(const tlp::PluginContext *context = nullptr);
  ~Diamond() override;
  void getIncludeBoundingBox(BoundingBox &boundingBox, node) override;
  void draw(node n, float lod) override;
  Coord getAnchor(const Coord &vector) const override;
};
PLUGIN(Diamond)
Diamond::Diamond(const tlp::PluginContext *context) : Glyph(context) {}
Diamond::~Diamond() {}
void Diamond::getIncludeBoundingBox(BoundingBox &boundingBox, node) {
  boundingBox[0] = Coord(-0.35f, -0.35f, 0);
  boundingBox[1] = Coord(0.35f, 0.35f, 0);
}
void Diamond::draw(node n, float lod) {
  string textureName = glGraphInputData->getElementTexture()->getNodeValue(n);

  if (!textureName.empty())
    textureName = glGraphInputData->parameters->getTexturePath() + textureName;

  drawDiamond(glGraphInputData->getElementColor()->getNodeValue(n),
              glGraphInputData->getElementBorderColor()->getNodeValue(n),
              glGraphInputData->getElementBorderWidth()->getNodeValue(n), textureName, lod, true);
}
Coord Diamond::getAnchor(const Coord &vector) const {
  Coord v(vector);
  float x, y, z;
  v.get(x, y, z);
  // initialize anchor as top corner
  Coord anchor(0, 0.5, 0);
  float distMin = x * x + ((y - 0.5) * (y - 0.5));
  // check with the right corner
  float dist = ((x - 0.5) * (x - 0.5)) + y * y;

  if (distMin > dist) {
    distMin = dist;
    anchor = Coord(0.5, 0, 0);
  }

  // check with the bottom corner
  dist = x * x + ((y + 0.5) * (y + 0.5));

  if (distMin > dist) {
    distMin = dist;
    anchor = Coord(0, -0.5, 0);
  }

  // check with left corner
  if (distMin > ((x + 0.5) * (x + 0.5)) + y * y)
    return Coord(-0.5, 0, 0);

  return anchor;
}

class EEDiamond : public EdgeExtremityGlyph {
public:
  GLYPHINFORMATION("2D - Diamond extremity", "Patrick Mary", "23/06/2011",
                   "Textured Diamond for edge extremities", "1.0", EdgeExtremityShape::Diamond)

  EEDiamond(const tlp::PluginContext *context) : EdgeExtremityGlyph(context) {}

  void draw(edge e, node, const Color &glyphColor, const Color &borderColor, float lod) override {
    string textureName = edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e);

    if (!textureName.empty())
      textureName = edgeExtGlGraphInputData->parameters->getTexturePath() + textureName;

    drawDiamond(glyphColor, borderColor,
                edgeExtGlGraphInputData->getElementBorderWidth()->getEdgeValue(e), textureName, lod,
                false);
  }
};

PLUGIN(EEDiamond)

} // end of namespace tlp
