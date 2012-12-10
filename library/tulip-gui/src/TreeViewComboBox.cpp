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
#include <QtGui/QStyledItemDelegate>
#include <QtGui/QPainter>
#include <QtGui/QStyleOptionViewItem>

class TreeViewDelegate: public QStyledItemDelegate {
public:
  TreeViewDelegate(QObject* parent = 0): QStyledItemDelegate(parent) {
  }

  QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QSize result = QStyledItemDelegate::sizeHint(option,index);
    result.setHeight(result.height()+10);
    return result;
  }
};

TreeViewComboBox::TreeViewComboBox(QWidget *parent): QComboBox(parent), _treeView(NULL), _expandingItem(false) {
  _treeView = new QTreeView(this);
  _treeView->setEditTriggers(QTreeView::NoEditTriggers);
  _treeView->setAlternatingRowColors(true);
  _treeView->setSelectionBehavior(QTreeView::SelectRows);
  _treeView->setRootIsDecorated(false);
  _treeView->setAllColumnsShowFocus(true);
  _treeView->header()->setVisible(false);
  connect(_treeView, SIGNAL(collapsed(const QModelIndex &)), this, SLOT(itemExpanded()));
  connect(_treeView, SIGNAL(expanded(const QModelIndex &)), this, SLOT(itemExpanded()));
  setView(_treeView);
  _treeView->setItemDelegate(new TreeViewDelegate(_treeView));
}

void TreeViewComboBox::setGraphsModel(QAbstractItemModel *model) {
  setModel(model);

  for(int i=1; i<model->columnCount(); ++i)
    _treeView->hideColumn(i);

  _treeView->setItemsExpandable(true);
  _treeView->expandAll();
  connect(model, SIGNAL(rowsInserted(const QModelIndex &, int, int)), this, SLOT(rowsInserted(QModelIndex,int,int)));
}

void TreeViewComboBox::showPopup() {
  setRootModelIndex(QModelIndex());
  QComboBox::showPopup();
}

void TreeViewComboBox::hidePopup() {
  if (_expandingItem)
    _expandingItem = false;
  else
    QComboBox::hidePopup();
}

void TreeViewComboBox::selectIndex(const QModelIndex& index) {
  setRootModelIndex(index.parent());
  setCurrentIndex(index.row());
}

void TreeViewComboBox::itemExpanded() {
  _expandingItem = true;
}

QModelIndex TreeViewComboBox::selectedIndex() const {
  QModelIndex current = model()->index(currentIndex(),0,rootModelIndex());
  QModelIndex selected = view()->selectionModel()->currentIndex();

  if (!selected.isValid())
    return current;

  return selected;
}

void TreeViewComboBox::rowsInserted(const QModelIndex &parent, int, int) {
  _treeView->setExpanded(parent, true);
}
