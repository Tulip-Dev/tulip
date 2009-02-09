/*
 * GlQuantitativeAxis.cpp
 *
 *  Created on: 5 févr. 2009
 *      Author: antoine
 */

#include <tulip/GlLine.h>

#include <sstream>
#include <algorithm>

#include "tulip/GlQuantitativeAxis.h"

template <typename T>
std::string getStringFromNumber(T number, unsigned int precision = 5) {
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
	  ascendingOrder(ascendingOrder), addArrow(addArrow), logScale(false), logBase(10) {
	if (addArrow) {
		addArrowDrawing();
	}
}

void GlQuantitativeAxis::setAxisParameters(const double minV, const double maxV, const unsigned int nbGrads,
										   const LabelPosition &axisGradsLabelsPos, const bool firstLabel) {
	min = minV;
	max = maxV;
	nbGraduations = nbGrads;
	if (min == max) {
		max += nbGraduations;
	}
	axisGradsLabelsPosition = axisGradsLabelsPos;
	drawFistLabel = firstLabel;
}

void GlQuantitativeAxis::buildAxisGraduations() {

	double minV, maxV, increment;

	if (!logScale) {
		minV = min;
		maxV = max;
	} else {
		if (min >= 1) {
			minV = minLog = log(min) / log(logBase);
			maxV = maxLog = log(max) / log(logBase);
		} else {
			minV = minLog = 0;
			maxV = maxLog = log(max - min + 1) / log(logBase);
		}
	}

	increment = (maxV - minV) / nbGraduations;

    scale = axisLength / (maxV - minV);

    vector<string> axisLabels;

	axisLabels.push_back(getStringFromNumber(min));
	for (double i = minV + increment ; i <= (maxV - increment) ; i += increment) {
		string gradLabel;
		if (!logScale) {
			gradLabel = getStringFromNumber(i);
		} else {
			double labelValue = pow(logBase, i);
			if (min < 1) {
				labelValue -= (1 - min);
			}
			gradLabel = getStringFromNumber(labelValue);
		}
		axisLabels.push_back(gradLabel);
	}
	axisLabels.push_back(getStringFromNumber(max));

	if (!ascendingOrder && (min + max) > 0) {
		reverse(axisLabels.begin(), axisLabels.end());
	}

	if (!drawFistLabel) {
		if (ascendingOrder) {
			axisLabels[0] = "";
		} else {
			axisLabels[axisLabels.size() - 1] = "";
		}
	}

	setAxisGraduations(axisLabels, axisGradsLabelsPosition);
}

Coord GlQuantitativeAxis::getAxisPointCoordForValue(double value) const {
	double offset;
	Coord ret;
	double minV, maxV;
	if (!logScale) {
		minV = min;
		maxV = max;
	} else {
		minV = minLog;
		maxV = maxLog;
	}

	double val = value;
	if (logScale) {
		if (val < 1) {
			val += (1 - min);
		}
		val = log(val) / log(logBase);
	}
	if (ascendingOrder) {
		 offset = (val - minV) * scale;
	} else {
		offset = (maxV - val) * scale;
	}
	if (axisOrientation == HORIZONTAL_AXIS) {
		ret = Coord(axisBaseCoord.getX() + offset, axisBaseCoord.getY(), 0);
	} else if (axisOrientation == VERTICAL_AXIS) {
		ret = Coord(axisBaseCoord.getX(), axisBaseCoord.getY() + offset, 0);
	}
	return ret;
}

double GlQuantitativeAxis::getValueForAxisPoint(const Coord &axisPointCoord) {
	double offset = 0;
	if (axisOrientation == HORIZONTAL_AXIS) {
		offset = axisPointCoord.getX() - axisBaseCoord.getX();
	} else if (axisOrientation == VERTICAL_AXIS) {
		offset = axisPointCoord.getY() - axisBaseCoord.getY();
	}
	double minV, maxV;
	if (!logScale) {
		minV = min;
		maxV = max;
	} else {
		minV = minLog;
		maxV = maxLog;
	}

	double value;
	if (ascendingOrder) {
		value = minV + (offset / scale);
	} else {
		value = maxV - (offset / scale);
	}
	if (logScale) {
		value = pow(logBase, value);
		if (min < 1) {
			value -= (1 - min);
		}
	}

	return value;
}

void GlQuantitativeAxis::addArrowDrawing() {
	GlLine *arrowLine1 = new GlLine();
	GlLine *arrowLine2 = new GlLine();
	GlLine *arrowLine3 = new GlLine();
	arrowLine1->setStencil(1);
	arrowLine2->setStencil(1);
	arrowLine3->setStencil(1);
	float axisExtensionLength = axisLength / 20.;
	float arrowWidth = axisLength / 20.;
	float arrowLength = (1./2.) * axisExtensionLength;
	Coord arrowEndCoord;
	if (axisOrientation == HORIZONTAL_AXIS) {
		if (ascendingOrder) {
			arrowEndCoord = Coord(axisBaseCoord.getX() + axisLength + axisExtensionLength, axisBaseCoord.getY());
			arrowLine1->addPoint(Coord(axisBaseCoord.getX() + axisLength, axisBaseCoord.getY()), axisColor);
			arrowLine1->addPoint(arrowEndCoord, axisColor);
			arrowLine2->addPoint(arrowEndCoord, axisColor);
			arrowLine2->addPoint(Coord(axisBaseCoord.getX() + axisLength + (axisExtensionLength - arrowLength), axisBaseCoord.getY() + arrowWidth / 2), axisColor);
			arrowLine3->addPoint(arrowEndCoord, axisColor);
			arrowLine3->addPoint(Coord(axisBaseCoord.getX() + axisLength + (axisExtensionLength - arrowLength) , axisBaseCoord.getY() - arrowWidth / 2), axisColor);
		} else {
			arrowEndCoord = Coord(axisBaseCoord.getX() - axisExtensionLength, axisBaseCoord.getY());
			arrowLine1->addPoint(axisBaseCoord, axisColor);
			arrowLine1->addPoint(arrowEndCoord, axisColor);
			arrowLine2->addPoint(arrowEndCoord, axisColor);
			arrowLine2->addPoint(Coord(axisBaseCoord.getX() - (axisExtensionLength - arrowLength), axisBaseCoord.getY() + arrowWidth / 2), axisColor);
			arrowLine3->addPoint(arrowEndCoord, axisColor);
			arrowLine3->addPoint(Coord(axisBaseCoord.getX() - (axisExtensionLength - arrowLength), axisBaseCoord.getY() - arrowWidth / 2), axisColor);
		}
	} else if (axisOrientation == VERTICAL_AXIS) {
		if (ascendingOrder) {
			arrowEndCoord = Coord(axisBaseCoord.getX(), axisBaseCoord.getY() + axisLength + axisExtensionLength);
			arrowLine1->addPoint(Coord(axisBaseCoord.getX(), axisBaseCoord.getY() + axisLength), axisColor);
			arrowLine1->addPoint(arrowEndCoord, axisColor);
			arrowLine2->addPoint(arrowEndCoord, axisColor);
			arrowLine2->addPoint(Coord(axisBaseCoord.getX() + arrowWidth / 2, axisBaseCoord.getY() + axisLength + (axisExtensionLength - arrowLength)), axisColor);
			arrowLine3->addPoint(arrowEndCoord, axisColor);
			arrowLine3->addPoint(Coord(axisBaseCoord.getX() - arrowWidth / 2, axisBaseCoord.getY() + axisLength + (axisExtensionLength - arrowLength)), axisColor);
		} else {
			arrowEndCoord = Coord(axisBaseCoord.getX(), axisBaseCoord.getY() - axisExtensionLength);
			arrowLine1->addPoint(axisBaseCoord, axisColor);
			arrowLine1->addPoint(arrowEndCoord, axisColor);
			arrowLine2->addPoint(arrowEndCoord, axisColor);
			arrowLine2->addPoint(Coord(axisBaseCoord.getX() + arrowWidth / 2, axisBaseCoord.getY() - (axisExtensionLength - arrowLength)), axisColor);
			arrowLine3->addPoint(arrowEndCoord, axisColor);
			arrowLine3->addPoint(Coord(axisBaseCoord.getX() - arrowWidth / 2, axisBaseCoord.getY() - (axisExtensionLength - arrowLength)), axisColor);
		}
	}
	ostringstream oss;
	oss << axisName << " axis arrow line 1";
	addGlEntity(arrowLine1, oss.str());
	oss.str("");
	oss << axisName << " axis arrow line 2";
	addGlEntity(arrowLine2, oss.str());
	oss.str("");
	oss << axisName << " axis arrow line 3";
	addGlEntity(arrowLine3, oss.str());
}

void GlQuantitativeAxis::updateAxis() {
	GlAxis::updateAxis();
	if (addArrow) {
		addArrowDrawing();
	}
	buildAxisGraduations();
}

void GlQuantitativeAxis::setLogScale(const bool log, const unsigned int base) {
	logScale = log;
	logBase = base;
}

}
