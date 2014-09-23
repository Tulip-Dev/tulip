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

#ifndef PARAMETRICCURVES_H_
#define PARAMETRICCURVES_H_

#include <vector>

#include <tulip/tulipconf.h>
#include <tulip/Coord.h>

namespace tlp {

/**
 * Compute Pascal triangle until nth row
 *
 * \param n the number of Pascal triangle rows to compute
 * \param pascalTriangle a vector of vector of double to store the result. If that vector already contains m Pascal triangle rows and n > m, the first m row are not recomputed and the vector is expanded to store the new rows.
 */
TLP_GL_SCOPE void buildPascalTriangle(unsigned int n, std::vector<std::vector<double> > &pascalTriangle);

/**
 * Compute the position of a point 'p' at t (0 <= t <= 1)
 * along Bezier curve defined by a set of control points
 *
 * \param controlPoints a vector of control points
 * \param t curve parameter value (0 <= t <= 1)
 */
TLP_GL_SCOPE Coord computeBezierPoint(const std::vector<Coord> &controlPoints, const float t);

/** Compute a set of points approximating a Bézier curve
 *
 *  \param controlPoints a vector of control points
 *  \param curvePoints an empty vector to store the computed points
 *  \param nbCurvePoints number of points to generate
 */
TLP_GL_SCOPE void computeBezierPoints(const std::vector<Coord> &controlPoints, std::vector<Coord> &curvePoints, const unsigned int nbCurvePoints = 100);


/**
 * Compute the position of a point 'p' at t (0 <= t <= 1)
 * along Catmull-Rom curve defined by a set of control points.
 * The features of this type of spline are the following :
 *    -> the spline passes through all of the control points
 *    -> the spline is C1 continuous, meaning that there are no discontinuities in the tangent direction and magnitude
 *      -> the spline is not C2 continuous.  The second derivative is linearly interpolated within each segment, causing the curvature to vary linearly over the length of the segment
 *
 * \param controlPoints a vector of control points
 * \param t curve parameter value (0 <= t <= 1)
 * \param closedCurve if true, the curve will be closed, meaning a Bézier segment will connect the last and first control point
 * \param alpha curve parameterization parameter (0 <= alpha <= 1), alpha = 0 -> uniform parameterization, alpha = 0.5 -> centripetal parameterization, alpha = 1.0 -> chord-length parameterization
 */
TLP_GL_SCOPE Coord computeCatmullRomPoint(const std::vector<Coord> &controlPoints, const float t, const bool closedCurve = false, const float alpha = 0.5);

/** Compute a set of points approximating a Catmull-Rom curve
 *
 *  \param controlPoints a vector of control points
 *  \param curvePoints an empty vector to store the computed points
 *  \param closedCurve if true, the curve will be closed, meaning a Bézier segment will connect the last and first control point
 *  \param alpha curve parameterization parameter (0 <= alpha <= 1), alpha = 0 -> uniform parameterization, alpha = 0.5 -> centripetal parameterization, alpha = 1.0 -> chord-length parameterization
 *  \param nbCurvePoints number of points to generate
 */
TLP_GL_SCOPE void computeCatmullRomPoints(const std::vector<Coord> &controlPoints, std::vector<Coord> &curvePoints, const bool closedCurve = false, const unsigned int nbCurvePoints = 100, const float alpha = 0.5);

/**
  * Compute the position of a point 'p' at t (0 <= t <= 1)
  * along open uniform B-spline curve defined by a set of control points.
  * An uniform B-spline is a piecewise collection of Bézier curves of the same degree, connected end to end.
  * The features of this type of spline are the following :
  *   -> the spline is C^2 continuous, meaning there is no discontinuities in curvature
  *     -> the spline has local control : its parameters only affect a small part of the entire spline
  * A B-spline is qualified as open when it passes through its first and last control points.
  * \param controlPoints a vector of control points
  * \param t curve parameter value (0 <= t <= 1)
  * \param curveDegree the B-spline degree
  */


TLP_GL_SCOPE Coord computeOpenUniformBsplinePoint(const std::vector<Coord> &controlPoints, const float t, const unsigned int curveDegree = 3);

/** Compute a set of points approximating an open uniform B-spline curve
 *
 *  \param controlPoints a vector of control points
 *  \param curvePoints an empty vector to store the computed points
 *  \param curveDegree the B-spline degree
 *  \param nbCurvePoints number of points to generate
 */
TLP_GL_SCOPE void computeOpenUniformBsplinePoints(const std::vector<Coord> &controlPoints, std::vector<Coord> &curvePoints, const unsigned int curveDegree = 3, const unsigned int nbCurvePoints = 100);


}

#endif /* PARAMETRICCURVES_H_ */
///@endcond
