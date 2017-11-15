/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include <tulip/GlLine.h>
#include <tulip/GlPolygon.h>
#include <tulip/GlTriangle.h>

#include <sstream>
#include <algorithm>

#include <tulip/GlQuantitativeAxis.h>

template <typename T>
std::string getStringFromNumber(T number, unsigned int precision = 4) {
  std::ostringstream oss;
  oss.precision(precision);
  oss << number;
  return oss.str();
}

using namespace std;

namespace tlp {

GlQuantitativeAxis::GlQuantitativeAxis(const std::string &axisName, const Coord &axisBaseCoord, const float axisLength,
                                       const AxisOrientation &axisOrientation, const Color &axisColor,
                                       const bool addArrow, const bool ascendingOrder)
  : GlAxis(axisName, axisBaseCoord, axisLength, axisOrientation, axisColor) ,
    ascendingOrder(ascendingOrder), addArrow(addArrow), logScale(false), logBase(10),
    integerScale(false), incrementStep(0), minMaxSet(false) {
  if (addArrow) {
    addArrowDrawing();
  }
}

void GlQuantitativeAxis::setAxisParameters(const double minV, const double maxV, const unsigned int nbGrads,
    const LabelPosition &axisGradsLabelsPos, const bool firstLabel) {
  integerScale = false;
  min = minV;
  max = maxV;
  nbGraduations = nbGrads;

  if (min == max) {
    max += nbGraduations;
  }

  axisGradsLabelsPosition = axisGradsLabelsPos;
  drawFistLabel = firstLabel;
  minMaxSet = true;
}


void GlQuantitativeAxis::setAxisParameters(const long long minV, const long long maxV, const unsigned long long incrementStepV,
    const LabelPosition &axisGradsLabelsPos, const bool firstLabel) {
  integerScale = true;
  min = minV;
  long long maxVCp = maxV;

  if (incrementStepV)
    while (((maxVCp - minV)%incrementStepV) != 0) ++maxVCp;

  max = maxVCp;
  incrementStep = incrementStepV;

  if (min == max) {
    max += incrementStep;
  }

  axisGradsLabelsPosition = axisGradsLabelsPos;
  drawFistLabel = firstLabel;
  minMaxSet = true;
  nbGraduations = (maxV - minV) / (incrementStepV > 0 ? incrementStepV : 1) + 1;
}

void GlQuantitativeAxis::buildAxisGraduations() {

  double minV, maxV, increment;

  if (!logScale) {
    minV = min;
    maxV = max;
  }
  else {
    if (min >= 1) {
      minV = minLog = log(min) / log(double(logBase));
      maxV = maxLog = log(max) / log(double(logBase));
    }
    else {
      minV = minLog = 0;
      maxV = maxLog = log(1 + max - min) / log(double(logBase));
    }
  }

  if (!integerScale || logScale) {
    increment = (maxV - minV) / (nbGraduations - 1);
  }
  else {
    increment = incrementStep;
  }

  scale = axisLength / (maxV - minV);

  vector<string> axisLabels;

  string minStr = getStringFromNumber(min);

  if (minStr.size() == 1) {
    minStr = " " + minStr;
  }

  axisLabels.push_back(minStr);

  for (double i = minV + increment ; i < maxV ; i += increment) {

    if (axisLabels.size() == nbGraduations - 1)
      break;

    string gradLabel;

    if (!logScale) {
      gradLabel = getStringFromNumber(i);
    }
    else {
      double labelValue = pow(double(logBase), i);

      if (min < 1) {
        labelValue -= (1 - min);
      }

      gradLabel = getStringFromNumber(labelValue);
    }

    if (gradLabel.size() == 1) {
      gradLabel = " " + gradLabel;
    }

    axisLabels.push_back(gradLabel);
  }

  string maxStr = getStringFromNumber(max);

  if (maxStr.size() == 1) {
    maxStr = " " + maxStr;
  }

  axisLabels.push_back(maxStr);

  if (!ascendingOrder && (min + max) > 0) {
    reverse(axisLabels.begin(), axisLabels.end());
  }

  if (!drawFistLabel) {
    if (ascendingOrder) {
      axisLabels[0] = " ";
    }
    else {
      axisLabels[axisLabels.size() - 1] = " ";
    }
  }

  setAxisGraduations(axisLabels, axisGradsLabelsPosition);
}

Coord GlQuantitativeAxis::getAxisPointCoordForValue(double value) const {
  float offset;
  Coord ret;
  double minV, maxV;

  if (!logScale) {
    minV = min;
    maxV = max;
  }
  else {
    minV = minLog;
    maxV = maxLog;
  }

  double val = value;

  if (logScale) {
    if (min < 1) {
      val += (1 - min);
    }

    val = log(val) / log(double(logBase));
  }

  if (ascendingOrder) {
    offset = (val - minV) * scale;
  }
  else {
    offset = (maxV - val) * scale;
  }

  if (axisOrientation == HORIZONTAL_AXIS) {
    ret = Coord(axisBaseCoord.getX() + offset, axisBaseCoord.getY(), 0);
  }
  else if (axisOrientation == VERTICAL_AXIS) {
    ret = Coord(axisBaseCoord.getX(), axisBaseCoord.getY() + offset, 0);
  }

  return ret;
}

double GlQuantitativeAxis::getValueForAxisPoint(const Coord &axisPointCoord) {
  double offset = 0;

  if (axisOrientation == HORIZONTAL_AXIS) {
    offset = axisPointCoord.getX() - axisBaseCoord.getX();
  }
  else if (axisOrientation == VERTICAL_AXIS) {
    offset = axisPointCoord.getY() - axisBaseCoord.getY();
  }

  double minV, maxV;

  if (!logScale) {
    minV = min;
    maxV = max;
  }
  else {
    minV = minLog;
    maxV = maxLog;
  }

  double value;

  if (ascendingOrder) {
    value = minV + (offset / scale);
  }
  else {
    value = maxV - (offset / scale);
  }

  if (logScale) {
    value = pow(double(logBase), value);

    if (min < 1) {
      value -= (1 - min);
    }
  }

  if (!integerScale) {
    return value;
  }
  else {
    return ceil(value);
  }
}

void GlQuantitativeAxis::addArrowDrawing() {
  GlLine *arrowLine = new GlLine();
  GlTriangle *arrow;

  arrowLine->setStencil(1);
  arrowLine->setLineWidth(2.0);
  float axisExtensionLength = captionOffset;
  float arrowOrientation;
  float pi = 3.141592654f;
  Coord arrowEndCoord;
  Size arrowSize = Size(axisLength/50, axisLength/50, 0);

  if (axisOrientation == HORIZONTAL_AXIS) {
    if (ascendingOrder) {
      arrowEndCoord = Coord(axisBaseCoord.getX() + axisLength + axisExtensionLength, axisBaseCoord.getY());
      arrowOrientation = 0.0;
      arrowLine->addPoint(Coord(axisBaseCoord.getX() + axisLength, axisBaseCoord.getY()), axisColor);
      arrowLine->addPoint(arrowEndCoord, axisColor);
    }
    else {
      arrowEndCoord = Coord(axisBaseCoord.getX() - axisExtensionLength, axisBaseCoord.getY());
      arrowOrientation = pi;
      arrowLine->addPoint(axisBaseCoord, axisColor);
      arrowLine->addPoint(arrowEndCoord, axisColor);
    }
  }
  else {
    assert(axisOrientation == VERTICAL_AXIS);

    if (ascendingOrder) {
      arrowEndCoord = Coord(axisBaseCoord.getX(), axisBaseCoord.getY() + axisLength + axisExtensionLength);
      arrowOrientation = 1./2.*pi;
      arrowLine->addPoint(Coord(axisBaseCoord.getX(), axisBaseCoord.getY() + axisLength), axisColor);
      arrowLine->addPoint(arrowEndCoord, axisColor);
    }
    else {
      arrowEndCoord = Coord(axisBaseCoord.getX(), axisBaseCoord.getY() - axisExtensionLength);
      arrowOrientation = -1./2.*pi;
      arrowLine->addPoint(axisBaseCoord, axisColor);
      arrowLine->addPoint(arrowEndCoord, axisColor);
    }
  }

  arrow = new GlTriangle(arrowEndCoord, arrowSize, axisColor, axisColor, true, true);
  arrow->setStartAngle(arrowOrientation);
  arrow->setStencil(1);

  ostringstream oss;
  oss << axisName << " axis arrow line";
  axisLinesComposite->addGlEntity(arrowLine, oss.str());
  oss.str("");
  oss << axisName << " axis arrow";
  axisLinesComposite->addGlEntity(arrow, oss.str());
  computeBoundingBox();
}

void GlQuantitativeAxis::updateAxis() {
  if (minMaxSet) {
    buildAxisGraduations();
  }

  GlAxis::updateAxis();

  if (addArrow) {
    addArrowDrawing();
  }
}

void GlQuantitativeAxis::setLogScale(const bool log, const unsigned int base) {
  logScale = log;
  logBase = base;
}

}
