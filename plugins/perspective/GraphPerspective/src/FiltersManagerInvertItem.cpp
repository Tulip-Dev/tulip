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

#include "FiltersManagerInvertItem.h"

#include "ui_FiltersManagerInvertItem.h"

#include <tulip/Graph.h>
#include <tulip/BooleanProperty.h>

using namespace tlp;

FiltersManagerInvertItem::FiltersManagerInvertItem(QWidget* parent): AbstractFiltersManagerItem(parent), _ui(new Ui::FiltersManagerInvertItem) {
  _ui->setupUi(this);
  connect(_ui->elementCombo,SIGNAL(currentIndexChanged(int)),this,SIGNAL(titleChanged()));
}

FiltersManagerInvertItem::~FiltersManagerInvertItem() {
  delete _ui;
}


void FiltersManagerInvertItem::applyFilter(BooleanProperty* prop) {
  int i = _ui->elementCombo->currentIndex();

  if (i == 0 || i == 2) {
    node n;
    forEach(n, _graph->getNodes())
    prop->setNodeValue(n,!(prop->getNodeValue(n)));
  }

  if (i == 1 || i == 2) {
    edge e;
    forEach(e, _graph->getEdges())
    prop->setEdgeValue(e,!(prop->getEdgeValue(e)));
  }
}
QString FiltersManagerInvertItem::title() const {
  return trUtf8("Invert selection ") + _ui->elementCombo->currentText().toLower();
}
