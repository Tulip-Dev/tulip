/*
    FishEyeConfigWidget.cpp

   Created on: 4 juin 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

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
