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

#include "tulip/TreeViewComboBox.h"

#include <QHeaderView>
#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QMouseEvent>
#include <QTreeView>
#include <QTimer>
#include <QSortFilterProxyModel>

class TreeViewUpdateTimer : public QTimer {

public:

  TreeViewUpdateTimer(const QModelIndex &topLeft, const QModelIndex &bottomRight) :
    topLeft(topLeft), bottomRight(bottomRight) {}

  QModelIndex topLeft, bottomRight;

};

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

DeferredUpdateTreeView::DeferredUpdateTreeView(QWidget *parent) : QTreeView(parent) {}

DeferredUpdateTreeView::~DeferredUpdateTreeView() {
  QMap<QPair<QModelIndex, QModelIndex>, QTimer *>::iterator it = _updateTimers.begin();

  for( ; it != _updateTimers.end() ; ++it) {
    delete it.value();
  }
}

void DeferredUpdateTreeView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight) {
  QPair<QModelIndex, QModelIndex> p = qMakePair(topLeft, bottomRight);

  if (_updateTimers.find(p) == _updateTimers.end()) {
    _updateTimers[p] = new TreeViewUpdateTimer(topLeft, bottomRight);
    _updateTimers[p]->setSingleShot(true);
    connect(_updateTimers[p], SIGNAL(timeout()), this, SLOT(callDataChanged()));
  }

  _updateTimers[p]->start(50);
}

void DeferredUpdateTreeView::callDataChanged() {
  // As the view update is asynchronous, if the original model is wrapped by a QSortFilterProxyModel
  // it needs to be invalidate in order to rebuild the index mapping and prevent sefgault
  QSortFilterProxyModel *qsfpm = dynamic_cast<QSortFilterProxyModel*>(model());

  if (qsfpm) {
    qsfpm->invalidate();
  }

  TreeViewUpdateTimer *tvut = static_cast<TreeViewUpdateTimer*>(sender());
  QTreeView::dataChanged(tvut->topLeft, tvut->bottomRight);
  QPair<QModelIndex, QModelIndex> p = qMakePair(tvut->topLeft, tvut->bottomRight);
  delete _updateTimers[p];
  _updateTimers.remove(p);
}

void DeferredUpdateTreeView::rowsAboutToBeRemoved(const QModelIndex &parent, int start, int end) {
  // Cancel all deferred updates for child indices of parent index to prevent segfault
  // as the data they point to can be deleted before the call to QTreeView::dataChanged
  cancelUpdates(parent);
  QTreeView::rowsAboutToBeRemoved(parent, start, end);
}

void DeferredUpdateTreeView::rowsInserted(const QModelIndex &parent, int start, int end) {
  // Cancel all deferred updates for child indices of parent index to prevent segfault
  // as the data they point to can be deleted before the call to QTreeView::dataChanged
  cancelUpdates(parent);
  QTreeView::rowsInserted(parent, start, end);
}

void DeferredUpdateTreeView::cancelUpdates(const QModelIndex &parent) {
  _updateTimers.clear();
  QMap<QPair<QModelIndex, QModelIndex>, QTimer *>::iterator it = _updateTimers.begin();

  while (it != _updateTimers.end()) {
    if (it.key().first.parent() == parent || it.key().second.parent() == parent) {
      delete it.value();
      it = _updateTimers.erase(it);
    }
    else {
      ++it;
    }
  }
}

TreeViewComboBox::TreeViewComboBox(QWidget *parent): QComboBox(parent), _treeView(NULL), _skipNextHide(false), _popupVisible(false) {
  _treeView = new DeferredUpdateTreeView(this);
  _treeView->setEditTriggers(QTreeView::NoEditTriggers);
  _treeView->setAlternatingRowColors(true);
  _treeView->setSelectionBehavior(QTreeView::SelectRows);
  _treeView->setRootIsDecorated(false);
  _treeView->setAllColumnsShowFocus(true);
  _treeView->header()->setVisible(false);
  _treeView->setItemDelegate(new TreeViewDelegate(_treeView));
  _treeView->setItemsExpandable(true);
  setView(_treeView);
  view()->viewport()->installEventFilter(this);
  connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(currentIndexChanged()));
}

void TreeViewComboBox::setModel(QAbstractItemModel *model) {
  QComboBox::setModel(model);
  connect(model, SIGNAL(rowsRemoved (const QModelIndex&, int, int)), this , SLOT(rowsRemoved(const QModelIndex&, int, int)));

  for(int i=1; i<model->columnCount(); ++i)
    _treeView->hideColumn(i);
}

void TreeViewComboBox::showPopup() {
  setRootModelIndex(QModelIndex());
  _treeView->expandAll();
  _treeView->resizeColumnToContents(0);
  QComboBox::showPopup();
  QWidget *popup = findChild<QFrame*>();

  if (_treeView->columnWidth(0) > popup->width()) {
    popup->resize(_treeView->columnWidth(0), popup->height());
  }

  _popupVisible = true;
}

void TreeViewComboBox::hidePopup() {
  if (!_popupVisible)
    return;

  if (_skipNextHide)
    _skipNextHide = false;
  else {
    QComboBox::hidePopup();
    _popupVisible = false;
    selectIndex(view()->currentIndex());
  }
}

void TreeViewComboBox::selectIndex(const QModelIndex& index) {
  if (!_popupVisible && index != _lastIndex) {
    _lastIndex = index;
    setRootModelIndex(index.parent());
    setCurrentIndex(index.row());
    emit currentItemChanged();
  }
}

QModelIndex TreeViewComboBox::selectedIndex() const {
  return model()->index(currentIndex(),0,rootModelIndex());
}

bool TreeViewComboBox::eventFilter(QObject* object, QEvent* event) {
  if (event->type() == QEvent::MouseButtonPress && object == view()->viewport()) {
    QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
    QModelIndex index = view()->indexAt(mouseEvent->pos());

    if (!view()->visualRect(index).contains(mouseEvent->pos()))
      _skipNextHide = true;
  }

  return false;
}


void TreeViewComboBox::rowsRemoved(const QModelIndex &parent, int, int) {
  QModelIndex currentIndex = selectedIndex();

  if (currentIndex.isValid())
    selectIndex(currentIndex);
  else
    selectIndex(parent);
}

void TreeViewComboBox::currentIndexChanged() {
  selectIndex(selectedIndex());
}
