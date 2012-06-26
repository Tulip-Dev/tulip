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
#include "tulip/GlMainWidgetGraphicsItem.h"

using namespace tlp;


GlMainView::GlMainView(): _glMainWidget(NULL), _overviewItem(NULL), _sceneConfigurationWidget(NULL), _sceneLayersConfigurationWidget(NULL), _quickAccessBar(NULL), _quickAccessBarItem(NULL) {
}

GlMainView::~GlMainView() {
  delete _sceneConfigurationWidget;
  delete _sceneLayersConfigurationWidget;
  delete _overviewItem;
}

void GlMainView::draw(tlp::PluginProgress*) {
  _glMainWidget->draw();
}

void GlMainView::refresh(PluginProgress *) {
  _glMainWidget->draw(false);
}

void GlMainView::drawOverview(bool generatePixmap) {
  if(_overviewItem == NULL) {
    _overviewItem=new GlOverviewGraphicsItem(this,*_glMainWidget->getScene());
    addToScene(_overviewItem);
    _overviewItem->setPos(QPointF(0,0));
    generatePixmap=true;
  }

  _overviewItem->draw(generatePixmap);
}

void GlMainView::assignNewGlMainWidget(GlMainWidget *glMainWidget, bool deleteOldGlMainWidget) {
  _glMainWidget=glMainWidget;
  setCentralWidget(_glMainWidget,deleteOldGlMainWidget);
  GlMainWidgetGraphicsItem *glMainWidgetGraphicsItem=dynamic_cast<GlMainWidgetGraphicsItem*>(centralItem());
  delete _sceneConfigurationWidget;
  _sceneConfigurationWidget = new SceneConfigWidget();
  _sceneConfigurationWidget->setGlMainWidget(_glMainWidget);
  _sceneLayersConfigurationWidget = new SceneLayersConfigWidget();
  _sceneLayersConfigurationWidget->setGlMainWidget(_glMainWidget);
  connect(_sceneLayersConfigurationWidget,SIGNAL(drawNeeded()),this,SIGNAL(drawNeeded()));
  connect(glMainWidgetGraphicsItem,SIGNAL(widgetPainted(bool)),this,SLOT(glMainViewDrawn(bool)));
  connect(graphicsView()->scene(),SIGNAL(sceneRectChanged(QRectF)),this,SLOT(sceneRectChanged(QRectF)));
}

GlOverviewGraphicsItem *GlMainView::overviewItem() const {
  return _overviewItem;
}

void GlMainView::setupWidget() {
  assignNewGlMainWidget(new GlMainWidget(NULL,this),true);
}

GlMainWidget* GlMainView::getGlMainWidget() const {
  return _glMainWidget;
}

void GlMainView::centerView() {
  getGlMainWidget()->getScene()->centerScene();
  emit drawNeeded();
}

void GlMainView::glMainViewDrawn(bool graphChanged) {
  drawOverview(graphChanged);
}

QList<QWidget*> GlMainView::configurationWidgets() const {
  return QList<QWidget*>() << _sceneConfigurationWidget << _sceneLayersConfigurationWidget;
}

void GlMainView::setOverviewVisible(bool display) {
  drawOverview(true);
  _overviewItem->setVisible(display);
}

bool GlMainView::overviewVisible() const {
  return _overviewItem != NULL && _overviewItem->isVisible();
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

  return QPixmap::fromImage(_glMainWidget->createPicture(realSize.width(),realSize.height(),false));
}

void GlMainView::graphDeleted() {
  setGraph(NULL);
}

void GlMainView::fillContextMenu(QMenu *menu, const QPointF &) {
  QAction* a = menu->addAction(trUtf8("Show overview"));
  a->setCheckable(true);
  a->setChecked(overviewVisible());
  connect(a,SIGNAL(triggered(bool)),this,SLOT(setOverviewVisible(bool)));
}

void GlMainView::applySettings() {
  _sceneConfigurationWidget->applySettings();
}
