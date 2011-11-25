#ifndef TREEVIEWCOMBOBOX_H
#define TREEVIEWCOMBOBOX_H

#include <QtGui/QComboBox>
#include <QtGui/QTreeView>

class TreeViewComboBox : public QComboBox {
  Q_OBJECT
  QModelIndex _selectedIndex;
  QTreeView* _treeView;
public:
  explicit TreeViewComboBox(QWidget *parent = 0);
  virtual void showPopup();
};

#endif // TREEVIEWCOMBOBOX_H
