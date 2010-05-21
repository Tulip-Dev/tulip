/*
	  GlUniformCubicBSpline.h

   Created on: 10 avr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#ifndef GLUNIFORMCUBICBSPLINE_H_
#define GLUNIFORMCUBICBSPLINE_H_

#include <tulip/AbstractGlCurve.h>
#include <tulip/GlTools.h>

namespace tlp {

/**
 * \brief A class to draw open uniform cubic B-splines
 *
 *  A B-spline is a convenient form for representing complicated, smooth curves. A cubic uniform B-spline is a
 *  piecewise collection of cubic Bézier curves, connected end to end. A cubic B-spline is C^2 continuous, meaning there is no discontinuities in curvature.
 *  B-splines have local control : parameters of a B-spline only affect a small part of the entire spline.
 *  A B-spline is qualified as open when it passes through its first and last control points.
 *
 */
class TLP_GL_SCOPE GlOpenUniformCubicBSpline : public AbstractGlCurve {

public:

	/**
	 * GlOpenUniformCubicBSpline constructor
	 *
	 * \param controlPoints a vector of control points (size must be greater or equal to 4)
	 * \param startColor the color at the start of the curve
	 * \param endColor the color at the end of the curve
	 * \param startSize the width at the start of the curve
	 * \param endSize the width at the end of the curve
	 * \param nbCurvePoints the number of curve points to generate
	*/
	GlOpenUniformCubicBSpline(const std::vector<Coord> &controlPoints, const Color &startColor, const Color &endColor,
							  const float startSize, const float endSize, const unsigned int nbCurvePoints = 200);

	~GlOpenUniformCubicBSpline();

	void draw(float lod, Camera *camera);

protected :

	void setCurveVertexShaderRenderingSpecificParameters();

	Coord computeCurvePointOnCPU(float t);

private:

	unsigned nbKnots;
	float stepKnots;

};

}

#endif
