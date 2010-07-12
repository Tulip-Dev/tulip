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

#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTools.h>
#include <tulip/GlGraphInputData.h>
#include <string>
using namespace tlp;
using namespace std;
class GlArrow2DEdgeExtremity: public EdgeExtremityGlyphFrom2DGlyph {
public:
	GlArrow2DEdgeExtremity(EdgeExtremityGlyphContext *gc);
	virtual ~GlArrow2DEdgeExtremity();

	void draw(edge e, node n, const Color& glyphColor, const Color &borderColor, float lod);
};

EEGLYPHPLUGIN(GlArrow2DEdgeExtremity,"2D - Arrow","Jonathan Dubois","09/04/09","Edge Extremity with 2D arrow","1.0",50)
;

GlArrow2DEdgeExtremity::GlArrow2DEdgeExtremity(EdgeExtremityGlyphContext *gc) :
	EdgeExtremityGlyphFrom2DGlyph(gc) {

}

GlArrow2DEdgeExtremity::~GlArrow2DEdgeExtremity() {
}

void GlArrow2DEdgeExtremity::draw(edge e, node, const Color& glyphColor, const Color &borderColor,
		float lod) {
  glDisable(GL_LIGHTING);
	if (GlDisplayListManager::getInst().beginNewDisplayList("Arrow 2D")) {
		glBegin(GL_TRIANGLES);
		glVertex3f(.5, 0, 0);
		glVertex3f(-.5, -.5, 0);
		glVertex3f(-.5, .5, 0);
		glEnd();
		GlDisplayListManager::getInst().endNewDisplayList();
	}
	if (GlDisplayListManager::getInst().beginNewDisplayList("Arrow 2D Border")) {
		glBegin(GL_LINE_LOOP);
		glVertex3f(.5, 0, 0);
		glVertex3f(-.5, -.5, 0);
		glVertex3f(-.5, .5, 0);
		glEnd();
		GlDisplayListManager::getInst().endNewDisplayList();
	}
	setMaterial(glyphColor);
	glDisable(GL_CULL_FACE);
	GlDisplayListManager::getInst().callDisplayList("Arrow 2D");
	glEnable(GL_CULL_FACE);

	double width = edgeExtGlGraphInputData->elementBorderWidth->getEdgeValue(e);
	if (lod > 20) {
		if (width < 1e-6)
			glLineWidth(1e-6); //no negative borders
		else
			glLineWidth(width);


		setColor(borderColor);
		GlDisplayListManager::getInst().callDisplayList("Arrow 2D Border");
	}
}
