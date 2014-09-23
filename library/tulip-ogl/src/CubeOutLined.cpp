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
#include <tulip/Glyph.h>
#include <tulip/GlBox.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/TulipViewSettings.h>

using namespace std;

namespace tlp {

/// A 3D glyph.
/** This glyph draws a textured cube using the "viewTexture" node
 * property value. If this property has no value, the cube is then colored
 * using the "viewColor" node property value. The "viewBorderColor"
 * node property value is used to draw its edges.
 */
class CubeOutLined : public Glyph {
public:
  GLYPHINFORMATION("3D - Cube OutLined", "David Auber", "09/07/2002", "Textured cubeOutLined", "1.0", NodeShape::CubeOutlined)
  CubeOutLined(tlp::PluginContext* context);
  virtual ~CubeOutLined();
  virtual void draw(node n,float lod);
  virtual Coord getAnchor(const Coord & vector) const;
protected:
  static GlBox* box;
};
}
tlp::GlBox* tlp::CubeOutLined::box=NULL;

namespace tlp {
PLUGIN(CubeOutLined)


//===================================================================================
CubeOutLined::CubeOutLined(tlp::PluginContext* context): Glyph(context) {
  if(!box)
    box = new GlBox(Coord(0,0,0),Size(1,1,1),Color(0,0,0,255),Color(0,0,0,255));
}

CubeOutLined::~CubeOutLined() {
}

void CubeOutLined::draw(node n, float lod) {

  const string& texFile = glGraphInputData->getElementTexture()->getNodeValue(n);

  if (texFile.size() != 0) {
    const string& texturePath=glGraphInputData->parameters->getTexturePath();
    box->setTextureName(texturePath+texFile);
  }
  else
    box->setTextureName("");

  box->setFillColor(glGraphInputData->getElementColor()->getNodeValue(n));
  box->setOutlineColor(glGraphInputData->getElementBorderColor()->getNodeValue(n));
  double lineWidth=glGraphInputData->getElementBorderWidth()->getNodeValue(n);

  if(lineWidth < 1e-6)
    lineWidth=1e-6;

  box->setOutlineSize(lineWidth);

  box->draw(lod,NULL);
}


Coord CubeOutLined::getAnchor(const Coord & vector ) const {
  float x,y,z, fmax;
  vector.get(x,y,z);
  fmax = std::max(std::max(fabsf(x), fabsf(y)), fabsf(z));

  if( fmax > 0.0f )
    return vector * (0.5f/fmax);
  else
    return vector;
}
}
