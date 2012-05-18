#ifndef TREEVIEWCOMBOBOX_H
#define TREEVIEWCOMBOBOX_H

#include <tulip/tulipconf.h>

#include <QtGui/QComboBox>
#include <QtGui/QTreeView>

class TLP_QT_SCOPE TreeViewComboBox : public QComboBox {
  Q_OBJECT
  QTreeView* _treeView;

public:
  explicit TreeViewComboBox(QWidget *parent = NULL);
  virtual void showPopup();

  QModelIndex selectedIndex() const;

public slots:
  void selectIndex(const QModelIndex&);
};

#endif // TREEVIEWCOMBOBOX_H
