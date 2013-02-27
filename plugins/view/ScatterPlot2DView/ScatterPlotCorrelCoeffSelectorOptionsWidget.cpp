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

#include <QtGui/QColorDialog>
#include <QtGui/QPainter>
#include <QtGui/QLinearGradient>

#include "ScatterPlotCorrelCoeffSelectorOptionsWidget.h"

namespace tlp {

ScatterPlotCorrelCoeffSelectorOptionsWidget::ScatterPlotCorrelCoeffSelectorOptionsWidget(QWidget *parent) : QWidget(parent) {
  setupUi(this);

  setButtonBackgroundColor(minusOneColorButton, Color(0,0,255,150));
  setButtonBackgroundColor(zeroColorButton, Color(255,0,0,150));
  setButtonBackgroundColor(oneColorButton, Color(0,255,0,150));
  updateColorScale();

  connect(minusOneColorButton, SIGNAL(clicked()), this, SLOT(pressMinusOneColorButton()));
  connect(zeroColorButton, SIGNAL(clicked()), this, SLOT(pressZeroColorButton()));
  connect(oneColorButton, SIGNAL(clicked()), this, SLOT(pressOneColorButton()));
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
  return getButtonColor(minusOneColorButton);
}

Color ScatterPlotCorrelCoeffSelectorOptionsWidget::getZeroColor() const {
  return getButtonColor(zeroColorButton);
}

Color ScatterPlotCorrelCoeffSelectorOptionsWidget::getOneColor() const {
  return getButtonColor(oneColorButton);
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
  changeButtonBackgroundColor(minusOneColorButton);
  updateColorScale();
}

void ScatterPlotCorrelCoeffSelectorOptionsWidget::pressZeroColorButton() {
  changeButtonBackgroundColor(zeroColorButton);
  updateColorScale();
}

void ScatterPlotCorrelCoeffSelectorOptionsWidget::pressOneColorButton() {
  changeButtonBackgroundColor(oneColorButton);
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
  QPixmap pixmap(colorScaleLabel->width(), colorScaleLabel->height());
  pixmap.fill(Qt::transparent);
  QPainter painter;
  painter.begin(&pixmap);
  Color minusOneColor = getMinusOneColor();
  Color zeroColor = getZeroColor();
  Color oneColor = getOneColor();
  QLinearGradient qLinearGradient(0, colorScaleLabel->height() / 2, colorScaleLabel->width()-1, colorScaleLabel->height() / 2);
  qLinearGradient.setColorAt(0, QColor(minusOneColor.getR(), minusOneColor.getG(), minusOneColor.getB(), minusOneColor.getA()));
  qLinearGradient.setColorAt(1./2., QColor(zeroColor.getR(), zeroColor.getG(), zeroColor.getB(), zeroColor.getA()));
  qLinearGradient.setColorAt(1, QColor(oneColor.getR(), oneColor.getG(), oneColor.getB(), oneColor.getA()));
  painter.fillRect(0, 0, colorScaleLabel->width(), colorScaleLabel->height(), qLinearGradient);
  painter.end();
  colorScaleLabel->setPixmap(pixmap.scaled(colorScaleLabel->width(), colorScaleLabel->height()));
}

void ScatterPlotCorrelCoeffSelectorOptionsWidget::showEvent(QShowEvent*) {
  updateColorScale();
}

}
