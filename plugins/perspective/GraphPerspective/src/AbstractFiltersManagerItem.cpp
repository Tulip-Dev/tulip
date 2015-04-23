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

#include "AbstractFiltersManagerItem.h"

using namespace tlp;

#include <QStandardItem>
#include <QComboBox>

QStandardItem* itemAt(QComboBox* combo, int i) {
  QStandardItemModel* model = qobject_cast<QStandardItemModel*>(combo->model());
  QModelIndex index = model->index(i,combo->modelColumn(),combo->rootModelIndex());
  return model->itemFromIndex(index);
}

QStandardItem* lastItem(QComboBox* combo) {
  return itemAt(combo,combo->count()-1);
}

void fillTitle(QComboBox* combo, const QString& s) {
  combo->addItem(s);
  QStandardItem* item = lastItem(combo);
  QFont f;
  f.setBold(true);
  item->setFont(f);
  item->setSelectable(false);
}

AbstractFiltersManagerItem::AbstractFiltersManagerItem(QWidget* parent): QWidget(parent), _graph(NULL) {}

void AbstractFiltersManagerItem::setGraph(Graph *g) {
  _graph = g;
  graphChanged();
}
void AbstractFiltersManagerItem::graphChanged() {
}
