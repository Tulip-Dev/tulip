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
#include <QPainter>
#include <QLinearGradient>

#include "ScatterPlotCorrelCoeffSelectorOptionsWidget.h"
#include "ui_ScatterPlotCorrelCoeffSelectorOptionsWidget.h"

namespace tlp {

ScatterPlotCorrelCoeffSelectorOptionsWidget::ScatterPlotCorrelCoeffSelectorOptionsWidget(
    QWidget *parent)
    : QWidget(parent), _ui(new Ui::ScatterPlotCorrelCoeffSelectorOptionsWidgetData) {
  _ui->setupUi(this);
  _ui->minusOneColorButton->setDialogParent(parent);
  _ui->zeroColorButton->setDialogParent(parent);
  _ui->oneColorButton->setDialogParent(parent);
  _ui->minusOneColorButton->setDialogTitle("Choose the color for -1");
  _ui->zeroColorButton->setDialogTitle("Choose the color for 0");
  _ui->oneColorButton->setDialogTitle("Choose the color for 1");
  setButtonColor(_ui->minusOneColorButton, Color(0, 0, 255, 150));
  setButtonColor(_ui->zeroColorButton, Color(255, 0, 0, 150));
  setButtonColor(_ui->oneColorButton, Color(0, 255, 0, 150));
  updateColorScale();

  connect(_ui->minusOneColorButton, SIGNAL(clicked()), this, SLOT(updateColorScale()));
  connect(_ui->zeroColorButton, SIGNAL(clicked()), this, SLOT(updateColorScale()));
  connect(_ui->oneColorButton, SIGNAL(clicked()), this, SLOT(updateColorScale()));
}

ScatterPlotCorrelCoeffSelectorOptionsWidget::~ScatterPlotCorrelCoeffSelectorOptionsWidget() {
  delete _ui;
}

void ScatterPlotCorrelCoeffSelectorOptionsWidget::setButtonColor(ColorButton *button,
                                                                 const Color &color) {
  button->setTulipColor(color);
}

Color ScatterPlotCorrelCoeffSelectorOptionsWidget::getMinusOneColor() const {
  return _ui->minusOneColorButton->tulipColor();
}

Color ScatterPlotCorrelCoeffSelectorOptionsWidget::getZeroColor() const {
  return _ui->zeroColorButton->tulipColor();
}

Color ScatterPlotCorrelCoeffSelectorOptionsWidget::getOneColor() const {
  return _ui->oneColorButton->tulipColor();
}

void ScatterPlotCorrelCoeffSelectorOptionsWidget::updateColorScale() {
  QPixmap pixmap(_ui->colorScaleLabel->width(), _ui->colorScaleLabel->height());
  pixmap.fill(Qt::transparent);
  QPainter painter;
  painter.begin(&pixmap);
  QLinearGradient qLinearGradient(0, _ui->colorScaleLabel->height() / 2,
                                  _ui->colorScaleLabel->width() - 1,
                                  _ui->colorScaleLabel->height() / 2);
  qLinearGradient.setColorAt(0, _ui->minusOneColorButton->color());
  qLinearGradient.setColorAt(1. / 2., _ui->zeroColorButton->color());
  qLinearGradient.setColorAt(1, _ui->oneColorButton->color());
  painter.fillRect(0, 0, _ui->colorScaleLabel->width(), _ui->colorScaleLabel->height(),
                   qLinearGradient);
  painter.end();
  _ui->colorScaleLabel->setPixmap(pixmap);
}

void ScatterPlotCorrelCoeffSelectorOptionsWidget::showEvent(QShowEvent *) {
  updateColorScale();
}
}
