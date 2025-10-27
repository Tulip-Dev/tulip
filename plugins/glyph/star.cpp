/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <tulip/GlStar.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

namespace tlp {

static void drawStar(const Color &fillColor, const Color &borderColor, float borderWidth,
                     const std::string &textureName, float lod) {
  static GlStar star(Coord(0, 0, 0), Size(.5, .5, 0), 5);
  star.setFillColor(fillColor);

  if (borderWidth > 0) {
    star.setOutlineMode(true);
    star.setOutlineColor(borderColor);
    star.setOutlineSize(borderWidth);
  } else {
    star.setOutlineMode(false);
  }

  star.setTextureName(textureName);
  star.draw(lod, nullptr);
}

/** \addtogroup glyph */

/// A 2D glyph
/**
 * This glyph draws a textured star using the "viewTexture"
 * node property value. If this property has no value, the star
 * is then colored using the "viewColor" node property value.
 */
class Star : public Glyph {
public:
  GLYPHINFORMATION("2D - Star", "David Auber", "09/07/2002", "Textured Star", "1.0",
                   NodeShape::Star)
  Star(const tlp::PluginContext *context = nullptr);
  ~Star() override;
  void getIncludeBoundingBox(BoundingBox &boundingBox, node) override;
  void draw(node n, float lod) override;
};
PLUGIN(Star)
Star::Star(const tlp::PluginContext *context) : Glyph(context) {}
Star::~Star() {}
void Star::getIncludeBoundingBox(BoundingBox &boundingBox, node) {
  boundingBox[0] = Coord(-0.3f, -0.35f, 0);
  boundingBox[1] = Coord(0.3f, 0.35f, 0);
}
void Star::draw(node n, float lod) {
  string textureName = glGraphInputData->getElementTexture()->getNodeValue(n);

  if (!textureName.empty())
    textureName = glGraphInputData->parameters->getTexturePath() + textureName;

  drawStar(glGraphInputData->getElementColor()->getNodeValue(n),
           glGraphInputData->getElementBorderColor()->getNodeValue(n),
           glGraphInputData->getElementBorderWidth()->getNodeValue(n), textureName, lod);
}

class EEStar : public EdgeExtremityGlyph {
public:
  GLYPHINFORMATION("2D - Star extremity", "David Auber", "09/07/2002",
                   "Textured Star for edge extremities", "1.0", EdgeExtremityShape::Star)

  EEStar(const tlp::PluginContext *context) : EdgeExtremityGlyph(context) {}

  void draw(edge e, node, const Color &glyphColor, const Color &borderColor, float lod) override {
    string textureName = edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e);

    if (!textureName.empty())
      textureName = edgeExtGlGraphInputData->parameters->getTexturePath() + textureName;

    drawStar(glyphColor, borderColor,
             edgeExtGlGraphInputData->getElementBorderWidth()->getEdgeValue(e), textureName, lod);
  }
};
PLUGIN(EEStar)

} // end of namespace tlp
