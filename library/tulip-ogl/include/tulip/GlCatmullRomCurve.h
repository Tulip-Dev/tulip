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



#ifndef GLCATMULLROMCURVE_H_
#define GLCATMULLROMCURVE_H_

#include <vector>

#include <tulip/AbstractGlCurve.h>

namespace tlp {


/**
 * @ingroup OpenGL
 * @brief A class to draw a Catmull-Rom curve
 *
 * This class allow to draw a Catmull-Rom curve, a smooth curve which passes through all its control points.
 * Catmull-Rom splines are a family of cubic interpolating splines formulated such that the tangent at each
 * control point is calculated using the previous and next control point point of the spline.
 * Catmull-Rom splines have C^1 continuity, local control, and interpolation, but do not lie within the convex
 * hull of their control points.
 */
class TLP_GL_SCOPE GlCatmullRomCurve : public AbstractGlCurve {

  enum ParameterizationType {UNIFORM, CHORD_LENGTH, CENTRIPETAL};

public :

  GlCatmullRomCurve();

  /**
   * @brief GlCatmullRomCurve constructor
   *
   * @param controlPoints a vector of control points (size must be greater or equal to 4)
   * @param startColor the color at the start of the curve
   * @param endColor the color at the end of the curve
   * @param startSize the width at the start of the curve
   * @param endSize the width at the end of the curve
   * @param closedCurve if true, the curve will be closed and a bezier segment will be drawn between the last and first control point
   * @param paramType curve parameterization type (GlCatmullRomCurve::UNIFORM | GlCatmullRomCurve::CENTRIPETAL | GlCatmullRomCurve::CHORD_LENGTH (default))
   * @param nbCurvePoints the number of curve points to generate
   */
  GlCatmullRomCurve(const std::vector<Coord> &controlPoints, const Color &startColor, const Color &endColor,
                    const float startSize, const float endSize, const bool closedCurve = false,
                    const unsigned int nbCurvePoints = 200, const ParameterizationType paramType = CENTRIPETAL);

  ~GlCatmullRomCurve();

  void setParameterizationType(const ParameterizationType paramType) {
    this->paramType = paramType;
  }

  void drawCurve(std::vector<Coord> &controlPoints, const Color &startColor, const Color &endColor, const float startSize, const float endSize, const unsigned int nbCurvePoints=200);

  void setClosedCurve(const bool closedCurve) {
    this->closedCurve = closedCurve;
  }

protected :

  void setCurveVertexShaderRenderingSpecificParameters();

  Coord computeCurvePointOnCPU(const std::vector<Coord> &controlPoints, float t);

  void computeCurvePointsOnCPU(const std::vector<Coord> &controlPoints, std::vector<Coord> &curvePoints, unsigned int nbCurvePoints);

private :

  bool closedCurve;
  float totalLength;
  float alpha;
  ParameterizationType paramType;
};


}

#endif /* GLCATMULLROMCURVE_H_ */

