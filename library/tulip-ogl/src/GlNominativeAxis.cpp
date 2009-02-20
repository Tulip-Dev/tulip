/*
 * GlNominativeAxis.cpp
 *
 *  Created on: 6 févr. 2009
 *      Author: antoine
 */

#include "tulip/GlNominativeAxis.h"

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
		} else if (axisOrientation == VERTICAL_AXIS) {
			labelsCoord[labelsOrder[i]] = Coord(axisBaseCoord.getX(), axisBaseCoord.getY() + i * spaceBetweenAxisGrads);
		}
	}
}

Coord GlNominativeAxis::getAxisPointCoordForValue(string value)  {
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
