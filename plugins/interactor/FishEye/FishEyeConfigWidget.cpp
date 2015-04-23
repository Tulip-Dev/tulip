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

#include "FishEyeConfigWidget.h"

#include "ui_FishEyeConfigWidget.h"

using namespace tlp;

FishEyeConfigWidget::FishEyeConfigWidget(QWidget *parent) : QWidget(parent),_ui(new Ui::FishEyeConfigWidget) {
  _ui->setupUi(this);
}

FishEyeConfigWidget::~FishEyeConfigWidget() {
  delete _ui;
}

int FishEyeConfigWidget::getFishEyeType() const {
  if (_ui->furnasFisheyeRb->isChecked()) {
    return 1;
  }
  else if (_ui->auberFishEyeRb->isChecked()) {
    return 2;
  }
  else {
    return 3;
  }
}

float FishEyeConfigWidget::getFishEyeRadius() const {
  return (float) _ui->radiusValSP->value();
}

void FishEyeConfigWidget::setFishEyeRadius(const float radius) {
  _ui->radiusValSP->setValue(radius);
}

float FishEyeConfigWidget::getFishEyeHeight() const {
  return (float) _ui->heightValSP->value();
}

void FishEyeConfigWidget::setFishEyeHeight(const float height) {
  _ui->heightValSP->setValue(height);
}

float FishEyeConfigWidget::getFishEyeRadiusIncrementStep() const {
  return (float) _ui->radiusIncrementStepSP->value();
}

float FishEyeConfigWidget::getFishEyeHeightIncrementStep() const {
  return (float) _ui->heightIncrementStepSP->value();
}
