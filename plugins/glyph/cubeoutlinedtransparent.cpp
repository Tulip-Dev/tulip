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

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/GlGraphRenderingParameters.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/Graph.h>
#include <tulip/GlBox.h>
#include <tulip/GlGraphInputData.h>
#include <tulip/TulipViewSettings.h>

using namespace std;
using namespace tlp;

namespace tlp {

static GlBox* box = NULL;
static void drawBox(const Color &borderColor,float borderWidth,const std::string &textureName, float lod, GlGraphInputData* glGraphInputData) {
  if (textureName.size() != 0) {
    const string& texturePath=glGraphInputData->parameters->getTexturePath();
    box->setTextureName(texturePath+textureName);
  }
  else
    box->setTextureName("");

  box->setFillColor(Color(0,0,0,0));
  box->setOutlineColor(borderColor);
  double lineWidth=borderWidth;

  if(lineWidth < 1e-6)
    lineWidth=1e-6;

  box->setOutlineSize(lineWidth);

  box->draw(lod,NULL);
}


/** \addtogroup glyph */

/// A 3D glyph.
/** This glyph draws a transparent cube using the "viewBorderColor" node
 * property value to draw its edges.
 */
class CubeOutLinedTransparent: public Glyph {
public:
  GLYPHINFORMATION("3D - Cube OutLined Transparent", "David Auber", "09/07/2002", "Textured cubeOutLined", "1.0", NodeShape::CubeOutlinedTransparent)
  CubeOutLinedTransparent(const tlp::PluginContext *context = NULL);
  virtual ~CubeOutLinedTransparent();
  virtual void draw(node n, float lod);
  virtual Coord getAnchor(const Coord & vector) const;
};
PLUGIN(CubeOutLinedTransparent)

CubeOutLinedTransparent::CubeOutLinedTransparent(const tlp::PluginContext* context) :
  Glyph(context) {
  if(!box)
    box = new GlBox(Coord(0,0,0),Size(1,1,1),Color(0,0,0,255),Color(0,0,0,255));
}

CubeOutLinedTransparent::~CubeOutLinedTransparent() {
}

void CubeOutLinedTransparent::draw(node n, float lod) {
  string textureName=glGraphInputData->getElementTexture()->getNodeValue(n);

  if(textureName!="")
    textureName=glGraphInputData->parameters->getTexturePath()+textureName;

  drawBox(glGraphInputData->getElementBorderColor()->getNodeValue(n),
          glGraphInputData->getElementBorderWidth()->getNodeValue(n),
          textureName,
          lod,glGraphInputData);
}

Coord CubeOutLinedTransparent::getAnchor(const Coord & vector) const {
  float x, y, z, fmax;
  vector.get(x, y, z);
  fmax = std::max(std::max(fabsf(x), fabsf(y)), fabsf(z));

  if (fmax > 0.0f)
    return vector * (0.5f / fmax);
  else
    return vector;
}


class EECubeOutlinedTransparent: public EdgeExtremityGlyph {
public:
  GLYPHINFORMATION("3D - Cube OutLined Transparent extremity", "David Auber", "09/07/2002", "Textured cubeOutLined for edge extremities", "1.0", EdgeExtremityShape::CubeOutlinedTransparent)

  EECubeOutlinedTransparent(const tlp::PluginContext* context): EdgeExtremityGlyph(context) {
    if (!box)
      box = new GlBox(Coord(0,0,0),Size(1,1,1),Color(0,0,0,255),Color(0,0,0,255));
  }

  void draw(edge e, node, const Color &borderColor, const Color&,
            float lod) {
    glEnable(GL_LIGHTING);
    string textureName=edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e);

    if(textureName!="")
      textureName=edgeExtGlGraphInputData->parameters->getTexturePath()+textureName;

    drawBox(borderColor,
            edgeExtGlGraphInputData->getElementBorderWidth()->getEdgeValue(e),
            textureName,
            lod,edgeExtGlGraphInputData);
    glDisable(GL_LIGHTING);
  }
};
PLUGIN(EECubeOutlinedTransparent)

} // end of namespace tlp

