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
#ifndef FILTERSMANAGER_H
#define FILTERSMANAGER_H

#include <QWidget>
#include "FiltersManagerItem.h"

namespace Ui {
class FiltersManagerData;
}

class QToolButton;

class FiltersManager: public QWidget {
  Q_OBJECT

  enum PreselectionModes {
    None = 0,
    Current = 1,
    Nodes = 2,
    Edges = 3,
    All = 4
  };

  Ui::FiltersManagerData *_ui;
  QList<FiltersManagerItem*> _items;

  QToolButton* _playButton;
  QToolButton* _saveButton;
  QToolButton* _loadButton;

public:
  explicit FiltersManager(QWidget *parent=NULL);
  virtual ~FiltersManager();

public slots:
  void applyFilter();

protected slots:
  void delItem();
  void delItem(FiltersManagerItem* item);
  void addItem();
  void itemModeChanged(FiltersManagerItem::Mode);
  void currentGraphChanged(tlp::Graph*);
};

#endif // FILTERSMANAGER_H
