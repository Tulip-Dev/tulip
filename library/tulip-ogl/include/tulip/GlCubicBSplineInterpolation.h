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
///@cond DOXYGEN_HIDDEN


#include <tulip/GlOpenUniformCubicBSpline.h>

namespace tlp {

/**
 * A class to draw a curve interpolating a set of points with C^2 continuity
 *
 * This class allows to draw a cubic B-spline interpolating a set of points. The resulting curve
 * is C^2 continous, so there is no discontinuities in curvature.
 */
class TLP_GL_SCOPE GlCubicBSplineInterpolation : public GlOpenUniformCubicBSpline {

public :

  /**
   * GlCubicBSplineInterpolation constructor
   *
   * \param pointsToInterpolate the set of points to interpolate
   * \param startColor the color at the start of the curve
   * \param endColor the color at the end of the curve
   * \param startSize the width at the start of the curve
   * \param endSize the width at the end of the curve
   * \param nbCurvePoints the number of curve points to generate
   */
  GlCubicBSplineInterpolation(const std::vector<Coord> &pointsToInterpolate, const Color &startColor, const Color &endColor,
                              const float startSize, const float endSize, const unsigned int nbCurvePoints = 100);


private :

  std::vector<Coord> constructInterpolatingCubicBSpline(const std::vector<Coord> &pointsToInterpolate);

};

}
///@endcond
