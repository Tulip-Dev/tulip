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
#include <tulip/Coord.h>
#include <tulip/Size.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlHexagon.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

namespace tlp {

static void drawHexagon(const Color &fillColor, const Color &borderColor, float borderWidth,
                        const std::string &textureName, float lod, bool mode) {
  static GlHexagon hexagon(Coord(0, 0, 0), Size(.5, .5, 0));

  hexagon.setLightingMode(mode);
  hexagon.setFillColor(fillColor);

  if (borderWidth > 0) {
    hexagon.setOutlineMode(true);
    hexagon.setOutlineColor(borderColor);
    hexagon.setOutlineSize(borderWidth);
  } else {
    hexagon.setOutlineMode(false);
  }

  hexagon.setTextureName(textureName);
  hexagon.draw(lod, nullptr);
}

/** \addtogroup glyph */

/// A 2D glyph
/**
 * This glyph draws a textured hexagon using the "viewTexture"
 * node property value. If this property has no value, the hexagon
 * is then colored using the "viewColor" node property value.
 */
class Hexagon : public Glyph {
public:
  GLYPHINFORMATION("2D - Hexagon", "David Auber", "09/07/2002", "Textured Hexagon", "1.0",
                   NodeShape::Hexagon)
  Hexagon(const tlp::PluginContext *context = nullptr);
  ~Hexagon() override;
  void getIncludeBoundingBox(BoundingBox &boundingBox, node) override;
  void draw(node n, float lod) override;
};
PLUGIN(Hexagon)
Hexagon::Hexagon(const tlp::PluginContext *context) : Glyph(context) {}
Hexagon::~Hexagon() {}
void Hexagon::getIncludeBoundingBox(BoundingBox &boundingBox, node) {
  boundingBox[0] = Coord(-0.35f, -0.35f, 0);
  boundingBox[1] = Coord(0.35f, 0.35f, 0);
}
void Hexagon::draw(node n, float lod) {
  string textureName = glGraphInputData->getElementTexture()->getNodeValue(n);

  if (!textureName.empty())
    textureName = glGraphInputData->parameters->getTexturePath() + textureName;

  drawHexagon(glGraphInputData->getElementColor()->getNodeValue(n),
              glGraphInputData->getElementBorderColor()->getNodeValue(n),
              glGraphInputData->getElementBorderWidth()->getNodeValue(n), textureName, lod, true);
}

class EEHexagon : public EdgeExtremityGlyph {
public:
  GLYPHINFORMATION("2D - Hexagon extremity", "David Auber", "09/07/2002",
                   "Textured Hexagon for edge extremities", "1.0", EdgeExtremityShape::Hexagon)

  EEHexagon(const tlp::PluginContext *context) : EdgeExtremityGlyph(context) {}

  void draw(edge e, node, const Color &glyphColor, const Color &borderColor, float lod) override {
    string textureName = edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e);

    if (!textureName.empty())
      textureName = edgeExtGlGraphInputData->parameters->getTexturePath() + textureName;

    drawHexagon(glyphColor, borderColor,
                edgeExtGlGraphInputData->getElementBorderWidth()->getEdgeValue(e), textureName, lod,
                false);
  }
};
PLUGIN(EEHexagon)

} // end of namespace tlp
