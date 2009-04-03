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

#include "ParallelTools.h"
#include "ParallelCoordinatesView.h"

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

#include <tulip/tulipconf.h>
#include <tulip/InteractorComponent.h>
#include <tulip/GlLayer.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/GlRect.h>
#include <tulip/GlCircle.h>
#include <tulip/GlMainWidget.h>

#include "ParallelCoordsAxisSwapper.h"

using namespace std;

namespace tlp {

static Color axisHighlight(14,241,212,0);

static Color axisToSwapHighlight(0,255,0,0);

ParallelCoordsAxisSwapper::~ParallelCoordsAxisSwapper() {
	selectedAxis = NULL;
	parallelView->refresh();
}

void ParallelCoordsAxisSwapper::setView(View *view) {
	InteractorComponent::setView(view);
	parallelView = (ParallelCoordinatesView *) view;
}

bool ParallelCoordsAxisSwapper::eventFilter(QObject *widget, QEvent *e) {

	GlMainWidget *glWidget = (GlMainWidget *) widget;

	mouseMove = false;

	if (e->type() == QEvent::MouseMove && !axisSwapStarted) {
		QMouseEvent *me = (QMouseEvent *) e;
		mouseMove = true;
		if (!dragStarted) {
			selectedAxis = parallelView->getAxisUnderPointer(me->x(), me->y());
		} else {
			x = glWidget->width() - me->x();
			y = me->y();
			Coord screenCoords((double) x, (double) y, 0);
			Coord sceneCoords = glWidget->getScene()->getCamera()->screenTo3DWorld(screenCoords);
			Coord translationVector = sceneCoords - selectedAxis->getBaseCoord();
			selectedAxis->translate(Coord(translationVector.getX(), 0, 0));
			otherAxisToSwap = parallelView->getAxisUnderPointer(me->x(), me->y());
		}
		parallelView->refresh();
		return true;

	} else if (e->type() == QEvent::MouseButtonPress && ((QMouseEvent *) e)->button() == Qt::LeftButton) {
		if (selectedAxis != NULL && !dragStarted) {
			dragStarted = true;
			parallelView->removeAxis(selectedAxis);
			initialSelectedAxisCoord = selectedAxis->getBaseCoord();
			parallelView->getGlMainWidget()->draw();
		}
		return true;

	} else if (e->type() == QEvent::MouseButtonRelease && ((QMouseEvent *) e)->button() == Qt::LeftButton) {

		if (selectedAxis != NULL && dragStarted) {
			Coord translationVector = initialSelectedAxisCoord - selectedAxis->getBaseCoord();
			selectedAxis->translate(Coord(translationVector.getX(), 0, 0));
			parallelView->addAxis(selectedAxis);

			if (otherAxisToSwap != NULL && otherAxisToSwap != selectedAxis) {
				axisSwapStarted = true;
				parallelView->swapAxis(selectedAxis, otherAxisToSwap);
				axisSwapStarted = false;
				otherAxisToSwap = NULL;
			}
			selectedAxis = NULL;
			dragStarted = false;
			parallelView->draw();
		}
		return true;
	}
	selectedAxis = NULL;
	return false;
}

bool ParallelCoordsAxisSwapper::draw(GlMainWidget *glMainWidget) {

	if (selectedAxis != NULL) {

		glMainWidget->getScene()->getLayer("Main")->getCamera()->initGl();

		GlRect *axisHighlightRect = NULL;
		BoundingBox axisBB;

		if (!dragStarted) {
			axisBB = selectedAxis->getBoundingBox();
			axisHighlightRect = new GlRect(Coord(axisBB.first.getX(), axisBB.second.getY(), 0),
										   Coord(axisBB.second.getX(), axisBB.first.getY(), 0),
										   axisHighlight, axisHighlight);

		} else {
			if (otherAxisToSwap != NULL && otherAxisToSwap != selectedAxis) {
				axisBB = otherAxisToSwap->getBoundingBox();
				axisHighlightRect = new GlRect(Coord(axisBB.first.getX(), axisBB.second.getY(), 0),
											   Coord(axisBB.second.getX(), axisBB.first.getY(), 0),
											   axisToSwapHighlight, axisToSwapHighlight);
			}
		}

		if (axisHighlightRect != NULL) {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_SRC_COLOR);
			glEnable(GL_LIGHTING);
			axisHighlightRect->draw(0,0);
			glDisable(GL_LIGHTING);
			glDisable(GL_BLEND);
			delete axisHighlightRect;
		}

		if (dragStarted && mouseMove) {
			selectedAxis->draw(0,0);
		}
		return true;
	}

	return false;
}

}
