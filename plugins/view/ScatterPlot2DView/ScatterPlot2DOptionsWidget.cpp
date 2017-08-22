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

#include <QPainter>
#include <QLinearGradient>
#include <QMainWindow>

#include "ScatterPlot2DOptionsWidget.h"
#include "ui_ScatterPlot2DOptionsWidget.h"

#include <tulip/Perspective.h>

namespace tlp {

ScatterPlot2DOptionsWidget::ScatterPlot2DOptionsWidget(QWidget *parent) : QWidget(parent),oldValuesInitialized(false),_ui(new Ui::ScatterPlot2DOptionsWidgetData) {
  _ui->setupUi(this);
  _ui->backColorButton->setDialogParent(Perspective::instance()->mainWindow());
  _ui->minusOneColorButton->setDialogParent(Perspective::instance()->mainWindow());
  _ui->zeroColorButton->setDialogParent(Perspective::instance()->mainWindow());
  _ui->oneColorButton->setDialogParent(Perspective::instance()->mainWindow());
  _ui->backColorButton->setDialogTitle("Choose the background color");
  _ui->minusOneColorButton->setDialogTitle("Choose the color for -1");
  _ui->zeroColorButton->setDialogTitle("Choose the color for 0");
  _ui->oneColorButton->setDialogTitle("Choose the color for 1");
  setBackgroundColor(Color(255,255,255));
  setButtonColor(_ui->minusOneColorButton, Color(0,0,255));
  setButtonColor(_ui->zeroColorButton, Color(255,255,255));
  setButtonColor(_ui->oneColorButton, Color(0,255,0));
  updateColorScale();
  connect(_ui->minusOneColorButton, SIGNAL(clicked()), this, SLOT(updateColorScale()));
  connect(_ui->zeroColorButton, SIGNAL(clicked()), this, SLOT(updateColorScale()));
  connect(_ui->oneColorButton, SIGNAL(clicked()), this, SLOT(updateColorScale()));
  connect(_ui->minSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(minSizeSpinBoxValueChanged(int)));
  connect(_ui->maxSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(maxSizeSpinBoxValueChanged(int)));
  connect(_ui->useXScaleCheckBox, SIGNAL(toggled(bool)), this, SLOT(pressXScaleCheckBox(bool)));
  connect(_ui->useYScaleCheckBox, SIGNAL(toggled(bool)), this, SLOT(pressYScaleCheckBox(bool)));
}

ScatterPlot2DOptionsWidget::~ScatterPlot2DOptionsWidget() {
  delete _ui;
}

void ScatterPlot2DOptionsWidget::setWidgetEnabled(const bool enabled) {
  _ui->xAxisGroupBox->setEnabled(enabled);
  _ui->yAxisGroupBox->setEnabled(enabled);
}

Color ScatterPlot2DOptionsWidget::getMinusOneColor() const {
  return _ui->minusOneColorButton->tulipColor();
}

Color ScatterPlot2DOptionsWidget::getZeroColor() const {
  return _ui->zeroColorButton->tulipColor();
}

Color ScatterPlot2DOptionsWidget::getOneColor() const {
  return _ui->oneColorButton->tulipColor();
}

Color ScatterPlot2DOptionsWidget::getBackgroundColor() const {
  return _ui->backColorButton->tulipColor();
}

void ScatterPlot2DOptionsWidget::setBackgroundColor(const Color &color) {
  _ui->backColorButton->setTulipColor(color);
}

void ScatterPlot2DOptionsWidget::setButtonColor(ColorButton *button, const Color &color) {
  button->setTulipColor(color);
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
  _ui-> minSizeSpinBox->setValue(int(minSize));
}

void ScatterPlot2DOptionsWidget::setMaxSizeMapping(const float maxSize) {
  _ui->maxSizeSpinBox->setValue(int(maxSize));
}

bool ScatterPlot2DOptionsWidget::useCustomXAxisScale() const {
  return _ui->useXScaleCheckBox->isChecked();
}

void ScatterPlot2DOptionsWidget::useCustomXAxisScale(const bool value) {
  _ui->useXScaleCheckBox->setChecked(value);
}

bool ScatterPlot2DOptionsWidget::useCustomYAxisScale() const {
  return _ui->useYScaleCheckBox->isChecked();
}

void ScatterPlot2DOptionsWidget::useCustomYAxisScale(const bool value) {
  _ui->useYScaleCheckBox->setChecked(value);
}

std::pair<double, double> ScatterPlot2DOptionsWidget::getXAxisScale() const {
  return std::make_pair(_ui->useXMinSpinBox->value(), _ui->useXMaxSpinBox->value());
}
void ScatterPlot2DOptionsWidget::setXAxisScale(const std::pair<double, double>& value) {
  _ui->useXMinSpinBox->setValue(value.first);
  _ui->useXMaxSpinBox->setValue(value.second);
}
std::pair<double, double> ScatterPlot2DOptionsWidget::getYAxisScale() const {
  return std::make_pair(_ui->useYMinSpinBox->value(), _ui->useYMaxSpinBox->value());
}
void ScatterPlot2DOptionsWidget::setYAxisScale(const std::pair<double, double>& value) {
  _ui->useYMinSpinBox->setValue(value.first);
  _ui->useYMaxSpinBox->setValue(value.second);
}

void ScatterPlot2DOptionsWidget::setInitXAxisScale(const std::pair<double, double>& value) {
  initXAxisScale = value;
}
void ScatterPlot2DOptionsWidget::setInitYAxisScale(const std::pair<double, double>& value) {
  initYAxisScale = value;
}

void ScatterPlot2DOptionsWidget::resetAxisScale() {
  std::pair<double, double> tmp_pair = std::make_pair(0,0);
  setXAxisScale(tmp_pair);
  setYAxisScale(tmp_pair);
  useCustomXAxisScale(false);
  useCustomYAxisScale(false);
  setInitXAxisScale(tmp_pair);
  setInitYAxisScale(tmp_pair);
  oldXAxisScale = tmp_pair;
  oldYAxisScale = tmp_pair;
  initXAxisScale = tmp_pair;
  initYAxisScale = tmp_pair;
  oldUseCustomXAxisScale = false;
  oldUseCustomYAxisScale = false;
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
  QLinearGradient qLinearGradient(0, _ui->colorScaleLabel->height() / 2, _ui->colorScaleLabel->width()-1, _ui->colorScaleLabel->height() / 2);
  qLinearGradient.setColorAt(0, _ui->minusOneColorButton->color());
  qLinearGradient.setColorAt(1./2., _ui->zeroColorButton->color());
  qLinearGradient.setColorAt(1, _ui->oneColorButton->color());
  painter.fillRect(0, 0, _ui->colorScaleLabel->width(), _ui->colorScaleLabel->height(), qLinearGradient);
  painter.end();
  _ui->colorScaleLabel->setPixmap(pixmap);
}

void ScatterPlot2DOptionsWidget::showEvent(QShowEvent*) {
  updateColorScale();
}

bool ScatterPlot2DOptionsWidget::displayNodeLabels() const {
  return _ui->showLabelCB->isChecked();
}

void ScatterPlot2DOptionsWidget::setDisplayNodeLabels(const bool showLabels) {
  _ui->showLabelCB->setChecked(showLabels);
}

bool ScatterPlot2DOptionsWidget::displayScaleLabels() const {
  return _ui->scaleLabels->isChecked();
}

void ScatterPlot2DOptionsWidget::setDisplayScaleLabels(const bool scaleLabels) {
  _ui->scaleLabels->setChecked(scaleLabels);
}

bool ScatterPlot2DOptionsWidget::displayGraphEdges() const {
  return _ui->showEdgesCB->isChecked();
}

void ScatterPlot2DOptionsWidget::setDisplayGraphEdges(const bool showEdges) {
  _ui->showEdgesCB->setChecked(showEdges);
}

void ScatterPlot2DOptionsWidget::pressXScaleCheckBox(bool checked) {
  _ui->useXMinLabel->setEnabled(checked);
  _ui->useXMinSpinBox->setEnabled(checked);
  _ui->useXMaxLabel->setEnabled(checked);
  _ui->useXMaxSpinBox->setEnabled(checked);
}

void ScatterPlot2DOptionsWidget::pressYScaleCheckBox(bool checked) {
  _ui->useYMinLabel->setEnabled(checked);
  _ui->useYMinSpinBox->setEnabled(checked);
  _ui->useYMaxLabel->setEnabled(checked);
  _ui->useYMaxSpinBox->setEnabled(checked);
}

bool ScatterPlot2DOptionsWidget::configurationChanged() {
  bool confChanged=false;

  if(oldValuesInitialized) {
    if (oldXAxisScale!=getXAxisScale()) {
      confChanged = useCustomXAxisScale();
    }

    if (oldYAxisScale!=getYAxisScale()) {
      confChanged = confChanged || useCustomYAxisScale();
    }

    if(oldUniformBackground!=uniformBackground() ||
        oldUniformBackgroundColor!=getBackgroundColor() ||
        oldMinusOneColor!=getMinusOneColor() ||
        oldZeroColor!=getZeroColor() ||
        oldOneColor!=getOneColor() ||
        oldMinSizeMapping!=getMinSizeMapping() ||
        oldMaxSizeMapping!=getMaxSizeMapping() ||
        oldDisplayGraphEdges!=displayGraphEdges() ||
        oldDisplayNodeLabels!=displayNodeLabels() ||
        oldlabelscaled!=displayScaleLabels()||
        oldUseCustomXAxisScale!=useCustomXAxisScale() ||
        oldUseCustomYAxisScale!=useCustomYAxisScale()) {
      confChanged=true;
    }
  }
  else {
    confChanged=true;
    oldValuesInitialized=true;
  }

  if(confChanged) {
    oldUniformBackground=uniformBackground();
    oldUniformBackgroundColor=getBackgroundColor();
    oldMinusOneColor=getMinusOneColor();
    oldZeroColor=getZeroColor();
    oldOneColor=getOneColor();
    oldMinSizeMapping=getMinSizeMapping();
    oldMaxSizeMapping=getMaxSizeMapping();
    oldDisplayGraphEdges=displayGraphEdges();
    oldDisplayNodeLabels=displayNodeLabels();
    oldlabelscaled=displayScaleLabels();
    oldUseCustomXAxisScale=useCustomXAxisScale();
    oldUseCustomYAxisScale=useCustomYAxisScale();
    oldXAxisScale=getXAxisScale();
    oldYAxisScale=getYAxisScale();

    if (_ui->useXMinSpinBox->value() > initXAxisScale.first)
      _ui->useXMinSpinBox->setValue(initXAxisScale.first);

    if (_ui->useXMaxSpinBox->value() < initXAxisScale.second)
      _ui->useXMaxSpinBox->setValue(initXAxisScale.second);

    if (_ui->useYMinSpinBox->value() > initYAxisScale.first)
      _ui->useYMinSpinBox->setValue(initYAxisScale.first);

    if (_ui->useYMaxSpinBox->value() < initYAxisScale.second)
      _ui->useYMaxSpinBox->setValue(initYAxisScale.second);
  }

  return confChanged;
}

}
