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
#include "tulip/BaseGraphicsViewComponent.h"

#include <QtGui/QMenuBar>
#include <QtGui/QImageWriter>
#include <QtGui/QStatusBar>
#include <QtGui/QInputDialog>
#include <QtGui/QMouseEvent>
#include <QtGui/QToolTip>
#include <QtGui/QFrame>

#include <tulip/ExtendedClusterOperation.h>
#include <tulip/ColorProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/StableIterator.h>
#include <tulip/StringProperty.h>

#include <tulip/GlMetaNodeTrueRenderer.h>

#include "tulip/GWOverviewWidget.h"
#include "tulip/RenderingParametersDialog.h"
#include "tulip/LayerManagerWidget.h"
#include "tulip/AugmentedDisplayDialog.h"
#include "tulip/GridOptionsWidget.h"
#include "tulip/InteractorManager.h"
#include "tulip/TlpQtTools.h"
#include "tulip/GlMainWidgetGraphicsView.h"
#include "tulip/Interactor.h"
#include "tulip/NodeLinkDiagramComponent.h"
#include "tulip/TabWidgetHidableMenuGraphicsProxy.h"
#include "tulip/GlMainWidgetItem.h"

using namespace std;

namespace tlp {

  BaseGraphicsViewComponent::BaseGraphicsViewComponent(const string &realViewName):realViewName(realViewName),graphicsView(NULL),baseView(ViewPluginLister::getPluginObject(realViewName, NULL)){}

  BaseGraphicsViewComponent::~BaseGraphicsViewComponent() {}

  QWidget *BaseGraphicsViewComponent::construct(QWidget *parent) {
    QWidget *widget=AbstractView::construct(parent);
    baseView->construct(parent);
    connect(baseView,SIGNAL(elementSelected(unsigned int,bool)),this,SLOT(elementSelectedSlot(unsigned int,bool)));

    widget->resize(512, 512);

    GWOverviewWidget *overviewWidget = static_cast<GlMainView *>(baseView)->getOverviewWidget();
    QAction *overviewAction = static_cast<GlMainView *>(baseView)->getOverviewAction();

    graphicsView = new GlMainWidgetGraphicsView(widget,static_cast<GlMainView *>(baseView)->getGlMainWidget());
    graphicsView->resize(512, 512);

    setCentralWidget(graphicsView);

    tabWidgetProxy = new TabWidgetHidableMenuGraphicsProxy(30);
    tabWidgetProxy->setPos(0,0);
    tabWidgetProxy->resize(370, 470);
    tabWidgetProxy->scale(0.7,0.7);
    tabWidgetProxy->hideTabWidget();
    tabWidgetProxy->setZValue(10);
    list<pair<QWidget *,string> > configWidgets=baseView->getConfigurationWidget();
    for(list<pair<QWidget *,string> >::iterator it=configWidgets.begin();it!=configWidgets.end();++it){
    	tabWidgetProxy->addTab((*it).first, (*it).second.c_str());
    }

    graphicsView->scene()->addItem(tabWidgetProxy);

    overviewItem = NULL;
    if(overviewWidget){
    	overviewWidget->setDrawIfNotVisible(true);
    	overviewItem=new GlMainWidgetItem(overviewWidget->getView(),100,100,true);
    	overviewItem->setPos(0,0);
    	overviewItem->setZValue(1);
    	graphicsView->scene()->addItem(overviewItem);

    	connect(overviewWidget, SIGNAL(hideOverview(bool)), this, SLOT(hideOverview(bool)));
    	connect(overviewAction, SIGNAL(triggered(bool)), this, SLOT(setVisibleOverview(bool)));

    	tabWidgetProxy->translate(0,overviewItem->boundingRect().height()+60);
    }

    return graphicsView;

  }
  void BaseGraphicsViewComponent::draw() {
    baseView->draw();
    graphicsView->draw();
  }

  void BaseGraphicsViewComponent::refresh() {
    baseView->refresh();
    graphicsView->redraw();
  }

  void BaseGraphicsViewComponent::init() {
    baseView->init();
    draw();
  }

  void BaseGraphicsViewComponent::elementSelectedSlot(unsigned int id,bool node){
    emit elementSelected(id,node);
  }

  string BaseGraphicsViewComponent::getRealViewName() const {
    return realViewName;
  }

  void BaseGraphicsViewComponent::hideOverview(bool hide){
  	if(hide){
  		overviewItem->setVisible(false);
  	}else{
  		overviewItem->setVisible(true);
  	}
  }

  void BaseGraphicsViewComponent::setVisibleOverview(bool visible){
  	hideOverview(!visible);
  }


}

