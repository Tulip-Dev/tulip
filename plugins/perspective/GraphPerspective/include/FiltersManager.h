#ifndef FILTERSMANAGER_H
#define FILTERSMANAGER_H

#include <QtGui/QWidget>

namespace Ui {
class FiltersManagerData;
}

class FiltersManager: public QWidget {
  Ui::FiltersManagerData *_ui;

public:
  explicit FiltersManager(QWidget *parent=0);
  virtual ~FiltersManager();
};

#endif // FILTERSMANAGER_H
