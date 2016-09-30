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

/**
 *
 * tulip-ogles library : rewrite of old tulip-ogl library using OpenGL ES API
 * Copyright (c) 2016 Antoine Lambert, Thales Services SAS
 * antoine-e.lambert@thalesgroup.com / antoine.lambert33@gmail.com
 *
 */

#ifndef CONCAVEHULLBUILDER_H
#define CONCAVEHULLBUILDER_H

#include <tulip/Coord.h>

#include <vector>

namespace tlp {

class Graph;

class TLP_GLES_SCOPE ConcaveHullBuilder {

public:
  ConcaveHullBuilder();

  void setScalingFactor(const float scalingFactor);

  void setHullWithHoles(const bool hullWithHoles);

  void setHullZValue(const float z);

  void addPolygon(const std::vector<tlp::Coord> &polygon);

  void computeHulls();

  unsigned int nbComputedHulls() const;

  const std::vector<std::vector<tlp::Coord>> &getHullWithHoles(unsigned int hullId) const;

  const std::vector<tlp::Coord> &getHullOuterContour(unsigned int hullId) const;

  void clear();

  static std::vector<std::vector<tlp::Coord>> computeGraphHullVertices(tlp::Graph *graph, const bool withHoles = true, const float spacing = 0.1f,
                                                                       const float z = 0);

private:
  float _scalingFactor;

  bool _hullWithHoles;

  float _z;

  std::vector<std::vector<tlp::Coord>> _polygons;

  std::vector<std::vector<std::vector<tlp::Coord>>> _computedHulls;
};
}

#endif // CONCAVEHULLBUILDER_H
