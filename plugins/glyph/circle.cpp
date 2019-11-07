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
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlCircle.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

namespace tlp {

static void drawCircle(const Color &fillColor, const Color &borderColor, float borderWidth,
                       const std::string &textureName, float lod, bool mode) {
  static GlCircle circle(Coord(0, 0, 0), 0.5, Color(0, 0, 0, 255), Color(0, 0, 0, 255), true, true,
                         0., 30);
  circle.setFillColor(fillColor);
  circle.setLightingMode(mode);

  if (borderWidth > 0) {
    circle.setOutlineMode(true);
    circle.setOutlineColor(borderColor);
    circle.setOutlineSize(borderWidth);
  } else {
    circle.setOutlineMode(false);
  }

  circle.setTextureName(textureName);
  circle.draw(lod, nullptr);
}

/** \addtogroup glyph */

/// A 2D glyph.
/**
 * This glyph draws a textured disc using the "viewTexture" node
 * property value. If this property has no value, the disc is then colored
 * using the "viewColor" node property value.
 */
class Circle : public Glyph {
public:
  GLYPHINFORMATION("2D - Circle", "David Auber", "09/07/2002", "Textured Circle", "1.1",
                   NodeShape::Circle)
  Circle(const tlp::PluginContext *context = nullptr);
  ~Circle() override;
  void getIncludeBoundingBox(BoundingBox &boundingBox, node) override;
  void draw(node n, float lod) override;
};
PLUGIN(Circle)
Circle::Circle(const tlp::PluginContext *context) : Glyph(context) {}
Circle::~Circle() {}
void Circle::getIncludeBoundingBox(BoundingBox &boundingBox, node) {
  boundingBox[0] = Coord(-0.35f, -0.35f, 0);
  boundingBox[1] = Coord(0.35f, 0.35f, 0);
}
void Circle::draw(node n, float lod) {
  string textureName = glGraphInputData->getElementTexture()->getNodeValue(n);

  if (!textureName.empty())
    textureName = glGraphInputData->parameters->getTexturePath() + textureName;

  drawCircle(Glyph::glGraphInputData->getElementColor()->getNodeValue(n),
             Glyph::glGraphInputData->getElementBorderColor()->getNodeValue(n),
             Glyph::glGraphInputData->getElementBorderWidth()->getNodeValue(n), textureName, lod,
             true);
}

class EECircle : public EdgeExtremityGlyph {
public:
  GLYPHINFORMATION("2D - Circle extremity", "David Auber", "09/07/2002",
                   "Textured Circle for edge extremities", "1.1", EdgeExtremityShape::Circle)
  EECircle(const tlp::PluginContext *context) : EdgeExtremityGlyph(context) {}
  void draw(edge e, node, const Color &glyphColor, const Color &borderColor, float lod) override {
    string textureName = edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e);

    if (!textureName.empty())
      textureName = edgeExtGlGraphInputData->parameters->getTexturePath() + textureName;

    drawCircle(glyphColor, borderColor,
               edgeExtGlGraphInputData->getElementBorderWidth()->getEdgeValue(e), textureName, lod,
               false);
  }
};

PLUGIN(EECircle)

} // end of namespace tlp
