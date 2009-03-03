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

#include <QtCore/QDir>
#include <QtGui/QFileDialog>
#include <QtGui/QColorDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLinearGradient>
#include <QtGui/QPainter>

#include <vector>

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
  connect(predefinedGradientsList, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
  this, SLOT(displayImageGradientPreview()));
  connect(predefinedColorScaleRb, SIGNAL(toggled(bool)), this, SLOT(predefinedColorScaleRbToggled(bool)));
  connect(userColorScaleImageRb, SIGNAL(toggled(bool)), this, SLOT(userColorScaleImageRbToggled(bool)));
  connect(browseButton, SIGNAL(clicked()), this, SLOT(pressButtonBrowse()));
  connect(nbColors, SIGNAL(valueChanged(int)), this, SLOT(nbColorsValueChanged(int)));
  connect(colorsTable, SIGNAL(itemDoubleClicked (QTableWidgetItem *)), this, SLOT(colorTableItemDoubleClicked(QTableWidgetItem *)));
  connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(displayImageGradientPreview()));
  connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(displayUserGradientPreview()));
  connect(gradientCB, SIGNAL(clicked()), this, SLOT(displayUserGradientPreview()));

  setColorScale(colorScale);
}

void ColorScaleConfigDialog::setPredefinedGradientsDirectory(
    const std::string &gradientsDir) {
  gradientsImageDirectory = gradientsDir;
  QDir qdir(gradientsDir.c_str());
  QStringList gradientsImageList = qdir.entryList(QDir::Files
      | QDir::NoDotAndDotDot);
  for (int i = 0; i < gradientsImageList.size(); ++i) {
    predefinedGradientsList->addItem(gradientsImageList.at(i));
  }
  if (gradientsImageList.size() > 0) {
    predefinedGradientsList->setCurrentRow(0);
  }
}

void ColorScaleConfigDialog::accept() {
  if (tabWidget->currentIndex() == 1) {
    if (predefinedColorScaleRb->isChecked()) {
      if (predefinedGradientsList->count() > 0) {
        setColorScaleFromImage(QString(gradientsImageDirectory.c_str())
            + predefinedGradientsList->currentItem()->text());
      }
    } else if (userGradientFile->text() != "") {
      setColorScaleFromImage(userGradientFile->text());
    }
  } else {
    vector<Color> colors;
    for (int i = 0; i < colorsTable->rowCount(); ++i) {
      QColor itemColor = colorsTable->item(i, 0)->backgroundColor();
      colors.push_back(Color(itemColor.red(), itemColor.green(),
          itemColor.blue(), itemColor.alpha()));
    }
    std::reverse(colors.begin(), colors.end());
    colorScale->setColorScale(colors, gradientCB->isChecked());
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
  displayImageGradientPreview();
}

void ColorScaleConfigDialog::displayImageGradientPreview() {
  QString gradientFilePath;
  if (predefinedColorScaleRb->isChecked()) {
    if (predefinedGradientsList->count() > 0) {
      gradientFilePath = QString(gradientsImageDirectory.c_str())
          + predefinedGradientsList->currentItem()->text();
    }
  } else {
    gradientFilePath = userGradientFile->text();
  }
  if (gradientFilePath != "") {
    QPixmap gradientImage(gradientFilePath);
    imageGradientPreview->setPixmap(gradientImage.scaled(
        imageGradientPreview->width(), imageGradientPreview->height()));
  }
}

void ColorScaleConfigDialog::displayUserGradientPreview() {

  vector<QColor> colorsVector;
  vector<QColor>::iterator it;
  for (int i = 0; i < colorsTable->rowCount(); ++i) {
    colorsVector.push_back(colorsTable->item(i, 0)->backgroundColor());
  }

  QPixmap pixmap(userGradientPreview->width(), userGradientPreview->height());
  pixmap.fill(Qt::transparent);
  QPainter painter;
  painter.begin(&pixmap);
  if (gradientCB->isChecked()) {
    QLinearGradient qLinearGradient(userGradientPreview->width() / 2, 0,
        userGradientPreview->width() / 2, userGradientPreview->height() - 1);
    float increment = 1. / (colorsVector.size() - 1);
    float relPos = 0;
    for (it = colorsVector.begin(); it != colorsVector.end(); ++it) {
      qLinearGradient.setColorAt(relPos, *it);
      relPos += increment;
    }
    painter.fillRect(0, 0, userGradientPreview->width(),
        userGradientPreview->height(), qLinearGradient);
  } else {
    float rectHeight = userGradientPreview->height() / colorsVector.size();
    for (unsigned int i = 0; i < colorsVector.size(); ++i) {
      painter.fillRect(0, i * rectHeight, userGradientPreview->width(), (i + 1)
          * rectHeight, QBrush(colorsVector[i]));
    }
  }
  painter.end();
  userGradientPreview->setPixmap(pixmap.scaled(userGradientPreview->width(),
      userGradientPreview->height()));
}

void ColorScaleConfigDialog::predefinedColorScaleRbToggled(bool toggled) {
  if (toggled) {
    predefinedGradientsList->setEnabled(true);
    userGradientFile->setEnabled(false);
    browseButton->setEnabled(false);
    displayImageGradientPreview();
  }
}

void ColorScaleConfigDialog::userColorScaleImageRbToggled(bool toggled) {
  if (toggled) {
    predefinedGradientsList->setEnabled(false);
    userGradientFile->setEnabled(true);
    browseButton->setEnabled(true);
    displayImageGradientPreview();
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
  bool ok = true;
  QRgb newColor = QColorDialog::getRgba(qRgba(itemBgColor.red(),
      itemBgColor.green(), itemBgColor.blue(), itemBgColor.alpha()), &ok, this);
  item->setBackgroundColor(QColor(qRed(newColor), qGreen(newColor), qBlue(
      newColor), qAlpha(newColor)));
  displayUserGradientPreview();
}

void ColorScaleConfigDialog::resizeEvent(QResizeEvent * event) {
  displayImageGradientPreview();
  displayUserGradientPreview();
  colorsTable->setColumnWidth(0, colorsTable->width());
}

void ColorScaleConfigDialog::showEvent(QShowEvent * event) {
  displayImageGradientPreview();
  displayUserGradientPreview();
  colorsTable->setColumnWidth(0, colorsTable->width());
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
    colorsTable->setRowCount(2);
    QTableWidgetItem *item1 = new QTableWidgetItem();
    item1->setBackgroundColor(QColor(255, 255, 0, 255));
    item1->setFlags(Qt::ItemIsEnabled);
    QTableWidgetItem *item2 = new QTableWidgetItem();
    item2->setBackgroundColor(QColor(0, 0, 255, 255));
    item2->setFlags(Qt::ItemIsEnabled);
    colorsTable->setItem(0, 0, item1);
    colorsTable->setItem(1, 0, item2);
    nbColors->setValue(2);
    gradientCB->setChecked(true);
  }
  connect(nbColors, SIGNAL(valueChanged(int)), this, SLOT(nbColorsValueChanged(int)));
}

}
