#include <tulip/BooleanProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/Iterator.h>
#include <tulip/BoundingBox.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/GlPolyQuad.h>
#include <tulip/GlCatmullRomCurve.h>

#include "ParallelCoordinatesDrawing.h"
#include "NominalParallelAxis.h"
#include "QuantitativeParallelAxis.h"
#include "ParallelTools.h"

#include <sstream>
#include <climits>
#include <algorithm>

const int CIRCLE_GLYPH_ID = 14;

namespace tlp {

ParallelCoordinatesDrawing::ParallelCoordinatesDrawing(ParallelCoordinatesGraphProxy *graph, Graph *axisPointsGraph) :
	firstAxisPos(Coord(0,0,0)), height(DEFAULT_AXIS_HEIGHT), spaceBetweenAxis(height/2), linesColorAlphaValue(DEFAULT_LINES_COLOR_ALPHA_VALUE), drawPointsOnAxis(true),
	graphProxy(graph), backgroundColor(Color(255,255,255)), createAxisFlag(true), axisPointsGraph(axisPointsGraph), layoutType(PARALLEL) {

	axisPointsGraphLayout = axisPointsGraph->getProperty<LayoutProperty>("viewLayout");
	axisPointsGraphSize = axisPointsGraph->getProperty<SizeProperty>("viewSize");
	axisPointsGraphShape = axisPointsGraph->getProperty<IntegerProperty>("viewShape");
	axisPointsGraphLabels = axisPointsGraph->getProperty<StringProperty>("viewLabel");
	axisPointsGraphColors = axisPointsGraph->getProperty<ColorProperty>("viewColor");
	axisPointsGraphSelection = axisPointsGraph->getProperty<BooleanProperty>("viewSelection");

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
	vector<string> selectedProperties = graphProxy->getSelectedProperties();
	vector<string>::iterator it;
	GlAxis::LabelPosition captionPosition;

	map<string, ParallelAxis *>::iterator it2;
	for (it2 = parallelAxis.begin() ; it2 != parallelAxis.end() ; ++it2) {
		(it2->second)->setHidden(true);
	}

	nbAxis = graphProxy->getNumberOfSelectedProperties();
	axisOrder.clear();
	if (nbAxis > 1) {
		width = spaceBetweenAxis * (nbAxis - 1);
	} else {
		width = spaceBetweenAxis;
	}

	float circleLayoutYOffset = (nbAxis * 100.) / 4.;

	Color axisColor;
	int bgV = backgroundColor.getV();
	if (bgV < 128) {
		axisColor = Color(255,255,255);
	} else {
		axisColor = Color(0,0,0);
	}

	double maxCaptionWidth = (8./10.) * spaceBetweenAxis;

	if (selectedProperties.size() < 3) {
		layoutType = PARALLEL;
	}

	float rotationAngleBase = 0;
	if (layoutType == CIRCULAR) {
		rotationAngleBase = -(2. * M_PI) / selectedProperties.size();
		captionPosition = GlAxis::RIGHT_OR_ABOVE;
	} else {
		captionPosition = GlAxis::LEFT_OR_BELOW;
	}

	unsigned int cpt = 0;
	for (it = selectedProperties.begin(); it != selectedProperties.end() ; ++it) {

		ParallelAxis *axis = NULL;
		float rotationAngle = (cpt++ * rotationAngleBase) * (180. / M_PI);
		Coord coord;

		if (layoutType == PARALLEL) {
			if (nbAxis!=1) {
				coord = Coord(firstAxisPos.getX() + pos * (width/(nbAxis - 1)),firstAxisPos.getY());
			} else {
				coord = Coord(firstAxisPos.getX() + (width/2), firstAxisPos.getY());
			}
		} else {
			coord = Coord(0, circleLayoutYOffset, 0);
		}

		if (parallelAxis.find(*it) != parallelAxis.end()) {
			axis = (parallelAxis.find(*it))->second;
			if (layoutType == PARALLEL) {
				axis->setRotationAngle(0);
				axis->translate(coord - axis->getBaseCoord());
			} else {
				axis->setBaseCoord(coord);
				axis->setRotationAngle(rotationAngle);
			}
			axis->setCaptionPosition(captionPosition);
			axis->setAxisHeight(height);
			axis->setMaxCaptionWidth(maxCaptionWidth);
			axis->setAxisColor(axisColor);
			axis->redraw();
			axis->setHidden(false);
		} else {
			string typeName = (graphProxy->getProperty(*it))->getTypename();

			if (typeName == "string") {
				axis = new NominalParallelAxis(coord, height, maxCaptionWidth, graphProxy, *it, axisColor, rotationAngle, captionPosition);
			} else if (typeName == "int" || typeName == "double") {
				axis = new QuantitativeParallelAxis(coord, height, maxCaptionWidth, graphProxy, *it, true, axisColor, rotationAngle, captionPosition);
			}
		}

		if (axis != NULL) {
			axisPlotComposite->addGlEntity(axis, *it);
			axisOrder.push_back(*it);
			parallelAxis[*it] = axis;
			++pos;
		}
	}
}

void ParallelCoordinatesDrawing::destroyAxisIfNeeded() {
	map<string, ParallelAxis *>::iterator it;
	for (it = parallelAxis.begin() ; it != parallelAxis.end() ; ++it) {
		if (!graphProxy->existProperty(it->first)) {
			delete it->second;
			parallelAxis.erase(it->first);
		}
	}
}

void ParallelCoordinatesDrawing::plotAllData() {

	Color color;
	computeResizeFactor();

	Iterator<unsigned int> *dataUnselIt = graphProxy->getDataIterator();
	while (dataUnselIt->hasNext()) {
		unsigned int dataId = dataUnselIt->next();
		if (!graphProxy->isDataSelected(dataId)) {
			color = graphProxy->getDataColor(dataId);
			if (linesColorAlphaValue <= 255 &&
					((graphProxy->highlightedEltsSet() && graphProxy->isDataHighlighted(dataId)) || (!graphProxy->highlightedEltsSet()))) {
				color.setA(linesColorAlphaValue);
			}
		} else {
			color = COLOR_SELECT;
		}
		plotData(dataId, color);
		++nbDataProcessed;
	}
	delete dataUnselIt;

	lastHighlightedElements = graphProxy->getHighlightedElts();
}

void ParallelCoordinatesDrawing::plotData(const unsigned int dataId, const Color &color) {

	Size eltMinSize = ((SizeProperty*)(graphProxy->getProperty("viewSize")))->getMin();
	Size dataViewSize = graphProxy->getDataViewSize(dataId);
	Size adjustedViewSize = axisPointMinSize + resizeFactor * (dataViewSize - eltMinSize);
	float pointRadius =((adjustedViewSize[0] + adjustedViewSize[1] + adjustedViewSize[2]) / 3.) / 2;
	double lineHalfWidth = pointRadius - (1./10) * pointRadius;


	vector<Coord> quadCoords;
	vector<Coord> splineCurvePassPoints;

	for (unsigned int j = 0; j < axisOrder.size() ; j++) {

		Coord pointCoord = parallelAxis[axisOrder[j]]->getPointCoordOnAxisForData(dataId);
		float axisRotAngle = parallelAxis[axisOrder[j]]->getRotationAngle();
		ostringstream oss;
		oss << "data " << dataId << " var " << axisOrder[j];

		if (drawPointsOnAxis) {

			if (!graphProxy->highlightedEltsSet() || (graphProxy->highlightedEltsSet() && graphProxy->isDataHighlighted(dataId)) || graphProxy->isDataSelected(dataId)) {
				node n = axisPointsGraph->addNode();
				axisPointsDataMap[n] = dataId;
				axisPointsGraphLayout->setNodeValue(n, pointCoord);
				axisPointsGraphSize->setNodeValue(n, adjustedViewSize);
				if (graphProxy->getDataLocation() == NODE) {
					axisPointsGraphShape->setNodeValue(n, graphProxy->getPropertyValueForData<IntegerProperty, IntegerType>("viewShape", dataId));
				} else {
					axisPointsGraphShape->setNodeValue(n, CIRCLE_GLYPH_ID);
				}
				axisPointsGraphLabels->setNodeValue(n, graphProxy->getPropertyValueForData<StringProperty, StringType>("viewLabel", dataId));
				axisPointsGraphColors->setNodeValue(n, graphProxy->getPropertyValueForData<ColorProperty, ColorType>("viewColor", dataId));
				if (graphProxy->isDataSelected(dataId)) {
					axisPointsGraphSelection->setNodeValue(n, true);
				}
			}
		}

		if (linesType == STRAIGHT) {
			Coord vec1(0, - lineHalfWidth);
			Coord vec2(0, lineHalfWidth);
			if (axisRotAngle != 0) {
				rotateVector(vec1, axisRotAngle, Z_ROT);
				rotateVector(vec2, axisRotAngle, Z_ROT);
			}
			quadCoords.push_back(pointCoord + vec1);
			quadCoords.push_back(pointCoord + vec2);

		} else {
			splineCurvePassPoints.push_back(pointCoord);
		}
	}

	GlSimpleEntity *line;
	if (linesType == STRAIGHT) {
		if (layoutType == CIRCULAR) {
			quadCoords.push_back(quadCoords[0]);
			quadCoords.push_back(quadCoords[1]);
		}
		vector<Coord> newQuadCoords;
		for (unsigned int i = 0 ; i < quadCoords.size() - 2 ; i = i+2) {
			newQuadCoords.push_back(quadCoords[i]);
			newQuadCoords.push_back(quadCoords[i+1]);
			const unsigned int nbLineSegments = 20;
			Coord step1 = (quadCoords[i+2] - quadCoords[i]) / nbLineSegments;
			Coord step2 = (quadCoords[i+3] - quadCoords[i+1]) / nbLineSegments;
			for (unsigned int j = 1 ; j <= nbLineSegments ; ++j) {
				newQuadCoords.push_back(quadCoords[i] + j * step1);
				newQuadCoords.push_back(quadCoords[i+1] + j * step2);
			}
		}
		newQuadCoords.push_back(quadCoords[quadCoords.size() - 2]);
		newQuadCoords.push_back(quadCoords[quadCoords.size() - 1]);
		line = new GlPolyQuad(newQuadCoords, color, lineTextureFilename, true, 1, color);
	} else {
		bool closeSpline = (layoutType == CIRCULAR);
		line = new GlCatmullRomCurve(splineCurvePassPoints, color, color, pointRadius, pointRadius, lineTextureFilename, closeSpline);
//		map<string, GlSimpleEntity*> *glEntities = ((GlComposite *)line)->getDisplays();
//		map<string, GlSimpleEntity*>::iterator it;
//		for (it = glEntities->begin(); it != glEntities->end() ; ++it) {
//			glEntitiesDataMap[it->second] = dataId;
//		}
	}
	if (graphProxy->isDataHighlighted(dataId)) {
		line->setStencil(4);
	}
	if (graphProxy->isDataSelected(dataId)) {
		line->setStencil(3);
	}
	ostringstream oss2;
	oss2 << "data " << dataId << " line ";
	dataPlotComposite->addGlEntity(line, oss2.str());
	glEntitiesDataMap[line] = dataId;
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

	if (layoutType == PARALLEL) {

		Coord ci = parallelAxis[axis1->getAxisName()]->getBaseCoord();
		Coord cj = parallelAxis[axis2->getAxisName()]->getBaseCoord();

		parallelAxis[axis1->getAxisName()]->translate(cj - ci);
		parallelAxis[axis2->getAxisName()]->translate(ci - cj);

	} else {

		float axis1RotAngle = parallelAxis[axis1->getAxisName()]->getRotationAngle();
		parallelAxis[axis1->getAxisName()]->setRotationAngle(parallelAxis[axis2->getAxisName()]->getRotationAngle());
		parallelAxis[axis2->getAxisName()]->setRotationAngle(axis1RotAngle);
	}

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

bool ParallelCoordinatesDrawing::getDataIdFromAxisPoint(node axisPoint, unsigned int &dataId) {

	bool dataMatch = axisPointsDataMap.find(axisPoint) != axisPointsDataMap.end();

	if (dataMatch) {
		dataId = axisPointsDataMap[axisPoint];
	}

	return dataMatch;
}

void ParallelCoordinatesDrawing::update() {
	destroyAxisIfNeeded();
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
	BooleanProperty *wholeGraphSelec = new BooleanProperty(axisPointsGraph);
	wholeGraphSelec->setAllNodeValue(true);
	removeFromGraph(axisPointsGraph, wholeGraphSelec);
	delete wholeGraphSelec;
	glEntitiesDataMap.clear();
	axisPointsDataMap.clear();
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
		axis->setHidden(true);
		axisPlotComposite->deleteGlEntity(axis);
	}
}

void ParallelCoordinatesDrawing::addAxis(ParallelAxis *axis) {
	if (axisPlotComposite->findKey(axis) == "") {
		axis->setHidden(false);
		axisPlotComposite->addGlEntity(axis, axis->getAxisName());
	}
}

const vector<string> & ParallelCoordinatesDrawing::getAxisNames() const {
	return axisOrder;
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
		if (!(it->second)->isHidden()) {
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

void ParallelCoordinatesDrawing::delNode(Graph *,const node n) {
	if (graphProxy->getDataLocation() == NODE) {
		removeHighlightedElt(n.id);
	}
}

void ParallelCoordinatesDrawing::delEdge(Graph *,const edge e) {
	if (graphProxy->getDataLocation() == EDGE) {
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

