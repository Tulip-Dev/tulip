#include "tulip/TreeViewComboBox.h"

#include <QtGui/QHeaderView>

TreeViewComboBox::TreeViewComboBox(QWidget *parent): QComboBox(parent), _treeView(NULL) {
  _treeView = new QTreeView(this);
  _treeView->setFrameShape(QFrame::NoFrame);
  _treeView->setEditTriggers(QTreeView::NoEditTriggers);
  _treeView->setAlternatingRowColors(true);
  _treeView->setSelectionBehavior(QTreeView::SelectRows);
  _treeView->setRootIsDecorated(false);
  _treeView->setWordWrap(true);
  _treeView->setAllColumnsShowFocus(true);
  _treeView->header()->setVisible(false);
  setView(_treeView);
}

void TreeViewComboBox::showPopup() {
  setRootModelIndex(QModelIndex());

  for(int i=1;i<model()->columnCount();++i)
    _treeView->hideColumn(i);

  _treeView->expandAll();
  _treeView->setItemsExpandable(false);
  QComboBox::showPopup();
}

void TreeViewComboBox::selectIndex(const QModelIndex& index) {
  setRootModelIndex(index.parent());
  setCurrentIndex(index.row());
}
