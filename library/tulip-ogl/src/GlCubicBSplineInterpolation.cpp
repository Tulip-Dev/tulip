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
#include <tulip/GlCubicBSplineInterpolation.h>

using namespace std;

namespace tlp {

GlCubicBSplineInterpolation::GlCubicBSplineInterpolation(const vector<Coord> &pointsToInterpolate, const Color &startColor, const Color &endColor,
    const float startSize, const float endSize, const unsigned int nbCurvePoints) :
  GlOpenUniformCubicBSpline(constructInterpolatingCubicBSpline(pointsToInterpolate), startColor, endColor, startSize, endSize, nbCurvePoints) {}


vector<Coord> GlCubicBSplineInterpolation::constructInterpolatingCubicBSpline(const vector<Coord> &pointsToInterpolate) {
  vector<Coord> Ai(pointsToInterpolate.size());
  vector<float> Bi(pointsToInterpolate.size());
  vector<Coord> di(pointsToInterpolate.size());
  di[0] = (pointsToInterpolate[1] - pointsToInterpolate[0]) / 3.0f;
  di[pointsToInterpolate.size() - 1] = (pointsToInterpolate[pointsToInterpolate.size()-1] - pointsToInterpolate[pointsToInterpolate.size()-2]) / 3.0f;
  Bi[1] = -0.25f;
  Ai[1] = (pointsToInterpolate[2] - pointsToInterpolate[0] - di[0]) / 4.0f;

  for (size_t i = 2; i < pointsToInterpolate.size() - 1; ++i) {
    Bi[i] = -1.0f /(4.0f + Bi[i-1]);
    Ai[i] = Coord(-(pointsToInterpolate[i+1] - pointsToInterpolate[i-1] - Ai[i-1])*Bi[i]);
  }

  for (size_t i = pointsToInterpolate.size() - 2; i > 0; --i) {
    di[i] = Ai[i] + di[i+1]*Bi[i];
  }

  vector<Coord> bSplineControlPoints;
  bSplineControlPoints.push_back(pointsToInterpolate[0]);
  bSplineControlPoints.push_back(pointsToInterpolate[0]+di[0]);

  for (size_t i = 1 ; i < pointsToInterpolate.size() - 1 ; ++i) {
    bSplineControlPoints.push_back(pointsToInterpolate[i]-di[i]);
    bSplineControlPoints.push_back(pointsToInterpolate[i]);
    bSplineControlPoints.push_back(pointsToInterpolate[i]+di[i]);
  }

  bSplineControlPoints.push_back(pointsToInterpolate[pointsToInterpolate.size() - 1]-di[pointsToInterpolate.size() - 1]);
  bSplineControlPoints.push_back(pointsToInterpolate[pointsToInterpolate.size() - 1]);
  return bSplineControlPoints;
}

}
