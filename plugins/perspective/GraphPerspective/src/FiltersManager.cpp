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
#include "FiltersManager.h"

#include "GraphPerspective.h"
#include <tulip/GraphHierarchiesModel.h>

#include "ui_FiltersManager.h"

using namespace tlp;

FiltersManager::FiltersManager(QWidget *parent): QWidget(parent), _ui(new Ui::FiltersManagerData) {
  _ui->setupUi(this);
  _ui->filtersListContents->layout()->setAlignment(Qt::AlignTop);
  addItem();
  GraphHierarchiesModel* model = Perspective::typedInstance<GraphPerspective>()->model();
  connect(model,SIGNAL(currentGraphChanged(tlp::Graph*)),this,SLOT(currentGraphChanged(tlp::Graph*)));
  currentGraphChanged(model->currentGraph());
}

FiltersManager::~FiltersManager() {
  delete _ui;
}

void FiltersManager::addItem() {
  FiltersManagerItem* item = new FiltersManagerItem();
  _items << item;
  _ui->filtersListContentsLayout->insertWidget(_ui->filtersListContentsLayout->count()-1,item);
  connect(item,SIGNAL(removed()),this,SLOT(delItem()));
  connect(item,SIGNAL(modeChanged(FiltersManagerItem::Mode)),this,SLOT(itemModeChanged(FiltersManagerItem::Mode)));
}

void FiltersManager::delItem() {
  delItem(static_cast<FiltersManagerItem*>(sender()));
}

void FiltersManager::delItem(FiltersManagerItem* item) {
  _items.removeAll(item);
  delete item;

  if (_items.empty())
    addItem();
}

void FiltersManager::itemModeChanged(FiltersManagerItem::Mode m) {
  if (m != FiltersManagerItem::Invalid)
    addItem();
}


void FiltersManager::currentGraphChanged(tlp::Graph* g) {
  _ui->filtersList->setEnabled(g != NULL);
}
