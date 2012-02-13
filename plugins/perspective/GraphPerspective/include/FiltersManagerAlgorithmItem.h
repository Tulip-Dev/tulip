#ifndef FILTERSMANAGERALGORITHMITEM_H
#define FILTERSMANAGERALGORITHMITEM_H

#include "AbstractFiltersManagerItem.h"

namespace Ui {
class FiltersManagerAlgorithmItem;
}

class FiltersManagerAlgorithmItem: public AbstractFiltersManagerItem {
  Q_OBJECT
  Ui::FiltersManagerAlgorithmItem* _ui;
public:
  explicit FiltersManagerAlgorithmItem(QWidget* parent = NULL);
  void applyFilter(tlp::BooleanProperty*);
  QString title() const;
protected slots:
  void algorithmSelected(int);
protected:
  void graphChanged();
};

#endif // FILTERSMANAGERALGORITHMITEM_H
