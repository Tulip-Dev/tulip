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

#include "tulip/DoublePropertyAnimation.h"

using namespace std;
using namespace tlp;

DoublePropertyAnimation::DoublePropertyAnimation(Graph *graph, DoubleProperty *start, DoubleProperty *end, DoubleProperty *out, BooleanProperty *selection , int frameCount , bool computeNodes , bool computeEdges , QObject *parent):CachedPropertyAnimation<DoubleProperty, double, double>(graph,start,end,out,selection,frameCount,computeNodes,computeEdges,parent) {

}

double DoublePropertyAnimation::getNodeFrameValue(const double &startValue, const double &endValue, int frame) {
  if(frame != 0) {
    return startValue + ((endValue-startValue)/(frameCount()))*frame;
  }
  else {
    return startValue;
  }
}

double DoublePropertyAnimation::getEdgeFrameValue(const double &startValue, const double &endValue, int frame) {
  if(frame != 0) {
    return startValue + ((endValue-startValue)/(frameCount()))*frame;
  }
  else {
    return startValue;
  }
}
