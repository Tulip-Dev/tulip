#include <tulip/GlCircle.h>
#include <tulip/GlCurve.h>
#include <tulip/BooleanProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/Iterator.h>
#include <tulip/BoundingBox.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/GlPolyQuad.h>

#include "ParallelCoordinatesDrawing.h"
#include "NominalParallelAxis.h"
#include "QuantitativeParallelAxis.h"
#include "GlNodeGlyph.h"
#include "ParallelTools.h"

#include <sstream>
#include <climits>
#include <algorithm>
#include <ctime>

namespace tlp {

ParallelCoordinatesDrawing::ParallelCoordinatesDrawing(ParallelCoordinatesGraphProxy *graph) :
	firstAxisPos(Coord(0,0,0)), height(DEFAULT_AXIS_HEIGHT), spaceBetweenAxis(height/2), linesColorAlphaValue(DEFAULT_LINES_COLOR_ALPHA_VALUE), drawPointsOnAxis(true),
	graphProxy(graph), vType(VIEW_2D), backgroundColor(Color(255,255,255)), createAxisFlag(true), elementDeleted(false) {

	graphData = new GlGraphInputData(graphProxy, &graphRenderingParameters);
	dataPlotComposite = new GlComposite();
	axisPlotComposite = new GlComposite();
	addGlEntity(dataPlotComposite, "data plot composite");
	addGlEntity(axisPlotComposite, "axis plot composite");
}

ParallelCoordinatesDrawing::~ParallelCoordinatesDrawing() {
	erase();
	reset(true);
}

void ParallelCoordinatesDrawing::createAxis() {

	string propertyName;
	unsigned int pos = 0;
	vector<string> l = graphProxy->getSelectedProperties();
	vector<string>::iterator it;

	map<string, ParallelAxis *>::iterator it2;
	for (it2 = parallelAxis.begin() ; it2 != parallelAxis.end() ; ++it2) {
		(it2->second)->setVisible(false);
	}

	nbAxis = graphProxy->getNumberOfSelectedProperties();
	axisOrder.clear();
	if (nbAxis > 1) {
		width = spaceBetweenAxis * (nbAxis - 1);
	} else {
		width = spaceBetweenAxis;
	}

	Color axisColor;
	int bgV = backgroundColor.getV();
	if (bgV < 128) {
		axisColor = Color(255,255,255);
	} else {
		axisColor = Color(0,0,0);
	}

	double maxCaptionWidth = (8./10.) * spaceBetweenAxis;

	for (it = l.begin(); it != l.end() ; ++it) {

		ParallelAxis *axis = NULL;

		Coord coord;
		if (nbAxis!=1) {
			coord = Coord(firstAxisPos.getX() + pos * (width/(nbAxis - 1)),firstAxisPos.getY());
		} else {
			coord = Coord(firstAxisPos.getX() + (width/2), firstAxisPos.getY());
		}

		if (parallelAxis.find(*it) != parallelAxis.end()) {
			axis = (parallelAxis.find(*it))->second;
			axis->translate(coord - axis->getBaseCoord());
			axis->setAxisHeight(height);
			axis->setMaxCaptionWidth(maxCaptionWidth);
			axis->setAxisColor(axisColor);
			axis->redraw();
			axis->setVisible(true);
		} else {
			string typeName = (graphProxy->getProperty(*it))->getTypename();
			if (typeName == "string") {
				axis = new NominalParallelAxis(coord, height, maxCaptionWidth, graphProxy, *it, axisColor);
			} else if (typeName == "int" || typeName == "double") {
				axis = new QuantitativeParallelAxis(coord, height, maxCaptionWidth, graphProxy, *it, true, axisColor);
			}
		}

		if (axis != NULL) {
			axis->computeBoundingBox();
			axisPlotComposite->addGlEntity(axis, *it);
			axisOrder.push_back(*it);
			parallelAxis[*it] = axis;
			++pos;
		}
	}
}

void ParallelCoordinatesDrawing::plotAllData() {
	Color color;
	computeResizeFactor();
	Iterator<unsigned int> *dataIt = graphProxy->getDataIterator();
	while (dataIt->hasNext()) {
		unsigned int dataId = dataIt->next();
		if (graphProxy->isDataSelected(dataId)) {
			color = COLOR_SELECT;
		} else {
			color = graphProxy->getDataColor(dataId);
			color.setA(linesColorAlphaValue);
		}

		plotData(dataId, color);
		++nbDataProcessed;
	}
	delete dataIt;

	lastHighlightedElements = graphProxy->getHighlightedElts();
	elementDeleted = false;
}

void ParallelCoordinatesDrawing::plotData(const unsigned int dataId, const Color &color) {

	Size dataViewSize = graphProxy->getDataViewSize(dataId);
	Size adjustedViewSize = axisPointMinSize + resizeFactor * (dataViewSize + Size(-1,-1,-1));
	float pointRadius =((adjustedViewSize[0] + adjustedViewSize[1] + adjustedViewSize[2]) / 3.) / 2;
	double lineHalfWidth = pointRadius - (1./10) * pointRadius;

	vector<Coord> lineCoords;
	for (unsigned int j = 0; j < axisOrder.size() ; j++) {

		Coord pointCoord = parallelAxis[axisOrder[j]]->getPointCoordOnAxisForData(dataId);
		ostringstream oss;
		oss << "data " << dataId << " var " << axisOrder[j];

		if (drawPointsOnAxis) {

			GlSimpleEntity *point = NULL;

			if (graphProxy->getDataLocation() == NODE) {
				if (!graphProxy->highlightedEltsSet() || (graphProxy->highlightedEltsSet() && graphProxy->isDataHighlighted(dataId))) {
					point = new GlNodeGlyph(pointCoord, adjustedViewSize, graphData, node(dataId));
				}
			} else {
				point = new GlCircle(pointCoord, pointRadius, color, color, true, true);
			}

			if (point != NULL) {
				point->setStencil(1);
				dataPlotComposite->addGlEntity(point, oss.str());
				glEntitiesDataMap[point] = dataId;
			}
		}

		if ((vType == VIEW_2D_SPLINE) && (j != (axisOrder.size() - 1))) {
			Coord endLineCoord = parallelAxis[axisOrder[j + 1]]->getPointCoordOnAxisForData(dataId);
			double dx = (endLineCoord.getX() - pointCoord.getX()) / 4.;
			lineCoords.push_back(pointCoord);
			lineCoords.push_back(Coord(pointCoord.getX() + dx, pointCoord.getY(), 0));
			lineCoords.push_back(Coord(endLineCoord.getX() - dx, endLineCoord.getY(), 0));
			lineCoords.push_back(endLineCoord);
			GlSimpleEntity *line = new GlCurve(lineCoords, color, color, lineHalfWidth, lineHalfWidth);
			if (lineTextureFilename != "") {
				((GlCurve *)line)->setTexture(lineTextureFilename);
			}
			if (graphProxy->isDataSelected(dataId) || graphProxy->isDataHighlighted(dataId)) {
					line->setStencil(2);
			}
			ostringstream oss2;
			oss2 << "data " << dataId << " line " << parallelAxis[axisOrder[j]]->getAxisName() << " -> " << parallelAxis[axisOrder[j+1]]->getAxisName();
			dataPlotComposite->addGlEntity(line, oss2.str());
			glEntitiesDataMap[line] = dataId;
			lineCoords.clear();

		} else if (vType == VIEW_2D) {
			lineCoords.push_back(pointCoord + Coord(0, - lineHalfWidth));
			lineCoords.push_back(pointCoord + Coord(0, lineHalfWidth));
		}
	}

	if (vType == VIEW_2D) {
		GlSimpleEntity *line = new GlPolyQuad(lineCoords, color, lineTextureFilename);
		if (graphProxy->isDataSelected(dataId) || graphProxy->isDataHighlighted(dataId)) {
			line->setStencil(2);
		}
		ostringstream oss2;
		oss2 << "data " << dataId << " line ";
		dataPlotComposite->addGlEntity(line, oss2.str());
		glEntitiesDataMap[line] = dataId;
	}
}

unsigned int ParallelCoordinatesDrawing::nbParallelAxis() const {
	return nbAxis;
}

void ParallelCoordinatesDrawing::swapAxis(ParallelAxis *axis1, ParallelAxis *axis2) {
	vector<string>::iterator it;
	int pi, pj;
	int pos = 0;

	for (it = axisOrder.begin(); it != axisOrder.end(); ++it, ++pos) {
		if (*it == axis1->getAxisName()) {
			pi = pos;
		}
		if (*it == axis2->getAxisName()) {
			pj = pos;
		}
	}

	string tmp = axisOrder[pi];
	axisOrder[pi] = axisOrder[pj];
	axisOrder[pj] = tmp;

	Coord ci = parallelAxis[axis1->getAxisName()]->getBaseCoord();
	Coord cj = parallelAxis[axis2->getAxisName()]->getBaseCoord();

	parallelAxis[axis1->getAxisName()]->translate(cj - ci);
	parallelAxis[axis2->getAxisName()]->translate(ci - cj);

	parallelAxis[axis1->getAxisName()]->computeBoundingBox();
	parallelAxis[axis2->getAxisName()]->computeBoundingBox();

	graphProxy->setSelectedProperties(axisOrder);

	createAxisFlag = false;
}

bool ParallelCoordinatesDrawing::getDataIdFromGlEntity(GlEntity *glEntity, unsigned int &dataId) {

	bool dataMatch = glEntitiesDataMap.find(glEntity) != glEntitiesDataMap.end();

	if (dataMatch) {
		dataId = glEntitiesDataMap[glEntity];
	}

	return dataMatch;
}

void ParallelCoordinatesDrawing::update() {
	if (createAxisFlag) {
		axisPlotComposite->reset(false);
		createAxis();
	}
	eraseDataPlot();
	plotAllData();
	createAxisFlag = true;
}

void ParallelCoordinatesDrawing::eraseDataPlot() {
	dataPlotComposite->reset(true);
	glEntitiesDataMap.clear();
}

void ParallelCoordinatesDrawing::eraseAxisPlot() {
	axisPlotComposite->reset(true);
	parallelAxis.clear();
}

void ParallelCoordinatesDrawing::erase() {
	eraseDataPlot();
	eraseAxisPlot();
}

void ParallelCoordinatesDrawing::removeAxis(ParallelAxis *axis) {
	if (axisPlotComposite->findKey(axis) != "") {
		axisPlotComposite->deleteGlEntity(axis);
	}
}

void ParallelCoordinatesDrawing::addAxis(ParallelAxis *axis) {
	if (axisPlotComposite->findKey(axis) == "") {
		axisPlotComposite->addGlEntity(axis, axis->getAxisName());
	}
}

const vector<string> & ParallelCoordinatesDrawing::getAxisNames() const {
	return axisOrder;
}

ParallelAxis *ParallelCoordinatesDrawing::getAxisUnderPoint(const Coord &coord) {
	map<string, ParallelAxis *>::iterator it;
	ParallelAxis *axis = NULL;
	for (it = parallelAxis.begin() ; it != parallelAxis.end() ; ++it) {
		if ((it->second)->isVisible()) {
			BoundingBox axisBB = (it->second)->getBoundingBox();
			if (coord.getX() >= axisBB.first.getX() && coord.getX() <= axisBB.second.getX()) { //&& coord.getY() >= axisBB.first.getY() && coord.getY() <= axisBB.second.getY()) {
				axis = it->second;
				break;
			}
		}
	}
	return axis;
}

void ParallelCoordinatesDrawing::computeResizeFactor() {
	Size eltMinSize = ((SizeProperty*)(graphProxy->getProperty("viewSize")))->getMin();
	Size eltMaxSize = ((SizeProperty*)(graphProxy->getProperty("viewSize")))->getMax();

	Size deltaSize = eltMaxSize - eltMinSize;
	for (unsigned int i = 0 ; i < 3 ; ++i) {
		if (deltaSize[i] != 0) {
			resizeFactor[i] = (axisPointMaxSize[i] - axisPointMinSize[i]) / deltaSize[i];
		} else {
			resizeFactor[i] = 0;
		}
	}
}

vector<ParallelAxis *> ParallelCoordinatesDrawing::getAllAxis() {
	vector<ParallelAxis *> axis;
	map<string, ParallelAxis *>::iterator it;
	for (it = parallelAxis.begin() ; it != parallelAxis.end() ; ++it) {
		if ((it->second)->isVisible()) {
			axis.push_back(it->second);
		}
	}
	return axis;
}

void ParallelCoordinatesDrawing::updateWithAxisSlidersRange(ParallelAxis *axis, bool multiFiltering) {
	set<unsigned int> dataSubset;
	if (multiFiltering) {
		set<unsigned int> eltsInSlidersRange = axis->getDataInSlidersRange();
		set<unsigned int> currentHighlightedElts = graphProxy->getHighlightedElts();
		unsigned int size = eltsInSlidersRange.size() > currentHighlightedElts.size() ? eltsInSlidersRange.size() : currentHighlightedElts.size();
		vector<unsigned int> intersection(size);
		vector<unsigned int>::iterator intersectionEnd = std::set_intersection(eltsInSlidersRange.begin(), eltsInSlidersRange.end(),
																			   currentHighlightedElts.begin(), currentHighlightedElts.end(),
																			   intersection.begin());
		dataSubset = set<unsigned int>(intersection.begin(), intersectionEnd);
	} else {
		dataSubset = axis->getDataInSlidersRange();
	}
	if (dataSubset.size() > 0) {
		graphProxy->unsetHighlightedElts();
		set<unsigned int>::iterator it;
		for (it = dataSubset.begin() ; it != dataSubset.end() ; ++it) {
			graphProxy->addOrRemoveEltToHighlight(*it);
		}
		map<string, ParallelAxis *>::iterator it2;
		for (it2 = parallelAxis.begin() ; it2 != parallelAxis.end() ; ++it2) {
			if ((it2->second) != axis) {
				(it2->second)->updateSlidersWithDataSubset(dataSubset);
			}
		}
		createAxisFlag = false;
	}
}

void ParallelCoordinatesDrawing::resetAxisSlidersPosition() {
	vector<ParallelAxis *> axis = getAllAxis();
	vector<ParallelAxis *>::iterator it;
	for (it = axis.begin() ; it != axis.end() ; ++it) {
		(*it)->resetSlidersPosition();
	}
}

void ParallelCoordinatesDrawing::deleteAxisGlEntities() {

	if (!createAxisFlag)
		return;

	map<string, ParallelAxis *>::iterator it;
	for (it = parallelAxis.begin() ; it != parallelAxis.end() ; ++it) {
		(it->second)->reset(true);
	}
}

void ParallelCoordinatesDrawing::delNode(Graph *,const node n) {
	if (graphProxy->getDataLocation() == NODE) {
		elementDeleted = true;
		removeHighlightedElt(n.id);
	}
}

void ParallelCoordinatesDrawing::delEdge(Graph *,const edge e) {
	if (graphProxy->getDataLocation() == EDGE) {
		elementDeleted = true;
		removeHighlightedElt(e.id);
	}
}

void ParallelCoordinatesDrawing::removeHighlightedElt(const unsigned int dataId) {
	if (lastHighlightedElements.find(dataId) != lastHighlightedElements.end()) {
		lastHighlightedElements.erase(dataId);
		graphProxy->removeHighlightedElement(dataId);
		if (!graphProxy->highlightedEltsSet()) {
			graphProxy->colorDataAccordingToHighlightedElts();
		}
	}
}

}

