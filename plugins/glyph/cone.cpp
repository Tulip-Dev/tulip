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

#include <tulip/StringProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTextureManager.h>

#include <tulip/Graph.h>
#include <tulip/Glyph.h>
#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlTools.h>

using namespace std;
using namespace tlp;

/** \addtogroup glyph */
/*@{*/
/// A 3D glyph.
/**
 * This glyph draws a textured cone using the "viewTexture" node
 * property value. If this property has no value, the cone is then colored
 * using the "viewColor" node property value.
 */
//===========================================================
class Cone: public Glyph, public EdgeExtremityGlyphFrom3DGlyph {
public:
	Cone(GlyphContext *gc = NULL);
	Cone(EdgeExtremityGlyphContext *gc = NULL);
	virtual ~Cone();
	virtual void getIncludeBoundingBox(BoundingBox &boundingBox);
	virtual void draw(node n, float lod);
	virtual Coord getAnchor(const Coord &vector) const;
	virtual void draw(edge e, node n, const Color& glyphColor, const Color &borderColor, float lod);

private:
	void drawCone();
};
//===========================================================
GLYPHPLUGIN(Cone, "3D - Cone", "Bertrand Mathieu", "09/07/2002", "Textured cone", "1.0", 3)
;
EEGLYPHPLUGIN(Cone, "3D - Cone", "Bertrand Mathieu", "09/07/2002", "Cone", "1.0", 4)
;
//===================================================================================
Cone::Cone(GlyphContext *gc) :
	Glyph(gc), EdgeExtremityGlyphFrom3DGlyph(NULL) {
}
Cone::Cone(EdgeExtremityGlyphContext *gc) :
	Glyph(NULL), EdgeExtremityGlyphFrom3DGlyph(gc) {
}
//===========================================================
Cone::~Cone() {
}
//===========================================================
void Cone::getIncludeBoundingBox(BoundingBox& boundingBox) {
    boundingBox[0] = Coord(0.25, 0.25, 0);
    boundingBox[1] = Coord(0.75, 0.75, 0.5);
}
//===========================================================
void Cone::draw(node n, float lod) {
	if (GlDisplayListManager::getInst().beginNewDisplayList("Cone_cone")) {
		drawCone();
    GlDisplayListManager::getInst().endNewDisplayList();
	}
	setMaterial(glGraphInputData->elementColor->getNodeValue(n));
	string texFile = glGraphInputData->elementTexture->getNodeValue(n);
	if (texFile != "") {
		string texturePath = glGraphInputData->parameters->getTexturePath();
		GlTextureManager::getInst().activateTexture(texturePath + texFile);
	}

	GlDisplayListManager::getInst().callDisplayList("Cone_cone");

	GlTextureManager::getInst().desactivateTexture();
}
//===========================================================
Coord Cone::getAnchor(const Coord &vector) const {
	Coord anchor = vector;

	float x, y, z, n;
	anchor.get(x, y, z);
	n = sqrt(x * x + y * y);
	float vx0, vy0, vx1, vy1, x0, y0, x1, y1, px, py;
	x0 = 0;
	y0 = 0.5;
	vx0 = 0.5;
	vy0 = -1.0;
	x1 = 0;
	y1 = 0;
	vx1 = sqrt(x * x + y * y);
	vy1 = z;
	py = -1.0 * (vy1 * (vx0 / vy0 * y0 + x0 - x1) - vx1 * y1) / (vx1 - vy1
			/ vy0 * vx0);
	px = vx0 * (py - y0) / vy0 + x0;

	if (fabsf(py) > 0.5) {
		n = anchor.norm();
		py = n * 0.5 / fabsf(z);
		anchor.setX(x * py / n);
		anchor.setY(y * py / n);
		anchor.setZ(z * py / n);
	} else {
		anchor.setX(x * px / n);
		anchor.setY(y * px / n);
		anchor.setZ(py);
	}
	return anchor;
}

void Cone::draw(edge e, node n, const Color& glyphColor,const Color &borderColor, float lod) {
	if (GlDisplayListManager::getInst().beginNewDisplayList("Cone_cone")) {
		drawCone();
    GlDisplayListManager::getInst().endNewDisplayList();
	}
	setMaterial(glyphColor);
	string texFile = edgeExtGlGraphInputData->elementTexture->getEdgeValue(e);
	if (texFile != "") {
		string texturePath =
				edgeExtGlGraphInputData->parameters->getTexturePath();
		GlTextureManager::getInst().activateTexture(texturePath + texFile);
	}
	GlDisplayListManager::getInst().callDisplayList("Cone_cone");
	GlTextureManager::getInst().desactivateTexture();
}

void Cone::drawCone() {
	GLUquadricObj *quadratic;
	quadratic = gluNewQuadric();
	gluQuadricNormals(quadratic, GLU_SMOOTH);
	gluQuadricTexture(quadratic, GL_TRUE);
	glTranslatef(0.0f, 0.0f, -0.5f);
	gluQuadricOrientation(quadratic, GLU_OUTSIDE);
	gluCylinder(quadratic, 0.5f, 0.0f, 1.0f, 10, 10);
	gluQuadricOrientation(quadratic, GLU_INSIDE);
	gluDisk(quadratic, 0.0f, 0.5f, 10, 10);
	gluDeleteQuadric(quadratic);
}
//===========================================================
/*@}*/
