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
#include "tulip/LayoutPropertyAnimation.h"

#include <tulip/ForEach.h>
#include <tulip/LayoutProperty.h>

using namespace std;
using namespace tlp;

LayoutPropertyAnimation::LayoutPropertyAnimation(Graph *g, LayoutProperty *start, LayoutProperty *end, LayoutProperty *out,
    BooleanProperty *selection, int frameCount, bool computeNodes, bool computeEdges,QObject* parent) :
  CachedPropertyAnimation<LayoutProperty, Coord, vector<Coord> > (g, start, end, out, selection, frameCount, computeNodes,
      computeEdges,parent) {
}

Coord LayoutPropertyAnimation::getNodeFrameValue(const Coord &startValue, const Coord &endValue, int frame) {
  pair<Coord, Coord> values(startValue, endValue);
  map<pair<Coord, Coord> , Vector<double, 3> >::iterator it = steps.find(values);
  Vector<double, 3> stepVal;

  if (it != steps.end())
    stepVal = it->second;
  else {
    for (unsigned i = 0; i < 3; ++i)
      stepVal[i] = ((double) endValue[i] - (double) startValue[i]) * 1. / (frameCount() - 1);
  }

  Coord result;

  for (unsigned i = 0; i < 3; ++i)
    result[i] = startValue[i] + frame * stepVal[i];

  return result;
}

vector<Coord> LayoutPropertyAnimation::getEdgeFrameValue(const vector<Coord> &startValue, const vector<Coord> &endValue, int frame) {
  vector<Coord> result;

  for (unsigned i = 0; i < min<unsigned> (startValue.size(), endValue.size()); ++i)
    result.push_back(getNodeFrameValue(startValue[i], endValue[i], frame));

  return result;
}

bool LayoutPropertyAnimation::equalEdges(const vector<Coord> &v1, const vector<Coord> &v2) {
  if (v1.size() != v2.size())
    return false;

  for (unsigned i = 0; i < v1.size(); ++i) {
    if (v1[i] != v2[i])
      return false;
  }

  return true;
}
