/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#include <QColorDialog>
#include <QMainWindow>

#include <tulip/Perspective.h>
#include <tulip/TlpQtTools.h>

#include "PixelOrientedOptionsWidget.h"
#include "ui_PixelOrientedOptionsWidget.h"

using namespace std;
using namespace tlp;

PixelOrientedOptionsWidget::PixelOrientedOptionsWidget(QWidget *parent)
    : QWidget(parent), _ui(new Ui::PixelOrientedOptionsWidgetData), oldValuesInitialized(false) {
  _ui->setupUi(this);
  _ui->backColorButton->setDialogParent(Perspective::instance()->mainWindow());
  _ui->backColorButton->setDialogTitle("Choose the background color");
  setBackgroundColor(Color(255, 255, 255));
}

PixelOrientedOptionsWidget::~PixelOrientedOptionsWidget() {
  delete _ui;
}

Color PixelOrientedOptionsWidget::getBackgroundColor() const {
  return _ui->backColorButton->tulipColor();
}

void PixelOrientedOptionsWidget::setBackgroundColor(const Color &color) {
  _ui->backColorButton->setTulipColor(color);
}

string PixelOrientedOptionsWidget::getLayoutType() const {
  return QStringToTlpString(_ui->layoutTypeCB->currentText());
}

void PixelOrientedOptionsWidget::setLayoutType(const string &layoutType) {
  int idx = _ui->layoutTypeCB->findText(QString(layoutType.c_str()));

  if (idx != -1) {
    _ui->layoutTypeCB->setCurrentIndex(idx);
  }
}

bool PixelOrientedOptionsWidget::configurationChanged() {
  bool confChanged = false;

  if (oldValuesInitialized) {
    if (oldBackgroundColor != getBackgroundColor() || oldLayoutType != getLayoutType()) {
      confChanged = true;
    }
  } else {
    confChanged = true;
    oldValuesInitialized = true;
  }

  if (confChanged) {
    oldBackgroundColor = getBackgroundColor();
    oldLayoutType = getLayoutType();
  }

  return confChanged;
}
