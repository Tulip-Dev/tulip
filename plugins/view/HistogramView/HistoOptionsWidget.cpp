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

#include <iostream>

#include <QtGui/QColorDialog>

#include "HistoOptionsWidget.h"

using namespace std;

namespace tlp {

HistoOptionsWidget::HistoOptionsWidget(QWidget *parent) : QWidget(parent),oldValueInitialized(false) {
  setupUi(this);
  setBackgroundColor(Color(255,255,255));
  connect(backColorButton, SIGNAL(clicked()), this, SLOT(pressBackgroundColorButton()));
  connect(applyButton,SIGNAL(clicked()),this,SLOT(applySettings()));
}

void HistoOptionsWidget::setWidgetEnabled(const bool enabled) {
  frame->setEnabled(enabled);
}

void HistoOptionsWidget::setNbOfHistogramBins(const unsigned int nbOfHistogramBins) {
  nbHistoBins->setValue(nbOfHistogramBins);
}

unsigned int HistoOptionsWidget::getNbOfHistogramBins() {
  return nbHistoBins->value();
}

void HistoOptionsWidget::setNbXGraduations(const unsigned int nbXGrads) {
  nbXGraduations->setValue(nbXGrads);
}

unsigned int HistoOptionsWidget::getNbXGraduations() {
  return nbXGraduations->value();
}

void HistoOptionsWidget::setYAxisIncrementStep(const unsigned int yAxisIncrementStep) {
  YAxisIncrementStep->setValue(yAxisIncrementStep);
}

unsigned int HistoOptionsWidget::getYAxisIncrementStep() {
  return YAxisIncrementStep->value();
}

void HistoOptionsWidget::setCumulativeFrequenciesHistogram(const bool cumulHisto) {
  cumulFreqHisto->setChecked(cumulHisto);
}

bool HistoOptionsWidget::cumulativeFrequenciesHisto() {
  return cumulFreqHisto->isChecked();
}

void HistoOptionsWidget::setUniformQuantification(const bool uniformQuantification) {
  uniformQuantificationCB->setChecked(uniformQuantification);
}

bool HistoOptionsWidget::uniformQuantification() {
  return uniformQuantificationCB->isChecked();
}

void HistoOptionsWidget::enableOrDisableNbXGraduationsSP(int uniQuantState) {
  bool uniQuantActivated = (uniQuantState == Qt::Checked);
  nbXGraduations->setEnabled(!uniQuantActivated);
  xAxisLogscale->setEnabled(!uniQuantActivated);
}

void HistoOptionsWidget::setXAxisLogScale(const bool xAxisLogScale) {
  xAxisLogscale->setChecked(xAxisLogScale);
}

bool HistoOptionsWidget::xAxisLogScaleSet() const {
  return xAxisLogscale->isChecked();
}

void HistoOptionsWidget::setYAxisLogScale(const bool yAxisLogScale) {
  yAxisLogscale->setChecked(yAxisLogScale);
}

bool HistoOptionsWidget::yAxisLogScaleSet() const {
  return yAxisLogscale->isChecked();
}

void HistoOptionsWidget::setBinWidth(const double width) {
  binWidth->setText(QString::number(width));
}

Color HistoOptionsWidget::getBackgroundColor() const {
  QString buttonStyleSheet(backColorButton->styleSheet());
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
  backColorButton->setStyleSheet("QPushButton { background-color: #"+colorStr +"}");
}

void HistoOptionsWidget::pressBackgroundColorButton() {
  QColor newColor(QColorDialog::getColor(backColorButton->palette().color(QPalette::Button)));

  if (newColor.isValid()) {
    setBackgroundColor(Color(newColor.red(), newColor.green(), newColor.blue()));
  }
}

bool HistoOptionsWidget::showGraphEdges() const {
  return showEdgesCB->isChecked();
}

void HistoOptionsWidget::enableShowGraphEdgesCB(const bool enable) {
  showEdgesCB->setEnabled(enable);
}

void HistoOptionsWidget::setShowGraphEdges(const bool showGraphEdges) {
  showEdgesCB->setChecked(showGraphEdges);
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
