/*
 * edgeExtremityArrow.cpp
 *
 *  Created on: 7 mai 2009
 *      Author: jdubois
 */

#include <tulip/EdgeExtremityGlyph.h>
#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTools.h>
#include <tulip/GlGraphInputData.h>
#include <string>
using namespace tlp;
using namespace std;
class GlArrow2DEdgeExtremity: public tlp::EdgeExtremityGlyphFrom2DGlyph {
public:
	GlArrow2DEdgeExtremity(tlp::EdgeExtremityGlyphContext *gc);
	virtual ~GlArrow2DEdgeExtremity();

	void
			draw(tlp::edge e, tlp::node n, const tlp::Color& glyphColor,
					float lod);
};

EEGLYPHPLUGIN(GlArrow2DEdgeExtremity,"2D - Arrow","Jonathan Dubois","09/04/09","Edge Extremity with 2D arrow","1.0",50)
;

GlArrow2DEdgeExtremity::GlArrow2DEdgeExtremity(EdgeExtremityGlyphContext *gc) :
	EdgeExtremityGlyphFrom2DGlyph(gc) {

}

GlArrow2DEdgeExtremity::~GlArrow2DEdgeExtremity() {
}

void GlArrow2DEdgeExtremity::draw(edge e, node n, const Color& glyphColor,
		float lod) {
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


		setColor(edgeExtGlGraphInputData->elementBorderColor->getEdgeValue(e));
		GlDisplayListManager::getInst().callDisplayList("Arrow 2D Border");
	}
}
