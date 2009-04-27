#ifndef BEZIER_H
#define BEZIER_H

#include <tulip/Coord.h>

#include <vector>

namespace tlp {

// This class contains a set of static functions for computing a set of points approximating a bezier curve
// The computation of the points is optimized by the use of the "forward difference" algorithm which greatly reduces the number of multiplications besides the "brute-force" bezier algorithm.
class FastBezier {

public :

	static void computeLinearBezierPoints(const Coord &p0, const Coord &p1, std::vector<Coord> &curvePoints, const unsigned int nbCurvePoints = 20);

	// This function computes nbCurvesPoints approximating the quadratic bezier curve defined by the three control points : p0, p1, p2 and p3
	// The computed points are stored in the curvePoints vector passed as parameter (this one has to be empty, the function takes care of its resizement).
	// This function is optimized and is called by computeBezierPoints if the number of control points equals 3
	static void computeQuadraticBezierPoints(const Coord &p0, const Coord &p1, const Coord &p2, std::vector<Coord> &curvePoints, const unsigned int nbCurvePoints = 20);

	// This function computes nbCurvesPoints approximating the cubic bezier curve defined by the four control points : p0, p1, p2 and p3
	// The computed points are stored in the curvePoints vector passed as parameter (this one has to be empty, the function takes care of its resizement).
	// This function is optimized and is called by computeBezierPoints if the number of control points equals 4
	static void computeCubicBezierPoints(const Coord &p0, const Coord &p1, const Coord &p2, const Coord &p3,
			                             std::vector<Coord> &curvePoints, const unsigned int nbCurvePoints = 20);

	// This function computes nbCurvesPoints approximating the bezier curve defined by an arbitrary number of control points (>= 2)
	// The computed points are stored in the curvePoints vector passed as parameter (this one has to be empty, the function takes care of its resizement).
	static void computeBezierPoints(const std::vector<Coord> &controlPoints, std::vector<Coord> &curvePoints,
									const unsigned int nbCurvePoints = 20);

	// This function computes the points approximating a "Catmull Rom Spline"
	// The features of this type of spline are the following :
	//    -> the spline passes through all of the control points
	//    -> the spline is C1 continuous, meaning that there are no discontinuities in the tangent direction and magnitude
	//    -> the spline is not C2 continuous.  The second derivative is linearly interpolated within each segment, causing the curvature to vary linearly over the length of the segment
	static void computeSplinePoints(const std::vector<Coord> &splinePassPoints, std::vector<Coord> &splinePoints, const bool closedSpline = false, const unsigned int nbPointsPerSegment = 20);

};

}

#endif //BEZIER_H
