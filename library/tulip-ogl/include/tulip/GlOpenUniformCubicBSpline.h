/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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


#ifndef GLUNIFORMCUBICBSPLINE_H_
#define GLUNIFORMCUBICBSPLINE_H_

#include <tulip/AbstractGlCurve.h>

namespace tlp {

/**
 * @ingroup OpenGL
 * @brief A class to draw open uniform cubic B-splines
 *
 * A B-spline is a convenient form for representing complicated, smooth curves. A cubic uniform B-spline is a
 * piecewise collection of cubic Bezier curves, connected end to end. A cubic B-spline is C^2 continuous, meaning there is no discontinuities in curvature.
 * B-splines have local control : parameters of a B-spline only affect a small part of the entire spline.
 * A B-spline is qualified as open when it passes through its first and last control points.
 *
 */
class TLP_GL_SCOPE GlOpenUniformCubicBSpline : public AbstractGlCurve {

public:

  /**
   * @brief Constructor
   * @warning Don't use it, see other construstor
   */
  GlOpenUniformCubicBSpline();

  /**
   * @brief GlOpenUniformCubicBSpline constructor
   *
   * @param controlPoints a vector of control points (size must be greater or equal to 4)
   * @param startColor the color at the start of the curve
   * @param endColor the color at the end of the curve
   * @param startSize the width at the start of the curve
   * @param endSize the width at the end of the curve
   * @param nbCurvePoints the number of curve points to generate
  */
  GlOpenUniformCubicBSpline(const std::vector<Coord> &controlPoints, const Color &startColor, const Color &endColor,
                            const float startSize, const float endSize, const unsigned int nbCurvePoints = 200);

  ~GlOpenUniformCubicBSpline();

  void drawCurve(std::vector<Coord> &controlPoints, const Color &startColor, const Color &endColor, const float startSize, const float endSize, const unsigned int nbCurvePoints=200);

protected :

  void setCurveVertexShaderRenderingSpecificParameters();

  Coord computeCurvePointOnCPU(const std::vector<Coord> &controlPoints, float t);

  void computeCurvePointsOnCPU(const std::vector<Coord> &controlPoints, std::vector<Coord> &curvePoints, unsigned int nbCurvePoints);

private:

  unsigned nbKnots;
  float stepKnots;

};

}

#endif
