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

#include <QtGui/QApplication>
#include <QtGui/QDialog>
#include <QtGui/QMenu>
#include <QtGui/QResizeEvent>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QToolBar>
#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLPixelBuffer>

#include <tulip/GlDisplayListManager.h>
#include <tulip/GlTextureManager.h>
#include <tulip/Interactor.h>
#include <tulip/QGlPixelBufferManager.h>
#include "tulip/GlMainWidgetGraphicsView.h"
#include "tulip/GlMainWidgetItem.h"

using namespace std;

namespace tlp {

//==================================================
static QGLFormat GlInit() {
	QGLFormat tmpFormat;
	tmpFormat.setDirectRendering(true);
	tmpFormat.setDoubleBuffer(true);
	tmpFormat.setAccum(false);
	tmpFormat.setStencil(true);
	tmpFormat.setOverlay(false);
	tmpFormat.setDepth(true);
	tmpFormat.setRgba(true);
	tmpFormat.setAlpha(true);
	tmpFormat.setOverlay(false);
	tmpFormat.setStereo(false);
	tmpFormat.setSampleBuffers(true);
	return tmpFormat;
}

GlMainWidgetGraphicsView::GlMainWidgetGraphicsView(QWidget *parent,GlMainWidget *glMainWidget) : QGraphicsView(new QGraphicsScene(), parent),
		glMainWidget(glMainWidget), drawNeeded(true){
	setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing | QPainter::TextAntialiasing);
	viewportWidet = new QGLWidget(GlInit(), 0, GlMainWidget::getFirstQGLWidget());
	setViewport(viewportWidet);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	setFrameStyle(QFrame::NoFrame);
	scene()->setBackgroundBrush(Qt::white);
	glSceneItem=new GlMainWidgetItem(glMainWidget,width(),height());
	scene()->addItem(glSceneItem);
	glSceneItem->setPos(0,0);
	glSceneItem->setZValue(0);
}

GlMainWidgetGraphicsView::~GlMainWidgetGraphicsView() {
	delete viewportWidet;
}

void GlMainWidgetGraphicsView::draw(bool) {
	glSceneItem->setRedrawNeeded(true);
	scene()->update();
}

void GlMainWidgetGraphicsView::redraw() {
	scene()->update();
}


void GlMainWidgetGraphicsView::resizeEvent(QResizeEvent *event) {
	QGraphicsView::resizeEvent(event);
	if (scene()) {
		scene()->setSceneRect(QRect(QPoint(0, 0), size()));
	}
	glSceneItem->resize(width(),height());
	if (scene()) {
		scene()->update();
	}
	// Hack : send a mouse event to force redraw of the scene (otherwise artifacts was displayed when maximizing or minimizing the graphics view)
	QMouseEvent *eventModif = new QMouseEvent(QEvent::MouseMove,QPoint(size().width()/2, size().height()/2), Qt::NoButton, Qt::NoButton, Qt::NoModifier);
	QApplication::sendEvent(this, eventModif);
}

}

