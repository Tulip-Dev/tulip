#include <cmath>

#include <string>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTextureManager.h>

#include <tulip/Graph.h>
#include <tulip/GlTools.h>

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
protected:
	inline void drawGlyph(const Color& glyphColor, const string& texture,
			const string& texturePath, double borderWidth,
			const Color& borderColor, float lod);
	void drawHexagone();
	void drawHexagoneBorder();
};
//=====================================================
GLYPHPLUGIN(Hexagone, "2D - Hexagone", "David Auber", "09/07/2002", "Textured Hexagone", "1.0", 13)
;
EEGLYPHPLUGIN(Hexagone,"2D - Hexagone", "David Auber", "09/07/2002", "Textured Hexagone", "1.0", 14)
;
//===================================================================================
Hexagone::Hexagone(GlyphContext *gc) :
	Glyph(gc), EdgeExtremityGlyphFrom2DGlyph(NULL) {
}
Hexagone::Hexagone(EdgeExtremityGlyphContext *gc) :
	Glyph(NULL), EdgeExtremityGlyphFrom2DGlyph(gc) {
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
	//	if (GlDisplayListManager::getInst().beginNewDisplayList("Hexagone_hexagone")) {
	//		drawHexagone();
	//		GlDisplayListManager::getInst().endNewDisplayList();
	//	}
	//	if (GlDisplayListManager::getInst().beginNewDisplayList(
	//			"Hexagone_hexagoneborder")) {
	//		drawHexagoneBorder();
	//		GlDisplayListManager::getInst().endNewDisplayList();
	//	}
	//	setMaterial(glGraphInputData->elementColor->getNodeValue(n));
	//	string texFile = glGraphInputData->elementTexture->getNodeValue(n);
	//	if (texFile != "") {
	//		string texturePath = glGraphInputData->parameters->getTexturePath();
	//		GlTextureManager::getInst().activateTexture(texturePath + texFile);
	//	}
	//
	//	GlDisplayListManager::getInst().callDisplayList("Hexagone_hexagone");
	//
	//	GlTextureManager::getInst().desactivateTexture();
	//
	//	if (lod > 20) {
	//		ColorProperty *borderColor = glGraphInputData->getGraph()->getProperty<
	//				ColorProperty> ("viewBorderColor");
	//		DoubleProperty *borderWidth = 0;
	//		if (glGraphInputData->getGraph()->existProperty("viewBorderWidth"))
	//			borderWidth = glGraphInputData->getGraph()->getProperty<
	//					DoubleProperty> ("viewBorderWidth");
	//		GlTextureManager::getInst().desactivateTexture();
	//		if (borderWidth == 0)
	//			glLineWidth(2);
	//		else {
	//			double lineWidth = borderWidth->getNodeValue(n);
	//			if (lineWidth < 1e-6)
	//				glLineWidth(1e-6); //no negative borders
	//			else
	//				glLineWidth(lineWidth);
	//		}
	//		glDisable(GL_LIGHTING);
	//		setColor(borderColor->getNodeValue(n));
	//		GlDisplayListManager::getInst().callDisplayList(
	//				"Hexagone_hexagoneborder");
	//		glEnable(GL_LIGHTING);
	//	}
	drawGlyph(glGraphInputData->elementColor->getNodeValue(n),
			glGraphInputData->elementTexture->getNodeValue(n),
			glGraphInputData->parameters->getTexturePath(),
			glGraphInputData->elementBorderWidth->getNodeValue(n),
			glGraphInputData->elementBorderColor->getNodeValue(n), lod);
}

void Hexagone::draw(edge e, node n, const Color& glyphColor, const Color &borderColor, float lod) {
	drawGlyph(glyphColor,
			edgeExtGlGraphInputData->elementTexture->getEdgeValue(e),
			edgeExtGlGraphInputData->parameters->getTexturePath(),
			edgeExtGlGraphInputData->elementBorderWidth->getEdgeValue(e),
			borderColor, lod);
	glDisable(GL_LIGHTING);
}
//=====================================================
void Hexagone::drawHexagone() {
	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
	gluQuadricNormals(quadratic, GLU_SMOOTH);
	gluQuadricTexture(quadratic, GL_TRUE);
	gluQuadricOrientation(quadratic, GLU_OUTSIDE);
	gluDisk(quadratic, 0.0f, 0.5f, 6, 1);
	gluQuadricOrientation(quadratic, GLU_INSIDE);
	gluDisk(quadratic, 0.0f, 0.5f, 6, 1);
	gluDeleteQuadric(quadratic);
}

void Hexagone::drawHexagoneBorder() {
	glBegin(GL_LINE_LOOP);
	double alpha = M_PI / 2.;
	double delta = 2. * M_PI / 6.0;
	for (unsigned int i = 0; i < 6; ++i) {
		glVertex3f(0.5 * cos(alpha), 0.5 * sin(alpha), 0.0);
		alpha += delta;
	}
	glEnd();
}
/*@}*/
void Hexagone::drawGlyph(const Color& glyphColor, const string& texture,
		const string& texturePath, double borderWidth,
		const Color& borderColor, float lod) {
	if (GlDisplayListManager::getInst().beginNewDisplayList("Hexagone_hexagone")) {
		drawHexagone();
		GlDisplayListManager::getInst().endNewDisplayList();
	}
	if (GlDisplayListManager::getInst().beginNewDisplayList(
			"Hexagone_hexagoneborder")) {
		drawHexagoneBorder();
		GlDisplayListManager::getInst().endNewDisplayList();
	}
	setMaterial(glyphColor);
	if (texture != "") {
		GlTextureManager::getInst().activateTexture(texturePath + texture);
	}

	GlDisplayListManager::getInst().callDisplayList("Hexagone_hexagone");

	GlTextureManager::getInst().desactivateTexture();

	if (lod > 20) {
		if (borderWidth < 1e-6)
			glLineWidth(1e-6); //no negative borders
		else
			glLineWidth(borderWidth);
		glDisable(GL_LIGHTING);
		setColor(borderColor);
		GlDisplayListManager::getInst().callDisplayList(
				"Hexagone_hexagoneborder");
		glEnable(GL_LIGHTING);
	}
}
