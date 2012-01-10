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
#include <tulip/GlOverviewGraphicsItem.h>

using namespace tlp;

GlMainView::GlMainView(): _displayOverview(false),_overview(NULL), _sceneConfigurationWidget(NULL) {
}

GlMainView::~GlMainView() {
}

void GlMainView::draw(tlp::PluginProgress*) {
  _glMainWidget->draw();
}

void GlMainView::drawOverview(bool generatePixmap) {
  if(!_displayOverview)
    return;

  if(_overview == NULL) {
    _overview=new GlOverviewGraphicsItem(this,*_glMainWidget->getScene());
    addToScene(_overview);
    _overview->setPos(QPointF(0,0));
    generatePixmap=true;
  }

  _overview->draw(generatePixmap);
}

void GlMainView::setupWidget() {
  _glMainWidget = new GlMainWidget(NULL,this);
  delete _sceneConfigurationWidget;
  _sceneConfigurationWidget = new SceneConfigWidget();
  _sceneConfigurationWidget->setGlMainWidget(_glMainWidget);
  setCentralWidget(_glMainWidget);
  connect(_glMainWidget,SIGNAL(viewDrawn(GlMainWidget*,bool)),this,SLOT(glMainViewDrawn(GlMainWidget*,bool)));
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
  return QList<QWidget*>() << _sceneConfigurationWidget;
}

void GlMainView::setDisplayOverview(bool display) {
  if (_displayOverview != display) {
    _displayOverview=display;
    //drawOverview();
  }
}

bool GlMainView::displayOverview() const {
  return _displayOverview;
}
