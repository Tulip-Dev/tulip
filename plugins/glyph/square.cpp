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
#include <tulip/GlRect.h>

using namespace std;
using namespace tlp;

/** \addtogroup glyph */
/*@{*/
/// A 2D glyph.
/**
 * This glyph draws a textured square using the "viewTexture" node
 * property value. If this property has no value, the square
 * is then colored using the "viewColor" node property value.
 */
class Square: public Glyph, public EdgeExtremityGlyphFrom2DGlyph {
public:
	Square(GlyphContext *gc = NULL);
	Square(EdgeExtremityGlyphContext*gc = NULL);
	virtual ~Square();
	virtual void draw(node n, float lod);
	virtual Coord getAnchor(const Coord &vector) const;
	virtual void draw(edge e, node n, const Color& glyphColor, const Color &borderColor, float lod);

protected:
	inline void drawGlyph(const Color& glyphColor, const string& texture,
			const string& texturePath, double borderWidth,
			const Color& borderColor, float lod);

	static GlRect *rect;
};

GlRect* Square::rect=0;

//=====================================================
GLYPHPLUGIN(Square, "2D - Square", "David Auber", "09/07/2002", "Textured square", "1.0", 4)
EEGLYPHPLUGIN(Square,"2D - Square", "David Auber", "09/07/2002", "Textured square", "1.0", 4)
//===================================================================================
Square::Square(GlyphContext *gc) :
	Glyph(gc), EdgeExtremityGlyphFrom2DGlyph(NULL) {
	if(!rect)
		rect = new GlRect(Coord(0,0,0),Size(1,1,0),Color(0,0,0,255),Color(0,0,0,255));
}
Square::Square(EdgeExtremityGlyphContext *gc) :
	Glyph(NULL), EdgeExtremityGlyphFrom2DGlyph(gc) {
	if(!rect)
		rect = new GlRect(Coord(0,0,0),Size(1,1,0),Color(0,0,0,255),Color(0,0,0,255));
}
//=====================================================
Square::~Square() {
}
//=====================================================
void Square::draw(node n, float lod) {
	drawGlyph(glGraphInputData->getElementColor()->getNodeValue(n),
			glGraphInputData->getElementTexture()->getNodeValue(n),
			glGraphInputData->parameters->getTexturePath(),
			glGraphInputData->getElementBorderWidth()->getNodeValue(n),
			glGraphInputData->getElementBorderColor()->getNodeValue(n), lod);

}

void Square::draw(edge e, node, const Color& glyphColor, const Color &borderColor, float lod) {
  glDisable(GL_LIGHTING);
	drawGlyph(glyphColor,
			edgeExtGlGraphInputData->getElementTexture()->getEdgeValue(e),
			edgeExtGlGraphInputData->parameters->getTexturePath(),
			edgeExtGlGraphInputData->getElementBorderWidth()->getEdgeValue(e),
			borderColor, lod);
}

//=====================================================
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

void Square::drawGlyph(const Color& glyphColor, const string& texture,
		const string& texturePath, double borderWidth,
		const Color& borderColor, float lod) {
	rect->setFillColor(glyphColor);
	rect->setOutlineColor(borderColor);
	rect->setTextureName(texturePath+texture);
	if (borderWidth < 1e-6)
		borderWidth=1e-6;
	rect->setOutlineSize(borderWidth);
	rect->draw(lod,NULL);
}
/*@}*/
