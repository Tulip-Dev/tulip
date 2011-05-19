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
#include <QtGui/QTabBar>
#include <QtCore/QTimeLine>
#include <QtCore/QEventLoop>
#include <QtGui/QApplication>

#include <iostream>

#include "tulip/TabWidgetHidableMenuGraphicsProxy.h"

using namespace std;

namespace tlp {
TabWidgetHidableMenuGraphicsProxy::TabWidgetHidableMenuGraphicsProxy(const unsigned int offsetWhenHiding) :
			hidden(false), offsetWhenHiding(offsetWhenHiding) {
	tabWidget = new QTabWidget();
	tabWidget->setTabPosition(QTabWidget::East);
	setWidget(tabWidget);
	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
	tabWidget->setToolTip("Double click to show Options Panel\nUse mouse wheel to scale the panel\nUse Ctrl + mouse wheel to change opacity");
}

void TabWidgetHidableMenuGraphicsProxy::wheelEvent(QGraphicsSceneWheelEvent * event) {
	if (event->modifiers() != Qt::ControlModifier && hidden) {
		return;
	}
	GraphicsWidgetBase::wheelEvent(event);
}

void TabWidgetHidableMenuGraphicsProxy::addTab(QWidget *widget, const QString &tabName) {
	tabWidget->addTab(widget, tabName);
}

const int nbAnimStep = 100;

void TabWidgetHidableMenuGraphicsProxy::hideTabWidget() {
	hidden = true;
	double dx = sceneBoundingRect().width() - transform().m11() * offsetWhenHiding;
	setPos(-dx, 0);
}

void TabWidgetHidableMenuGraphicsProxy::mouseDoubleClickEvent(
		QGraphicsSceneMouseEvent *event) {
	double dx = sceneBoundingRect().width() - transform().m11() * offsetWhenHiding;
	xStart = scenePos().x();
	if (hidden) {
		xEnd = xStart + dx;
		tabWidget->setToolTip("Double click to hide Options Panel\nUse mouse wheel to scale the panel\nUse Ctrl + mouse wheel to change opacity");
	} else {
		if (event->scenePos().x() < scenePos().x() + dx)
			return;
		xEnd = xStart - dx;
		tabWidget->setToolTip("Double click to show Options Panel\nUse mouse wheel to scale the panel\nUse Ctrl + mouse wheel to change opacity");
	}

	QTimeLine timeLine;
	timeLine.setFrameRange(0, nbAnimStep);
	timeLine.setCurveShape(QTimeLine::LinearCurve);
	connect(&timeLine, SIGNAL(frameChanged(int)), this, SLOT(animateTranslationStep(int)));
	timeLine.start();
	while (timeLine.state() != QTimeLine::NotRunning) {
		QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
	}
	hidden = !hidden;
}

void TabWidgetHidableMenuGraphicsProxy::animateTranslationStep(int animStep) {
	setPos(QPointF(xStart + (animStep / (float) nbAnimStep) * (xEnd - xStart), pos().y()));
}

}
