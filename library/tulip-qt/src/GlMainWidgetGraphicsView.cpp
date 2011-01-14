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
#include "tulip/TabWidgetHidableMenuGraphicsProxy.h"
#include "tulip/GlMainWidgetItem.h"
#include "tulip/GWOverviewWidget.h"

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

GlMainWidgetGraphicsView::GlMainWidgetGraphicsView(AbstractView *tulipView, QWidget *widget,GlMainWidget *glMainWidget,GWOverviewWidget *overviewWidget,QAction *overviewAction,bool fullWindow) : QGraphicsView(new QGraphicsScene(widget)),
		glMainWidget(glMainWidget), tulipView(tulipView), drawNeeded(true){
	setRenderHints(QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
	glWidget = new QGLWidget(GlInit(), 0, GlMainWidget::getFirstQGLWidget());
	setViewport(glWidget);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	setFrameStyle(QFrame::NoFrame);
	scene()->setBackgroundBrush(Qt::white);

	tabWidgetProxy = new TabWidgetHidableMenuGraphicsProxy(30);
	/*QToolBar *toolBar=new QToolBar;
	list<Interactor *> interactorsList=tulipView->getInteractors();
	list<QAction *> interactorsActionList;
	for(list<Interactor *>::iterator it=interactorsList.begin();it!=interactorsList.end();++it){
	  interactorsActionList.push_back((*it)->getAction());
	}

	for(list<QAction *>::iterator it=interactorsActionList.begin();it!=interactorsActionList.end();++it) {
	  toolBar->addAction(*it);
	}

	QGraphicsProxyWidget *proxy = scene()->addWidget(toolBar);*/
	tabWidgetProxy->setPos(0,0);
	tabWidgetProxy->resize(370, 370);
	tabWidgetProxy->scale(0.8,0.8);
	tabWidgetProxy->hideTabWidget();
	tabWidgetProxy->setZValue(1);
	scene()->addItem(tabWidgetProxy);

	if(fullWindow){
		glSceneItem=new GlMainWidgetItem(glMainWidget,width(),height());
		scene()->addItem(glSceneItem);
		glSceneItem->setPos(0,0);
	}else{
		glSceneItem=new GlMainWidgetItem(glMainWidget,256,256);
		scene()->addItem(glSceneItem);
		glSceneItem->setPos(64,64);
	}

	glSceneItem->setZValue(0);


	overviewItem = NULL;
	if(overviewWidget){
		overviewWidget->setDrawIfNotVisible(true);
		overviewItem=new GlMainWidgetItem(overviewWidget->getView(),100,100,true);
		overviewItem->setPos(0,0);
		overviewItem->setZValue(1);
		scene()->addItem(overviewItem);

		connect(overviewWidget, SIGNAL(hideOverview(bool)), this, SLOT(hideOverview(bool)));
		connect(overviewAction, SIGNAL(triggered(bool)), this, SLOT(setVisibleOverview(bool)));
		if(fullWindow){
			tabWidgetProxy->translate(0,overviewItem->boundingRect().height()+40);
		}
	}
}

void GlMainWidgetGraphicsView::draw(bool) {
	glSceneItem->setRedrawNeeded(true);
	scene()->update();
}

void GlMainWidgetGraphicsView::redraw() {
	scene()->update();
}

void GlMainWidgetGraphicsView::addToTabWidget(const std::string &name, QWidget *widget){
	tabWidgetProxy->addTab(widget,name.c_str());
}

void GlMainWidgetGraphicsView::resizeEvent(QResizeEvent *event) {
	QGraphicsView::resizeEvent(event);
	if (scene()) {
		scene()->setSceneRect(QRect(QPoint(0, 0), size()));
	}
	glSceneItem->resize(width(),height());
	overviewItem->setRedrawNeeded(true);
	if (scene()) {
		scene()->update();
	}
}

void GlMainWidgetGraphicsView::hideOverview(bool hide){
	if(hide){
		overviewItem->setVisible(false);
		tabWidgetProxy->translate(0,-(overviewItem->boundingRect().height()+40));
	}else{
		overviewItem->setVisible(true);
		tabWidgetProxy->translate(0,overviewItem->boundingRect().height()+40);
	}
}

void GlMainWidgetGraphicsView::setVisibleOverview(bool visible){
	hideOverview(!visible);
}

}

