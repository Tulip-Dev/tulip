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

#include "tulip/SceneLayersConfigWidget.h"

#include <tulip/GlMainView.h>
#include <tulip/GlMainWidget.h>
#include <tulip/SceneLayersModel.h>
#include "ui_SceneLayersConfigWidget.h"

using namespace tlp;

SceneLayersConfigWidget::SceneLayersConfigWidget(QWidget *parent): QWidget(parent), _ui(new Ui::SceneLayersConfigWidget), _glMainWidget(NULL) {
  _ui->setupUi(this);
}

SceneLayersConfigWidget::~SceneLayersConfigWidget() {
  delete _ui;
}

void SceneLayersConfigWidget::setGlMainWidget(GlMainWidget *glMainWidget) {
  _glMainWidget = glMainWidget;
  SceneLayersModel* model = new SceneLayersModel(_glMainWidget->getScene(),_ui->treeView);
  _ui->treeView->setModel(model);
  connect(model,SIGNAL(drawNeeded(tlp::GlScene*)),this,SIGNAL(drawNeeded()));
}
