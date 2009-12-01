#include <cmath>

#include <string>

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
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
/// A 3D glyph.
/** This glyph draws a christmas tree special for christmas release of 2008
 */
class ChristmasTree: public Glyph, public EdgeExtremityGlyphFrom3DGlyph {
public:
	ChristmasTree(GlyphContext *gc = NULL);
	ChristmasTree(EdgeExtremityGlyphContext *gc = NULL);
	virtual void draw(node n, float lod);
	virtual void draw(edge e, node n, const Color& glyphColor,const Color &borderColor, float lod);

protected:
	void drawTree();

};

GLYPHPLUGIN(ChristmasTree, "3D - ChristmasTree", "Morgan Mathiaut", "16/12/2008", "Christmas tree", "1.0" , 28)
;
EEGLYPHPLUGIN(ChristmasTree,"3D - ChristmasTree", "Morgan Mathiaut", "16/12/2008", "Christmas tree", "1.0" , 29)
;

//===================================================================================
ChristmasTree::ChristmasTree(GlyphContext *gc) :
	Glyph(gc), EdgeExtremityGlyphFrom3DGlyph(NULL) {
}
ChristmasTree::ChristmasTree(EdgeExtremityGlyphContext *gc) :
	Glyph(NULL), EdgeExtremityGlyphFrom3DGlyph(gc) {
}
//=======================================================
void ChristmasTree::draw(node n, float lod) {

	drawTree();
	glRotatef(-90., 1., 0., 0.);
	GlDisplayListManager::getInst().callDisplayList("ChristmasTree_tree");
	setMaterial(glGraphInputData->elementColor->getNodeValue(n));
	GlDisplayListManager::getInst().callDisplayList("ChristmasTree_sphere");
	setMaterial(Color(255, 255, 255, 50));
	GlDisplayListManager::getInst().callDisplayList("ChristmasTree_outsphere");
}

void ChristmasTree::draw(edge e, node n, const Color& glyphColor,const Color &borderColor, float lod) {
	drawTree();
	GlDisplayListManager::getInst().callDisplayList("ChristmasTree_tree");
	setMaterial(glyphColor);
	GlDisplayListManager::getInst().callDisplayList("ChristmasTree_sphere");
	setMaterial(Color(255, 255, 255, 50));
	GlDisplayListManager::getInst().callDisplayList("ChristmasTree_outsphere");
}

void ChristmasTree::drawTree() {
	if (GlDisplayListManager::getInst().beginNewDisplayList(
			"ChristmasTree_tree")) {
		GLUquadricObj *quadratic1;
		GLUquadricObj *quadratic2;
		GLUquadricObj *quadratic3;
		GLUquadricObj *quadratic4;
		setMaterial(Color(125, 80, 0, 255));
		quadratic1 = gluNewQuadric();
		gluQuadricNormals(quadratic1, GLU_SMOOTH);
		gluQuadricTexture(quadratic1, GL_TRUE);
		glTranslatef(0.0f, 0.0f, -0.5f);
		gluQuadricOrientation(quadratic1, GLU_INSIDE);
		gluDisk(quadratic1, 0.0f, 0.1f, 10, 10);
		gluQuadricOrientation(quadratic1, GLU_OUTSIDE);
		gluCylinder(quadratic1, 0.1f, 0.1f, 0.2f, 10, 10);
		glTranslatef(0.0f, 0.0f, 0.5f);

		setMaterial(Color(26, 121, 0, 255));
		quadratic2 = gluNewQuadric();
		gluQuadricNormals(quadratic2, GLU_SMOOTH);
		gluQuadricTexture(quadratic2, GL_TRUE);
		glTranslatef(0.0f, 0.0f, -0.3f);
		gluQuadricOrientation(quadratic2, GLU_OUTSIDE);
		gluCylinder(quadratic2, 0.4f, 0.0f, 0.5f, 10, 10);
		gluQuadricOrientation(quadratic2, GLU_INSIDE);
		gluDisk(quadratic2, 0.0f, 0.4f, 10, 10);
		glTranslatef(0.0f, 0.0f, 0.3f);

		quadratic3 = gluNewQuadric();
		gluQuadricNormals(quadratic3, GLU_SMOOTH);
		gluQuadricTexture(quadratic3, GL_TRUE);
		glTranslatef(0.0f, 0.0f, -0.1f);
		gluQuadricOrientation(quadratic3, GLU_OUTSIDE);
		gluCylinder(quadratic3, 0.3f, 0.0f, 0.4f, 10, 10);
		gluQuadricOrientation(quadratic3, GLU_INSIDE);
		gluDisk(quadratic3, 0.0f, 0.3f, 10, 10);
		glTranslatef(0.0f, 0.0f, 0.1f);

		quadratic4 = gluNewQuadric();
		gluQuadricNormals(quadratic4, GLU_SMOOTH);
		gluQuadricTexture(quadratic4, GL_TRUE);
		glTranslatef(0.0f, 0.0f, 0.1f);
		gluQuadricOrientation(quadratic4, GLU_OUTSIDE);
		gluCylinder(quadratic4, 0.2f, 0.0f, 0.3f, 10, 10);
		gluQuadricOrientation(quadratic4, GLU_INSIDE);
		gluDisk(quadratic4, 0.0f, 0.2f, 10, 10);
		glTranslatef(0.0f, 0.0f, -0.1f);

		GlDisplayListManager::getInst().endNewDisplayList();
		gluDeleteQuadric(quadratic1);
		gluDeleteQuadric(quadratic2);
		gluDeleteQuadric(quadratic3);
		gluDeleteQuadric(quadratic4);
	}
	if (GlDisplayListManager::getInst().beginNewDisplayList(
			"ChristmasTree_sphere")) {
		GLUquadricObj *quadratic = gluNewQuadric();
		gluQuadricNormals(quadratic, GLU_SMOOTH);
		gluQuadricTexture(quadratic, GL_TRUE);
		glTranslatef(0.0f, 0.0f, 0.45f);
		gluSphere(quadratic, 0.05f, 30, 30);
		glTranslatef(0.0f, 0.0f, -0.45f);
		GlDisplayListManager::getInst().endNewDisplayList();
		gluDeleteQuadric(quadratic);
	}
	if (GlDisplayListManager::getInst().beginNewDisplayList(
			"ChristmasTree_outsphere")) {
		GLUquadricObj *quadratic = gluNewQuadric();
		gluQuadricNormals(quadratic, GLU_SMOOTH);
		gluQuadricTexture(quadratic, GL_TRUE);
		gluSphere(quadratic, 0.5f, 30, 30);
		GlDisplayListManager::getInst().endNewDisplayList();
		gluDeleteQuadric(quadratic);
	}
}
