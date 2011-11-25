#include "tulip/TreeViewComboBox.h"

#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>
#include <QtCore/QEvent>

//FIXME: remove me
#include <QtCore/QTimer>

TreeViewComboBox::TreeViewComboBox(QWidget *parent): QComboBox(parent), _selectedIndex(QModelIndex()), _treeView(NULL) {
  _treeView = new QTreeView(this);
  setView(_treeView);
}

void TreeViewComboBox::showPopup() {
  _treeView->expandAll();
  _treeView->setItemsExpandable(false);
  QComboBox::showPopup();
}
