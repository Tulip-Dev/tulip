/*
    FishEyeConfigWidget.h

   Created on: 4 juin 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */

#ifndef FISHEYECONFIGWIDGET_H_
#define FISHEYECONFIGWIDGET_H_

#include <QtGui/QWidget>

#include "ui_FishEyeConfigWidget.h"

class FishEyeConfigWidget : public QWidget, public Ui::FishEyeConfigWidget {

public :

  FishEyeConfigWidget(QWidget *parent = 0);

  int getFishEyeType();

  float getFishEyeRadius();
  void setFishEyeRadius(const float radius);

  float getFishEyeHeight();
  void setFishEyeHeight(const float height);

  float getFishEyeRadiusIncrementStep();

  float getFishEyeHeightIncrementStep();

};

#endif /* FISHEYECONFIGWIDGET_H_ */
