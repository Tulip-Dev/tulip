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
#include "tulip/GlMainView.h"
#include <tulip/GlMainWidget.h>
#include <tulip/SceneConfigWidget.h>
#include <tulip/SceneLayersConfigWidget.h>
#include <tulip/GlOverviewGraphicsItem.h>
#include <tulip/QuickAccessBar.h>
#include <tulip/GlGraphComposite.h>
#include <QtGui/QGraphicsProxyWidget>
#include <QtGui/QGraphicsView>

using namespace tlp;


GlMainView::GlMainView(): _overviewVisible(false), _glMainWidget(NULL), _overviewItem(NULL), _sceneConfigurationWidget(NULL), _sceneLayersConfigurationWidget(NULL), _quickAccessBar(NULL), _quickAccessBarItem(NULL),_overviewContextMenu(NULL) {
}

GlMainView::~GlMainView() {
  delete _sceneConfigurationWidget;
  delete _sceneLayersConfigurationWidget;

  if(_overviewItem)
    delete _overviewItem;

  if(_overviewContextMenu)
    delete _overviewContextMenu;
}

void GlMainView::draw(tlp::PluginProgress*) {
  _glMainWidget->draw();
}

void GlMainView::refresh(PluginProgress *) {
  _glMainWidget->draw(false);
}

void GlMainView::drawOverview(bool generatePixmap) {
  if(!_overviewVisible)
    return;

  if(_overviewItem == NULL) {
    _overviewItem=new GlOverviewGraphicsItem(this,*_glMainWidget->getScene());
    addToScene(_overviewItem);
    _overviewItem->setPos(QPointF(0,0));
    generatePixmap=true;


  }

  if(_overviewContextMenu == NULL) {
    graphicsView()->setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction* viewSeparator = new QAction(trUtf8(""),this);
    viewSeparator->setSeparator(true);
    graphicsView()->addAction(viewSeparator);
    _overviewContextMenu = new QAction(trUtf8("Overview visible"),this);
    _overviewContextMenu->setCheckable(true);
    _overviewContextMenu->setChecked(true);
    connect(_overviewContextMenu,SIGNAL(triggered(bool)),this,SLOT(setOverviewVisible(bool)));
    graphicsView()->addAction(_overviewContextMenu);
  }

  if(!_overviewItem->isVisible()) {
    return;
  }

  _overviewItem->draw(generatePixmap);
}

void GlMainView::setupWidget() {
  _glMainWidget = new GlMainWidget(NULL,this);
  setCentralWidget(_glMainWidget);
  delete _sceneConfigurationWidget;
  _sceneConfigurationWidget = new SceneConfigWidget();
  _sceneConfigurationWidget->setGlMainWidget(_glMainWidget);
  _sceneLayersConfigurationWidget = new SceneLayersConfigWidget();
  _sceneLayersConfigurationWidget->setView(this);
  connect(_sceneLayersConfigurationWidget,SIGNAL(drawNeeded()),this,SIGNAL(drawNeeded()));
  connect(_glMainWidget,SIGNAL(viewDrawn(GlMainWidget*,bool)),this,SLOT(glMainViewDrawn(GlMainWidget*,bool)));
  connect(graphicsView()->scene(),SIGNAL(sceneRectChanged(QRectF)),this,SLOT(sceneRectChanged(QRectF)));
}

GlMainWidget* GlMainView::getGlMainWidget() const {
  return _glMainWidget;
}

void GlMainView::centerView() {
  getGlMainWidget()->getScene()->centerScene();
}

void GlMainView::glMainViewDrawn(GlMainWidget *, bool graphChanged) {
  drawOverview(graphChanged);
}

QList<QWidget*> GlMainView::configurationWidgets() const {
  return QList<QWidget*>() << _sceneConfigurationWidget << _sceneLayersConfigurationWidget;
}

void GlMainView::setOverviewVisible(bool display) {
  if (!display) {
    delete _overviewItem;
    _overviewItem = NULL;
  }

  _overviewVisible = display;

  if(_overviewVisible)
    drawOverview(true);
}

bool GlMainView::overviewVisible() const {
  return _overviewVisible;
}

void GlMainView::setQuickAccessBarVisible(bool visible) {
  if (!visible) {
    delete _quickAccessBar;
    delete _quickAccessBarItem;
    _quickAccessBar = NULL;
    _quickAccessBarItem = NULL;
  }

  else if (!quickAccessBarVisible()) {
    _quickAccessBarItem = new QGraphicsProxyWidget();
    _quickAccessBar = new QuickAccessBar(_quickAccessBarItem);
    _quickAccessBar->setGlMainView(this);
    _quickAccessBarItem->setWidget(_quickAccessBar);
    addToScene(_quickAccessBarItem);
    _quickAccessBarItem->setZValue(10);
  }
}

bool GlMainView::quickAccessBarVisible() const {
  return _quickAccessBarItem != NULL;
}

void GlMainView::sceneRectChanged(const QRectF& rect) {
  if (_quickAccessBar != NULL) {
    _quickAccessBarItem->setPos(0,rect.height()-_quickAccessBarItem->size().height());
    _quickAccessBarItem->resize(rect.width(),_quickAccessBarItem->size().height());
  }
}

QPixmap GlMainView::snapshot(const QSize &outputSize) {
  if (_glMainWidget == NULL)
    return QPixmap();

  QSize realSize = outputSize;

  if (!realSize.isValid())
    realSize = _glMainWidget->size();

  return QPixmap::fromImage(_glMainWidget->createPicture(realSize.width(),realSize.height()));
}

void GlMainView::graphDeleted() {
  setGraph(NULL);
}

void GlMainView::applySettings() {
  _sceneConfigurationWidget->applySettings();
}

bool GlMainView::isLayoutProperty(tlp::PropertyInterface* pi) const {
  return pi == getGlMainWidget()->getScene()->getGlGraphComposite()->getInputData()->getElementLayout();
}
