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

#include <tulip/AbstractGlCurve.h>

#include <vector>

namespace tlp {


/**
 * \brief A class to draw a Catmull-Rom curve
 *
 * This class allow to draw a Catmull-Rom curve, a smooth curve which passes through all its control points.
 * Catmull-Rom splines are a family of cubic interpolating splines formulated such that the tangent at each
 * control point is calculated using the previous and next control point point of the spline.
 * Catmull-Rom splines have C^1 continuity, local control, and interpolation, but do not lie within the convex
 * hull of their control points.
 */
class TLP_GL_SCOPE GlCatmullRomCurve : public AbstractGlCurve {

public :

	/**
	 * GlCatmullRomCurve constructor
	 *
	 * \param controlPoints a vector of control points (size must be greater or equal to 4)
	 * \param startColor the color at the start of the curve
	 * \param endColor the color at the end of the curve
	 * \param startSize the width at the start of the curve
	 * \param endSize the width at the end of the curve
	 * \param closedCurve if true, the curve will be closed and a bezier segment will be drawn between the last and first control point
	 * \param nbCurvePoints the number of curve points to generate
	 */
	GlCatmullRomCurve(const std::vector<Coord> &controlPoints, const Color &startColor, const Color &endColor,
					  const float startSize, const float endSize, const bool closedCurve = false, const unsigned int nbCurvePoints = 200);

	~GlCatmullRomCurve();

	void draw(float lod, Camera *camera);

protected :

	void setCurveVertexShaderRenderingSpecificParameters();

	Coord computeCurvePointOnCPU(float t);

private :

	void computeBezierSegmentControlPoints(const Coord &pBefore, const Coord &pStart, const Coord &pEnd, const Coord &pAfter,
			std::vector<Coord> &bezierSegmentControlPoints);

	int computeSegmentIndex(float t);

	bool closedCurve;
};


}

#endif /* GLCATMULLROMCURVE_H_ */

