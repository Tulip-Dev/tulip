/*
	  ParallelCoordsDrawConfigWidget.cpp

   Created on: 6 avr. 2009
       Author: Antoine Lambert
       E-mail: antoine.lambert@labri.fr

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

 */


#include <QtGui/QFileDialog>
#include <QtGui/QColorDialog>

#include <tulip/TlpTools.h>

#include "ParallelTools.h"
#include "ParallelCoordsDrawConfigWidget.h"

using namespace std;

namespace tlp {

ParallelCoordsDrawConfigWidget::ParallelCoordsDrawConfigWidget(QWidget *parent) : QWidget(parent) {
	setupUi(this);
	setBackgroundColor(Color(255,255,255));
	connect(browseButton,SIGNAL(clicked()),this,SLOT(pressButtonBrowse()));
	connect(userTexture, SIGNAL(toggled(bool)), this, SLOT(userTextureRbToggled(bool)));
	connect(minAxisPointSize, SIGNAL(valueChanged(int)), this, SLOT(minAxisPointSizeValueChanged(int)));
	connect(maxAxisPointSize, SIGNAL(valueChanged(int)), this, SLOT(maxAxisPointSizeValueChanged(int)));
	connect(bgColorButton,SIGNAL(clicked()),this,SLOT(pressColorButton()));
}

void ParallelCoordsDrawConfigWidget::pressButtonBrowse() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Texture File"), "./", tr("Image Files (*.png *.jpg *.bmp)"));
	userTextureFile->setText(fileName);
}

unsigned int ParallelCoordsDrawConfigWidget::getAxisHeight() const {
	return axisHeight->value();
}

void ParallelCoordsDrawConfigWidget::setAxisHeight(const unsigned int aHeight) {
	axisHeight->setValue(aHeight);
}

bool ParallelCoordsDrawConfigWidget::drawPointOnAxis() const {
	return gBoxAxisPoints->isChecked();
}

string ParallelCoordsDrawConfigWidget::getLinesTextureFilename() const {
	if (gBoxLineTexture->isChecked()) {
		if (defaultTexture->isChecked()) {
			return string(TulipBitmapDir + DEFAULT_TEXTURE_FILE);
		} else {
			return userTextureFile->text().toStdString();
		}
	} else {
		return "";
	}
}

void ParallelCoordsDrawConfigWidget::setLinesTextureFilename(const std::string &linesTextureFileName) {
	if (linesTextureFileName != "") {
		gBoxLineTexture->setChecked(true);
		if (linesTextureFileName == string(TulipBitmapDir + DEFAULT_TEXTURE_FILE)) {
			defaultTexture->setChecked(true);
		} else {
			userTexture->setChecked(true);
			userTextureFile->setText(QString(linesTextureFileName.c_str()));
		}
	} else {
		gBoxLineTexture->setChecked(false);
	}
}

Size ParallelCoordsDrawConfigWidget::getAxisPointMinSize() const {
	float pointSize = minAxisPointSize->text().toFloat();
	return Size(pointSize, pointSize, pointSize);
}

Size ParallelCoordsDrawConfigWidget::getAxisPointMaxSize() const {
	float pointSize = maxAxisPointSize->text().toFloat();
	return Size(pointSize, pointSize, pointSize);
}

void ParallelCoordsDrawConfigWidget::setAxisPointMinSize(const unsigned int axisPointMinSize) {
	minAxisPointSize->setValue(axisPointMinSize);
}

void ParallelCoordsDrawConfigWidget::setAxisPointMaxSize(const unsigned int axisPointMaxSize) {
	maxAxisPointSize->setValue(axisPointMaxSize);
}

bool ParallelCoordsDrawConfigWidget::displayNodesLabels() const {
	return displayLabelsCB->isChecked();
}

void ParallelCoordsDrawConfigWidget::userTextureRbToggled(bool checked) {
	if (checked) {
		userTextureFile->setEnabled(true);
		browseButton->setEnabled(true);
	} else {
		userTextureFile->setEnabled(false);
		browseButton->setEnabled(false);
	}
}

void ParallelCoordsDrawConfigWidget::minAxisPointSizeValueChanged(int newValue) {
	if (maxAxisPointSize->value() < newValue) {
		maxAxisPointSize->setValue(newValue + 1);
	}
}

void ParallelCoordsDrawConfigWidget::maxAxisPointSizeValueChanged(int newValue) {
	if (minAxisPointSize->value() > newValue) {
		minAxisPointSize->setValue(newValue - 1);
	}
}

void ParallelCoordsDrawConfigWidget::setLinesColorAlphaValue(unsigned int value) {
	if (value > 255) {
		viewColorAlphaRb->setChecked(true);
		userAlphaRb->setChecked(false);
	} else {
		viewColorAlphaRb->setChecked(false);
		userAlphaRb->setChecked(true);
		viewColorAlphaValue->setValue(value);
	}
}

unsigned int ParallelCoordsDrawConfigWidget::getLinesColorAlphaValue() const {
	if (viewColorAlphaRb->isChecked()) {
		return 300;
	} else {
		return (unsigned int) viewColorAlphaValue->value();
	}
}

Color ParallelCoordsDrawConfigWidget::getBackgroundColor() const {
	QString buttonStyleSheet = bgColorButton->styleSheet();
	QString backgroundColorCodeHex = buttonStyleSheet.mid(buttonStyleSheet.indexOf("#") + 1, 6);
	bool ok;
	return Color(backgroundColorCodeHex.mid(0, 2).toInt(&ok, 16),
				 backgroundColorCodeHex.mid(2, 2).toInt(&ok, 16),
				 backgroundColorCodeHex.mid(4, 2).toInt(&ok, 16));
}

void ParallelCoordsDrawConfigWidget::setBackgroundColor(const Color &color) {
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

	bgColorButton->setStyleSheet("QPushButton { background-color: #"+colorStr +"}");
}

void ParallelCoordsDrawConfigWidget::pressColorButton() {
	QColor newColor = QColorDialog::getColor(bgColorButton->palette().color(QPalette::Button), this);
	if (newColor.isValid()) {
		setBackgroundColor(Color(newColor.red(), newColor.green(), newColor.blue()));
	}
}

unsigned int ParallelCoordsDrawConfigWidget::getSpaceBetweenAxis() const {
	return spaceBetweenAxis->value();
}

void ParallelCoordsDrawConfigWidget::setSpaceBetweenAxis(const unsigned int spaceBetweenAxis) {
	this->spaceBetweenAxis->setValue(spaceBetweenAxis);
}

void ParallelCoordsDrawConfigWidget::setDrawPointOnAxis(const bool drawPointOnAxis) {
	gBoxAxisPoints->setChecked(drawPointOnAxis);
}

unsigned int ParallelCoordsDrawConfigWidget::getUnhighlightedEltsColorsAlphaValue() const {
	return (unsigned int) nonHighlightedEltsAlphaValue->value();
}

void ParallelCoordsDrawConfigWidget::setUnhighlightedEltsColorsAlphaValue(const unsigned int alphaValue) {
	nonHighlightedEltsAlphaValue->setValue(alphaValue);
}

}
