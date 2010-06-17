/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include <tulip/GlHexagon.h>

#include <tulip/Graph.h>

using namespace std;
using namespace tlp;

/** \addtogroup glyph */
/*@{*/
/// A 2D glyph
/**
 * This glyph draws a textured hexagone using the "viewTexture"
 * node property value. If this property has no value, the hexagone
 * is then colored using the "viewColor" node property value.
 */
class Hexagone: public Glyph, public EdgeExtremityGlyphFrom2DGlyph {
public:
	Hexagone(GlyphContext *gc = NULL);
	Hexagone(EdgeExtremityGlyphContext *gc);
	virtual ~Hexagone();
	virtual void getIncludeBoundingBox(BoundingBox &boundingBox);
	virtual void draw(node n, float lod);
	virtual void draw(edge e, node n, const Color& glyphColor, const Color &borderColor, float lod);
  virtual void draw(const Color &fillColor,const Color &borderColor,float borderWidth,const std::string &textureName, float lod);
protected :

  static GlHexagon *hexagon;
};

#ifdef _WIN32
#ifdef DLL_EXPORT
GlHexagon* Hexagone::hexagon=0;
#endif
#else
GlHexagon* Hexagone::hexagon=0;
#endif

//=====================================================
GLYPHPLUGIN(Hexagone, "2D - Hexagone", "David Auber", "09/07/2002", "Textured Hexagone", "1.0", 13)
;
EEGLYPHPLUGIN(Hexagone,"2D - Hexagone", "David Auber", "09/07/2002", "Textured Hexagone", "1.0", 14)
;
//===================================================================================
Hexagone::Hexagone(GlyphContext *gc) :
	Glyph(gc), EdgeExtremityGlyphFrom2DGlyph(NULL) {
  if(!hexagon)
    hexagon=new GlHexagon(Coord(0,0,0),Size(.5,.5,0));
}
Hexagone::Hexagone(EdgeExtremityGlyphContext *gc) :
	Glyph(NULL), EdgeExtremityGlyphFrom2DGlyph(gc) {
  if(!hexagon)
    hexagon=new GlHexagon(Coord(0,0,0),Size(.5,.5,0));
}
//=====================================================
Hexagone::~Hexagone() {
}
//=====================================================
void Hexagone::getIncludeBoundingBox(BoundingBox &boundingBox) {
        boundingBox[0] = Coord(0.15, 0.15, 0);
        boundingBox[1] = Coord(0.85, 0.85, 0);
}
//=====================================================
void Hexagone::draw(node n, float lod) {
  string textureName=glGraphInputData->elementTexture->getNodeValue(n);
  if(textureName!="")
    textureName=glGraphInputData->parameters->getTexturePath()+textureName;

  draw(glGraphInputData->elementColor->getNodeValue(n),
       glGraphInputData->elementBorderColor->getNodeValue(n),
       glGraphInputData->elementBorderWidth->getNodeValue(n),
       textureName,
       lod);
}

void Hexagone::draw(edge e,
                    node,
                    const Color & glyphColor,
                    const Color &borderColor,
                    float lod) {
  string textureName=edgeExtGlGraphInputData->elementTexture->getEdgeValue(e);
  if(textureName!="")
    textureName=edgeExtGlGraphInputData->parameters->getTexturePath()+textureName;

  draw(glyphColor,
       borderColor,
       edgeExtGlGraphInputData->elementBorderWidth->getEdgeValue(e),
       textureName,
       lod);
}
//=====================================================
void Hexagone::draw(const Color &fillColor,
                    const Color &borderColor,
                    float borderWidth,
                    const std::string &textureName,
                    float lod){
  if(borderWidth<1e-6)
    borderWidth=1e-6;

  hexagon->setFillColor(fillColor);
  hexagon->setOutlineColor(borderColor);
  hexagon->setOutlineSize(borderWidth);
  hexagon->setTextureName(textureName);
  hexagon->draw(lod,NULL);
}
