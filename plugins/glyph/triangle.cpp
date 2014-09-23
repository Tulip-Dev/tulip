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
#include <tulip/GlTriangle.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/Graph.h>
#include <tulip/GlTools.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

namespace tlp {

class Triangle : public Glyph {
public:
  GLYPHINFORMATION("2D - Triangle", "David Auber", "09/07/2002", "Textured Triangle", "1.0", NodeShape::Triangle)
  Triangle(const tlp::PluginContext *context =NULL);
  virtual ~Triangle();
  virtual void getIncludeBoundingBox(BoundingBox &boundingBox,node);
  virtual void draw(node n,float lod);

protected:

  static GlTriangle *triangle;

};

GlTriangle* Triangle::triangle=NULL;

//=====================================================
PLUGIN(Triangle)
//===================================================================================
Triangle::Triangle(const tlp::PluginContext* context): Glyph(context) {
  if(!triangle)
    triangle = new GlTriangle(Coord(0,0,0),Size(0.5,0.5,0));
}
//=====================================================
Triangle::~Triangle() {
}
//=====================================================
void Triangle::getIncludeBoundingBox(BoundingBox &boundingBox,node) {
  boundingBox[0] = Coord(-0.25,-0.5,0);
  boundingBox[1] = Coord(0.25,0,0);
}
//=====================================================
void Triangle::draw(node n,float lod) {

  triangle->setFillColor(glGraphInputData->getElementColor()->getNodeValue(n));

  string texFile = glGraphInputData->getElementTexture()->getNodeValue(n);

  if (texFile != "") {
    string texturePath=glGraphInputData->parameters->getTexturePath();
    triangle->setTextureName(texturePath+texFile);
  }
  else {
    triangle->setTextureName("");
  }

  double lineWidth=glGraphInputData->getElementBorderWidth()->getNodeValue(n);

  if (lineWidth > 0) {
    triangle->setOutlineMode(true);
    triangle->setOutlineColor(glGraphInputData->getElementBorderColor()->getNodeValue(n));
    triangle->setOutlineSize(lineWidth);
  }
  else {
    triangle->setOutlineMode(false);
  }

  triangle->draw(lod,NULL);
}
//=====================================================

} // end of namespace tlp
