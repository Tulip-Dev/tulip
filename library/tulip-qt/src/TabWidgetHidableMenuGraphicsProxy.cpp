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
	tabWidget->setToolTip("Double click to show Options Panel");
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
		QGraphicsSceneMouseEvent * event) {
	double dx = sceneBoundingRect().width() - transform().m11() * offsetWhenHiding;
	xStart = scenePos().x();
	if (hidden) {
		xEnd = xStart + dx;
		tabWidget->setToolTip("Double click to hide Options Panel");
	} else {
		xEnd = xStart - dx;
		tabWidget->setToolTip("Double click to show Options Panel");
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
