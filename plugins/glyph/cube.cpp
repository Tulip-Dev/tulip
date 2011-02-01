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
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>

#include <tulip/Graph.h>
#include <tulip/GlBox.h>

using namespace std;
using namespace tlp;

/** \addtogroup glyph */
/*@{*/
/// A 3D glyph.
/** This glyph draws a textured cube using the "viewTexture" node
 * property value. If this property has no value, the cube is then colored
 * using the "viewColor" node property value.
 */
class Cube: public Glyph, public EdgeExtremityGlyphFrom3DGlyph {
public:
	Cube(GlyphContext *gc = NULL);
	Cube(EdgeExtremityGlyphContext *gc = NULL);
	virtual ~Cube();
	virtual void draw(node n, float lod);
	virtual Coord getAnchor(const Coord & vector) const;
	virtual void draw(edge e, node n, const Color& glyphColor,const Color &borderColor, float lod);
  virtual void draw(const Color &fillColor,const std::string &textureName, float lod);

protected:

  static GlBox* box;

};

GlBox* Cube::box=0;

GLYPHPLUGIN(Cube, "3D - Cube", "Bertrand Mathieu", "09/07/2002", "Textured cube", "1.0" , 0)
EEGLYPHPLUGIN(Cube,"3D - Cube","Bertrand Mathieu", "09/07/2002", "Textured cube", "1.0" , 0)
//===================================================================================
Cube::Cube(GlyphContext *gc) :
	Glyph(gc), EdgeExtremityGlyphFrom3DGlyph(NULL) {
  if(!box)
    box = new GlBox(Coord(0,0,0),Size(1,1,1),Color(0,0,0,255),Color(0,0,0,255));
}

Cube::Cube(EdgeExtremityGlyphContext *gc) :
	Glyph(NULL), EdgeExtremityGlyphFrom3DGlyph(gc) {
  if(!box)
    box = new GlBox(Coord(0,0,0),Size(1,1,1),Color(0,0,0,255),Color(0,0,0,255));
}
//=======================================================
Cube::~Cube() {
}
//=======================================================
void Cube::draw(node n, float lod) {
  draw(glGraphInputData->getElementColor()->getNodeValue(n),
       glGraphInputData->getElementTexture()->getNodeValue(n),
       lod);
}

void Cube::draw(edge, node n, const Color& glyphColor,const Color&, float lod) {
  glEnable(GL_LIGHTING);
  draw(glyphColor,
       edgeExtGlGraphInputData->getElementTexture()->getNodeValue(n),
       lod);
}

void Cube::draw(const Color &fillColor,const std::string &textureName, float lod){
  if (textureName.size() != 0){
    const string& texturePath=glGraphInputData->parameters->getTexturePath();
    box->setTextureName(texturePath+textureName);
  }else
    box->setTextureName("");

  box->setFillColor(fillColor);
  box->setOutlineColor(Color(0,0,0,0));

  box->draw(lod,NULL);
}


//=======================================================
Coord Cube::getAnchor(const Coord & vector) const {
	float x, y, z, fmax;
	vector.get(x, y, z);
	fmax = std::max(std::max(fabsf(x), fabsf(y)), fabsf(z));
	if (fmax > 0.0f)
		return vector * (0.5f / fmax);
	else
		return vector;
}
/*@}*/
