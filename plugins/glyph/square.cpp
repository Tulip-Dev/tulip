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
#include <tulip/GlRect.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

namespace tlp {

static GlRect *rect = NULL;

static void drawGlyph(const Color& glyphColor, const string& texture,
                      const string& texturePath, double borderWidth,
                      const Color& borderColor, float lod) {
  rect->setFillColor(glyphColor);
  rect->setTextureName(texturePath+texture);

  if (borderWidth > 0) {
    rect->setOutlineMode(true);
    rect->setOutlineColor(borderColor);
    rect->setOutlineSize(borderWidth);
  }
  else {
    rect->setOutlineMode(false);
  }

  rect->draw(lod,NULL);

}


/** \addtogroup glyph */

/// A 2D glyph.
/**
 * This glyph draws a textured square using the "viewTexture" node
 * property value. If this property has no value, the square
 * is then colored using the "viewColor" node property value.
 */
class Square: public Glyph {
public:
  GLYPHINFORMATION("2D - Square", "David Auber", "09/07/2002", "Textured square", "1.0", NodeShape::Square)
  Square(const tlp::PluginContext *context = NULL);
  virtual ~Square();
  virtual void draw(node n, float lod);
  virtual Coord getAnchor(const Coord &vector) const;
};
PLUGIN(Square)
Square::Square(const tlp::PluginContext* context) :
  Glyph(context) {
  if(!rect)
    rect = new GlRect(Coord(0,0,0), 1., 1., Color(0,0,0,255),Color(0,0,0,255));
}
Square::~Square() {
}
void Square::draw(node n, float lod) {
  drawGlyph(glGraphInputData->getElementColor()->getNodeValue(n),
            glGraphInputData->getElementTexture()->getNodeValue(n),
            glGraphInputData->parameters->getTexturePath(),
            glGraphInputData->getElementBorderWidth()->getNodeValue(n),
            glGraphInputData->getElementBorderColor()->getNodeValue(n), lod);

}
Coord Square::getAnchor(const Coord &vector) const {
  Coord v(vector);
  float x, y, z, fmax;
  v.get(x, y, z);
  v.setZ(0.0f);
  fmax = std::max(fabsf(x), fabsf(y));

  if (fmax > 0.0f)
    return v * (0.5f / fmax);
  else
    return v;
}

class EESquare: public EdgeExtremityGlyph {
public:
  GLYPHINFORMATION("2D - Square extremity", "David Auber", "09/07/2002", "Textured square for edge extremities", "1.0", EdgeExtremityShape::Square)
  EESquare(const tlp::PluginContext* context): EdgeExtremityGlyph(context) {
    if(!rect)
      rect = new GlRect(Coord(0,0,0), 1., 1.,Color(0,0,0,255),Color(0,0,0,255));
  }

  void draw(edge e, node, const Color& glyphColor, const Color &borderColor, float lod) {
    glDisable(GL_LIGHTING);
    drawGlyph(glyphColor,
              edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e),
              edgeExtGlGraphInputData->parameters->getTexturePath(),
              edgeExtGlGraphInputData->getElementBorderWidth()->getEdgeValue(e),
              borderColor, lod);
  }
};
PLUGIN(EESquare)

} // end of namespace tlp
