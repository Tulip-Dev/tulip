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
#include <tulip/GlTools.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTextureManager.h>

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
	virtual void getIncludeBoundingBox(BoundingBox &boundingBox);
	virtual void draw(node n, float lod);
	virtual Coord getAnchor(const Coord &vector) const;
	virtual void draw(edge e, node n, const Color& glyphColor, const Color &borderColor, float lod);

protected:
	inline void drawGlyph(const Color& glyphColor, const string& texture,
			const string& texturePath, double borderWidth,
			const Color& borderColor, float lod);
	inline void drawSquare();
	inline void drawSquareBorder();
};
//=====================================================
GLYPHPLUGIN(Square, "2D - Square", "David Auber", "09/07/2002", "Textured square", "1.0", 4)
;
EEGLYPHPLUGIN(Square,"2D - Square", "David Auber", "09/07/2002", "Textured square", "1.0", 5)
;
//===================================================================================
Square::Square(GlyphContext *gc) :
	Glyph(gc), EdgeExtremityGlyphFrom2DGlyph(NULL) {
}
Square::Square(EdgeExtremityGlyphContext *gc) :
	Glyph(NULL), EdgeExtremityGlyphFrom2DGlyph(gc) {
}
//=====================================================
Square::~Square() {
}
//=====================================================
void Square::getIncludeBoundingBox(BoundingBox &boundingBox) {
	boundingBox.first = Coord(0, 0, 0);
	boundingBox.second = Coord(1, 1, 0);
}
//=====================================================
void Square::draw(node n, float lod) {
	drawGlyph(glGraphInputData->elementColor->getNodeValue(n),
			glGraphInputData->elementTexture->getNodeValue(n),
			glGraphInputData->parameters->getTexturePath(),
			glGraphInputData->elementBorderWidth->getNodeValue(n),
			glGraphInputData->elementBorderColor->getNodeValue(n), lod);
}

void Square::draw(edge e, node n, const Color& glyphColor, const Color &borderColor, float lod) {
	drawGlyph(glyphColor,
			edgeExtGlGraphInputData->elementTexture->getEdgeValue(e),
			edgeExtGlGraphInputData->parameters->getTexturePath(),
			edgeExtGlGraphInputData->elementBorderWidth->getEdgeValue(e),
			borderColor, lod);
	glDisable(GL_LIGHTING);
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
	if (GlDisplayListManager::getInst().beginNewDisplayList("Square_square")) {
		drawSquare();
		GlDisplayListManager::getInst().endNewDisplayList();
	}
	if (GlDisplayListManager::getInst().beginNewDisplayList(
			"Square_squareborder")) {
		drawSquareBorder();
		GlDisplayListManager::getInst().endNewDisplayList();
	}

	setMaterial(glyphColor);
	if (texture != "") {
		GlTextureManager::getInst().activateTexture(texturePath + texture);
	}
	GlDisplayListManager::getInst().callDisplayList("Square_square");

	GlTextureManager::getInst().desactivateTexture();

	if (lod > 20) {

		if (borderWidth < 1e-6)
			glLineWidth(1e-6); //no negative borders
		else
			glLineWidth(borderWidth);

		glDisable(GL_LIGHTING);
		setColor(borderColor);
		GlDisplayListManager::getInst().callDisplayList("Square_squareborder");
		glEnable(GL_LIGHTING);
	}
}

//=====================================================
void Square::drawSquare() {
	glBegin(GL_QUADS);
	/* front face */
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(-0.5f, -0.5f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(0.5f, -0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(0.5f, 0.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(-0.5f, 0.5f);
	/* back face */
	glNormal3f(0.0f, 0.0f, -1.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex2f(-0.5f, -0.5f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex2f(-0.5f, 0.5f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(0.5f, 0.5f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex2f(0.5f, -0.5f);
	glEnd();
}
//=====================================================
void Square::drawSquareBorder() {
	glBegin(GL_LINE_LOOP);
	/* front face */
	glVertex2f(-0.5f, -0.5f);
	glVertex2f(0.5f, -0.5f);
	glVertex2f(0.5f, 0.5f);
	glVertex2f(-0.5f, 0.5f);
	glEnd();
}
/*@}*/
