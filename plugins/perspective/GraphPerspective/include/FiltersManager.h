#ifndef FILTERSMANAGER_H
#define FILTERSMANAGER_H

#include <QtGui/QWidget>

namespace Ui {
class FiltersManagerData;
class FiltersManagerItemData;
}

// Helper class
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

class FiltersManager: public QWidget {
  Ui::FiltersManagerData *_ui;

public:
  explicit FiltersManager(QWidget *parent=0);
  virtual ~FiltersManager();
};

#endif // FILTERSMANAGER_H
