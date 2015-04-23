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

#include "PerspectiveSelectionDialog.h"

#include "ui_PerspectiveSelectionDialog.h"

#include <tulip/PluginModel.h>
#include <tulip/Perspective.h>

PerspectiveSelectionDialog::PerspectiveSelectionDialog(QWidget *parent): QDialog(parent), _ui(new Ui::PerspectiveSelectionDialog), _perspective("") {
  _ui->setupUi(this);
  _ui->perspectiveList->setModel(new tlp::PluginModel<tlp::Perspective>(_ui->perspectiveList));
  _ui->perspectiveList->setRootIndex(_ui->perspectiveList->model()->index(0,0));
  connect(_ui->perspectiveList->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(selectionChanged(QModelIndex,QModelIndex)));
}

QString PerspectiveSelectionDialog::perspectiveName() const {
  return _perspective;
}

void PerspectiveSelectionDialog::selectionChanged(const QModelIndex &a, const QModelIndex &) {
  _perspective = a.data().toString();
}
