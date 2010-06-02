#ifndef PARAMETRICCURVES_H_
#define PARAMETRICCURVES_H_

#include <vector>

#include "tulip/Coord.h"

namespace tlp {

/**
 * Compute the position of a point 'p' at t (0 <= t <= 1)
 * along Bézier curve defined by a set of control points
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
 * 		-> the spline passes through all of the control points
 *		-> the spline is C1 continuous, meaning that there are no discontinuities in the tangent direction and magnitude
 *      -> the spline is not C2 continuous.  The second derivative is linearly interpolated within each segment, causing the curvature to vary linearly over the length of the segment
 *
 * \param controlPoints a vector of control points
 * \param t curve parameter value (0 <= t <= 1)
 * \param closedCurve if true, the curve will be closed, meaning a Bézier segment will connect the last and first control point
 */
TLP_GL_SCOPE Coord computeCatmullRomPoint(const std::vector<Coord> &controlPoints, const float t, const bool closedCurve = false);

/** Compute a set of points approximating a Catmull-Rom curve
 *
 *  \param controlPoints a vector of control points
 *  \param curvePoints an empty vector to store the computed points
 *  \param closedCurve if true, the curve will be closed, meaning a Bézier segment will connect the last and first control point
 *  \param nbCurvePoints number of points to generate
 */
TLP_GL_SCOPE void computeCatmullRomPoints(const std::vector<Coord> &controlPoints, std::vector<Coord> &curvePoints, const bool closedCurve = false, const unsigned int nbCurvePoints = 100);

/**
  * Compute the position of a point 'p' at t (0 <= t <= 1)
  * along open uniform B-spline curve defined by a set of control points.
  * An uniform B-spline is a piecewise collection of Bézier curves of the same degree, connected end to end.
  * The features of this type of spline are the following :
  *		-> the spline is C^2 continuous, meaning there is no discontinuities in curvature
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
