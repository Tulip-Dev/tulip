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
#include <tulip/GlGraphInputData.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlTriangle.h>
#include <tulip/Glyph.h>
#include <tulip/Size.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

namespace tlp {

class Triangle : public Glyph {
public:
  GLYPHINFORMATION("2D - Triangle", "David Auber", "09/07/2002",
                   "Textured Triangle", "1.0", NodeShape::Triangle)
  Triangle(const tlp::PluginContext *context = nullptr);
  ~Triangle() override;
  void getIncludeBoundingBox(BoundingBox &boundingBox, node) override;
  void draw(node n, float lod) override;
};

//=====================================================
PLUGIN(Triangle)
//===================================================================================
Triangle::Triangle(const tlp::PluginContext *context) : Glyph(context) {}
//=====================================================
Triangle::~Triangle() {}
//=====================================================
void Triangle::getIncludeBoundingBox(BoundingBox &boundingBox, node) {
  boundingBox[0] = Coord(-0.25, -0.5, 0);
  boundingBox[1] = Coord(0.25, 0, 0);
}
//=====================================================
void Triangle::draw(node n, float lod) {
  GlTriangle triangle(Coord(0, 0, 0), Size(0.5, 0.5, 0));

  triangle.setFillColor(glGraphInputData->getElementColor()->getNodeValue(n));

  string texFile = glGraphInputData->getElementTexture()->getNodeValue(n);

  if (!texFile.empty()) {
    string texturePath = glGraphInputData->parameters->getTexturePath();
    triangle.setTextureName(texturePath + texFile);
  } else {
    triangle.setTextureName("");
  }

  double lineWidth = glGraphInputData->getElementBorderWidth()->getNodeValue(n);

  if (lineWidth > 0) {
    triangle.setOutlineMode(true);
    triangle.setOutlineColor(
        glGraphInputData->getElementBorderColor()->getNodeValue(n));
    triangle.setOutlineSize(lineWidth);
  } else {
    triangle.setOutlineMode(false);
  }

  triangle.draw(lod, nullptr);
}
//=====================================================

} // end of namespace tlp
