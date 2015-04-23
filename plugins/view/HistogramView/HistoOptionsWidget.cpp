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

#include "HistoOptionsWidget.h"
#include "ui_HistoOptionsWidget.h"

using namespace std;

namespace tlp {

HistoOptionsWidget::HistoOptionsWidget(QWidget *parent) : QWidget(parent),oldValueInitialized(false),_ui(new Ui::HistoOptionsWidgetData) {
  _ui->setupUi(this);
  setBackgroundColor(Color(255,255,255));
  connect(_ui->backColorButton, SIGNAL(clicked()), this, SLOT(pressBackgroundColorButton()));
  connect(_ui->useXScaleCheckBox, SIGNAL(toggled(bool)), this, SLOT(pressXScaleCheckBox(bool)));
  connect(_ui->useYScaleCheckBox, SIGNAL(toggled(bool)), this, SLOT(pressYScaleCheckBox(bool)));
}

HistoOptionsWidget::~HistoOptionsWidget() {
  delete _ui;
}

void HistoOptionsWidget::setWidgetEnabled(const bool enabled) {
  _ui->frame->setEnabled(enabled);
}

void HistoOptionsWidget::setNbOfHistogramBins(const unsigned int nbOfHistogramBins) {
  _ui->nbHistoBins->setValue(nbOfHistogramBins);
}

unsigned int HistoOptionsWidget::getNbOfHistogramBins() {
  return _ui->nbHistoBins->value();
}

void HistoOptionsWidget::setNbXGraduations(const unsigned int nbXGrads) {
  _ui->nbXGraduations->setValue(nbXGrads);
}

unsigned int HistoOptionsWidget::getNbXGraduations() {
  return _ui->nbXGraduations->value();
}

void HistoOptionsWidget::setYAxisIncrementStep(const unsigned int yAxisIncrementStep) {
  _ui->YAxisIncrementStep->setValue(yAxisIncrementStep);
}

unsigned int HistoOptionsWidget::getYAxisIncrementStep() {
  return _ui->YAxisIncrementStep->value();
}

void HistoOptionsWidget::setCumulativeFrequenciesHistogram(const bool cumulHisto) {
  _ui->cumulFreqHisto->setChecked(cumulHisto);
}

bool HistoOptionsWidget::cumulativeFrequenciesHisto() {
  return _ui->cumulFreqHisto->isChecked();
}

void HistoOptionsWidget::setUniformQuantification(const bool uniformQuantification) {
  _ui->uniformQuantificationCB->setChecked(uniformQuantification);
}

bool HistoOptionsWidget::uniformQuantification() {
  return _ui->uniformQuantificationCB->isChecked();
}

void HistoOptionsWidget::enableOrDisableNbXGraduationsSP(int uniQuantState) {
  bool uniQuantActivated = (uniQuantState == Qt::Checked);
  _ui->nbXGraduations->setEnabled(!uniQuantActivated);
  _ui->xAxisLogscale->setEnabled(!uniQuantActivated);
}

void HistoOptionsWidget::setXAxisLogScale(const bool xAxisLogScale) {
  _ui->xAxisLogscale->setChecked(xAxisLogScale);
}

bool HistoOptionsWidget::xAxisLogScaleSet() const {
  return _ui->xAxisLogscale->isChecked();
}

void HistoOptionsWidget::setYAxisLogScale(const bool yAxisLogScale) {
  _ui->yAxisLogscale->setChecked(yAxisLogScale);
}

bool HistoOptionsWidget::yAxisLogScaleSet() const {
  return _ui->yAxisLogscale->isChecked();
}

bool HistoOptionsWidget::useCustomXAxisScale() const {
  return _ui->useXScaleCheckBox->isChecked();
}

void HistoOptionsWidget::useCustomXAxisScale(const bool value) {
  _ui->useXScaleCheckBox->setChecked(value);
}

bool HistoOptionsWidget::useCustomYAxisScale() const {
  return _ui->useYScaleCheckBox->isChecked();
}

void HistoOptionsWidget::useCustomYAxisScale(const bool value) {
  _ui->useYScaleCheckBox->setChecked(value);
}

std::pair<double, double> HistoOptionsWidget::getXAxisScale() const {
  return make_pair(_ui->useXMinSpinBox->value(), _ui->useXMaxSpinBox->value());
}
void HistoOptionsWidget::setXAxisScale(const std::pair<double, double>& value) {
  _ui->useXMinSpinBox->setValue(value.first);
  _ui->useXMaxSpinBox->setValue(value.second);
}
std::pair<double, double> HistoOptionsWidget::getYAxisScale() const {
  return std::make_pair(_ui->useYMinSpinBox->value(), _ui->useYMaxSpinBox->value());
}
void HistoOptionsWidget::setYAxisScale(const std::pair<double, double>& value) {
  _ui->useYMinSpinBox->setValue(value.first);
  _ui->useYMaxSpinBox->setValue(value.second);
}

void HistoOptionsWidget::setInitXAxisScale(const std::pair<double, double>& value) {
  initXAxisScale = value;
}
void HistoOptionsWidget::setInitYAxisScale(const std::pair<double, double>& value) {
  initYAxisScale = value;
}

void HistoOptionsWidget::resetAxisScale() {
  std::pair<double, double> tmp_pair = make_pair(0,0);
  setXAxisScale(tmp_pair);
  setYAxisScale(tmp_pair);
  useCustomXAxisScale(false);
  useCustomYAxisScale(false);
  setInitXAxisScale(tmp_pair);
  setInitYAxisScale(tmp_pair);
}

void HistoOptionsWidget::setBinWidth(const double width) {
  _ui->binWidth->setText(QString::number(width));
}

Color HistoOptionsWidget::getBackgroundColor() const {
  QString buttonStyleSheet(_ui->backColorButton->styleSheet());
  QString backgroundColorCodeHex(buttonStyleSheet.mid(buttonStyleSheet.indexOf("#") + 1, 6));
  bool ok;
  return Color(backgroundColorCodeHex.mid(0, 2).toInt(&ok, 16),
               backgroundColorCodeHex.mid(2, 2).toInt(&ok, 16),
               backgroundColorCodeHex.mid(4, 2).toInt(&ok, 16));

}

void HistoOptionsWidget::setBackgroundColor(const Color &color) {
  QString colorStr;
  QString str;
  str.setNum(color.getR(),16);

  if(str.size()!=2)
    str.insert(0,"0");

  colorStr.append(str);

  str.setNum(color.getG(),16);

  if(str.size()!=2)
    str.insert(0,"0");

  colorStr.append(str);

  str.setNum(color.getB(),16);

  if(str.size()!=2)
    str.insert(0,"0");

  colorStr.append(str);
  _ui->backColorButton->setStyleSheet("QPushButton { background-color: #"+colorStr +"}");
}

void HistoOptionsWidget::pressBackgroundColorButton() {
  QColor newColor(QColorDialog::getColor(_ui->backColorButton->palette().color(QPalette::Button)));

  if (newColor.isValid()) {
    setBackgroundColor(Color(newColor.red(), newColor.green(), newColor.blue()));
  }
}

bool HistoOptionsWidget::showGraphEdges() const {
  return _ui->showEdgesCB->isChecked();
}

void HistoOptionsWidget::enableShowGraphEdgesCB(const bool enable) {
  _ui->showEdgesCB->setEnabled(enable);
}

void HistoOptionsWidget::setShowGraphEdges(const bool showGraphEdges) {
  _ui->showEdgesCB->setChecked(showGraphEdges);
}

void HistoOptionsWidget::pressXScaleCheckBox(bool checked) {
  _ui->useXMinLabel->setEnabled(checked);
  _ui->useXMinSpinBox->setEnabled(checked);
  _ui->useXMaxLabel->setEnabled(checked);
  _ui->useXMaxSpinBox->setEnabled(checked);
}

void HistoOptionsWidget::pressYScaleCheckBox(bool checked) {
  _ui->useYMinLabel->setEnabled(checked);
  _ui->useYMinSpinBox->setEnabled(checked);
  _ui->useYMaxLabel->setEnabled(checked);
  _ui->useYMaxSpinBox->setEnabled(checked);
}

bool HistoOptionsWidget::configurationChanged() {
  bool confChanged=false;

  if(oldValueInitialized) {
    if (oldXAxisScale!=getXAxisScale()) {
      confChanged = useCustomXAxisScale();
    }

    if (oldYAxisScale!=getYAxisScale()) {
      confChanged = confChanged || useCustomYAxisScale();
    }

    if(oldNbOfHistogramBins!=getNbOfHistogramBins() ||
        oldNbXGraduations!=getNbXGraduations() ||
        oldYAxisIncrementStep!=getYAxisIncrementStep() ||
        oldCumulativeFrequenciesHistogram!=cumulativeFrequenciesHisto() ||
        oldUniformQuantification!=uniformQuantification() ||
        oldXAxisLogScale!=xAxisLogScaleSet() ||
        oldYAxisLogScale!=yAxisLogScaleSet() ||
        oldBackgroundColor!=getBackgroundColor() ||
        oldShowGraphEdges!=showGraphEdges() ||
        oldUseCustomXAxisScale!=useCustomXAxisScale() ||
        oldUseCustomYAxisScale!=useCustomYAxisScale()) {
      confChanged=true;
    }
  }
  else {
    confChanged=true;
    oldValueInitialized=true;
  }

  if(confChanged) {
    oldNbOfHistogramBins=getNbOfHistogramBins();
    oldNbXGraduations=getNbXGraduations();
    oldYAxisIncrementStep=getYAxisIncrementStep();
    oldCumulativeFrequenciesHistogram=cumulativeFrequenciesHisto();
    oldUniformQuantification=uniformQuantification();
    oldXAxisLogScale=xAxisLogScaleSet();
    oldYAxisLogScale=yAxisLogScaleSet();
    oldBackgroundColor=getBackgroundColor();
    oldShowGraphEdges=showGraphEdges();
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
