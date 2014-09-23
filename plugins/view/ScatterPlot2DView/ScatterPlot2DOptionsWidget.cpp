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

#include "ScatterPlot2DOptionsWidget.h"
#include "ui_ScatterPlot2DOptionsWidget.h"

namespace tlp {

ScatterPlot2DOptionsWidget::ScatterPlot2DOptionsWidget(QWidget *parent) : QWidget(parent),oldValuesInitialized(false),_ui(new Ui::ScatterPlot2DOptionsWidgetData) {
  _ui->setupUi(this);
  setBackgroundColor(Color(255,255,255));
  setButtonBackgroundColor(_ui->minusOneColorButton, Color(0,0,255));
  setButtonBackgroundColor(_ui->zeroColorButton, Color(255,255,255));
  setButtonBackgroundColor(_ui->oneColorButton, Color(0,255,0));
  updateColorScale();
  connect(_ui->backColorButton, SIGNAL(clicked()), this, SLOT(pressBackgroundColorButton()));
  connect(_ui->minusOneColorButton, SIGNAL(clicked()), this, SLOT(pressMinusOneColorButton()));
  connect(_ui->zeroColorButton, SIGNAL(clicked()), this, SLOT(pressZeroColorButton()));
  connect(_ui->oneColorButton, SIGNAL(clicked()), this, SLOT(pressOneColorButton()));
  connect(_ui->minSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(minSizeSpinBoxValueChanged(int)));
  connect(_ui->maxSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(maxSizeSpinBoxValueChanged(int)));
}

ScatterPlot2DOptionsWidget::~ScatterPlot2DOptionsWidget() {
  delete _ui;
}

Color ScatterPlot2DOptionsWidget::getButtonColor(QPushButton *button) const {
  QString buttonStyleSheet = button->styleSheet();
  int pos = buttonStyleSheet.indexOf("rgba(") + 5;
  QString backgroundColorCode = buttonStyleSheet.mid(pos, buttonStyleSheet.length() - pos - 2);
  bool ok;
  QStringList rgbaStr = backgroundColorCode.split(",");
  return Color(rgbaStr.at(0).toInt(&ok), rgbaStr.at(1).toInt(&ok), rgbaStr.at(2).toInt(&ok), rgbaStr.at(3).toInt(&ok));
}

Color ScatterPlot2DOptionsWidget::getUniformBackgroundColor() const {
  return getButtonColor(_ui->backColorButton);
}

Color ScatterPlot2DOptionsWidget::getMinusOneColor() const {
  return getButtonColor(_ui->minusOneColorButton);
}

Color ScatterPlot2DOptionsWidget::getZeroColor() const {
  return getButtonColor(_ui->zeroColorButton);
}

Color ScatterPlot2DOptionsWidget::getOneColor() const {
  return getButtonColor(_ui->oneColorButton);
}

void ScatterPlot2DOptionsWidget::setBackgroundColor(const Color &color) {
  setButtonBackgroundColor(_ui->backColorButton, color);
}

void ScatterPlot2DOptionsWidget::setButtonBackgroundColor(QPushButton *button, const Color &color) {
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

void ScatterPlot2DOptionsWidget::pressBackgroundColorButton() {
  changeButtonBackgroundColor(_ui->backColorButton);
}

void ScatterPlot2DOptionsWidget::pressMinusOneColorButton() {
  changeButtonBackgroundColor(_ui->minusOneColorButton);
  updateColorScale();
}

void ScatterPlot2DOptionsWidget::pressZeroColorButton() {
  changeButtonBackgroundColor(_ui->zeroColorButton);
  updateColorScale();
}

void ScatterPlot2DOptionsWidget::pressOneColorButton() {
  changeButtonBackgroundColor(_ui->oneColorButton);
  updateColorScale();
}

Size ScatterPlot2DOptionsWidget::getMinSizeMapping() const {
  float minSize = _ui->minSizeSpinBox->value();
  return Size(minSize, minSize, minSize);
}

Size ScatterPlot2DOptionsWidget::getMaxSizeMapping() const {
  float maxSize = _ui->maxSizeSpinBox->value();
  return Size(maxSize, maxSize, maxSize);
}

void ScatterPlot2DOptionsWidget::setMinSizeMapping(const float minSize) {
  _ui-> minSizeSpinBox->setValue((int)minSize);
}

void ScatterPlot2DOptionsWidget::setMaxSizeMapping(const float maxSize) {
  _ui->maxSizeSpinBox->setValue((int)maxSize);
}

void ScatterPlot2DOptionsWidget::changeButtonBackgroundColor(QPushButton *button) {
  QColor currentButtonColor = button->palette().color(QPalette::Button);
#if (QT_VERSION >= QT_VERSION_CHECK(4, 5, 0))
  QColor newColor = QColorDialog::getColor(currentButtonColor, NULL, "Select Color", QColorDialog::ShowAlphaChannel);

  if (newColor.isValid()) {
    setButtonBackgroundColor(button, Color(newColor.red(), newColor.green(), newColor.blue(), newColor.alpha()));
  }

#else
  bool ok = true;
  QRgb newColor = QColorDialog::getRgba(qRgba(currentButtonColor.red(), currentButtonColor.green(), currentButtonColor.blue(), currentButtonColor.alpha()), &ok);

  if (ok) {
    setButtonBackgroundColor(button, Color(qRed(newColor), qGreen(newColor), qBlue(newColor), qAlpha(newColor)));
  }

#endif
}

void ScatterPlot2DOptionsWidget::minSizeSpinBoxValueChanged(int newValue) {
  if (_ui->maxSizeSpinBox->value() < newValue) {
    _ui->maxSizeSpinBox->setValue(newValue + 1);
  }
}

void ScatterPlot2DOptionsWidget::maxSizeSpinBoxValueChanged(int newValue) {
  if (_ui->minSizeSpinBox->value() > newValue) {
    _ui->minSizeSpinBox->setValue(newValue - 1);
  }
}

bool ScatterPlot2DOptionsWidget::uniformBackground() const {
  return _ui->uniformBackgroundRB->isChecked();
}

void ScatterPlot2DOptionsWidget::updateColorScale() {
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

void ScatterPlot2DOptionsWidget::showEvent(QShowEvent*) {
  updateColorScale();
}

bool ScatterPlot2DOptionsWidget::displayGraphEdges() const {
  return _ui->showEdgesCB->isChecked();
}

void ScatterPlot2DOptionsWidget::setDisplayGraphEdges(const bool showEdges) {
  _ui->showEdgesCB->setChecked(showEdges);
}

bool ScatterPlot2DOptionsWidget::configurationChanged() {
  bool confChanged=false;

  if(oldValuesInitialized) {
    if(oldUniformBackground!=uniformBackground() ||
        oldUniformBackgroundColor!=getUniformBackgroundColor() ||
        oldMinusOneColor!=getMinusOneColor() ||
        oldZeroColor!=getZeroColor() ||
        oldOneColor!=getOneColor() ||
        oldMinSizeMapping!=getMinSizeMapping() ||
        oldMaxSizeMapping!=getMaxSizeMapping() ||
        oldDisplayGraphEdges!=displayGraphEdges() ) {
      confChanged=true;
    }
  }
  else {
    confChanged=true;
    oldValuesInitialized=true;
  }

  if(confChanged) {
    oldUniformBackground=uniformBackground();
    oldUniformBackgroundColor=getUniformBackgroundColor();
    oldMinusOneColor=getMinusOneColor();
    oldZeroColor=getZeroColor();
    oldOneColor=getOneColor();
    oldMinSizeMapping=getMinSizeMapping();
    oldMaxSizeMapping=getMaxSizeMapping();
    oldDisplayGraphEdges=displayGraphEdges();
  }

  return confChanged;
}

}
