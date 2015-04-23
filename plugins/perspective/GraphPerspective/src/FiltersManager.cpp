/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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
#include <QToolButton>
#include <tulip/BooleanProperty.h>

#include "ui_FiltersManager.h"

using namespace tlp;

QToolButton* insertHeaderControl(HeaderFrame* header, int pos=-1) {
  QToolButton* result = new QToolButton(header);
  result->setMaximumSize(25,25);
  result->setMinimumSize(25,25);
  result->setIconSize(QSize(16,16));

  if (pos == -1)
    header->insertWidget(result);
  else {
    static_cast<QHBoxLayout*>(header->layout())->insertWidget(pos,result);
  }

  return result;
}

FiltersManager::FiltersManager(QWidget *parent): QWidget(parent), _ui(new Ui::FiltersManagerData) {
  _ui->setupUi(this);
  _ui->filtersListContents->layout()->setAlignment(Qt::AlignTop);
  addItem();
  GraphHierarchiesModel* model = Perspective::typedInstance<GraphPerspective>()->model();
  connect(model,SIGNAL(currentGraphChanged(tlp::Graph*)),this,SLOT(currentGraphChanged(tlp::Graph*)));
  currentGraphChanged(model->currentGraph());

  _playButton = insertHeaderControl(_ui->header);
  _playButton->setIcon(QIcon(":/tulip/gui/icons/22/media-playback-start.png"));
  connect(_playButton,SIGNAL(clicked()),this,SLOT(applyFilter()));
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

void FiltersManager::applyFilter() {
  Observable::holdObservers();
  _playButton->setEnabled(false);

  Graph* g = Perspective::typedInstance<GraphPerspective>()->model()->currentGraph();
  BooleanProperty* result = new BooleanProperty(g);
  result->setAllNodeValue(false);
  result->setAllEdgeValue(false);

  switch(_ui->header->currentMenuIndex()) {
  case None:
    break;

  case Current:
    *result = *(g->getProperty<BooleanProperty>("viewSelection"));
    break;

  case Nodes:
    result->setAllNodeValue(true);
    break;

  case Edges:
    result->setAllEdgeValue(true);
    break;

  case All:
    result->setAllNodeValue(true);
    result->setAllEdgeValue(true);
    break;

  default:
    break;
  }

  foreach(FiltersManagerItem* item, _items) {
    item->applyFilter(result);
    *(g->getProperty<BooleanProperty>("viewSelection")) = *result;
  }

  delete result;
  Observable::unholdObservers();

  _playButton->setEnabled(true);
}
