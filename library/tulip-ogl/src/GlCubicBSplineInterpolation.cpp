/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

GlCubicBSplineInterpolation::GlCubicBSplineInterpolation(const vector<Coord> &pointsToInterpolate,
                                                         const Color &startColor,
                                                         const Color &endColor,
                                                         const float startSize, const float endSize,
                                                         const unsigned int nbCurvePoints)
    : GlOpenUniformCubicBSpline(vector<Coord>(), startColor, endColor, startSize, endSize,
                                nbCurvePoints) {
  constructInterpolatingCubicBSpline(pointsToInterpolate);
  for (size_t i = 0; i < controlPoints.size(); ++i) {
    boundingBox.expand(controlPoints[i]);
  }
}

void GlCubicBSplineInterpolation::constructInterpolatingCubicBSpline(
    const vector<Coord> &pointsToInterpolate) {
  vector<Coord> Ai(pointsToInterpolate.size());
  vector<float> Bi(pointsToInterpolate.size());
  vector<Coord> di(pointsToInterpolate.size());
  di[0] = (pointsToInterpolate[1] - pointsToInterpolate[0]) / 3.0f;
  di[pointsToInterpolate.size() - 1] = (pointsToInterpolate[pointsToInterpolate.size() - 1] -
                                        pointsToInterpolate[pointsToInterpolate.size() - 2]) /
                                       3.0f;
  Bi[1] = -0.25f;
  Ai[1] = (pointsToInterpolate[2] - pointsToInterpolate[0] - di[0]) / 4.0f;

  for (size_t i = 2; i < pointsToInterpolate.size() - 1; ++i) {
    Bi[i] = -1.0f / (4.0f + Bi[i - 1]);
    Ai[i] = Coord(-(pointsToInterpolate[i + 1] - pointsToInterpolate[i - 1] - Ai[i - 1]) * Bi[i]);
  }

  for (size_t i = pointsToInterpolate.size() - 2; i > 0; --i) {
    di[i] = Ai[i] + di[i + 1] * Bi[i];
  }

  controlPoints.reserve(3 * pointsToInterpolate.size() - 2);
  controlPoints.emplace_back(pointsToInterpolate[0]);
  controlPoints.emplace_back(pointsToInterpolate[0] + di[0]);

  for (size_t i = 1; i < pointsToInterpolate.size() - 1; ++i) {
    controlPoints.emplace_back(pointsToInterpolate[i] - di[i]);
    controlPoints.emplace_back(pointsToInterpolate[i]);
    controlPoints.emplace_back(pointsToInterpolate[i] + di[i]);
  }

  controlPoints.emplace_back(pointsToInterpolate[pointsToInterpolate.size() - 1] -
                             di[pointsToInterpolate.size() - 1]);
  controlPoints.emplace_back(pointsToInterpolate[pointsToInterpolate.size() - 1]);
}
} // namespace tlp
