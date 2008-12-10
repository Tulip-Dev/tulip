#include "NominalParallelAxis.h"
#include "AxisConfigDialogs.h"

#include <tulip/StringProperty.h>
#include <tulip/Iterator.h>

using namespace std;

namespace tlp {

NominalParallelAxis::NominalParallelAxis(const Coord &base_coord, const float height, const float axisAreaWidth, ParallelCoordinatesGraphProxy *graph, const std::string &propertyName, const Color &axisColor) :
  ParallelAxis(base_coord, height, axisAreaWidth, propertyName, axisColor), graphProxy(graph) {
  setLabels();
}

void
NominalParallelAxis::setLabels() {

	Iterator<unsigned int> *dataIt = graphProxy->getDataIterator();
	while (dataIt->hasNext()) {
		unsigned int dataId = dataIt->next();
		string labelName = graphProxy->getPropertyValueForData<StringProperty,StringType> (axisName, dataId);
		if (labelsCoords.find(labelName) == labelsCoords.end()) {
			labelsCoords[labelName] = Coord(0, 0, 0);
			labelsOrder.push_back(labelName);
		}
	}
	delete dataIt;

	spaceBetweenAxisGrads = axisHeight / (labelsOrder.size() + 1);

	unsigned int idx = 1;
	unsigned int nbLabs = labelsOrder.size();
	float ySpace = axisHeight / (nbLabs + 1);
	vector<string>::iterator it2;
	for (it2 = labelsOrder.begin(); it2 != labelsOrder.end(); ++it2) {
		float xCoord = baseCoord.getX();
		float yCoord = (baseCoord.getY() + axisHeight) - (idx++) * ySpace;
		addLabelDrawing(*it2, yCoord);
		labelsCoords[*it2] = Coord(xCoord, yCoord);
	}
}

Coord NominalParallelAxis::getPointCoordOnAxisForData(const unsigned int dataIdx) {
  string propertyValue = graphProxy->getPropertyValueForData<StringProperty, StringType>(axisName, dataIdx);
  return labelsCoords[propertyValue];
}

void NominalParallelAxis::translate(const Coord &c) {
	ParallelAxis::translate(c);
	map<string, Coord>::iterator it;
	for (it = labelsCoords.begin() ; it !=labelsCoords.end() ; ++it) {
		it->second += c;
	}
}

void NominalParallelAxis::redraw() {
	drawAxisLine();
	setLabels();
	addCaption(axisName);
}

void NominalParallelAxis::showConfigDialog() {
	NominalAxisConfigDialog dialog(this);
	dialog.exec();
}

set<unsigned int> NominalParallelAxis::getDataInSlidersRange() {

	map<string, unsigned int> labelsInRange;
	std::map<std::string, Coord>::iterator it;
	for (it = labelsCoords.begin() ; it != labelsCoords.end() ; ++it) {
		if ((it->second).getY() >= bottomSliderCoord.getY() && (it->second).getY() <= topSliderCoord.getY()) {
			labelsInRange[it->first] = 1;
		}
	}

	set<unsigned int> dataInSlidersRange;
	Iterator<unsigned int> *dataIt = graphProxy->getDataIterator();
	while (dataIt->hasNext()) {
		unsigned int dataId = dataIt->next();
		string labelName = graphProxy->getPropertyValueForData<StringProperty, StringType>(axisName, dataId);
		if (labelsInRange.find(labelName) != labelsInRange.end()) {
			dataInSlidersRange.insert(dataId);
		}
	}
	delete dataIt;

	return dataInSlidersRange;
}

void NominalParallelAxis::updateSlidersWithDataSubset(const set<unsigned int> &dataSubset) {
	set<unsigned int>::iterator it;
	Coord max = baseCoord;
	Coord min = baseCoord + Coord(0, axisHeight);
	for (it = dataSubset.begin() ; it != dataSubset.end() ; ++it) {
		string labelName = graphProxy->getPropertyValueForData<StringProperty, StringType>(axisName, *it);
		if (labelsCoords[labelName].getY() < min.getY()) {
			min = labelsCoords[labelName];
		}

		if (labelsCoords[labelName].getY() > max.getY()) {
			max = labelsCoords[labelName];
		}
	}
	bottomSliderCoord = min;
	topSliderCoord = max;
}

}
