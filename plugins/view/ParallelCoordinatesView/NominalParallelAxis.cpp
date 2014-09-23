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

#include "NominalParallelAxis.h"
#include "AxisConfigDialogs.h"

#include <tulip/StringProperty.h>
#include <tulip/Iterator.h>

using namespace std;

namespace tlp {

NominalParallelAxis::NominalParallelAxis(const Coord &base_coord, const float height, const float axisAreaWidth, ParallelCoordinatesGraphProxy *graph, const std::string &propertyName, const Color &axisColor, const float rotationAngle, const GlAxis::CaptionLabelPosition captionPosition) :
  ParallelAxis(new GlNominativeAxis(propertyName, base_coord, height, GlAxis::VERTICAL_AXIS, axisColor), axisAreaWidth, rotationAngle, captionPosition), graphProxy(graph) {
  glNominativeAxis = dynamic_cast<GlNominativeAxis *>(glAxis);
  setLabels();
  ParallelAxis::redraw();
}

void
NominalParallelAxis::setLabels() {

  vector<string> labels;
  Iterator<unsigned int> *dataIt = graphProxy->getDataIterator();

  while (dataIt->hasNext()) {
    unsigned int dataId = dataIt->next();
    string labelName = graphProxy->getPropertyValueForData<StringProperty,StringType> (getAxisName(), dataId);

    if (std::find(labels.begin(), labels.end(), labelName) == labels.end()) {
      labels.push_back(labelName);
    }
  }

  delete dataIt;

  if (labelsOrder.empty() || (labelsOrder.size() != labels.size())) {
    labelsOrder = labels;
  }

  glNominativeAxis->setAxisGraduationsLabels(labelsOrder, GlAxis::RIGHT_OR_ABOVE);
}

Coord NominalParallelAxis::getPointCoordOnAxisForData(const unsigned int dataIdx) {
  string propertyValue(graphProxy->getPropertyValueForData<StringProperty, StringType>(getAxisName(), dataIdx));
  Coord axisPointCoord(glNominativeAxis->getAxisPointCoordForValue(propertyValue));

  if (rotationAngle != 0.0f) {
    rotateVector(axisPointCoord, rotationAngle, Z_ROT);
  }

  return axisPointCoord;
}

void NominalParallelAxis::showConfigDialog() {
  NominalAxisConfigDialog dialog(this);
  dialog.exec();
}

const set<unsigned int> &NominalParallelAxis::getDataInSlidersRange() {

  dataSubset.clear();
  map<string, unsigned int> labelsInRange;
  vector<string>::iterator it;

  for (it = labelsOrder.begin() ; it != labelsOrder.end() ; ++it) {
    Coord labelCoord = glNominativeAxis->getAxisPointCoordForValue(*it);

    if (labelCoord.getY() >= bottomSliderCoord.getY() && labelCoord.getY() <= topSliderCoord.getY()) {
      labelsInRange[*it] = 1;
    }
  }

  Iterator<unsigned int> *dataIt = graphProxy->getDataIterator();

  while (dataIt->hasNext()) {
    unsigned int dataId = dataIt->next();
    string labelName = graphProxy->getPropertyValueForData<StringProperty, StringType>(getAxisName(), dataId);

    if (labelsInRange.find(labelName) != labelsInRange.end()) {
      dataSubset.insert(dataId);
    }
  }

  delete dataIt;

  return dataSubset;
}

void NominalParallelAxis::updateSlidersWithDataSubset(const set<unsigned int> &dataSubset) {
  float rotAngleBak = rotationAngle;
  rotationAngle = 0.0f;
  set<unsigned int>::iterator it;
  Coord max(getBaseCoord());
  Coord min(getBaseCoord() + Coord(0.0f, getAxisHeight()));

  for (it = dataSubset.begin() ; it != dataSubset.end() ; ++it) {
    Coord labelCoord(getPointCoordOnAxisForData(*it));

    if (labelCoord.getY() < min.getY()) {
      min = labelCoord;
    }

    if (labelCoord.getY() > max.getY()) {
      max = labelCoord;
    }
  }

  bottomSliderCoord = min;
  topSliderCoord = max;
  rotationAngle = rotAngleBak;
}

void NominalParallelAxis::redraw() {
  setLabels();
  ParallelAxis::redraw();
}

}
