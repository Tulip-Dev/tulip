#include "NominalParallelAxis.h"
#include "AxisConfigDialogs.h"

#include <tulip/StringProperty.h>
#include <tulip/Iterator.h>

using namespace std;

namespace tlp {

NominalParallelAxis::NominalParallelAxis(const Coord &base_coord, const float height, const float axisAreaWidth, ParallelCoordinatesGraphProxy *graph, const std::string &propertyName, const Color &axisColor) :
  ParallelAxis(new GlNominativeAxis(propertyName, base_coord, height, GlAxis::VERTICAL_AXIS, axisColor), axisAreaWidth), graphProxy(graph) {
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

	if (labelsOrder.size() == 0 || (labelsOrder.size() > 0 && labelsOrder.size() != labels.size())) {
		labelsOrder = labels;
	}

	glNominativeAxis->setAxisGraduationsLabels(labelsOrder, GlAxis::RIGHT_OR_ABOVE);
}

Coord NominalParallelAxis::getPointCoordOnAxisForData(const unsigned int dataIdx) {
  string propertyValue = graphProxy->getPropertyValueForData<StringProperty, StringType>(getAxisName(), dataIdx);
  return glNominativeAxis->getAxisPointCoordForValue(propertyValue);
}

void NominalParallelAxis::showConfigDialog() {
	NominalAxisConfigDialog dialog(this);
	dialog.exec();
}

set<unsigned int> NominalParallelAxis::getDataInSlidersRange() {

	map<string, unsigned int> labelsInRange;
	vector<string>::iterator it;
	for (it = labelsOrder.begin() ; it != labelsOrder.end() ; ++it) {
		Coord labelCoord = glNominativeAxis->getAxisPointCoordForValue(*it);
		if (labelCoord.getY() >= bottomSliderCoord.getY() && labelCoord.getY() <= topSliderCoord.getY()) {
			labelsInRange[*it] = 1;
		}
	}

	set<unsigned int> dataInSlidersRange;
	Iterator<unsigned int> *dataIt = graphProxy->getDataIterator();
	while (dataIt->hasNext()) {
		unsigned int dataId = dataIt->next();
		string labelName = graphProxy->getPropertyValueForData<StringProperty, StringType>(getAxisName(), dataId);
		if (labelsInRange.find(labelName) != labelsInRange.end()) {
			dataInSlidersRange.insert(dataId);
		}
	}
	delete dataIt;

	return dataInSlidersRange;
}

void NominalParallelAxis::updateSlidersWithDataSubset(const set<unsigned int> &dataSubset) {
	set<unsigned int>::iterator it;
	Coord max = getBaseCoord();
	Coord min = getBaseCoord() + Coord(0, getAxisHeight());
	for (it = dataSubset.begin() ; it != dataSubset.end() ; ++it) {
		Coord labelCoord = getPointCoordOnAxisForData(*it);
		if (labelCoord.getY() < min.getY()) {
			min = labelCoord;
		}

		if (labelCoord.getY() > max.getY()) {
			max = labelCoord;
		}
	}
	bottomSliderCoord = min;
	topSliderCoord = max;
}

void NominalParallelAxis::redraw() {
	setLabels();
	ParallelAxis::redraw();
}

}
