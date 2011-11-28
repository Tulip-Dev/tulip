#ifndef TREEVIEWCOMBOBOX_H
#define TREEVIEWCOMBOBOX_H

#include <QtGui/QComboBox>
#include <QtGui/QTreeView>

class TreeViewComboBox : public QComboBox {
  Q_OBJECT
  QTreeView* _treeView;

public:
  explicit TreeViewComboBox(QWidget *parent = 0);
  virtual void showPopup();

  QModelIndex selectedIndex() const;

public slots:
  void selectIndex(const QModelIndex&);
};

#endif // TREEVIEWCOMBOBOX_H
