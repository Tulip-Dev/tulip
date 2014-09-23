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

#include <tulip/GlNominativeAxis.h>

using namespace std;

namespace tlp {

GlNominativeAxis::GlNominativeAxis(const string &axisName, const Coord &axisBaseCoord, const float axisLength,
                                   const AxisOrientation &axisOrientation, const Color &axisColor)
  : GlAxis(axisName, axisBaseCoord, axisLength, axisOrientation, axisColor) {}

void GlNominativeAxis::setAxisGraduationsLabels(const std::vector<std::string> &axisGradsLabels, const LabelPosition &labelsPos) {
  labelsOrder = axisGradsLabels;
  axisLabelsPosition = labelsPos;
}

void GlNominativeAxis::buildAxisGraduations() {
  setAxisGraduations(labelsOrder, axisLabelsPosition);
  labelsCoord.clear();

  for (unsigned int i = 0 ; i < labelsOrder.size() ; ++i) {
    if (axisOrientation == HORIZONTAL_AXIS) {
      labelsCoord[labelsOrder[i]] = Coord(axisBaseCoord.getX() + i * spaceBetweenAxisGrads, axisBaseCoord.getY());
    }
    else if (axisOrientation == VERTICAL_AXIS) {
      labelsCoord[labelsOrder[i]] = Coord(axisBaseCoord.getX(), axisBaseCoord.getY() + i * spaceBetweenAxisGrads);
    }
  }
}

Coord GlNominativeAxis::getAxisPointCoordForValue(const string& value)  {
  Coord ret;

  if (labelsCoord.find(value) != labelsCoord.end()) {
    ret = labelsCoord[value];
  }

  return ret;
}

string GlNominativeAxis::getValueAtAxisPoint(const Coord &axisPointCoord) {
  string ret;
  map<string, Coord>::iterator it;

  for (it = labelsCoord.begin() ; it != labelsCoord.end() ; ++it) {
    if (it->second == axisPointCoord) {
      ret = it->first;
      break;
    }
  }

  return ret;
}

void GlNominativeAxis::translate(const Coord &c) {
  GlAxis::translate(c);
  map<string, Coord>::iterator it;

  for (it = labelsCoord.begin() ; it != labelsCoord.end() ; ++it) {
    it->second += c;
  }
}

void GlNominativeAxis::updateAxis() {
  buildAxisGraduations();
  GlAxis::updateAxis();
}

}
