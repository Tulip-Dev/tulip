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

#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/GlTriangle.h>
#include <tulip/Glyph.h>
#include <tulip/TulipViewSettings.h>

using namespace tlp;
using namespace std;
class GlArrow2DEdgeExtremity: public EdgeExtremityGlyph {
public:
  GLYPHINFORMATION("2D - Arrow","Jonathan Dubois","09/04/09","Edge Extremity with 2D arrow","1.0",EdgeExtremityShape::Arrow)
  GlArrow2DEdgeExtremity(tlp::PluginContext *gc);
  virtual ~GlArrow2DEdgeExtremity();
  void draw(edge e, node n, const Color& glyphColor, const Color &borderColor, float lod);
protected :
  static GlTriangle *triangle;
};

PLUGIN(GlArrow2DEdgeExtremity)

GlTriangle* GlArrow2DEdgeExtremity::triangle=0;

GlArrow2DEdgeExtremity::GlArrow2DEdgeExtremity(tlp::PluginContext *gc) :
  EdgeExtremityGlyph(gc) {
  if(!triangle) {
    triangle=new GlTriangle(Coord(0,0,0),Size(0.5,0.5,0.5));
    triangle->setLightingMode(false);
    triangle->setStartAngle(0);
  }
}

GlArrow2DEdgeExtremity::~GlArrow2DEdgeExtremity() {
}

void GlArrow2DEdgeExtremity::draw(edge e, node, const Color& glyphColor, const Color &borderColor,
                                  float lod) {

  double width = edgeExtGlGraphInputData->getElementBorderWidth()->getEdgeValue(e);

  triangle->setFillColor(glyphColor);
  triangle->setOutlineSize(width);
  triangle->setOutlineColor(borderColor);
  triangle->draw(lod,NULL);
}
