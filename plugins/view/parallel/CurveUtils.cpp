/*
 * CurveUtils.cpp
 *
 *  Created on: 16 avr. 2009
 *      Author: tontonnemesis
 */

#include <tulip/GlTextureManager.h>
#include <tulip/Curves.h>
#include <tulip/GlTools.h>

#include "CurveUtils.h"
#include "FastBezier.h"
#include "ParallelTools.h"

namespace tlp {

void drawCurve(const std::vector<Coord> &curvePoints, const Color &startColor, const Color &endColor, const float startSize, const float endSize, const Coord &startN, const Coord &endN, const std::string &texture) {
	unsigned int size;
	GLfloat *points = buildCurvePoints(curvePoints, getSizes(curvePoints, startSize, endSize), startN, endN ,size);
	vector<Color> curveColors = getColors(curvePoints, startColor, endColor);
	if(texture != "") {
		GlTextureManager::getInst().activateTexture(texture);
	}

	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);

	glBegin(GL_QUAD_STRIP);
	for (unsigned int i = 0; i < size; ++i) {
		setMaterial(curveColors[i]);
		glTexCoord2f(0.0f, 0.0f);
		glVertex3fv(&points[i*3]);
		glTexCoord2f(1.0f, 1.0f);
		glVertex3fv(&points[i*3 + size*3]);
	}
	glEnd();

	// if no texture set, draw the curve outline for anti-aliasing
	if(texture == "") {
		glEnable(GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBegin(GL_LINE_STRIP);
		for (unsigned int i = 0 ; i < size ; ++i) {
			setMaterial(curveColors[i]);
			glVertex3fv(&points[i*3]);
		}
		glEnd();

		glBegin(GL_LINE_STRIP);
		for (unsigned int i = 0 ; i < size ; ++i) {
			setMaterial(curveColors[i]);
			glVertex3fv(&points[i*3+size*3]);
		}
		glEnd();
	} else {
		GlTextureManager::getInst().desactivateTexture();
	}

	glEnable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);

	delete [] points;
}
}
