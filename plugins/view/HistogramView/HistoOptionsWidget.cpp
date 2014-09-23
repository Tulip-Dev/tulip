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

bool HistoOptionsWidget::configurationChanged() {
  bool confChanged=false;

  if(oldValueInitialized) {
    if(oldNbOfHistogramBins!=getNbOfHistogramBins() ||
        oldNbXGraduations!=getNbXGraduations() ||
        oldYAxisIncrementStep!=getYAxisIncrementStep() ||
        oldCumulativeFrequenciesHistogram!=cumulativeFrequenciesHisto() ||
        oldUniformQuantification!=uniformQuantification() ||
        oldXAxisLogScale!=xAxisLogScaleSet() ||
        oldYAxisLogScale!=yAxisLogScaleSet() ||
        oldBackgroundColor!=getBackgroundColor() ||
        oldShowGraphEdges!=showGraphEdges() ) {
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
  }

  return confChanged;
}

}
