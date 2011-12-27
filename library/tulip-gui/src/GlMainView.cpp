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


using namespace tlp;

GlMainView::GlMainView():overview(NULL) {
}

GlMainView::~GlMainView() {
}

void GlMainView::draw(tlp::PluginProgress*) {
  _glMainWidget->draw();
}

void GlMainView::drawOverview() {
  if(!overview) {
    overview=new GlOverviewGraphicsItem(this,*_glMainWidget->getScene());
    addToScene(overview);
    overview->setPos(QPointF(0,0));
  }

  overview->draw();
}

void GlMainView::setupWidget() {
  _glMainWidget = new GlMainWidget(NULL,this);
  setCentralWidget(_glMainWidget);

}

GlMainWidget* GlMainView::getGlMainWidget() const {
  return _glMainWidget;
}

void GlMainView::centerView() {
  getGlMainWidget()->getScene()->centerScene();
}
