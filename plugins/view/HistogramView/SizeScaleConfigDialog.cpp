/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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

SizeScaleConfigDialog::SizeScaleConfigDialog(QWidget *) {
  setupUi(this);
  connect(minSizeSpinBox, SIGNAL(valueChanged(double)), this, SLOT(minSizeValueChanged(double)));
  connect(maxSizeSpinBox, SIGNAL(valueChanged(double)), this, SLOT(maxSizeValueChanged(double)));
  connect(viewSizeRB, SIGNAL(toggled(bool)), this, SLOT(viewSizeRadioButtonToggled(bool)));
}

float SizeScaleConfigDialog::getMinSize() const {
  return (float) minSizeSpinBox->value();
}

float SizeScaleConfigDialog::getMaxSize() const {
  return (float) maxSizeSpinBox->value();
}

bool SizeScaleConfigDialog::applySizeMappingOnX() const {
  return xCB->isChecked();
}

bool SizeScaleConfigDialog::applySizeMappingOnY() const {
  return yCB->isChecked();
}

bool SizeScaleConfigDialog::applySizeMappingOnZ() const {
  return zCB->isChecked();
}

void SizeScaleConfigDialog::minSizeValueChanged(double newValue) {
  if (newValue > maxSizeSpinBox->value()) {
    maxSizeSpinBox->setValue(newValue + 1);
  }
}

void SizeScaleConfigDialog::maxSizeValueChanged(double newValue) {
  if (newValue < minSizeSpinBox->value()) {
    minSizeSpinBox->setValue(newValue - 1);
  }
}

bool SizeScaleConfigDialog::doMappingOnViewSize() const {
  return viewSizeRB->isChecked();
}

void SizeScaleConfigDialog::viewSizeRadioButtonToggled(bool checked) {
  dimensionsMappingGB->setEnabled(checked);
}
