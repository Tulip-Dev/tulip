/*
	  GlBezierCurve.h

   Created on: 10 avr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#ifndef GLBEZIERCURVE_H_
#define GLBEZIERCURVE_H_

#include <tulip/GlSimpleEntity.h>
#include <tulip/GlTools.h>

namespace tlp {

class TLP_GL_SCOPE GlBezierCurve : public GlSimpleEntity {
public:

	GlBezierCurve(const std::vector<Coord> &controlPoints,
				  const Color &beginColor, const Color &endColor,
				  const float &beginSize, const float &endSize,
				  const std::string &texture = "");

	~GlBezierCurve();

	void draw(float lod,Camera *camera);

	void setTexture(const std::string &texture);

	void translate(const Coord& mouvement);

	void getXML(xmlNodePtr rootNode);

	void setWithXML(xmlNodePtr rootNode);

protected:

	bool vboOk, shaderProgramOk;
	std::string shaderProgramName;
	std::vector<Coord> controlPoints;
	GLfloat *controlPointsArray;
	int nbControlPoints;
	Color beginColor;
	Color endColor;
	float beginSize;
	float endSize;
	std::string texture;
};

}
#endif /* GLBEZIERCURVE_H_ */
