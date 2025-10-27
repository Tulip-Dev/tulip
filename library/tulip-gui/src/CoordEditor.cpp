/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#include <tulip/CoordEditor.h>

#include "ui_CoordEditor.h"

#include <cfloat>

#include <QDoubleValidator>

using namespace tlp;

CoordEditor::CoordEditor(QWidget *parent, bool editSize)
    : QDialog(parent), ui(new Ui::CoordEditor) {
  ui->setupUi(this);

  if (editSize) {
    setWindowTitle("Edit size");
    ui->xLabel->setText("W");
    ui->yLabel->setText("H");
    ui->zLabel->setText("D");
  }

  ui->xSP->setRange(-FLT_MAX, FLT_MAX);
  ui->ySP->setRange(-FLT_MAX, FLT_MAX);
  ui->zSP->setRange(-FLT_MAX, FLT_MAX);
  setCoord(Coord());
  connect(ui->xSP, SIGNAL(valueChanged(double)), this, SLOT(coordUpdated()));
  connect(ui->ySP, SIGNAL(valueChanged(double)), this, SLOT(coordUpdated()));
  connect(ui->zSP, SIGNAL(valueChanged(double)), this, SLOT(coordUpdated()));
  setModal(true);
}

CoordEditor::~CoordEditor() {
  delete ui;
}

Coord CoordEditor::coord() const {
  return currentCoord;
}
void CoordEditor::setCoord(const Coord &coord) {
  currentCoord = coord;
  blockSignals(true);
  ui->xSP->setValue(coord[0]);
  ui->ySP->setValue(coord[1]);
  ui->zSP->setValue(coord[2]);
  blockSignals(false);
  coordUpdated();
}

void CoordEditor::coordUpdated() {
  currentCoord = Coord(float(ui->xSP->value()), float(ui->ySP->value()), float(ui->zSP->value()));
  emit(coordChanged(coord()));
}

void CoordEditor::done(int r) {
  if (r == QDialog::Accepted)
    currentCoord = Coord(float(ui->xSP->value()), float(ui->ySP->value()), float(ui->zSP->value()));

  QDialog::done(r);
}

// to ensure it is shown in the center of its parent
void CoordEditor::showEvent(QShowEvent *ev) {
  QDialog::showEvent(ev);

  if (parentWidget())
    move(parentWidget()->window()->frameGeometry().topLeft() +
         parentWidget()->window()->rect().center() - rect().center());
}
