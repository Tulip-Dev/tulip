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

#ifndef FILTERSMANAGERITEM_H
#define FILTERSMANAGERITEM_H


#include <QFrame>
#include <tulip/WithParameter.h>

namespace Ui {
class FiltersManagerItem;
class FiltersManagerAlgorithmItem;
class FiltersManagerInvertItem;
class FiltersManagerCompareItem;
}
namespace tlp {
class Graph;
class BooleanProperty;
class PropertyInterface;
}
class QTableView;
class QComboBox;

class FiltersManagerItem: public QFrame {
  Q_OBJECT
  Ui::FiltersManagerItem *_ui;

  Q_ENUMS(Mode)
public:
  enum Mode {
    Invert,
    Compare,
    Algorithm,
    Invalid
  };

  explicit FiltersManagerItem(QWidget *parent=NULL);
  virtual ~FiltersManagerItem();

signals:
  void removed();
  void modeChanged(FiltersManagerItem::Mode);

public slots:
  void applyFilter(tlp::BooleanProperty*);

protected slots:
  void setCompareMode();
  void setAlgorithmMode();
  void setInvertMode();
  void setMode(Mode);

  void addButtonClicked();
  void dataBoxTitleChanged();

  void graphChanged(tlp::Graph*);
};

#endif // FILTERSMANAGERITEM_H
