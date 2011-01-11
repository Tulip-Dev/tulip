/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include <tulip/GlCircle.h>

using namespace std;
using namespace tlp;

/** \addtogroup glyph */
/*@{*/
/// A 2D glyph.
/**
 * This glyph draws a textured disc using the "viewTexture" node
 * property value. If this property has no value, the disc is then colored
 * using the "viewColor" node property value.
 */
class Circle: public Glyph, public EdgeExtremityGlyphFrom2DGlyph {
public:
	Circle(GlyphContext *gc = NULL);
	Circle(EdgeExtremityGlyphContext *gc = NULL);
	virtual ~Circle();
	virtual void getIncludeBoundingBox(BoundingBox &boundingBox,node);
	virtual void draw(node n, float lod);
	virtual void draw(edge e, node n, const Color& glyphColor,const Color &borderColor, float lod);
  virtual void draw(const Color &fillColor,const Color &borderColor,float borderWidth,const std::string &textureName, float lod);

protected:

  static GlCircle *circle;
};

#ifdef _WIN32
#ifdef DLL_EXPORT
GlCircle* Circle::circle=0;
#endif
#else
GlCircle* Circle::circle=0;
#endif

//=====================================================
GLYPHPLUGIN(Circle, "2D - Circle", "David Auber", "09/07/2002", "Textured Circle", "1.1", 14)
EEGLYPHPLUGIN(Circle,"2D - Circle", "David Auber", "09/07/2002", "Textured Circle", "1.1", 14)
//===================================================================================
Circle::Circle(GlyphContext *gc) :
	Glyph(gc), EdgeExtremityGlyphFrom2DGlyph(NULL) {
  if(!circle)
    circle=new GlCircle(Coord(0,0,0),0.5,Color(0,0,0,255),Color(0,0,0,255),true,true,0.,30);
}
Circle::Circle(EdgeExtremityGlyphContext  *gc) :
	Glyph(NULL), EdgeExtremityGlyphFrom2DGlyph(gc) {
  if(!circle)
    circle=new GlCircle(Coord(0,0,0),1.,Color(0,0,0,255),Color(0,0,0,255),true,true);
}
//=====================================================
Circle::~Circle() {
}
//=====================================================
void Circle::getIncludeBoundingBox(BoundingBox &boundingBox,node) {
        boundingBox[0] = Coord(-0.35, -0.35, 0);
        boundingBox[1] = Coord(0.35, 0.35, 0);
}
//=====================================================
void Circle::draw(edge e, node, const Color& glyphColor,const Color &borderColor, float lod) {
  circle->setLightingMode(false);
  string textureName=edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e);
  if(textureName!="")
    textureName=edgeExtGlGraphInputData->parameters->getTexturePath()+textureName;

  draw(glyphColor,
       borderColor,
       edgeExtGlGraphInputData->getElementBorderWidth()->getEdgeValue(e),
       textureName,
       lod);
}

//=====================================================
void Circle::draw(node n, float lod) {
  circle->setLightingMode(true);
  string textureName=glGraphInputData->getElementTexture()->getNodeValue(n);
  if(textureName!="")
    textureName=glGraphInputData->parameters->getTexturePath()+textureName;


  draw(Glyph::glGraphInputData->getElementColor()->getNodeValue(n),
       Glyph::glGraphInputData->getElementBorderColor()->getNodeValue(n),
       Glyph::glGraphInputData->getElementBorderWidth()->getNodeValue(n),
       textureName,
       lod);
}
//=====================================================
void Circle::draw(const Color &fillColor,
                  const Color &borderColor,
                  float borderWidth,
                  const std::string &textureName,
                  float lod){
  if(borderWidth<1e-6)
    borderWidth=1e-6;

  circle->setFillColor(fillColor);
  circle->setOutlineColor(borderColor);
  circle->setOutlineSize(borderWidth);
  circle->setTextureName(textureName);
  circle->draw(lod,NULL);
}

/*@}*/
