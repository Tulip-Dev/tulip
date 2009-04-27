/*
 * CurveUtils.h
 *
 *  Created on: 16 avr. 2009
 *      Author: tontonnemesis
 */

#ifndef CURVEUTILS_H_
#define CURVEUTILS_H_

#include <tulip/Coord.h>
#include <tulip/Color.h>
#include <tulip/GlTools.h>

#include <vector>

using namespace std;

namespace tlp {
//================================================
void drawCurve(const std::vector<Coord> &curvePoints, const Color &startColor, const Color &endColor,
			   const float startSize, const float endSize, const Coord &startN, const Coord &endN, const std::string &texture = "");
//================================================
GLfloat* buildCurvePoints (const vector<Coord> &vertices,
						   const vector<float> &sizes,
						   const Coord &startN,
						   const Coord &endN,
						   unsigned int &resultSize,
						   vector<unsigned int> *dec=NULL);
vector<Color> getColors(const vector<Coord> &line, const Color &c1, const Color &c2);
//================================================
vector<float> getSizes(const vector<Coord> &line, float s1, float s2);
//====================================================
void setColor(const Color &c);
//====================================================
void setColor(GLfloat *c);
//====================================================
void setMaterial(const Color &c);
//====================================================
}

#endif /* CURVEUTILS_H_ */
