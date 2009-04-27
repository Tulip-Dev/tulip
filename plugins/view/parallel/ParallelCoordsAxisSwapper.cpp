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

#include <tulip/GlLayer.h>
#include <tulip/GlBoundingBoxSceneVisitor.h>
#include <tulip/GlRect.h>
#include <tulip/GlCircle.h>
#include <tulip/GlQuad.h>
#include <tulip/GlMainWidget.h>

#include "ParallelTools.h"
#include "ParallelCoordinatesView.h"
#include "ParallelCoordsAxisSwapper.h"

using namespace std;

namespace tlp {

static Color axisHighlight(14,241,212,0);

static Color axisToSwapHighlight(0,255,0,0);

static inline float square(float x) {return x*x;}

// Given a triangle ABC, this function compute the (AB, AC) angle in degree using Al Kashi theorem
float computeABACAngleWithAlKashi(const Coord &A, const Coord &B, const Coord &C) {
	float AB = A.dist(B);
	float AC = A.dist(C);
	float BC = B.dist(C);
	return acos((square(AB)+square(AC)-square(BC)) / (2 * AB * AC)) * (180. / M_PI);
}

ParallelCoordsAxisSwapper::~ParallelCoordsAxisSwapper() {}

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
			if (parallelView->getLayoutType() == ParallelCoordinatesDrawing::CIRCULAR) {
				float rotAngle = computeABACAngleWithAlKashi(Coord(0,0,0), Coord(0, 50, 0), sceneCoords);
				if (sceneCoords.getX() < 0) {
					selectedAxis->setRotationAngle(rotAngle);
				} else {
					selectedAxis->setRotationAngle(-rotAngle);
				}

			} else {
				Coord translationVector = sceneCoords - selectedAxis->getBaseCoord();
				selectedAxis->translate(Coord(translationVector.getX(), translationVector.getY(), 0));
			}
			otherAxisToSwap = parallelView->getAxisUnderPointer(me->x(), me->y());
		}
		drawInteractor = true;
		parallelView->refresh();
		return true;

	} else if (e->type() == QEvent::MouseButtonPress && ((QMouseEvent *) e)->button() == Qt::LeftButton) {
		if (selectedAxis != NULL && !dragStarted) {
			dragStarted = true;
			parallelView->removeAxis(selectedAxis);
			initialSelectedAxisRotAngle = selectedAxis->getRotationAngle();
			selectedAxis->setRotationAngle(0);
			initialSelectedAxisCoord = selectedAxis->getBaseCoord();
			parallelView->getGlMainWidget()->draw();
		}
		return true;

	} else if (e->type() == QEvent::MouseButtonRelease && ((QMouseEvent *) e)->button() == Qt::LeftButton) {

		if (selectedAxis != NULL && dragStarted) {
			selectedAxis->setRotationAngle(0);
			Coord translationVector = initialSelectedAxisCoord - selectedAxis->getBaseCoord();
			selectedAxis->translate(Coord(translationVector.getX(), translationVector.getY(), 0));
			selectedAxis->setRotationAngle(initialSelectedAxisRotAngle);
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
	drawInteractor = true;
	return false;
}

bool ParallelCoordsAxisSwapper::draw(GlMainWidget *glMainWidget) {

	if (!drawInteractor) {
		return false;
	}

	drawInteractor = false;

	if (selectedAxis != NULL) {

		glMainWidget->getScene()->getLayer("Main")->getCamera()->initGl();

		GlQuad *axisHighlightRect = NULL;
		BoundingBox axisBB;

		if (!dragStarted) {
			Vector<Coord, 4> axisBP = selectedAxis->getBoundingPolygonCoords();
			Coord quadCoords[4] = {axisBP[0], axisBP[1], axisBP[2], axisBP[3]};
			axisHighlightRect = new GlQuad(quadCoords, axisHighlight);

		} else {
			if (otherAxisToSwap != NULL && otherAxisToSwap != selectedAxis) {
				Vector<Coord, 4> axisBP = otherAxisToSwap->getBoundingPolygonCoords();
				Coord quadCoords[4] = {axisBP[0], axisBP[1], axisBP[2], axisBP[3]};
				axisHighlightRect = new GlQuad(quadCoords, axisToSwapHighlight);
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
			selectedAxis->disableTrickForSelection();
			selectedAxis->draw(0,0);
			selectedAxis->enableTrickForSelection();
		}
		return true;
	}

	return false;
}

}
