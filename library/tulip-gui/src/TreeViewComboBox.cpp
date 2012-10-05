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

#include "tulip/TreeViewComboBox.h"

#include <QtGui/QHeaderView>

TreeViewComboBox::TreeViewComboBox(QWidget *parent): QComboBox(parent), _treeView(NULL) {
  _treeView = new QTreeView(this);
  _treeView->setEditTriggers(QTreeView::NoEditTriggers);
  _treeView->setAlternatingRowColors(true);
  _treeView->setSelectionBehavior(QTreeView::SelectRows);
  _treeView->setRootIsDecorated(false);
  _treeView->setAllColumnsShowFocus(true);
  _treeView->header()->setVisible(false);
  setView(_treeView);
}

void TreeViewComboBox::showPopup() {
  setRootModelIndex(QModelIndex());

  for(int i=1; i<model()->columnCount(); ++i)
    _treeView->hideColumn(i);

  _treeView->expandAll();
  _treeView->setItemsExpandable(false);
  QComboBox::showPopup();
}

void TreeViewComboBox::selectIndex(const QModelIndex& index) {
  setRootModelIndex(index.parent());
  setCurrentIndex(index.row());
}

QModelIndex TreeViewComboBox::selectedIndex() const {
  QModelIndex current = model()->index(currentIndex(),0,rootModelIndex());
  QModelIndex selected = view()->selectionModel()->currentIndex();

  if (!selected.isValid()) {
    return current;
  }

  return selected;

}

