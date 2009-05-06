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
}

#endif /* CURVEUTILS_H_ */
