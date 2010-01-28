#include <cmath>

#include <string>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/Size.h>
#include <tulip/Coord.h>
#include <tulip/Glyph.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTextureManager.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/Graph.h>
#include <tulip/GlTools.h>

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
	virtual void getIncludeBoundingBox(BoundingBox &boundingBox);
	virtual void draw(node n, float lod);
	virtual void draw(edge e, node n, const Color& glyphColor,const Color &borderColor, float lod);
protected:
	inline void drawGlyph(const Color& glyphColor, const string& texture,
			const string& texturePath, double borderWidth,
			const Color& borderColor, float lod);
	inline void drawCircle();
	inline void drawCircleBorder();
};
//=====================================================
GLYPHPLUGIN(Circle, "2D - Circle", "David Auber", "09/07/2002", "Textured Circle", "1.0", 14)
;
EEGLYPHPLUGIN(Circle,"2D - Circle", "David Auber", "09/07/2002", "Textured Circle", "1.0", 15)
;
//===================================================================================
Circle::Circle(GlyphContext *gc) :
	Glyph(gc), EdgeExtremityGlyphFrom2DGlyph(NULL) {
}
Circle::Circle(EdgeExtremityGlyphContext  *gc) :
	Glyph(NULL), EdgeExtremityGlyphFrom2DGlyph(gc) {
}
//=====================================================
Circle::~Circle() {
}
//=====================================================
void Circle::getIncludeBoundingBox(BoundingBox &boundingBox) {
	boundingBox.first = Coord(0.15, 0.15, 0);
	boundingBox.second = Coord(0.85, 0.85, 0);
}

void Circle::draw(edge e, node n, const Color& glyphColor,const Color &borderColor, float lod) {
	drawGlyph(glyphColor,
			edgeExtGlGraphInputData->parameters->getTexturePath(),
			edgeExtGlGraphInputData->elementTexture->getEdgeValue(e),
			edgeExtGlGraphInputData->elementBorderWidth->getEdgeValue(e),
			borderColor, lod);
	glDisable(GL_LIGHTING);
}

//=====================================================
void Circle::draw(node n, float lod) {
	drawGlyph(Glyph::glGraphInputData->elementColor->getNodeValue(n),
              Glyph::glGraphInputData->parameters->getTexturePath(),
              Glyph::glGraphInputData->elementTexture->getNodeValue(n),
              Glyph::glGraphInputData->elementBorderWidth->getNodeValue(n),
              Glyph::glGraphInputData->elementBorderColor->getNodeValue(n), lod);
}

void Circle::drawGlyph(const Color& glyphColor, const string& texturePath,
		const string& texture, double borderWidth, const Color& borderColor,
		float lod) {
	if (GlDisplayListManager::getInst().beginNewDisplayList("Circle_circle")) {
		drawCircle();
		GlDisplayListManager::getInst().endNewDisplayList();
	}
	if (GlDisplayListManager::getInst().beginNewDisplayList(
			"Circle_circleborder")) {
		drawCircleBorder();
		GlDisplayListManager::getInst().endNewDisplayList();
	}
	setMaterial(glyphColor);

    if(texture != "") {
      GlTextureManager::getInst().activateTexture(texturePath + texture);
	}

	GlDisplayListManager::getInst().callDisplayList("Circle_circle");

	GlTextureManager::getInst().desactivateTexture();

	if (lod > 20) {
		if (borderWidth == 0)
			glLineWidth(2);
		else {
			if (borderWidth < 1e-6)
				glLineWidth(1e-6); //no negative borders
			else
				glLineWidth(borderWidth);
		}
		glDisable(GL_LIGHTING);
		setColor(borderColor);
		GlDisplayListManager::getInst().callDisplayList("Circle_circleborder");
		glEnable(GL_LIGHTING);
	}
}

//=====================================================
void Circle::drawCircle() {
	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
	gluQuadricNormals(quadratic, GLU_SMOOTH);
	gluQuadricTexture(quadratic, GL_TRUE);
	gluQuadricOrientation(quadratic, GLU_OUTSIDE);
	gluDisk(quadratic, 0.0f, 0.5f, 30, 1);
	gluQuadricOrientation(quadratic, GLU_INSIDE);
	gluDisk(quadratic, 0.0f, 0.5f, 30, 1);
	gluDeleteQuadric(quadratic);
}

void Circle::drawCircleBorder() {
	glBegin(GL_LINE_LOOP);
	double alpha = M_PI / 2.;
	double delta = 2. * M_PI / 30.0;
	for (unsigned int i = 0; i < 30; ++i) {
		glVertex3f(0.5 * cos(alpha), 0.5 * sin(alpha), 0.0);
		alpha += delta;
	}
	glEnd();
}
/*@}*/
