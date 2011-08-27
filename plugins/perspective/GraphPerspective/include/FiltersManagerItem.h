#ifndef FILTERSMANAGERITEM_H
#define FILTERSMANAGERITEM_H

#include <QtGui/QWidget>

namespace Ui {
class FiltersManagerItemData;
}

class FiltersManagerItem: public QWidget {
  Q_OBJECT

  Ui::FiltersManagerItemData *_ui;

  Q_PROPERTY(bool removable READ isRemovable WRITE setRemovable)
  bool _removable;


public:
  explicit FiltersManagerItem(QWidget *parent=0);
  virtual ~FiltersManagerItem();

  bool isRemovable() const {
    return _removable;
  }

public slots:
  void setRemovable(bool f);
  void expand();
};

#endif // FILTERSMANAGERITEM_H
