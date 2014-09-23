/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */

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
  ~FiltersManagerAlgorithmItem();
  void applyFilter(tlp::BooleanProperty*);
  QString title() const;
protected slots:
  void algorithmSelected(int);
protected:
  void graphChanged();
};

#endif // FILTERSMANAGERALGORITHMITEM_H
