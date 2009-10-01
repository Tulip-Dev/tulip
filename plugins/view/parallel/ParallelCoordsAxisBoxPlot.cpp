//-*-c++-*-
/*
 Author: Antoine Lambert

 Email : antoine.lambert@labri.fr

 Last modification : 12/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#include <tulip/TlpTools.h>
#include <tulip/GlPolygon.h>
#include <tulip/GlLabel.h>
#include <tulip/GlQuad.h>
#include <tulip/GlLines.h>
#include <tulip/GlSimpleEntity.h>
#include <tulip/GlMainWidget.h>

#include "ParallelCoordinatesView.h"
#include "ParallelTools.h"
#include "QuantitativeParallelAxis.h"
#include "ParallelCoordsAxisBoxPlot.h"

using namespace std;

namespace tlp {

const Color lightBlue(0, 217, 255, 100);
const Color darkBlue(0, 0, 255, 255);

GlAxisBoxPlot::GlAxisBoxPlot(QuantitativeParallelAxis *axis, const Color& fillColor, const Color& outlineColor) :
		axis(axis), boxWidth(5 * axis->getAxisGradsWidth()), fillColor(fillColor), outlineColor(outlineColor),
		highlightRangeLowBound(NULL), highlightRangeHighBound(NULL) {}

void GlAxisBoxPlot::draw(float lod,Camera* camera) {

	float rotationAngle = axis->getRotationAngle();

	if (rotationAngle != 0) {
		glPushMatrix();
		glRotatef(rotationAngle, 0.0, 0.0, 1.0);
	}

	bottomOutlierCoord = axis->getBottomOutlierCoord();
	firstQuartileCoord = axis->getFirstQuartileCoord();
	medianCoord = axis->getMedianCoord();
	thirdQuartileCoord = axis->getThirdQuartileCoord();
	topOutlierCoord = axis->getTopOutlierCoord();

	Coord interQuartileRangeBoxCoords[4];

	if (axis->hasAscendingOrder()) {
		boundingBox.check(Coord(bottomOutlierCoord.getX() - boxWidth / 2, bottomOutlierCoord.getY(), 0));
		boundingBox.check(Coord(topOutlierCoord.getX() + boxWidth / 2, topOutlierCoord.getY(), 0));

		interQuartileRangeBoxCoords[0] = Coord(thirdQuartileCoord.getX() - boxWidth / 2, thirdQuartileCoord.getY(), 0);
		interQuartileRangeBoxCoords[1] = Coord(thirdQuartileCoord.getX() + boxWidth / 2, thirdQuartileCoord.getY(), 0);
		interQuartileRangeBoxCoords[2] = Coord(firstQuartileCoord.getX() + boxWidth / 2, firstQuartileCoord.getY(), 0);
		interQuartileRangeBoxCoords[3] = Coord(firstQuartileCoord.getX() - boxWidth / 2, firstQuartileCoord.getY(), 0);
	} else {
		boundingBox.check(Coord(topOutlierCoord.getX() - boxWidth / 2, topOutlierCoord.getY(), 0));
		boundingBox.check(Coord(bottomOutlierCoord.getX() + boxWidth / 2, bottomOutlierCoord.getY(), 0));

		interQuartileRangeBoxCoords[0] = Coord(firstQuartileCoord.getX() - boxWidth / 2, firstQuartileCoord.getY(), 0);
		interQuartileRangeBoxCoords[1] = Coord(firstQuartileCoord.getX() + boxWidth / 2, firstQuartileCoord.getY(), 0);
		interQuartileRangeBoxCoords[2] = Coord(thirdQuartileCoord.getX() + boxWidth / 2, thirdQuartileCoord.getY(), 0);
		interQuartileRangeBoxCoords[3] = Coord(thirdQuartileCoord.getX() - boxWidth / 2, thirdQuartileCoord.getY(), 0);
	}

	GlQuad interQuartileRangeBox(interQuartileRangeBoxCoords, fillColor);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_SRC_COLOR);
	glEnable(GL_LIGHTING);
	interQuartileRangeBox.draw(lod, camera);
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);

	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	GlLines::glDrawLine(interQuartileRangeBoxCoords[0], interQuartileRangeBoxCoords[1], 2, GlLines::TLP_PLAIN, outlineColor, outlineColor);
	GlLines::glDrawLine(interQuartileRangeBoxCoords[1], interQuartileRangeBoxCoords[2], 2, GlLines::TLP_PLAIN, outlineColor, outlineColor);
	GlLines::glDrawLine(interQuartileRangeBoxCoords[2], interQuartileRangeBoxCoords[3], 2, GlLines::TLP_PLAIN, outlineColor, outlineColor);
	GlLines::glDrawLine(interQuartileRangeBoxCoords[3], interQuartileRangeBoxCoords[0], 2, GlLines::TLP_PLAIN, outlineColor, outlineColor);

	GlLines::glDrawLine(bottomOutlierCoord + Coord(-(boxWidth / 2), 0, 0), bottomOutlierCoord + Coord(boxWidth / 2, 0, 0), 2, GlLines::TLP_PLAIN, outlineColor, outlineColor);
	GlLines::glDrawLine(medianCoord + Coord(-(boxWidth / 2), 0, 0), medianCoord + Coord(boxWidth / 2, 0, 0), 2, GlLines::TLP_PLAIN, outlineColor, outlineColor);
	GlLines::glDrawLine(topOutlierCoord + Coord(-(boxWidth / 2), 0, 0), topOutlierCoord + Coord(boxWidth / 2, 0, 0), 2, GlLines::TLP_PLAIN, outlineColor, outlineColor);

	GlLines::glDrawLine(bottomOutlierCoord, firstQuartileCoord, 2, GlLines::TLP_DASHED, outlineColor, outlineColor);
	GlLines::glDrawLine(thirdQuartileCoord, topOutlierCoord, 2, GlLines::TLP_DASHED, outlineColor, outlineColor);
	glDisable(GL_BLEND);

	drawLabel(bottomOutlierCoord, axis->getBottomOutlierStringValue());
	drawLabel(firstQuartileCoord, axis->getFirstQuartileStringValue());
	drawLabel(medianCoord, axis->getMedianStringValue());
	drawLabel(thirdQuartileCoord, axis->getThirdQuartileStringValue());
	drawLabel(topOutlierCoord, axis->getTopOutlierStringValue());

	if (highlightRangeLowBound != NULL && highlightRangeHighBound != NULL) {
		Coord highlightBoxCoords[4];
		highlightBoxCoords[0] = Coord(highlightRangeHighBound->getX() - boxWidth /2, highlightRangeHighBound->getY(), 0);
		highlightBoxCoords[1] = Coord(highlightRangeHighBound->getX() + boxWidth /2, highlightRangeHighBound->getY(), 0);
		highlightBoxCoords[2] = Coord(highlightRangeLowBound->getX() + boxWidth /2, highlightRangeLowBound->getY(), 0);
		highlightBoxCoords[3] = Coord(highlightRangeLowBound->getX() - boxWidth /2, highlightRangeLowBound->getY(), 0);
		Color outlineColorTranslucent(outlineColor);
		outlineColorTranslucent.setA(10);
		GlQuad highlightBox(highlightBoxCoords, outlineColorTranslucent);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_SRC_COLOR);
		glEnable(GL_LIGHTING);
		highlightBox.draw(lod, camera);
		glDisable(GL_BLEND);
		glDisable(GL_LIGHTING);
		highlightRangeLowBound = NULL;
		highlightRangeHighBound = NULL;
	}

	if (rotationAngle != 0) {
		glPopMatrix();
	}
}


void GlAxisBoxPlot::setHighlightRangeIfAny(Coord sceneCoords) {
	if (axis->hasAscendingOrder()) {
		if (sceneCoords.getY() < topOutlierCoord.getY() &&
				sceneCoords.getY() > thirdQuartileCoord.getY() &&
				sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2)) &&
				sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2))) {

			highlightRangeLowBound = &thirdQuartileCoord;
			highlightRangeHighBound = &topOutlierCoord;
			axis->setBoxPlotHighlightBounds(THIRD_QUARTILE, TOP_OUTLIER);

		} else if (sceneCoords.getY() < thirdQuartileCoord.getY() &&
				sceneCoords.getY() > (medianCoord.getY() + (1./3.) * (thirdQuartileCoord.getY() - medianCoord.getY()))&&
				sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2)) &&
				sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2))) {

			highlightRangeLowBound = &medianCoord;
			highlightRangeHighBound = &thirdQuartileCoord;
			axis->setBoxPlotHighlightBounds(MEDIAN, THIRD_QUARTILE);

		} else if (sceneCoords.getY() > (medianCoord.getY() - (1./3.) * (medianCoord.getY() - firstQuartileCoord.getY())) &&
				sceneCoords.getY() < (medianCoord.getY() + (1./3.) * (thirdQuartileCoord.getY() - medianCoord.getY()))&&
				sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2)) &&
				sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2))) {

			highlightRangeLowBound = &firstQuartileCoord;
			highlightRangeHighBound = &thirdQuartileCoord;
			axis->setBoxPlotHighlightBounds(FIRST_QUARTILE, THIRD_QUARTILE);

		} else if (sceneCoords.getY() > firstQuartileCoord.getY() &&
				sceneCoords.getY() < (medianCoord.getY() - (1./3.) * (medianCoord.getY() - firstQuartileCoord.getY()))&&
				sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2)) &&
				sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2))) {
			highlightRangeLowBound = &firstQuartileCoord;
			highlightRangeHighBound = &medianCoord;
			axis->setBoxPlotHighlightBounds(FIRST_QUARTILE, MEDIAN);
		} else if (sceneCoords.getY() > bottomOutlierCoord.getY() &&
				sceneCoords.getY() < firstQuartileCoord.getY()&&
				sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2)) &&
				sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2))) {

			highlightRangeLowBound = &bottomOutlierCoord;
			highlightRangeHighBound = &firstQuartileCoord;
			axis->setBoxPlotHighlightBounds(BOTTOM_OUTLIER, FIRST_QUARTILE);

		} else {
			highlightRangeLowBound = NULL;
			highlightRangeHighBound = NULL;
			axis->setBoxPlotHighlightBounds(NO_VALUE, NO_VALUE);
		}
	} else {
		if (sceneCoords.getY() > topOutlierCoord.getY() &&
				sceneCoords.getY() < thirdQuartileCoord.getY()&&
				sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2)) &&
				sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2))) {

			highlightRangeHighBound = &thirdQuartileCoord;
			highlightRangeLowBound = &topOutlierCoord;
			axis->setBoxPlotHighlightBounds(TOP_OUTLIER, THIRD_QUARTILE);

		} else if (sceneCoords.getY() > thirdQuartileCoord.getY() &&
				sceneCoords.getY() < (medianCoord.getY() + (1./3.) * (thirdQuartileCoord.getY() - medianCoord.getY()))&&
				sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2)) &&
				sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2))) {

			highlightRangeHighBound = &medianCoord;
			highlightRangeLowBound = &thirdQuartileCoord;
			axis->setBoxPlotHighlightBounds(THIRD_QUARTILE, MEDIAN);

		} else if (sceneCoords.getY() < (medianCoord.getY() - (1./3.) * (medianCoord.getY() - firstQuartileCoord.getY())) &&
				sceneCoords.getY() > (medianCoord.getY() + (1./3.) * (thirdQuartileCoord.getY() - medianCoord.getY()))&&
				sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2)) &&
				sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2))) {

			highlightRangeHighBound = &firstQuartileCoord;
			highlightRangeLowBound = &thirdQuartileCoord;
			axis->setBoxPlotHighlightBounds(THIRD_QUARTILE, FIRST_QUARTILE);

		} else if (sceneCoords.getY() < firstQuartileCoord.getY() &&
				sceneCoords.getY() > (medianCoord.getY() - (1./3.) * (medianCoord.getY() - firstQuartileCoord.getY()))&&
				sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2)) &&
				sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2))) {

			highlightRangeHighBound = &firstQuartileCoord;
			highlightRangeLowBound = &medianCoord;
			axis->setBoxPlotHighlightBounds(MEDIAN, FIRST_QUARTILE);

		} else if (sceneCoords.getY() < bottomOutlierCoord.getY() &&
				sceneCoords.getY() > firstQuartileCoord.getY()&&
				sceneCoords.getX() > (medianCoord.getX() - (boxWidth / 2)) &&
				sceneCoords.getX() < (medianCoord.getX() + (boxWidth / 2))) {

			highlightRangeHighBound = &bottomOutlierCoord;
			highlightRangeLowBound = &firstQuartileCoord;
			axis->setBoxPlotHighlightBounds(FIRST_QUARTILE, BOTTOM_OUTLIER);

		} else {
			highlightRangeLowBound = NULL;
			highlightRangeHighBound = NULL;
			axis->setBoxPlotHighlightBounds(NO_VALUE, NO_VALUE);
		}
	}
}



void GlAxisBoxPlot::drawLabel(Coord position, string labelName) {
	float labelHeight = axis->getLabelHeight();;

	float heightRef;
	if (axis->hasAscendingOrder()) {
		heightRef = topOutlierCoord.getY() - thirdQuartileCoord.getY();
	} else {
		heightRef = thirdQuartileCoord.getY() - topOutlierCoord.getY();
	}

	if (labelHeight > heightRef) {
		labelHeight = heightRef / 2;
	}

	float labelWidth = labelName.length() * (labelHeight / 2);

	if (labelName.length() == 1) {
		labelWidth *= 2;
	}

	GlLabel labelToDraw(Coord(position.getX() - boxWidth / 2 - labelWidth /2, position.getY(), 0),
			Coord(labelWidth, labelHeight), outlineColor);
	labelToDraw.setText(labelName);
	labelToDraw.draw(0, 0);
}

ParallelCoordsAxisBoxPlot::ParallelCoordsAxisBoxPlot() : currentGraph(NULL), selectedAxis(NULL), lastNbAxis(0) {}

ParallelCoordsAxisBoxPlot::~ParallelCoordsAxisBoxPlot() {
	deleteGlAxisPlot();
}

void ParallelCoordsAxisBoxPlot::setView(View *view) {
	InteractorComponent::setView(view);
	parallelView = (ParallelCoordinatesView *) view;
	initOrUpdateBoxPlots();
}

bool ParallelCoordsAxisBoxPlot::compute(GlMainWidget *glWidget) {
	initOrUpdateBoxPlots();
	return true;
}

void ParallelCoordsAxisBoxPlot::initOrUpdateBoxPlots() {
	vector<ParallelAxis *> allAxis = parallelView->getAllAxis();

	if (axisBoxPlotMap.size() == 0) {
		buildGlAxisPlot(allAxis);
		lastNbAxis = allAxis.size();
		parallelView->refresh();
		return;
	}

	if ((lastNbAxis != 0 && lastNbAxis != allAxis.size()) || (currentGraph != parallelView->getGraph())) {
		deleteGlAxisPlot();
		buildGlAxisPlot(allAxis);
		selectedAxis = NULL;
		parallelView->refresh();
	}

	lastNbAxis = allAxis.size();
	currentGraph = parallelView->getGraph();
}

void ParallelCoordsAxisBoxPlot::buildGlAxisPlot(vector<ParallelAxis *> currentAxis) {
	for (unsigned int i = 0 ; i < currentAxis.size() ; ++i) {
		if (dynamic_cast<QuantitativeParallelAxis *>(currentAxis[i])) {
			QuantitativeParallelAxis *quantitativeAxis = (QuantitativeParallelAxis *) currentAxis[i];
			if (quantitativeAxis->getMedianStringValue() != "KO")
				axisBoxPlotMap[quantitativeAxis] = new GlAxisBoxPlot(quantitativeAxis, lightBlue, darkBlue);
		}
	}
}

void ParallelCoordsAxisBoxPlot::deleteGlAxisPlot() {
	map<QuantitativeParallelAxis *, GlAxisBoxPlot *>::iterator it;
	for (it = axisBoxPlotMap.begin() ; it != axisBoxPlotMap.end() ; ++it) {
		delete (it->second);
	}
	axisBoxPlotMap.clear();
}

bool ParallelCoordsAxisBoxPlot::eventFilter(QObject *widget, QEvent *e) {

	GlMainWidget *glWidget = (GlMainWidget *) widget;

	initOrUpdateBoxPlots();

	if (e->type() == QEvent::MouseMove) {
		QMouseEvent *me = (QMouseEvent *) e;
		int x = glWidget->width() - me->x();
		int y = me->y();
		Coord screenCoords((double) x, (double) y, 0);
		Coord sceneCoords = glWidget->getScene()->getCamera()->screenTo3DWorld(screenCoords);
		selectedAxis = parallelView->getAxisUnderPointer(me->x(), me->y());
		if (selectedAxis != NULL && dynamic_cast<QuantitativeParallelAxis *>(selectedAxis)) {
			if (axisBoxPlotMap.find((QuantitativeParallelAxis *)selectedAxis) != axisBoxPlotMap.end())
				if (parallelView->getLayoutType() == ParallelCoordinatesDrawing::CIRCULAR) {
					rotateVector(sceneCoords, -(selectedAxis->getRotationAngle()), Z_ROT);
				}
				axisBoxPlotMap[(QuantitativeParallelAxis *)selectedAxis]->setHighlightRangeIfAny(sceneCoords);
		}
		parallelView->refresh();
		//return true;
	}

	if (e->type() == QEvent::MouseButtonPress) {
		return false;
	}

	if (e->type() == QEvent::MouseButtonRelease) {
		if (selectedAxis != NULL && dynamic_cast<QuantitativeParallelAxis *>(selectedAxis)) {
			Observable::holdObservers();
			if (axisBoxPlotMap.find((QuantitativeParallelAxis *)selectedAxis) != axisBoxPlotMap.end())
				parallelView->highlightDataInAxisBoxPlotRange((QuantitativeParallelAxis *) selectedAxis);
			Observable::unholdObservers();
			selectedAxis = NULL;
			parallelView->refresh();
			//return true;
		}
	}

	return false;
}

bool ParallelCoordsAxisBoxPlot::draw(GlMainWidget *glMainWidget) {

	Camera *camera = glMainWidget->getScene()->getLayer("Main")->getCamera();
	camera->initGl();

	map<QuantitativeParallelAxis *, GlAxisBoxPlot *>::iterator it;
	for (it = axisBoxPlotMap.begin(); it != axisBoxPlotMap.end(); ++it) {
		(it->second)->draw(0,0);
	}

	return true;
}

}
