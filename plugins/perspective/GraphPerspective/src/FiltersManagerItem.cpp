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

#include "FiltersManagerItem.h"

#include "ui_FiltersManagerItem.h"

#include <QMenu>

#include "GraphPerspective.h"
#include <tulip/GraphHierarchiesModel.h>

#include "AbstractFiltersManagerItem.h"
#include "FiltersManagerCompareItem.h"
#include "FiltersManagerInvertItem.h"
#include "FiltersManagerAlgorithmItem.h"

using namespace tlp;
using namespace std;

FiltersManagerItem::FiltersManagerItem(QWidget *parent): QFrame(parent), _ui(new Ui::FiltersManagerItem) {
  connect(Perspective::typedInstance<GraphPerspective>()->model(),SIGNAL(currentGraphChanged(tlp::Graph*)),this,SLOT(graphChanged(tlp::Graph*)));

  _ui->setupUi(this);
  connect(_ui->delButton,SIGNAL(clicked()),this,SIGNAL(removed()));
  setMode(Invalid);
}

FiltersManagerItem::~FiltersManagerItem() {
  delete _ui;
}

void FiltersManagerItem::setCompareMode() {
  setMode(Compare);
}

void FiltersManagerItem::setAlgorithmMode() {
  setMode(Algorithm);
}

void FiltersManagerItem::setInvertMode() {
  setMode(Invert);
}

void FiltersManagerItem::setMode(Mode m) {
  _ui->delButton->setVisible(m != Invalid);
  _ui->addButton->setVisible(m == Invalid);
  _ui->dataBox->setVisible(m != Invalid);

  AbstractFiltersManagerItem* w = NULL;

  if (m == Compare)
    w = new FiltersManagerCompareItem;
  else if (m == Invert)
    w = new FiltersManagerInvertItem;
  else if (m == Algorithm)
    w = new FiltersManagerAlgorithmItem;

  _ui->dataBox->setWidget(w);

  if (w != NULL) {
    connect(w,SIGNAL(titleChanged()),this,SLOT(dataBoxTitleChanged()));
    _ui->dataBox->setTitle(w->title());

    w->setGraph(Perspective::typedInstance<GraphPerspective>()->model()->currentGraph());
  }

  emit modeChanged(m);
}

void FiltersManagerItem::addButtonClicked() {
  QMenu addMenu;
  addMenu.addAction(trUtf8("Invert selection"),this,SLOT(setInvertMode()));
  addMenu.addAction(trUtf8("Compare values"),this,SLOT(setCompareMode()));
  addMenu.addAction(trUtf8("Filtering algorithm"),this,SLOT(setAlgorithmMode()));
  addMenu.exec(QCursor().pos());
}

void FiltersManagerItem::dataBoxTitleChanged() {
  _ui->dataBox->setTitle(static_cast<AbstractFiltersManagerItem*>(sender())->title());
}

void FiltersManagerItem::graphChanged(Graph *g) {
  if (_ui->dataBox->widget() != NULL)
    static_cast<AbstractFiltersManagerItem*>(_ui->dataBox->widget())->setGraph(g);
}

void FiltersManagerItem::applyFilter(tlp::BooleanProperty* prop) {
  if (_ui->dataBox->widget() != NULL) {
    static_cast<AbstractFiltersManagerItem*>(_ui->dataBox->widget())->applyFilter(prop);
  }
}
