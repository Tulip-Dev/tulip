#ifndef FILTERSMANAGERINVERTITEM_H
#define FILTERSMANAGERINVERTITEM_H

#include "AbstractFiltersManagerItem.h"

namespace Ui {
class FiltersManagerInvertItem;
}

class FiltersManagerInvertItem: public AbstractFiltersManagerItem {
  Q_OBJECT
  Ui::FiltersManagerInvertItem* _ui;
public:
  explicit FiltersManagerInvertItem(QWidget* parent = NULL);
  void applyFilter(tlp::BooleanProperty*);
  QString title() const;
};

#endif // FILTERSMANAGERINVERTITEM_H
