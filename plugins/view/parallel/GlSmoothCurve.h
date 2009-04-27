/*
	  GlCatmullRomCurve.h

   Created on: 21 avr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#ifndef GLCATMULLROMCURVE_H_
#define GLCATMULLROMCURVE_H_

#include <tulip/GlSimpleEntity.h>

#include <vector>

#include "GlBezierCurve.h"

namespace tlp {


/**
 * This class allow to draw a smooth curve which passes through all the points passes as parameters
 */
class GlSmoothCurve : public GlSimpleEntity {

public :

	GlSmoothCurve(const std::vector<Coord> &curvePassPoints, const Color &color, const float size,
			      const std::string &texture = "", const bool closedCurve = false);

	~GlSmoothCurve();

	void draw(float lod,Camera *camera);

	virtual void translate(const Coord& mouvement);

	virtual void getXML(xmlNodePtr rootNode) {}

	virtual void setWithXML(xmlNodePtr rootNode) {}

private :


	std::vector<Coord> curvePassPoints;
	Color color;
	float size;
	std::string texture;
	bool closedCurve;

};


}

#endif /* GLCATMULLROMCURVE_H_ */
