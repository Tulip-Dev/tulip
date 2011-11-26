/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

#include <tulip/GlQuad.h>
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
                                       const AxisOrientation &axisOrientation, const Color &axisColor, const float axisWidth,
                                       const bool addArrow, const bool ascendingOrder)
  : GlAxis(axisName, axisBaseCoord, axisLength, axisOrientation, axisColor, axisWidth) ,
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


void GlQuantitativeAxis::setAxisParameters(const int minV, const int maxV, const unsigned int incrementStepV,
    const LabelPosition &axisGradsLabelsPos, const bool firstLabel) {
  integerScale = true;
  min = minV;
  int maxVCp = maxV;

  if (incrementStepV) {
    while (((maxVCp - minV)%incrementStepV) != 0) ++maxVCp;

    incrementStep = incrementStepV;
  }
  else {
    incrementStep = 1;
  }

  max = maxVCp;

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
      minV = minLog = log(min) / log(static_cast<double>(logBase));
      maxV = maxLog = log(max) / log(static_cast<double>(logBase));
    }
    else {
      minV = minLog = 0;
      maxV = maxLog = log(1 + max - min) / log(static_cast<double>(logBase));
    }
  }

  if (!integerScale || (integerScale && logScale)) {
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
      double labelValue = pow(static_cast<double>(logBase), i);

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

    val = log(val) / log(static_cast<double>(logBase));
  }

  if (ascendingOrder) {
    offset = static_cast<float>((val - minV) * scale);
  }
  else {
    offset = static_cast<float>((maxV - val) * scale);
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
    value = pow(static_cast<double>(logBase), value);

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
  GlQuad *arrowLine = new GlQuad();
  arrowLine->setStencil(2);



  float axisExtensionLength = captionOffset;
  float arrowWidth = axisGradsWidth;
  float arrowLength = 0.5f * axisExtensionLength;
  Coord arrowStartCoord, arrowEndCoord;
  vector<Coord> trianglePoints;

  if (axisOrientation == HORIZONTAL_AXIS) {
    if (ascendingOrder) {
      arrowStartCoord = Coord(axisBaseCoord.getX() + axisLength + (axisExtensionLength - arrowLength), axisBaseCoord.getY());
      arrowEndCoord = Coord(axisBaseCoord.getX() + axisLength + axisExtensionLength, axisBaseCoord.getY());
      arrowLine->setPosition(0, Coord(axisBaseCoord.getX() + axisLength, axisBaseCoord.getY()) - Coord(0, axisWidth/2));
      arrowLine->setPosition(1, Coord(axisBaseCoord.getX() + axisLength, axisBaseCoord.getY()) + Coord(0, axisWidth/2));
      arrowLine->setPosition(2, Coord(arrowStartCoord.getX(), arrowStartCoord.getY()) + Coord(0, axisWidth/2));
      arrowLine->setPosition(3, Coord(arrowStartCoord.getX(), arrowStartCoord.getY()) - Coord(0, axisWidth/2));
      trianglePoints.push_back(arrowStartCoord - Coord(0, arrowWidth/2));
      trianglePoints.push_back(arrowStartCoord + Coord(0, arrowWidth/2));
      trianglePoints.push_back(arrowEndCoord);
    }
    else {
      arrowStartCoord = Coord(axisBaseCoord.getX() - (axisExtensionLength - arrowLength) , axisBaseCoord.getY());
      arrowEndCoord = Coord(axisBaseCoord.getX() - axisExtensionLength, axisBaseCoord.getY());
      arrowLine->setPosition(0, Coord(axisBaseCoord.getX(), axisBaseCoord.getY()) - Coord(0, axisWidth/2));
      arrowLine->setPosition(1, Coord(axisBaseCoord.getX(), axisBaseCoord.getY()) + Coord(0, axisWidth/2));
      arrowLine->setPosition(2, Coord(arrowStartCoord.getX(), arrowStartCoord.getY()) + Coord(0, axisWidth/2));
      arrowLine->setPosition(3, Coord(arrowStartCoord.getX(), arrowStartCoord.getY()) - Coord(0, axisWidth/2));
      trianglePoints.push_back(arrowStartCoord - Coord(0, arrowWidth/2));
      trianglePoints.push_back(arrowStartCoord + Coord(0, arrowWidth/2));
      trianglePoints.push_back(arrowEndCoord);
    }
  }
  else if (axisOrientation == VERTICAL_AXIS) {
    if (ascendingOrder) {
      arrowStartCoord = Coord(axisBaseCoord.getX(), axisBaseCoord.getY() + axisLength + (axisExtensionLength - arrowLength));
      arrowEndCoord = Coord(axisBaseCoord.getX(), axisBaseCoord.getY() + axisLength + axisExtensionLength);
      arrowLine->setPosition(0, Coord(axisBaseCoord.getX(), axisBaseCoord.getY() + axisLength) - Coord(axisWidth/2, 0));
      arrowLine->setPosition(1, Coord(axisBaseCoord.getX(), axisBaseCoord.getY() + axisLength) + Coord(axisWidth/2, 0));
      arrowLine->setPosition(2, Coord(arrowStartCoord.getX(), arrowStartCoord.getY()) + Coord(axisWidth/2, 0));
      arrowLine->setPosition(3, Coord(arrowStartCoord.getX(), arrowStartCoord.getY()) - Coord(axisWidth/2, 0));
      trianglePoints.push_back(arrowStartCoord - Coord(arrowWidth/2, 0));
      trianglePoints.push_back(arrowStartCoord + Coord(arrowWidth/2, 0));
      trianglePoints.push_back(arrowEndCoord);
    }
    else {
      arrowStartCoord = Coord(axisBaseCoord.getX(), axisBaseCoord.getY() - (axisExtensionLength - arrowLength));
      arrowEndCoord = Coord(axisBaseCoord.getX(), axisBaseCoord.getY() - axisExtensionLength);
      arrowLine->setPosition(0, Coord(axisBaseCoord.getX(), axisBaseCoord.getY()) - Coord(axisWidth/2, 0));
      arrowLine->setPosition(1, Coord(axisBaseCoord.getX(), axisBaseCoord.getY()) + Coord(axisWidth/2, 0));
      arrowLine->setPosition(2, Coord(arrowStartCoord.getX(), arrowStartCoord.getY()) + Coord(axisWidth/2, 0));
      arrowLine->setPosition(3, Coord(arrowStartCoord.getX(), arrowStartCoord.getY()) - Coord(axisWidth/2, 0));
      trianglePoints.push_back(arrowStartCoord - Coord(arrowWidth/2, 0));
      trianglePoints.push_back(arrowStartCoord + Coord(arrowWidth/2, 0));
      trianglePoints.push_back(arrowEndCoord);
    }
  }


  vector<Color> colors;
  colors.resize(3, Color(0,0,0));
  GlPolygon *arrowTriangle = new GlPolygon(trianglePoints, colors, colors, true, true);
  arrowTriangle->setStencil(2);


  ostringstream oss;
  oss << axisName << " axis arrow line";
  axisLinesComposite->addGlEntity(arrowLine, oss.str());
  oss.str("");
  oss << axisName << " axis arrow triangle";
  axisLinesComposite->addGlEntity(arrowTriangle, oss.str());
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
