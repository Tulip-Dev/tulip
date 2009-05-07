#include "FastBezier.h"

#include <vector>
#include <map>

using namespace std;

namespace tlp {

void FastBezier::computeLinearBezierPoints(const Coord &p0, const Coord &p1, std::vector<Coord> &curvePoints, const unsigned int nbCurvePoints) {
	float h = 1.0 / (float) (nbCurvePoints - 1);
	Coord firstFD = h * (p1 - p0);

	Coord c = p0;

	curvePoints.resize(nbCurvePoints);

	// Compute points at each step
	curvePoints[0] = c;

	for (unsigned int i = 0 ; i < nbCurvePoints - 2 ; ++i) {

		c += firstFD;

		curvePoints[i+1] = c;
	}

	curvePoints[nbCurvePoints - 1] = p1;
}

void FastBezier::computeQuadraticBezierPoints(const Coord &p0, const Coord &p1, const Coord &p2, std::vector<Coord> &curvePoints, const unsigned int nbCurvePoints) {
	float h, h2;
	Coord firstFD, secondFD;

	// Compute our step size
	h = 1.0 / (float) (nbCurvePoints - 1);
	h2 = h*h;

	// Compute Initial forward difference
	firstFD = (h2 - 2 * h) * p0 + (-2 * h2 + 2 * h) * p1 + h2 * p2;
	secondFD = 2 * h2 * p0 - 4 * h2 * p1 + 2 * h2 * p2;

	Coord c = p0;

	curvePoints.resize(nbCurvePoints);

	// Compute points at each step
	curvePoints[0] = c;

	for (unsigned int i = 0 ; i < nbCurvePoints - 2 ; ++i) {

		c += firstFD;

		firstFD += secondFD;

		curvePoints[i+1] = c;
	}

	curvePoints[nbCurvePoints - 1] = p2;

}

void FastBezier::computeCubicBezierPoints(const Coord &p0, const Coord &p1, const Coord &p2, const Coord &p3,
		std::vector<Coord> &curvePoints, const unsigned int nbCurvePoints) {
	Coord A, B, C;
	float h, h2, h3, h36, h22;
	Coord firstFD, secondFD, thirdFD;

	// Compute polynomial coefficients from Bezier points
	A = -1 * p0 + 3 * (p1 - p2) + p3;
	B = 3 * p0 - 6 * p1 + 3 * p2;
	C = -3 * p0 + 3 * p1;

	// Compute our step size
	h = 1.0 / (float) (nbCurvePoints - 1);
	h2 = h*h;
	h3 = h2*h;
	h36 = h3*6;
	h22 = h2*2;

	// Compute forward differences from Bezier points and "h"
	firstFD = A * h3 + B * h2 + C * h;
	thirdFD = A * h36;
	secondFD = thirdFD + B * h22;

	Coord c = p0;

	curvePoints.resize(nbCurvePoints);

	// Compute points at each step
	curvePoints[0] = c;

	for (unsigned int i = 0 ; i < nbCurvePoints - 2 ; ++i) {

		c += firstFD;

		firstFD += secondFD;

		secondFD += thirdFD;

		curvePoints[i+1] = c;
	}

	curvePoints[nbCurvePoints - 1] = p3;
}

Coord computeBezierPoint(const vector<Coord> &controlPoints, const float t) {
	vector<Coord> internalControlPoints = controlPoints;
	for (unsigned int i = controlPoints.size() - 1 ; i > 0 ; --i) {
		for (unsigned int j = 0 ; j < i ; ++j) {
			internalControlPoints[j] = internalControlPoints[j] + t * (internalControlPoints[j+1] - internalControlPoints[j]);
		}
	}
	return internalControlPoints[0];
}

void FastBezier::computeBezierPoints(const vector<Coord> &controlPoints, vector<Coord> &curvePoints, unsigned int nbCurvePoints) {
	if (controlPoints.size() == 2) {
		computeLinearBezierPoints(controlPoints[0], controlPoints[1], curvePoints, nbCurvePoints);
	} else if (controlPoints.size() == 3) {
		computeQuadraticBezierPoints(controlPoints[0], controlPoints[1], controlPoints[2], curvePoints, nbCurvePoints);
	} else if (controlPoints.size() == 4) {
		computeCubicBezierPoints(controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3], curvePoints, nbCurvePoints);

	} else {
		curvePoints.resize(nbCurvePoints);
		float h = 1.0 / (float) (nbCurvePoints - 1);
		for (unsigned int i = 0 ; i < nbCurvePoints ; ++i) {
			float curStep = i * h;
			curvePoints[i] = computeBezierPoint(controlPoints, curStep);
		}
	}
}

void computeBezierSegmentControlPoints(const Coord &pBefore, const Coord &pStart, const Coord &pEnd, const Coord &pAfter, vector<Coord> &bezierSegmentControlPoints) {
	bezierSegmentControlPoints.push_back(pStart);
	Coord d0, d1;
	d0 = (pEnd - pBefore) / 2.;
	bezierSegmentControlPoints.push_back(pStart + d0 / 3.);
	d1 = (pAfter - pStart) / 2.;
	bezierSegmentControlPoints.push_back(pEnd - d1 / 3.);
	bezierSegmentControlPoints.push_back(pEnd);
}

void FastBezier::computeSplinePoints(const std::vector<Coord> &splinePassPoints, std::vector<Coord> &splinePoints, const bool closedSpline, const unsigned int nbPointsPerSegment) {
	if (splinePassPoints.size() == 2) {
		splinePoints.push_back(splinePassPoints[0]);
		splinePoints.push_back(splinePassPoints[1]);
	} else {
		for (unsigned int i = 0 ; i < splinePassPoints.size() ; ++i) {
			vector<Coord> bezierSegmentControlPoints;
			vector<Coord> bezierSegmentPoints;
			if (i == 0 && !closedSpline) {
				computeBezierSegmentControlPoints(splinePassPoints[i], splinePassPoints[i], splinePassPoints[i+1], splinePassPoints[i+2], bezierSegmentControlPoints);
			} else if (i ==0 && closedSpline) {
				computeBezierSegmentControlPoints(splinePassPoints[splinePassPoints.size() - 1], splinePassPoints[i], splinePassPoints[i+1], splinePassPoints[i+2], bezierSegmentControlPoints);
			} else if (i == splinePassPoints.size() - 2 && !closedSpline) {
				computeBezierSegmentControlPoints(splinePassPoints[i-1], splinePassPoints[i], splinePassPoints[i+1], splinePassPoints[i+1], bezierSegmentControlPoints);
			} else if (i == splinePassPoints.size() - 2 && closedSpline) {
				computeBezierSegmentControlPoints(splinePassPoints[i-1], splinePassPoints[i], splinePassPoints[i+1], splinePassPoints[0], bezierSegmentControlPoints);
			} else if (i == splinePassPoints.size() -1) {
				if (closedSpline) {
					computeBezierSegmentControlPoints(splinePassPoints[i-1], splinePassPoints[i], splinePassPoints[0], splinePassPoints[1], bezierSegmentControlPoints);
				}
			} else {
				computeBezierSegmentControlPoints(splinePassPoints[i-1], splinePassPoints[i], splinePassPoints[i+1], splinePassPoints[i+2], bezierSegmentControlPoints);
			}
			if (bezierSegmentControlPoints.size() > 0) {
				FastBezier::computeBezierPoints(bezierSegmentControlPoints, bezierSegmentPoints, nbPointsPerSegment);
				if (bezierSegmentPoints.size() > 0) {
					if (splinePoints.size() > 0 && bezierSegmentPoints[0] == splinePoints[splinePoints.size() - 1]) {
						splinePoints.pop_back();
					}
					splinePoints.insert(splinePoints.end(), bezierSegmentPoints.begin(), bezierSegmentPoints.end());
				}
			}
		}
	}
}

}
