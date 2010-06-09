/*
 ColorScaleConfigDialog.cpp

 Created on: 13 fÃ©vr. 2009
 Author: Antoine Lambert
 E-mail: antoine.lambert@labri.fr

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 */

#include <QtGui/QFileDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLinearGradient>
#include <QtGui/QPainter>
#include <QtCore/QSettings>
#include <QtGui/QMessageBox>
#include <QtGui/QInputDialog>

#include <vector>

#include "tulip/TlpQtTools.h"
#include "tulip/ColorScaleConfigDialog.h"

using namespace std;

namespace tlp {

ColorScaleConfigDialog::ColorScaleConfigDialog(ColorScale *colorScale,
		QWidget *parent) :
			QDialog(parent){
	setupUi(this);
	colorsTable->setColumnWidth(0, colorsTable->width());
	colorsTable->horizontalHeader()->setHidden(true);
	QPalette palette;
	palette.setColor(QPalette::Window, Qt::white);
	imageGradientPreview->setPalette(palette);
	userGradientPreview->setPalette(palette);
	imageGradientPreview->setAutoFillBackground(true);
	userGradientPreview->setAutoFillBackground(true);
	connect(savedColorScalesList, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(displaySavedGradientPreview()));
	connect(savedColorScalesList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(reeditSaveColorScale(QListWidgetItem *)));
	connect(savedColorScaleRb, SIGNAL(toggled(bool)), this, SLOT(predefinedColorScaleRbToggled(bool)));
	connect(userColorScaleImageRb, SIGNAL(toggled(bool)), this, SLOT(userColorScaleImageRbToggled(bool)));
	connect(browseButton, SIGNAL(clicked()), this, SLOT(pressButtonBrowse()));
	connect(nbColors, SIGNAL(valueChanged(int)), this, SLOT(nbColorsValueChanged(int)));
	connect(colorsTable, SIGNAL(itemDoubleClicked (QTableWidgetItem *)), this, SLOT(colorTableItemDoubleClicked(QTableWidgetItem *)));
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(displaySavedGradientPreview()));
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(displayUserGradientPreview()));
	connect(gradientCB, SIGNAL(clicked()), this, SLOT(displayUserGradientPreview()));
	connect(saveColorScaleButton, SIGNAL(clicked()), this, SLOT(saveCurrentColorScale()));
	connect(deleteColorScaleButton, SIGNAL(clicked()), this, SLOT(deleteSavedColorScale()));
	loadUserSavedColorScales();
	setColorScale(colorScale);
}

void ColorScaleConfigDialog::accept() {
	vector<Color> colors;
	bool gradient;
	if (tabWidget->currentIndex() == 1) {
		if (savedColorScaleRb->isChecked()) {
			if (savedColorScalesList->count() > 0) {
				QString savedColorScaleId = savedColorScalesList->currentItem()->text();
				QSettings settings("TulipSoftware","Tulip");
				settings.beginGroup("ColorScales");
				QList<QVariant> colorsVector = settings.value(savedColorScaleId).toList();
				QString gradientScaleId = savedColorScaleId + "_gradient?";
				gradient = settings.value(gradientScaleId).toBool();
				settings.endGroup();
				for (int i = 0 ; i < colorsVector.size() ; ++i) {
					colors.push_back(Color(colorsVector.at(i).value<QColor>().red(), colorsVector.at(i).value<QColor>().green(),
										   colorsVector.at(i).value<QColor>().blue(), colorsVector.at(i).value<QColor>().alpha()));
				}
			}
		} else if (userGradientFile->text() != "") {
			setColorScaleFromImage(userGradientFile->text());
			QImage gradientImage(userGradientFile->text());
			unsigned int imageHeight = gradientImage.height();
			for (unsigned int i = 0; i < imageHeight; ++i) {
				QRgb pixelValue = gradientImage.pixel(0, i);
				colors.push_back(Color(qRed(pixelValue), qGreen(pixelValue), qBlue(pixelValue), qAlpha(pixelValue)));
			}
			gradient = true;
		}
	} else {
		for (int i = 0; i < colorsTable->rowCount(); ++i) {
			QColor itemColor = colorsTable->item(i, 0)->backgroundColor();
			colors.push_back(Color(itemColor.red(), itemColor.green(),
					itemColor.blue(), itemColor.alpha()));
		}
		gradient = gradientCB->isChecked();
	}
	if (colors.size() > 0) {
		std::reverse(colors.begin(), colors.end());
		colorScale->setColorScale(colors, gradient);
	}
	QDialog::accept();
}

void ColorScaleConfigDialog::setColorScaleFromImage(
		const QString &imageFilePath) {
	QImage gradientImage(imageFilePath);
	unsigned int imageHeight = gradientImage.height();
	vector<Color> colors;
	for (unsigned int i = 0; i < imageHeight; ++i) {
		QRgb pixelValue = gradientImage.pixel(0, i);
		colors.push_back(Color(qRed(pixelValue), qGreen(pixelValue), qBlue(
				pixelValue), qAlpha(pixelValue)));
	}
	std::reverse(colors.begin(), colors.end());
	colorScale->setColorScale(colors, true);
}

void ColorScaleConfigDialog::pressButtonBrowse() {
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image File"),
			"./", tr("Image Files (*.png *.jpg *.bmp)"));
	userGradientFile->setText(fileName);
	displaySavedGradientPreview();
}

void ColorScaleConfigDialog::displaySavedGradientPreview() {
	QString gradientFilePath;
	if (savedColorScaleRb->isChecked()) {
		if (savedColorScalesList->count() > 0 && savedColorScalesList->currentItem()) {
			QString savedColorScaleId = savedColorScalesList->currentItem()->text();
			QSettings settings("TulipSoftware","Tulip");
			settings.beginGroup("ColorScales");
			QList<QVariant> colorsListv = settings.value(savedColorScaleId).toList();
			QString gradientScaleId = savedColorScaleId + "_gradient?";
			bool gradient = settings.value(gradientScaleId).toBool();
			settings.endGroup();
			QList<QColor> colorsList;
			for (int i = 0 ; i < colorsListv.size() ; ++i) {
				colorsList.push_back(colorsListv.at(i).value<QColor>());
			}
			displayGradientPreview(colorsList, gradient, imageGradientPreview);
		}
	} else {
		gradientFilePath = userGradientFile->text();
		if (gradientFilePath != "") {
			QPixmap gradientImage(gradientFilePath);
			imageGradientPreview->setPixmap(gradientImage.scaled(imageGradientPreview->width(), imageGradientPreview->height()));
		}
	}
}

void ColorScaleConfigDialog::displayUserGradientPreview() {

	QList<QColor> colorsVector;
	for (int i = 0; i < colorsTable->rowCount(); ++i) {
		colorsVector.push_back(colorsTable->item(i, 0)->backgroundColor());
	}
	displayGradientPreview(colorsVector, gradientCB->isChecked(), userGradientPreview);
}

static qreal clamp(qreal f, qreal minVal, qreal maxVal) {
	return min(max(f, minVal), maxVal);
}

void ColorScaleConfigDialog::displayGradientPreview(const QList<QColor> &colorsVector, bool gradient, QLabel *displayLabel) {
	QPixmap pixmap(displayLabel->width(), displayLabel->height());
	pixmap.fill(Qt::transparent);
	QPainter painter;
	painter.begin(&pixmap);
	if (gradient) {
		QLinearGradient qLinearGradient(displayLabel->width() / 2, 0,
				displayLabel->width() / 2, displayLabel->height() - 1);
		qreal increment = 1.0 / (colorsVector.size() - 1);
		qreal relPos = 0;
		for (int i = 0 ; i < colorsVector.size() ; ++i) {
			qLinearGradient.setColorAt(clamp(relPos, 0.0, 1.0), colorsVector.at(i));
			relPos += increment;
		}
		painter.fillRect(0, 0, displayLabel->width(), displayLabel->height(), qLinearGradient);
	} else {
		float rectHeight = displayLabel->height() / colorsVector.size();
		for (int i = 0; i < colorsVector.size(); ++i) {
			painter.fillRect(0, i * rectHeight, displayLabel->width(), (i + 1)
					* rectHeight, QBrush(colorsVector.at(i)));
		}
	}
	painter.end();
	displayLabel->setPixmap(pixmap.scaled(displayLabel->width(), displayLabel->height()));
}

void ColorScaleConfigDialog::predefinedColorScaleRbToggled(bool toggled) {
	if (toggled) {
		savedColorScalesList->setEnabled(true);
		deleteColorScaleButton->setEnabled(true);
		userGradientFile->setEnabled(false);
		browseButton->setEnabled(false);
		displaySavedGradientPreview();
	}
}

void ColorScaleConfigDialog::userColorScaleImageRbToggled(bool toggled) {
	if (toggled) {
		savedColorScalesList->setEnabled(false);
		deleteColorScaleButton->setEnabled(false);
		userGradientFile->setEnabled(true);
		browseButton->setEnabled(true);
		displaySavedGradientPreview();
	}
}

void ColorScaleConfigDialog::nbColorsValueChanged(int value) {
	int lastCount = colorsTable->rowCount();
	colorsTable->setRowCount(value);
	if (lastCount < value) {
		QTableWidgetItem *item = new QTableWidgetItem();
		item->setBackgroundColor(QColor(255, 255, 255, 255));
		item->setFlags(Qt::ItemIsEnabled);
		colorsTable->setItem(0, value - 1, item);
	}
	displayUserGradientPreview();
}

void ColorScaleConfigDialog::colorTableItemDoubleClicked(QTableWidgetItem *item) {
	QColor itemBgColor = item->backgroundColor();
  QColor newColor;
  if(getColorDialog(itemBgColor, this, "Select Color",newColor)){
    item->setBackgroundColor(newColor);
    displayUserGradientPreview();
  }
}

void ColorScaleConfigDialog::saveCurrentColorScale() {
	QSettings settings("TulipSoftware","Tulip");
	settings.beginGroup("ColorScales");
	QStringList savedColorScalesList = settings.childKeys();
	bool ok;
	QString text = QInputDialog::getText(this, tr("Color scale saving"),
			tr("Enter a name for this color scale : "), QLineEdit::Normal,
			"unnamed", &ok);
	if (ok && !text.isEmpty()) {
		if (savedColorScalesList.contains(text)) {
			QString question = "There is already a color scale saved under the name " + text + ". Do you want to owerwrite it ?";
			if (QMessageBox::question (this, "Color scale saving", question, QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) != QMessageBox::Yes) {
				return;
			}
		}
		QList<QVariant> colorsVector;
		for (int i = 0; i < colorsTable->rowCount(); ++i) {
			colorsVector.push_back(QVariant(colorsTable->item(i, 0)->backgroundColor()));
		}
		settings.setValue(text, colorsVector);
		QString gradientId = text + "_gradient?";
		settings.setValue(gradientId, gradientCB->isChecked());
	}
	settings.endGroup();
	loadUserSavedColorScales();
}

void ColorScaleConfigDialog::deleteSavedColorScale() {
	if (savedColorScalesList->count() > 0 && savedColorScalesList->currentItem()) {
		QString savedColorScaleId = savedColorScalesList->currentItem()->text();
		if (QMessageBox::question (this, "Color scale deleting", "Delete saved color scale " + savedColorScaleId + " ?", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) != QMessageBox::Yes) {
			return;
		}
		QSettings settings("TulipSoftware","Tulip");
		settings.beginGroup("ColorScales");
		settings.remove(savedColorScaleId);
		settings.remove(savedColorScaleId+"_gradient?");
		settings.endGroup();
		loadUserSavedColorScales();
	}
}

void ColorScaleConfigDialog::loadUserSavedColorScales() {
	savedColorScalesList->clear();
	QSettings settings("TulipSoftware","Tulip");
	settings.beginGroup("ColorScales");
	QStringList savedColorScalesIdList = settings.childKeys();
	for (int i = 0 ; i < savedColorScalesIdList.size() ; ++i) {
		if (!savedColorScalesIdList.at(i).contains("_gradient?"))
			savedColorScalesList->addItem(savedColorScalesIdList.at(i));
	}
	settings.endGroup();
}

void ColorScaleConfigDialog::resizeEvent(QResizeEvent *) {
	displaySavedGradientPreview();
	displayUserGradientPreview();
	colorsTable->setColumnWidth(0, colorsTable->width());
}

void ColorScaleConfigDialog::showEvent(QShowEvent *) {
	displaySavedGradientPreview();
	displayUserGradientPreview();
	colorsTable->setColumnWidth(0, colorsTable->width());
}

void ColorScaleConfigDialog::reeditSaveColorScale(QListWidgetItem *savedColorScaleItem) {
	QString savedColorScaleId = savedColorScaleItem->text();
	QSettings settings("TulipSoftware","Tulip");
	settings.beginGroup("ColorScales");
	QList<QVariant> colorsListv = settings.value(savedColorScaleId).toList();
	QString gradientScaleId = savedColorScaleId + "_gradient?";
	bool gradient = settings.value(gradientScaleId).toBool();
	settings.endGroup();
	vector<Color> colorsList;
	for (int i = 0 ; i < colorsListv.size() ; ++i) {
		QColor color = colorsListv.at(i).value<QColor>();
		colorsList.push_back(Color(color.red(), color.green(), color.blue(), color.alpha()));
	}
	reverse(colorsList.begin(), colorsList.end());
	colorScale->setColorScale(colorsList, gradient);
	setColorScale(colorScale);
	tabWidget->setCurrentIndex(0);
}

void ColorScaleConfigDialog::setColorScale(ColorScale *colorScale) {
	this->colorScale = colorScale;

	disconnect(nbColors, SIGNAL(valueChanged(int)), this, SLOT(nbColorsValueChanged(int)));

	colorsTable->clear();

	if (colorScale->colorScaleInitialized()) {
		//init dialog with colors in the color Scale
		std::map<float, tlp::Color> colorMap = colorScale->getColorMap();
		unsigned int row = 0;
		if (colorScale->isGradient()) {
			colorsTable->setRowCount(colorMap.size());
			nbColors->setValue(colorMap.size());
			gradientCB->setChecked(true);
			row = colorMap.size() - 1;
		} else {
			colorsTable->setRowCount(colorMap.size() / 2);
			nbColors->setValue(colorMap.size() / 2);
			gradientCB->setChecked(false);
			row = (colorMap.size() / 2) - 1;
		}

		for (std::map<float, tlp::Color>::iterator it = colorMap.begin(); it
		!= colorMap.end();) {
			QTableWidgetItem *item = new QTableWidgetItem();
			item->setFlags(Qt::ItemIsEnabled);
			item->setBackgroundColor(QColor(it->second.getR(), it->second.getG(),
					it->second.getB(), it->second.getA()));
			colorsTable->setItem(row, 0, item);
			--row;
			if (colorScale->isGradient()) {
				++it;
			} else {
				++it;
				++it;
			}
		}

	} else {
		//init dialog with default colors
		colorsTable->setRowCount(5);
		QTableWidgetItem *item1 = new QTableWidgetItem();
		item1->setBackgroundColor(QColor(0, 255, 0, 40));
		item1->setFlags(Qt::ItemIsEnabled);
		QTableWidgetItem *item2 = new QTableWidgetItem();
		item2->setBackgroundColor(QColor(0, 0, 255, 40));
		item2->setFlags(Qt::ItemIsEnabled);
		QTableWidgetItem *item3 = new QTableWidgetItem();
		item3->setBackgroundColor(QColor(255, 255, 0, 40));
		item3->setFlags(Qt::ItemIsEnabled);
		QTableWidgetItem *item4 = new QTableWidgetItem();
		item4->setBackgroundColor(QColor(255, 0, 0, 40));
		item4->setFlags(Qt::ItemIsEnabled);
		QTableWidgetItem *item5 = new QTableWidgetItem();
		item5->setBackgroundColor(QColor(85, 0, 0, 40));
		item5->setFlags(Qt::ItemIsEnabled);
		colorsTable->setItem(0, 0, item1);
		colorsTable->setItem(1, 0, item2);
		colorsTable->setItem(2, 0, item3);
		colorsTable->setItem(3, 0, item4);
		colorsTable->setItem(4, 0, item5);
		nbColors->setValue(5);
		gradientCB->setChecked(true);
	}
	connect(nbColors, SIGNAL(valueChanged(int)), this, SLOT(nbColorsValueChanged(int)));
}

}
