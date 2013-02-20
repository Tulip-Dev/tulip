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

#include <iostream>

#include "FishEyeConfigWidget.h"

FishEyeConfigWidget::FishEyeConfigWidget(QWidget *parent) : QWidget(parent) {
  setupUi(this);
}

int FishEyeConfigWidget::getFishEyeType() {
  if (furnasFisheyeRb->isChecked()) {
    return 1;
  }
  else if (auberFishEyeRb->isChecked()) {
    return 2;
  }
  else {
    return 3;
  }
}

float FishEyeConfigWidget::getFishEyeRadius() {
  return (float) radiusValSP->value();
}

void FishEyeConfigWidget::setFishEyeRadius(const float radius) {
  radiusValSP->setValue(radius);
}

float FishEyeConfigWidget::getFishEyeHeight() {
  return (float) heightValSP->value();
}

void FishEyeConfigWidget::setFishEyeHeight(const float height) {
  heightValSP->setValue(height);
}

float FishEyeConfigWidget::getFishEyeRadiusIncrementStep() {
  return (float) radiusIncrementStepSP->value();
}

float FishEyeConfigWidget::getFishEyeHeightIncrementStep() {
  return (float) heightIncrementStepSP->value();
}
