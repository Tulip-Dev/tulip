/*
	  GlCatmullRomCurve.h

   Created on: 29 avr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */


#ifndef GLCATMULLROMCURVE_H_
#define GLCATMULLROMCURVE_H_

#include <tulip/GlComposite.h>

#include <vector>

#include "tulip/GlBezierCurve.h"

namespace tlp {


/**
 * This class allow to draw a smooth curve which passes through all the points passes as parameters
 */
class  TLP_GL_SCOPE GlCatmullRomCurve : public GlComposite {

public :

	GlCatmullRomCurve(const std::vector<Coord> &curvePassPoints, const Color &beginColor, const Color &endColor,
					  const float beginSize, const float endSize, const std::string &texture = "", const bool closedCurve = false);

	~GlCatmullRomCurve();

	void draw(float lod, Camera *camera);

	virtual void getXML(xmlNodePtr rootNode) {}

	virtual void setWithXML(xmlNodePtr rootNode) {}

private :

	void computeBezierSegmentControlPoints(const Coord &pBefore, const Coord &pStart, const Coord &pEnd, const Coord &pAfter,
										   std::vector<Coord> &bezierSegmentControlPoints);

	void computeBezierSegments(const std::vector<Coord> &curvePassPoints);

	Color beginColor, endColor;
	float beginSize, endSize;
	std::string texture;
	bool closedCurve;

};


}

#endif /* GLCATMULLROMCURVE_H_ */

