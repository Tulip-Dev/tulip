/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include <iostream>

#ifdef  _WIN32
// compilation pb workaround
#include <windows.h>
#endif
#include <QtGui/qevent.h>

#include <tulip/Graph.h>
#include <tulip/GlMainWidget.h>
#include <tulip/GlTools.h>
#include <tulip/DrawingTools.h>
#include <tulip/QtGlSceneZoomAndPanAnimator.h>

#include "tulip/MouseBoxZoomer.h"

using namespace std;
using namespace tlp;

MouseBoxZoomer::MouseBoxZoomer(Qt::MouseButton button,
		Qt::KeyboardModifier modifier)
: mButton(button), kModifier(modifier), x(0), y(0), w(0), h(0), started(false), graph(0) {}
MouseBoxZoomer::~MouseBoxZoomer() {}
//=====================================================================
bool MouseBoxZoomer::eventFilter(QObject *widget, QEvent *e) {
	GlMainWidget *glw = static_cast<GlMainWidget *>(widget);
	if (e->type() == QEvent::MouseButtonPress) {
		QMouseEvent * qMouseEv = static_cast<QMouseEvent *>(e);
		if (qMouseEv->buttons() == mButton &&
				(kModifier == Qt::NoModifier ||
						qMouseEv->modifiers() & kModifier)) {
			if (!started) {
				x = qMouseEv->x();
				y =  glw->height() - qMouseEv->y();
				w = 0; h = 0;
				started = true;
				graph = glw->getScene()->getGlGraphComposite()->getInputData()->getGraph();
			}
			else {
				if (glw->getScene()->getGlGraphComposite()->getInputData()->getGraph() != graph) {
					graph = NULL;
					started = false;
				}
			}
			return true;
		}
		if (qMouseEv->buttons()==Qt::MidButton){
			started = false;
			glw->redraw();
			return true;
		}
		return false;
	}
	if (e->type() == QEvent::MouseMove) {
		QMouseEvent * qMouseEv = static_cast<QMouseEvent *>(e);
		if ((qMouseEv->buttons() & mButton) &&
				(kModifier == Qt::NoModifier ||
						qMouseEv->modifiers() & kModifier)) {
			if (glw->getScene()->getGlGraphComposite()->getInputData()->getGraph() != graph) {
				graph = NULL;
				started = false;
			}
			if (started){
				if ((qMouseEv->x() > 0) && (qMouseEv->x() < glw->width()))
					w = qMouseEv->x() - x;
				if ((qMouseEv->y() > 0) && (qMouseEv->y() < glw->height()))
					h = y - (glw->height() - qMouseEv->y());
				glw->redraw();
				return true;
			}
		}
	}
	if (e->type() == QEvent::MouseButtonDblClick) {
		BoundingBox graphBB = computeBoundingBox(graph,graph->getProperty<LayoutProperty>("viewLayout"),
				graph->getProperty<SizeProperty>("viewSize"),
				graph->getProperty<DoubleProperty>("viewRotation"));
		QtGlSceneZoomAndPanAnimator zoomAnPan(glw, graphBB);
		zoomAnPan.animateZoomAndPan();
		return true;
	}
	if (e->type() == QEvent::MouseButtonRelease) {

		QMouseEvent * qMouseEv = static_cast<QMouseEvent *>(e);
		if ((qMouseEv->button() == mButton &&
				(kModifier == Qt::NoModifier ||
						qMouseEv->modifiers() & kModifier))) {
			if (glw->getScene()->getGlGraphComposite()->getInputData()->getGraph() != graph) {
				graph = NULL;
				started = false;
			}
			if (started) {
				started = false;
				if(!(w==0 && h==0)){
					//				if (abs(w) < 10){
					//					w=10;
					//				}
					//				if(abs(h) < 10) {
					//					h=10;
					//				}
					//      cerr << __FUNCTION__ << ": " << this << "(x,y)=(" << e->x() << "," << e->y() << ")" << endl;
					int width = glw->width();
					int height = glw->height();
					//	glw->getScene()->translateCamera(width/2 - (x+w/2), height/2 - (y-h/2), 0);
					//	w = abs(w); h = abs(h);
					//    Camera *cam = glw->getScene()->getCamera();
					//	//we prevent zooming in a minimal square area less than 4x4: a least
					//	//one of the 2 lengths must be higher than 3
					//  if(((double) width / (double) w) < ((double) height / (double) h))
					//    cam->setZoomFactor(cam->getZoomFactor() * ((double) width / (double) w));
					//  else
					//    cam->setZoomFactor(cam->getZoomFactor() * ((double) height / (double) h));
					//	glw->draw(false);
					Coord bbMin(width-x, height - y+h);
					Coord bbMax(width-(x+w), height - y);

					BoundingBox sceneBB;
					sceneBB[0] = glw->getScene()->getCamera().screenTo3DWorld(bbMin);
					sceneBB[1] = glw->getScene()->getCamera().screenTo3DWorld(bbMax);

					QtGlSceneZoomAndPanAnimator zoomAnPan(glw, sceneBB);
					zoomAnPan.animateZoomAndPan();
				}
			}
			return true;
		}
	}
	return false;
}
//=====================================================================
bool MouseBoxZoomer::draw(GlMainWidget *glw) {
	if (!started) return false;
	if (glw->getScene()->getGlGraphComposite()->getInputData()->getGraph() != graph) {
		graph = NULL;
		started = false;
	}
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode (GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)glw->width(), 0.0, (GLdouble)glw->height());
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_SRC_COLOR);

	float col[4] = {0.8, 0.4, 0.4, 0.2};
	setColor(col);
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x+w, y);
	glVertex2f(x+w, y-h);
	glVertex2f(x, y-h);
	glEnd();
	glDisable(GL_BLEND);

	glLineWidth(2);
	glLineStipple(2, 0xAAAA);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x, y);
	glVertex2f(x+w, y);
	glVertex2f(x+w, y-h);
	glVertex2f(x, y-h);
	glEnd();
	/*
  glLineWidth(1);
  glDisable(GL_LINE_STIPPLE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glEnable(GL_LIGHTING);
	 */
	glPopMatrix();
	glMatrixMode (GL_PROJECTION);
	glPopMatrix();
	glMatrixMode (GL_MODELVIEW);
	glPopAttrib();
	return true;
}

//=====================================================================
