#include <cmath>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTextureManager.h>

#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/Graph.h>
#include <tulip/GlTools.h>

using namespace std;
using namespace tlp;

/** \addtogroup glyph */
/*@{*/
/// A 3D glyph.
/** This glyph draws a transparent cube using the "viewBorderColor" node
 * property value to draw its edges.
 */
class CubeOutLinedTransparent: public Glyph,
		public EdgeExtremityGlyphFrom3DGlyph {
public:
	CubeOutLinedTransparent(GlyphContext *gc = NULL);
	CubeOutLinedTransparent(EdgeExtremityGlyphContext *gc = NULL);
	virtual ~CubeOutLinedTransparent();
	virtual void draw(node n, float lod);
	virtual void draw(edge e, node n, const Color& glyphColor, float lod);
	virtual Coord getAnchor(const Coord & vector) const;

private:
	inline void drawGlyph(const Color& glyohColor, const string& texture,
			const string& texturePath, double borderWidth,
			const Color& borderColor, float lod);
	void drawCubeSimple(GLenum);
};

GLYPHPLUGIN(CubeOutLinedTransparent, "3D - Cube OutLined Transparent", "David Auber", "09/07/2002", "Textured cubeOutLined", "1.0", 9)
;
EEGLYPHPLUGIN(CubeOutLinedTransparent, "3D - Cube OutLined Transparent", "David Auber", "09/07/2002", "Textured cubeOutLined", "1.0", 10)
;

//===================================================================================
CubeOutLinedTransparent::CubeOutLinedTransparent(GlyphContext *gc) :
	Glyph(gc), EdgeExtremityGlyphFrom3DGlyph(NULL) {
}
CubeOutLinedTransparent::CubeOutLinedTransparent(EdgeExtremityGlyphContext *gc) :
	Glyph(NULL), EdgeExtremityGlyphFrom3DGlyph(gc) {
}

CubeOutLinedTransparent::~CubeOutLinedTransparent() {
}

void CubeOutLinedTransparent::draw(node n, float lod) {
	//	if (GlDisplayListManager::getInst().beginNewDisplayList(
	//			"CubeOutLinedTransparent_cube")) {
	//		drawCubeSimple(GL_LINE_LOOP);
	//		GlDisplayListManager::getInst().endNewDisplayList();
	//	}
	//
	//	tlp::setMaterial(glGraphInputData->elementColor->getNodeValue(n));
	//	string texFile = glGraphInputData->elementTexture->getNodeValue(n);
	//	if (texFile != "") {
	//		string texturePath = glGraphInputData->parameters->getTexturePath();
	//		if (GlTextureManager::getInst().activateTexture(texturePath + texFile))
	//			setMaterial(Color(255, 255, 255,
	//					(glGraphInputData->elementColor->getNodeValue(n))[3]));
	//	}
	//	//glCallList(LList);
	//	ColorProperty *borderColor = glGraphInputData->getGraph()->getProperty<
	//			ColorProperty> ("viewBorderColor");
	//	DoubleProperty *borderWidth = 0;
	//	if (glGraphInputData->getGraph()->existProperty("viewBorderWidth"))
	//		borderWidth
	//				= glGraphInputData->getGraph()->getProperty<DoubleProperty> (
	//						"viewBorderWidth");
	//	GlTextureManager::getInst().desactivateTexture();
	//	if (borderWidth == 0)
	//		glLineWidth(2);
	//	else {
	//		double lineWidth = borderWidth->getNodeValue(n);
	//		if (lineWidth < 1e-6)
	//			glLineWidth(1e-6); //no negative borders
	//		else
	//			glLineWidth(lineWidth);
	//	}
	//	glDisable(GL_LIGHTING);
	//	setColor(borderColor->getNodeValue(n));
	//	GlDisplayListManager::getInst().callDisplayList(
	//			"CubeOutLinedTransparent_cube");
	//	glEnable(GL_LIGHTING);
	drawGlyph(glGraphInputData->elementColor->getNodeValue(n),
			glGraphInputData->elementTexture->getNodeValue(n),
			glGraphInputData->parameters->getTexturePath(),
			glGraphInputData->elementBorderWidth->getNodeValue(n),
			glGraphInputData->elementBorderColor->getNodeValue(n), lod);
}

void CubeOutLinedTransparent::draw(edge e, node n, const Color& glyphColor,
		float lod) {
	drawGlyph(glyphColor,
			edgeExtGlGraphInputData->elementTexture->getEdgeValue(e),
			edgeExtGlGraphInputData->parameters->getTexturePath(),
			edgeExtGlGraphInputData->elementBorderWidth->getEdgeValue(e),
			edgeExtGlGraphInputData->elementBorderColor->getEdgeValue(e), lod);
	glDisable(GL_LIGHTING);
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

void CubeOutLinedTransparent::drawCubeSimple(GLenum type) {
	/* front face */
	glBegin(type);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glEnd();
	/* back face */
	glBegin(type);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glEnd();
	/* right face */
	glBegin(type);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glEnd();
	/* left face */
	glBegin(type);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glEnd();
	/* top face */
	glBegin(type);
	glVertex3f(0.5f, 0.5f, 0.5f);
	glVertex3f(0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, -0.5f);
	glVertex3f(-0.5f, 0.5f, 0.5f);
	glEnd();
	/* bottom face */
	glBegin(type);
	glVertex3f(0.5f, -0.5f, -0.5f);
	glVertex3f(0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, 0.5f);
	glVertex3f(-0.5f, -0.5f, -0.5f);
	glEnd();
}
/*@}*/
void CubeOutLinedTransparent::drawGlyph(const Color& glyphColor,
		const string& texture, const string& texturePath, double borderWidth,
		const Color& borderColor, float lod) {
	if (GlDisplayListManager::getInst().beginNewDisplayList(
			"CubeOutLinedTransparent_cube")) {
		drawCubeSimple(GL_LINE_LOOP);
		GlDisplayListManager::getInst().endNewDisplayList();
	}

	tlp::setMaterial(glyphColor);

	if (texture != "") {
		if (GlTextureManager::getInst().activateTexture(texturePath + texture))
			setMaterial(Color(255, 255, 255, (glyphColor[3])));
	}
	//glCallList(LList);

	if (borderWidth < 1e-6)
		glLineWidth(1e-6); //no negative borders
	else
		glLineWidth(borderWidth);

	glDisable(GL_LIGHTING);
	setColor(borderColor);
	GlDisplayListManager::getInst().callDisplayList(
			"CubeOutLinedTransparent_cube");
	glEnable(GL_LIGHTING);
}
