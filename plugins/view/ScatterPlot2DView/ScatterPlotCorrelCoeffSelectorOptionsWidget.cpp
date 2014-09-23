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

#include <QColorDialog>
#include <QPainter>
#include <QLinearGradient>

#include "ScatterPlotCorrelCoeffSelectorOptionsWidget.h"
#include "ui_ScatterPlotCorrelCoeffSelectorOptionsWidget.h"

namespace tlp {

ScatterPlotCorrelCoeffSelectorOptionsWidget::ScatterPlotCorrelCoeffSelectorOptionsWidget(QWidget *parent) : QWidget(parent),_ui(new Ui::ScatterPlotCorrelCoeffSelectorOptionsWidgetData) {
  _ui->setupUi(this);

  setButtonBackgroundColor(_ui->minusOneColorButton, Color(0,0,255,150));
  setButtonBackgroundColor(_ui->zeroColorButton, Color(255,0,0,150));
  setButtonBackgroundColor(_ui->oneColorButton, Color(0,255,0,150));
  updateColorScale();

  connect(_ui->minusOneColorButton, SIGNAL(clicked()), this, SLOT(pressMinusOneColorButton()));
  connect(_ui->zeroColorButton, SIGNAL(clicked()), this, SLOT(pressZeroColorButton()));
  connect(_ui->oneColorButton, SIGNAL(clicked()), this, SLOT(pressOneColorButton()));
}

ScatterPlotCorrelCoeffSelectorOptionsWidget::~ScatterPlotCorrelCoeffSelectorOptionsWidget() {
  delete _ui;
}

Color ScatterPlotCorrelCoeffSelectorOptionsWidget::getButtonColor(QPushButton *button) const {
  QString buttonStyleSheet = button->styleSheet();
  int pos = buttonStyleSheet.indexOf("rgba(") + 5;
  QString backgroundColorCode = buttonStyleSheet.mid(pos, buttonStyleSheet.length() - pos - 2);
  bool ok;
  QStringList rgbaStr = backgroundColorCode.split(",");
  return Color(rgbaStr.at(0).toInt(&ok), rgbaStr.at(1).toInt(&ok), rgbaStr.at(2).toInt(&ok), rgbaStr.at(3).toInt(&ok));
}


Color ScatterPlotCorrelCoeffSelectorOptionsWidget::getMinusOneColor() const {
  return getButtonColor(_ui->minusOneColorButton);
}

Color ScatterPlotCorrelCoeffSelectorOptionsWidget::getZeroColor() const {
  return getButtonColor(_ui->zeroColorButton);
}

Color ScatterPlotCorrelCoeffSelectorOptionsWidget::getOneColor() const {
  return getButtonColor(_ui->oneColorButton);
}

void ScatterPlotCorrelCoeffSelectorOptionsWidget::setButtonBackgroundColor(QPushButton *button, const Color &color) {
  QString colorStr = "rgba(";
  QString str;
  str.setNum(color.getR());
  str.append(",");
  colorStr.append(str);
  str.setNum(color.getG());
  str.append(",");
  colorStr.append(str);
  str.setNum(color.getB());
  str.append(",");
  colorStr.append(str);
  str.setNum(color.getA());
  str.append(")");
  colorStr.append(str);
  button->setStyleSheet("QPushButton { background-color: "+colorStr +"}");
}

void ScatterPlotCorrelCoeffSelectorOptionsWidget::pressMinusOneColorButton() {
  changeButtonBackgroundColor(_ui->minusOneColorButton);
  updateColorScale();
}

void ScatterPlotCorrelCoeffSelectorOptionsWidget::pressZeroColorButton() {
  changeButtonBackgroundColor(_ui->zeroColorButton);
  updateColorScale();
}

void ScatterPlotCorrelCoeffSelectorOptionsWidget::pressOneColorButton() {
  changeButtonBackgroundColor(_ui->oneColorButton);
  updateColorScale();
}

void ScatterPlotCorrelCoeffSelectorOptionsWidget::changeButtonBackgroundColor(QPushButton *button) {
  QColor currentButtonColor = button->palette().color(QPalette::Button);
#if (QT_VERSION >= QT_VERSION_CHECK(4, 5, 0))
  QColor newColor = QColorDialog::getColor(currentButtonColor, this, "Select Color", QColorDialog::ShowAlphaChannel);

  if (newColor.isValid()) {
    setButtonBackgroundColor(button, Color(newColor.red(), newColor.green(), newColor.blue(), newColor.alpha()));
  }

#else
  bool ok = true;
  QRgb newColor = QColorDialog::getRgba(qRgba(currentButtonColor.red(), currentButtonColor.green(), currentButtonColor.blue(), currentButtonColor.alpha()), &ok, this);

  if (ok) {
    setButtonBackgroundColor(button, Color(qRed(newColor), qGreen(newColor), qBlue(newColor), qAlpha(newColor)));
  }

#endif
}

void ScatterPlotCorrelCoeffSelectorOptionsWidget::updateColorScale() {
  QPixmap pixmap(_ui->colorScaleLabel->width(), _ui->colorScaleLabel->height());
  pixmap.fill(Qt::transparent);
  QPainter painter;
  painter.begin(&pixmap);
  Color minusOneColor = getMinusOneColor();
  Color zeroColor = getZeroColor();
  Color oneColor = getOneColor();
  QLinearGradient qLinearGradient(0, _ui->colorScaleLabel->height() / 2, _ui->colorScaleLabel->width()-1, _ui->colorScaleLabel->height() / 2);
  qLinearGradient.setColorAt(0, QColor(minusOneColor.getR(), minusOneColor.getG(), minusOneColor.getB(), minusOneColor.getA()));
  qLinearGradient.setColorAt(1./2., QColor(zeroColor.getR(), zeroColor.getG(), zeroColor.getB(), zeroColor.getA()));
  qLinearGradient.setColorAt(1, QColor(oneColor.getR(), oneColor.getG(), oneColor.getB(), oneColor.getA()));
  painter.fillRect(0, 0, _ui->colorScaleLabel->width(), _ui->colorScaleLabel->height(), qLinearGradient);
  painter.end();
  _ui->colorScaleLabel->setPixmap(pixmap.scaled(_ui->colorScaleLabel->width(), _ui->colorScaleLabel->height()));
}

void ScatterPlotCorrelCoeffSelectorOptionsWidget::showEvent(QShowEvent*) {
  updateColorScale();
}

}
