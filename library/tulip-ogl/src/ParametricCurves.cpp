/**
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
#include <tulip/ParametricCurves.h>
#include <map>

using namespace std;

namespace tlp {

static void computeLinearBezierPoints(const Coord &p0, const Coord &p1, vector<Coord> &curvePoints, const unsigned int nbCurvePoints) {
  float h = 1.0 / static_cast<float>((nbCurvePoints - 1));
  Coord firstFD((p1 - p0) * h);
  Coord c(p0);

  curvePoints.resize(nbCurvePoints);

  // Compute points at each step
  curvePoints[0] = c;

  for (unsigned int i = 0 ; i < nbCurvePoints - 2 ; ++i) {

    c += firstFD;

    curvePoints[i+1] = c;
  }

  curvePoints[nbCurvePoints - 1] = p1;
}

static void computeQuadraticBezierPoints(const Coord &p0, const Coord &p1, const Coord &p2, vector<Coord> &curvePoints, const unsigned int nbCurvePoints) {

  // Compute our step size
  float h = 1.0 / static_cast<float>(nbCurvePoints - 1);
  float h2 = h*h;

  // Compute Initial forward difference
  Coord firstFD(p0 * (h2 - 2 * h) + p1 * (-2 * h2 + 2 * h) + p2 * h2);
  Coord secondFD(p0 * 2.f * h2  - p1 * 4.f * h2  + p2 * 2.f * h2);

  Coord c(p0);

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

static void computeCubicBezierPoints(const Coord &p0, const Coord &p1, const Coord &p2, const Coord &p3,
                                     vector<Coord> &curvePoints, const unsigned int nbCurvePoints) {

  // Compute polynomial coefficients from Bezier points
  Coord A(p0 * -1.f + (p1 - p2) * 3.f + p3);
  Coord B(p0 * 3.f  - p1 * 6.f + p2 * 3.f);
  Coord C(p0 * -3.f + p1 * 3.f);

  // Compute our step size
  float h = 1.0 / static_cast<float>(nbCurvePoints - 1);
  float h2 = h*h;
  float h3 = h2*h;
  float h36 = h3*6;
  float h22 = h2*2;

  // Compute forward differences from Bezier points and "h"
  Coord firstFD(A * h3 + B * h2 + C * h);
  Coord thirdFD(A * h36);
  Coord secondFD(thirdFD + B * h22);

  Coord c(p0);

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

void buildPascalTriangle(unsigned int n, vector<vector<double> > &pascalTriangle) {
  size_t curSize = pascalTriangle.size();

  if (curSize >= n)
    return;

  pascalTriangle.resize(n);

  for (unsigned int i = curSize ; i < n; ++i) {
    pascalTriangle[i].resize(i+1);
  }

  for (unsigned int i = curSize ; i < n ; ++i) {
    pascalTriangle[i][0] = 1;
    pascalTriangle[i][i] = 1;

    for (unsigned int j = 1; j < i; ++j) {
      pascalTriangle[i][j] = pascalTriangle[i-1][j-1] + pascalTriangle[i-1][j];
    }
  }
}

static map<double, vector<double> > tCoeffs;
static map<double, vector<double> > sCoeffs;
static map<unsigned int, unsigned int> computedCoefficients;

static void computeCoefficients(double t, unsigned int nbControlPoints) {
  double s = (1.0 - t);
#ifdef _OPENMP
  #pragma omp critical
#endif

  if (tCoeffs.find(t) == tCoeffs.end()) {
    vector<double> tCoeff, sCoeff;

    for (size_t i = 0 ; i < nbControlPoints ; ++i) {
      tCoeff.push_back(pow(t, static_cast<double>(i)));
      sCoeff.push_back(pow(s, static_cast<double>(i)));
    }

    tCoeffs[t] = tCoeff;
    sCoeffs[t] = sCoeff;
  }
  else {
    vector<double> &tCoeff = tCoeffs[t];
    vector<double> &sCoeff = sCoeffs[t];

    if (tCoeff.size() < nbControlPoints) {
      size_t oldSize = tCoeff.size();

      for (size_t i = oldSize ; i < nbControlPoints ; ++i) {
        tCoeff.push_back(pow(t, static_cast<double>(i)));
        sCoeff.push_back(pow(s, static_cast<double>(i)));
      }
    }
  }
}

Coord computeBezierPoint(const vector<Coord> &controlPoints, const float t) {

  size_t nbControlPoints = controlPoints.size();

  computeCoefficients(t, nbControlPoints);

  Vector<double, 3> bezierPoint;
  bezierPoint[0] = bezierPoint[1] = bezierPoint[2] = 0;
  double curCoeff = 1.0;
  double r = static_cast<double>(nbControlPoints);

  for (size_t i = 0 ; i < controlPoints.size() ; ++i) {
    Vector<double, 3> controlPoint;
    controlPoint[0] = controlPoints[i][0];
    controlPoint[1] = controlPoints[i][1];
    controlPoint[2] = controlPoints[i][2];
    bezierPoint += controlPoint * curCoeff * tCoeffs[static_cast<double>(t)][i] * sCoeffs[static_cast<double>(t)][nbControlPoints - 1 - i];
    double c = static_cast<double>(i+1);
    curCoeff *= (r -c)/c;
  }

  return Coord(bezierPoint[0], bezierPoint[1], bezierPoint[2]);
}


void computeBezierPoints(const vector<Coord> &controlPoints, vector<Coord> &curvePoints, unsigned int nbCurvePoints) {
  assert(controlPoints.size() > 1);

  switch(controlPoints.size()) {
  case 2:
    computeLinearBezierPoints(controlPoints[0], controlPoints[1], curvePoints, nbCurvePoints);
    break;

  case 3:
    computeQuadraticBezierPoints(controlPoints[0], controlPoints[1], controlPoints[2], curvePoints, nbCurvePoints);
    break;

  case 4:
    computeCubicBezierPoints(controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3], curvePoints, nbCurvePoints);
    break;

  default:
    curvePoints.resize(nbCurvePoints);
    float h = 1.0 / static_cast<float>(nbCurvePoints - 1);
// With Visual Studio, the parallelization of the curve points computation
// leads to incorrect results (why? I don't know ...)
#if defined(_OPENMP) && !defined(_MSC_VER)
    #pragma omp parallel for
#endif

    for (int i = 0 ; i < static_cast<int>(nbCurvePoints) ; ++i) {
      float curStep = i * h;
      curvePoints[i] = computeBezierPoint(controlPoints, curStep);
    }
  }
}


static void computeCatmullRomGlobalParameter(const vector<Coord> &controlPoints, vector<float> &globalParameter, const float alpha) {
  globalParameter.resize(controlPoints.size());
  globalParameter[0] = 0.0f;
  globalParameter[controlPoints.size() - 1] = 1.0f;
  vector<float> cumDist;
  cumDist.resize(controlPoints.size());
  cumDist[0] = 0.0f;
  float totalDist = 0.f;

  for (size_t i = 1 ; i < controlPoints.size() ; ++i) {
    float dist = pow((float) controlPoints[i-1].dist(controlPoints[i]), alpha);
    cumDist[i] = cumDist[i-1] + dist;
    totalDist += dist;
  }

  for (size_t i = 1 ; i < controlPoints.size() - 1 ; ++i) {
    globalParameter[i] = cumDist[i] / totalDist;
  }
}

static size_t computeSegmentIndex(float t, const vector<Coord> &controlPoints, const vector<float> &globalParameter) {
  if (t == 0.0) {
    return 0;
  }
  else if (t == 1.0)  {
    return controlPoints.size() - 1;
  }
  else {
    size_t i = 0;

    while (t >= globalParameter[i+1]) {
      ++i;
    }

    return i;
  }
}

static void computeBezierSegmentControlPoints(const Coord &pBefore, const Coord &pStart, const Coord &pEnd, const Coord &pAfter, vector<Coord> &bezierSegmentControlPoints, const float alpha) {
  bezierSegmentControlPoints.push_back(pStart);
  float d1 = pBefore.dist(pStart);
  float d2 = pStart.dist(pEnd);
  float d3 = pEnd.dist(pAfter);
  float d1alpha = pow(d1, alpha);
  float d12alpha = pow(d1, 2*alpha);
  float d2alpha = pow(d2, alpha);
  float d22alpha = pow(d2, 2*alpha);
  float d3alpha = pow(d3, alpha);
  float d32alpha = pow(d3, 2*alpha);
  bezierSegmentControlPoints.push_back(Coord((d12alpha*pEnd-d22alpha*pBefore+(2*d12alpha+3*d1alpha*d2alpha+d22alpha)*pStart)/(3*d1alpha*(d1alpha+d2alpha))));
  bezierSegmentControlPoints.push_back(Coord((d32alpha*pStart-d22alpha*pAfter+(2*d32alpha+3*d3alpha*d2alpha+d22alpha)*pEnd)/(3*d3alpha*(d3alpha+d2alpha))));
  bezierSegmentControlPoints.push_back(pEnd);
}

static Coord computeCatmullRomPointImpl(const vector<Coord> &controlPoints, const float t, const vector<float> &globalParameter, const bool closedCurve, const float alpha) {
  size_t i = computeSegmentIndex(t, controlPoints, globalParameter);
  float localT = 0.0;

  if (t >= 1.0) {
    localT = 1.0;
  }
  else if (t != 0.0) {
    localT = (t - globalParameter[i]) / (globalParameter[i+1] - globalParameter[i]);
  }

  vector<Coord> bezierControlPoints;

  if (i == 0) {
    computeBezierSegmentControlPoints(closedCurve ? controlPoints[controlPoints.size() - 2] : controlPoints[i] - (controlPoints[i+1] - controlPoints[i]), controlPoints[i], controlPoints[i+1], controlPoints[i+2], bezierControlPoints, alpha);
  }
  else if (i == controlPoints.size() - 2) {
    computeBezierSegmentControlPoints(controlPoints[i-1], controlPoints[i], controlPoints[i+1], closedCurve ? controlPoints[1] : controlPoints[i+1] + (controlPoints[i+1] - controlPoints[i]), bezierControlPoints, alpha);
  }
  else if (i == controlPoints.size() - 1) {
    computeBezierSegmentControlPoints(controlPoints[i-2], controlPoints[i-1], controlPoints[i], closedCurve ? controlPoints[1] : controlPoints[i] + (controlPoints[i] - controlPoints[i-1]), bezierControlPoints, alpha);
  }
  else {
    computeBezierSegmentControlPoints(controlPoints[i-1], controlPoints[i], controlPoints[i+1], controlPoints[i+2], bezierControlPoints, alpha);
  }

  float t2 = localT * localT;
  float t3 = t2 * localT;
  float s = 1.0 - localT;
  float s2 = s * s;
  float s3 = s2 * s;
  return bezierControlPoints[0] * s3 + bezierControlPoints[1] * 3.0f * localT * s2 + bezierControlPoints[2] * 3.0f * t2 * s + bezierControlPoints[3] * t3;
}

Coord computeCatmullRomPoint(const vector<Coord> &controlPoints, const float t, const bool closedCurve, const float alpha) {
  assert(controlPoints.size() > 2);
  vector<float> globalParameter;
  vector<Coord> controlPointsCp(controlPoints);

  if (closedCurve) {
    controlPointsCp.push_back(controlPoints[0]);
  }

  computeCatmullRomGlobalParameter(controlPointsCp, globalParameter, alpha);
  return computeCatmullRomPointImpl(controlPointsCp, t, globalParameter, closedCurve, alpha);
}

void computeCatmullRomPoints(const vector<Coord> &controlPoints, vector<Coord> &curvePoints, const bool closedCurve, const unsigned int nbCurvePoints, const float alpha) {
  //assert(controlPoints.size() > 2);
  if(controlPoints.size()<=2)
    return;

  vector<float> globalParameter;
  vector<Coord> controlPointsCp(controlPoints);

  if (closedCurve) {
    controlPointsCp.push_back(controlPoints[0]);
  }

  computeCatmullRomGlobalParameter(controlPointsCp, globalParameter, alpha);
  curvePoints.resize(nbCurvePoints);
#ifdef _OPENMP
  #pragma omp parallel for
#endif

  for (int i = 0 ; i < static_cast<int>(nbCurvePoints) ; ++i) {
    curvePoints[i] = computeCatmullRomPointImpl(controlPointsCp, i / static_cast<float>(nbCurvePoints - 1), globalParameter, closedCurve, alpha);
  }
}

static float clamp(float f, float minVal, float maxVal) {
  return min(max(f, minVal), maxVal);
}

Coord computeOpenUniformBsplinePoint(const vector<Coord> &controlPoints, const float t, const unsigned int curveDegree) {
  assert(controlPoints.size() > 3);
  unsigned int nbKnots = controlPoints.size() + curveDegree + 1;
  float stepKnots = 1.0f / ((static_cast<float>(nbKnots) - 2.0f * (static_cast<float>(curveDegree) + 1.0f)) + 2.0f - 1.0f);

  if (t == 0.0) {
    return controlPoints[0];
  }
  else if (t >= 1.0) {
    return controlPoints[controlPoints.size() - 1];
  }
  else {
    float* coeffs = new float[curveDegree + 1];
    memset(coeffs, 0, (curveDegree + 1) * sizeof(float));
    int k = curveDegree;
    int cpt = 0;

    while (t > (cpt*stepKnots) && t >= ((cpt+1)*stepKnots)) {
      ++k;
      ++cpt;
    }

    float knotVal = (cpt * stepKnots);
    coeffs[curveDegree] = 1.0;

    for (int i = 1 ; i <= static_cast<int>(curveDegree) ; ++i) {
      coeffs[curveDegree-i] = (clamp(knotVal + stepKnots, 0.0, 1.0) - t) / (clamp(knotVal + stepKnots, 0.0, 1.0) - clamp(knotVal + (-i+1) * stepKnots, 0.0, 1.0)) * coeffs[curveDegree-i+1];
      int tabIdx = curveDegree-i+1;

      for (int j = -i+1 ; j <= -1 ; ++j) {
        coeffs[tabIdx] = ((t - clamp(knotVal + j * stepKnots, 0.0, 1.0)) / (clamp(knotVal + (j+i) * stepKnots, 0.0, 1.0) - clamp(knotVal + j * stepKnots, 0.0, 1.0))) * coeffs[tabIdx] + ((clamp(knotVal + (j+i+1) * stepKnots, 0.0, 1.0) - t) / (clamp(knotVal + (j+i+1) * stepKnots, 0.0, 1.0) - clamp(knotVal + (j+1) * stepKnots, 0.0, 1.0))) * coeffs[tabIdx+1];
        ++tabIdx;
      }

      coeffs[curveDegree] = ((t - knotVal) / (clamp(knotVal + i * stepKnots, 0.0, 1.0) - knotVal)) * coeffs[curveDegree];
    }

    Coord curvePoint(0.0f, 0.0f, 0.0f);
    int startIdx = k - curveDegree;

    for (unsigned int i = 0 ; i <= curveDegree ; ++i) {
      curvePoint += coeffs[i] * controlPoints[startIdx + i];
    }

    delete [] coeffs;
    return curvePoint;
  }
}


void computeOpenUniformBsplinePoints(const vector<Coord> &controlPoints, vector<Coord> &curvePoints, const unsigned int curveDegree, const unsigned int nbCurvePoints) {
  curvePoints.resize(nbCurvePoints);
#ifdef _OPENMP
  #pragma omp parallel for
#endif

  for (int i = 0 ; i < static_cast<int>(nbCurvePoints) ; ++i) {
    curvePoints[i] = computeOpenUniformBsplinePoint(controlPoints, i / static_cast<float>(nbCurvePoints - 1), curveDegree);
  }
}

}
