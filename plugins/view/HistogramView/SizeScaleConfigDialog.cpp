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

#include "SizeScaleConfigDialog.h"
#include "ui_SizeScaleConfigDialog.h"

namespace tlp {

SizeScaleConfigDialog::SizeScaleConfigDialog(QWidget *d):QDialog(d),_ui(new Ui::SizeScaleConfigDialogData) {
  _ui->setupUi(this);
  connect(_ui->minSizeSpinBox, SIGNAL(valueChanged(double)), this, SLOT(minSizeValueChanged(double)));
  connect(_ui->maxSizeSpinBox, SIGNAL(valueChanged(double)), this, SLOT(maxSizeValueChanged(double)));
  connect(_ui->viewSizeRB, SIGNAL(toggled(bool)), this, SLOT(viewSizeRadioButtonToggled(bool)));
}

SizeScaleConfigDialog::~SizeScaleConfigDialog() {
  delete _ui;
}

float SizeScaleConfigDialog::getMinSize() const {
  return (float) _ui->minSizeSpinBox->value();
}

float SizeScaleConfigDialog::getMaxSize() const {
  return (float) _ui->maxSizeSpinBox->value();
}

bool SizeScaleConfigDialog::applySizeMappingOnX() const {
  return _ui->xCB->isChecked();
}

bool SizeScaleConfigDialog::applySizeMappingOnY() const {
  return _ui->yCB->isChecked();
}

bool SizeScaleConfigDialog::applySizeMappingOnZ() const {
  return _ui->zCB->isChecked();
}

void SizeScaleConfigDialog::minSizeValueChanged(double newValue) {
  if (newValue > _ui->maxSizeSpinBox->value()) {
    _ui->maxSizeSpinBox->setValue(newValue + 1);
  }
}

void SizeScaleConfigDialog::maxSizeValueChanged(double newValue) {
  if (newValue < _ui->minSizeSpinBox->value()) {
    _ui->minSizeSpinBox->setValue(newValue - 1);
  }
}

bool SizeScaleConfigDialog::doMappingOnViewSize() const {
  return _ui->viewSizeRB->isChecked();
}

void SizeScaleConfigDialog::viewSizeRadioButtonToggled(bool checked) {
  _ui->dimensionsMappingGB->setEnabled(checked);
}
}
