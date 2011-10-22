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
#include "GraphHierarchiesEditor.h"

#include "GraphHierarchiesModel.h"
#include "ui_GraphHierarchiesEditor.h"

#include <assert.h>

GraphHierarchiesEditor::GraphHierarchiesEditor(QWidget *parent): QWidget(parent), _ui(new Ui::GraphHierarchiesEditorData) {
  _ui->setupUi(this);
}

void GraphHierarchiesEditor::setModel(GraphHierarchiesModel *model) {
  _ui->hierarchiesTree->setModel(model);
  _ui->hierarchiesTree->header()->resizeSections(QHeaderView::ResizeToContents);
}

GraphHierarchiesEditor::~GraphHierarchiesEditor() {
  delete _ui;
}
