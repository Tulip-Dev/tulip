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

#include "ParallelCoordsAxisBoxPlot.h"

#include "ParallelCoordinatesView.h"
#include "ParallelTools.h"
#include "QuantitativeParallelAxis.h"

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qcursor.h>
#include <QtGui/qevent.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include <map>

#include <tulip/tulipconf.h>
#include <tulip/InteractorComponent.h>
#include <tulip/TlpTools.h>
#include <tulip/GlPolygon.h>
#include <tulip/GlLabel.h>
#include <tulip/GlQuad.h>
#include <tulip/GlLines.h>
#include <tulip/GlSimpleEntity.h>
#include <tulip/GlMainWidget.h>

using namespace std;

namespace tlp {

const Color lightBlue(0, 217, 255, 100);
const Color darkBlue(0, 0, 255, 50);

ParallelCoordsAxisBoxPlot::~ParallelCoordsAxisBoxPlot() {
	deleteGlAxisPlot();
	drawAxisBoxPlot = false;
	parallelView->refresh();
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
		drawAxisBoxPlot = true;
		lastNbAxis = allAxis.size();
		parallelView->refresh();
		return;
	}

	if ((lastNbAxis != 0 && lastNbAxis != allAxis.size()) || (currentGraph != parallelView->getGraph())) {
		deleteGlAxisPlot();
		buildGlAxisPlot(allAxis);
		drawAxisBoxPlot = true;
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
				axisBoxPlotMap[(QuantitativeParallelAxis *)selectedAxis]->setHighlightRangeIfAny(sceneCoords);
		}
		drawAxisBoxPlot = true;
		parallelView->refresh();
		return true;
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
			drawAxisBoxPlot = true;
			parallelView->refresh();
			return true;
		}
	}

	drawAxisBoxPlot = true;
	return false;
}

bool ParallelCoordsAxisBoxPlot::draw(GlMainWidget *glMainWidget) {

	if (!drawAxisBoxPlot) {
		return false;
	}

	Camera *camera = glMainWidget->getScene()->getLayer("Main")->getCamera();
	camera->initGl();

	map<QuantitativeParallelAxis *, GlAxisBoxPlot *>::iterator it;
	for (it = axisBoxPlotMap.begin(); it != axisBoxPlotMap.end(); ++it) {
		(it->second)->draw(0,0);
	}

	drawAxisBoxPlot = false;
	return true;
}

}
