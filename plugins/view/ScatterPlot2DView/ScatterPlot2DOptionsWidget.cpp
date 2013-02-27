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

#include "ScatterPlot2DOptionsWidget.h"

namespace tlp {

ScatterPlot2DOptionsWidget::ScatterPlot2DOptionsWidget(QWidget *parent) : QWidget(parent),oldValuesInitialized(false) {
	setupUi(this);
	setBackgroundColor(Color(255,255,255));
	setButtonBackgroundColor(minusOneColorButton, Color(0,0,255));
	setButtonBackgroundColor(zeroColorButton, Color(255,255,255));
	setButtonBackgroundColor(oneColorButton, Color(0,255,0));
	updateColorScale();
	connect(backColorButton, SIGNAL(clicked()), this, SLOT(pressBackgroundColorButton()));
	connect(minusOneColorButton, SIGNAL(clicked()), this, SLOT(pressMinusOneColorButton()));
	connect(zeroColorButton, SIGNAL(clicked()), this, SLOT(pressZeroColorButton()));
	connect(oneColorButton, SIGNAL(clicked()), this, SLOT(pressOneColorButton()));

	connect(minSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(minSizeSpinBoxValueChanged(int)));
	connect(maxSizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(maxSizeSpinBoxValueChanged(int)));
  connect(applyButton,SIGNAL(clicked()),this,SLOT(applySettings()));
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
	return getButtonColor(backColorButton);
}

Color ScatterPlot2DOptionsWidget::getMinusOneColor() const {
	return getButtonColor(minusOneColorButton);
}

Color ScatterPlot2DOptionsWidget::getZeroColor() const {
	return getButtonColor(zeroColorButton);
}

Color ScatterPlot2DOptionsWidget::getOneColor() const {
	return getButtonColor(oneColorButton);
}

void ScatterPlot2DOptionsWidget::setBackgroundColor(const Color &color) {
	setButtonBackgroundColor(backColorButton, color);
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
	changeButtonBackgroundColor(backColorButton);
}

void ScatterPlot2DOptionsWidget::pressMinusOneColorButton() {
	changeButtonBackgroundColor(minusOneColorButton);
	updateColorScale();
}

void ScatterPlot2DOptionsWidget::pressZeroColorButton() {
	changeButtonBackgroundColor(zeroColorButton);
	updateColorScale();
}

void ScatterPlot2DOptionsWidget::pressOneColorButton() {
	changeButtonBackgroundColor(oneColorButton);
	updateColorScale();
}

Size ScatterPlot2DOptionsWidget::getMinSizeMapping() const {
	float minSize = minSizeSpinBox->value();
	return Size(minSize, minSize, minSize);
}

Size ScatterPlot2DOptionsWidget::getMaxSizeMapping() const {
	float maxSize = maxSizeSpinBox->value();
	return Size(maxSize, maxSize, maxSize);
}

void ScatterPlot2DOptionsWidget::setMinSizeMapping(const float minSize) {
	minSizeSpinBox->setValue((int)minSize);
}

void ScatterPlot2DOptionsWidget::setMaxSizeMapping(const float maxSize) {
	maxSizeSpinBox->setValue((int)maxSize);
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
	if (maxSizeSpinBox->value() < newValue) {
		maxSizeSpinBox->setValue(newValue + 1);
	}
}

void ScatterPlot2DOptionsWidget::maxSizeSpinBoxValueChanged(int newValue) {
	if (minSizeSpinBox->value() > newValue) {
		minSizeSpinBox->setValue(newValue - 1);
	}
}

bool ScatterPlot2DOptionsWidget::uniformBackground() const {
	return uniformBackgroundRB->isChecked();
}

void ScatterPlot2DOptionsWidget::updateColorScale() {
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

void ScatterPlot2DOptionsWidget::showEvent(QShowEvent*) {
	updateColorScale();
}

bool ScatterPlot2DOptionsWidget::displayGraphEdges() const {
	return showEdgesCB->isChecked();
}

void ScatterPlot2DOptionsWidget::setDisplayGraphEdges(const bool showEdges) {
    showEdgesCB->setChecked(showEdges);
    }

bool ScatterPlot2DOptionsWidget::configurationChanged() {
  bool confChanged=false;
  if(oldValuesInitialized){
    if(oldUniformBackground!=uniformBackground() ||
       oldUniformBackgroundColor!=getUniformBackgroundColor() ||
       oldMinusOneColor!=getMinusOneColor() ||
       oldZeroColor!=getZeroColor() ||
       oldOneColor!=getOneColor() ||
       oldMinSizeMapping!=getMinSizeMapping() ||
       oldMaxSizeMapping!=getMaxSizeMapping() ||
       oldDisplayGraphEdges!=displayGraphEdges() ){
      confChanged=true;
    }
  }else{
    confChanged=true;
    oldValuesInitialized=true;
  }

  if(confChanged){
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
