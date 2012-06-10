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
#include <tulip/BaseGraphicsViewComponent.h>

#include <tulip/GWOverviewWidget.h>
#include <tulip/GlMainWidgetGraphicsView.h>
#include <tulip/ViewPluginsManager.h>
#include <tulip/GlMainView.h>
#include <tulip/TabWidgetHidableMenuGraphicsProxy.h>
#include <tulip/GlMainWidgetItem.h>

#include <QtGui/QMenu>

using namespace std;

namespace tlp {

BaseGraphicsViewComponent::BaseGraphicsViewComponent(const string &realViewName):realViewName(realViewName),graphicsView(NULL),baseView(ViewPluginsManager::getInst().createView(realViewName)), showHideOptionWidgets(NULL) {}

BaseGraphicsViewComponent::~BaseGraphicsViewComponent() {
  delete baseView;
}

QWidget *BaseGraphicsViewComponent::construct(QWidget *parent) {
  AbstractView::construct(parent);
  QWidget* baseViewWidget = baseView->construct(NULL);
  baseViewWidget->setObjectName("baseView Widget");
  baseViewWidget->hide();
  connect(baseView,SIGNAL(elementSelected(unsigned int,bool)),this,SLOT(elementSelectedSlot(unsigned int,bool)));

  GlMainView *glView = dynamic_cast<GlMainView *>(baseView);

  // Check if the view widget contains a QGraphicsView
  // if it is the case, use it as the view widget
  QGraphicsView *viewGV = NULL;
  const QObjectList &childrens = baseViewWidget->children();

  for (int i = 0 ; i < childrens.size() ; ++i) {
    viewGV = dynamic_cast<QGraphicsView*>(childrens.at(i));

    if (viewGV)
      break;
  }

  GWOverviewWidget *overviewWidget = NULL;
  QAction *overviewAction = NULL;
  GlMainWidget *glWidget = NULL;

  if (!viewGV) {

    if (glView) {
      overviewWidget = glView->getOverviewWidget();
      overviewAction = glView->getOverviewAction();
      glWidget = glView->getGlMainWidget();
      showHideOptionWidgets = glView->getDialogMenu()->addAction("Options widgets");
      showHideOptionWidgets->setCheckable(true);
      showHideOptionWidgets->setChecked(true);
      connect(showHideOptionWidgets, SIGNAL(toggled(bool)), this, SLOT(setOptionsTabWidgetVisible(bool)));
    }

    GlMainWidgetGraphicsView *glGraphicsView = new GlMainWidgetGraphicsView(NULL,glWidget);

    if (!glWidget) {
      glGraphicsView->setCentralWidget(baseViewWidget);
      baseViewWidget->show();
    }

    graphicsView = glGraphicsView;
  }
  else {
    graphicsView = viewGV;
  }

  setCentralWidget(graphicsView);

  list<pair<QWidget *,string> > configWidgets=baseView->getConfigurationWidget();

  tabWidgetProxy = new TabWidgetHidableMenuGraphicsProxy(30);
  tabWidgetProxy->setPos(0,0);

  // Hack for special case
  if (realViewName == "Table view") {
    tabWidgetProxy->setPos(0,100);
  }

  tabWidgetProxy->resize(370, 470);
  tabWidgetProxy->scale(0.7,0.7);
  tabWidgetProxy->hideTabWidget();
  tabWidgetProxy->setZValue(10);


  for(list<pair<QWidget *,string> >::iterator it=configWidgets.begin(); it!=configWidgets.end(); ++it) {
    tabWidgetProxy->addTab((*it).first, (*it).second.c_str());
  }

  graphicsView->scene()->addItem(tabWidgetProxy);


  overviewItem = NULL;

  if(overviewWidget) {

    overviewWidget->setDrawIfNotVisible(true);
    overviewItem=new GlMainWidgetItem(overviewWidget->getView(),100,100);
    overviewItem->setPos(0,0);
    overviewItem->setZValue(1);
    graphicsView->scene()->addItem(overviewItem);

    connect(overviewWidget, SIGNAL(hideOverview(bool)), this, SLOT(hideOverview(bool)));
    connect(overviewAction, SIGNAL(triggered(bool)), this, SLOT(setVisibleOverview(bool)));
    connect(overviewAction, SIGNAL(toggled(bool)), this, SLOT(setVisibleOverview(bool)));

    tabWidgetProxy->translate(0,1./tabWidgetProxy->transform().m22() * overviewItem->sceneBoundingRect().height());
  }

  graphicsView->resize(512, 512);

  return graphicsView;
}

void BaseGraphicsViewComponent::draw() {
  baseView->draw();
  GlMainWidgetGraphicsView *glGV = dynamic_cast<GlMainWidgetGraphicsView*>(graphicsView);

  if (glGV)
    glGV->draw();
}

void BaseGraphicsViewComponent::refresh() {
  baseView->refresh();
  GlMainWidgetGraphicsView *glGV = dynamic_cast<GlMainWidgetGraphicsView*>(graphicsView);

  if (glGV)
    glGV->redraw();
}

void BaseGraphicsViewComponent::init() {
  baseView->init();
  draw();
}

void BaseGraphicsViewComponent::elementSelectedSlot(unsigned int id,bool node) {
  emit elementSelected(id,node);
}

string BaseGraphicsViewComponent::getRealViewName() const {
  return realViewName;
}

void BaseGraphicsViewComponent::hideOverview(bool hide) {
  if (!overviewItem)
    return;

  if(hide && overviewItem->isVisible()) {
    overviewItem->setVisible(false);
    tabWidgetProxy->translate(0,-1./tabWidgetProxy->transform().m22() * overviewItem->sceneBoundingRect().height());
  }
  else if (!hide && !overviewItem->isVisible()) {
    overviewItem->setVisible(true);
    tabWidgetProxy->translate(0, 1./tabWidgetProxy->transform().m22() * overviewItem->sceneBoundingRect().height());
  }
}

void BaseGraphicsViewComponent::setVisibleOverview(bool visible) {
  hideOverview(!visible);
}

void BaseGraphicsViewComponent::setOptionsTabWidgetVisible(const bool visible) {
    if (showHideOptionWidgets) {
        disconnect(showHideOptionWidgets, SIGNAL(toggled(bool)), this, SLOT(setOptionsTabWidgetVisible(bool)));
    }
    tabWidgetProxy->setVisible(visible);
    if (showHideOptionWidgets) {
        showHideOptionWidgets->setChecked(visible);
        connect(showHideOptionWidgets, SIGNAL(toggled(bool)), this, SLOT(setOptionsTabWidgetVisible(bool)));
    }
}

}
