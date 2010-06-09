/*
 * GraphicsViewTest.cpp
 *
 *  Created on: 4 juil. 2009
 *      Author: antoine
 */

#include <QtGui/QDialog>
#include <QtGui/QMenu>
#include <QtGui/QResizeEvent>
#include <QtGui/QVBoxLayout>
#include <QtGui/QMessageBox>
#include <QtGui/QToolBar>
#include <QtOpenGL/QGLWidget>
#include <QtOpenGL/QGLPixelBuffer>

#include <tulip/GlRectTextured.h>
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

class GlWidget : public QGLWidget {

public :

	GlWidget(QWidget *parent = 0) : QGLWidget(GlInit(), parent, GlMainWidget::getFirstQGLWidget()) {}

	void makeCurrent() {
		QGLWidget::makeCurrent();
		GlDisplayListManager::getInst().changeContext((unsigned long)GlMainWidget::getFirstQGLWidget());
		GlTextureManager::getInst().changeContext((unsigned long)GlMainWidget::getFirstQGLWidget());
	}

};

GlMainWidgetGraphicsView::GlMainWidgetGraphicsView(AbstractView *tulipView, QWidget *widget,GlMainWidget *glMainWidget,GWOverviewWidget *overviewWidget,QAction *overviewAction,bool fullWindow) : QGraphicsView(new QGraphicsScene(widget)),
glMainWidget(glMainWidget), tulipView(tulipView), drawNeeded(true){
	setRenderHints(QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
	glWidget = new GlWidget();
	setViewport(glWidget);
	setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	setFrameStyle(QFrame::NoFrame);

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
	tabWidgetProxy->translate(0,0);
	tabWidgetProxy->resize(370, 370);
	tabWidgetProxy->scale(0.8,0.8);
	tabWidgetProxy->hideTabWidget();
	tabWidgetProxy->setZValue(1);
	//proxy->setZValue(1);

	if(fullWindow){
	  glSceneItem=new GlMainWidgetGraphicsWidget(this,glMainWidget,width(),height(),NULL,Qt::Window);
	  glSceneItem->setPos(0,0);
	  tabWidgetProxy->setParentItem(glSceneItem);
	}else{
	  glSceneItem=new GlMainWidgetGraphicsWidget(this,glMainWidget,256,256,NULL,Qt::Window);
	  glSceneItem->setPos(64,64);
	  scene()->addItem(tabWidgetProxy);
	}

	glSceneItem->setZValue(0);
	scene()->addItem(glSceneItem);

	if(overviewWidget){
	  overviewWidget->setDrawIfNotVisible(true);
	  overviewItem=new GlMainWidgetItem(this,overviewWidget->getView(),100,100,glSceneItem,true);
	  overviewItem->setPos(50,50);
	  scene()->addItem(overviewItem);
	  connect(overviewWidget, SIGNAL(hideOverview(bool)), this, SLOT(hideOverview(bool)));
	  connect(overviewAction, SIGNAL(triggered(bool)), this, SLOT(setVisibleOverview(bool)));
	  if(fullWindow){
	    tabWidgetProxy->translate(0,128);
	  }
	}
}

void GlMainWidgetGraphicsView::draw(bool) {
	glSceneItem->getGlMainWidgetItem()->setRedrawNeed(true);
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
	glMainWidget->resizeGL(size().width(),size().height());
	if (scene()) {
		scene()->setSceneRect(QRect(QPoint(0, 0), size()));
	}
	glSceneItem->resize(size().width(),size().height());
}

void GlMainWidgetGraphicsView::hideOverview(bool hide){
  if(hide){
    overviewItem->setVisible(false);
    tabWidgetProxy->translate(0,-128);
  }else{
    overviewItem->setVisible(true);
    tabWidgetProxy->translate(0,128);
  }
}

void GlMainWidgetGraphicsView::setVisibleOverview(bool visible){
  hideOverview(!visible);
}

}

