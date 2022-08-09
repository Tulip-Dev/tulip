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
#include <tulip/Glyph.h>
#include <tulip/GlRect.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

namespace tlp {

/** \addtogroup glyph */

/// A 2D glyph.
/**
 * This glyph draws a textured square using the "viewTexture"
 * node property value. If this property has no value, the square is
 *  then colored using the "viewColor" node property value.
 * It is unsensitive to any axis rotation and so always remains displayed
 * in the same position.
 */
class Billboard : public NoShaderGlyph {
public:
  GLYPHINFORMATION("2D - Billboard", "Gerald Gainant", "08/03/2004", "Textured billboard", "1.0",
                   NodeShape::Billboard)
  Billboard(const tlp::PluginContext *context = nullptr);
  ~Billboard() override;
  void draw(node n, float lod) override;
  Coord getAnchor(const Coord &vector) const override;
};

PLUGIN(Billboard)

//===================================================================================
Billboard::Billboard(const tlp::PluginContext *context) : NoShaderGlyph(context) {}
//========================================================
Billboard::~Billboard() {}
//========================================================
void Billboard::draw(node n, float) {
  static GlRect rect(Coord(0, 0, 0), 1., 1., Color(0, 0, 0, 255), Color(0, 0, 0, 255));

  rect.setFillColor(glGraphInputData->getElementColor()->getNodeValue(n));

  string texFile = glGraphInputData->getElementTexture()->getNodeValue(n);

  if (!texFile.empty()) {
    string texturePath = glGraphInputData->parameters->getTexturePath();
    rect.setTextureName(texturePath + texFile);
  } else {
    rect.setTextureName("");
  }

  double borderWidth = glGraphInputData->getElementBorderWidth()->getNodeValue(n);

  if (borderWidth > 0) {
    rect.setOutlineMode(true);
    rect.setOutlineColor(glGraphInputData->getElementBorderColor()->getNodeValue(n));
    rect.setOutlineSize(borderWidth);
  } else {
    rect.setOutlineMode(false);
  }

  Size sz(1, 1, 1);

  if (glGraphInputData->getElementSize())
    sz = glGraphInputData->getElementSize()->getNodeValue(n);

  // draw rect in the screen plane
  Glyph::drawRectInScreenPlane(rect, sz, false);
}
//========================================================
Coord Billboard::getAnchor(const Coord &vector) const {
  Coord v(vector);
  float x, y, z, fmax;
  v.get(x, y, z);
  fmax = std::max(fabsf(x), fabsf(y)); // fmax = fabsf(x) >? fabsf(y);

  if (fmax > 0.0f)
    return v * (0.5f / fmax);
  else
    return v;
}
//========================================================

} // end of namespace tlp
