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

#include <QFileDialog>
#include <QHeaderView>
#include <QLinearGradient>
#include <QPainter>
#include <QSettings>
#include <QMessageBox>
#include <QInputDialog>

#include <algorithm>
#include <vector>

#include <tulip/TlpQtTools.h>
#include <tulip/ColorScaleConfigDialog.h>
#include <tulip/TlpTools.h>

#include "ui_ColorScaleConfigDialog.h"

using namespace std;

namespace tlp {

map<QString, vector<Color> > ColorScaleConfigDialog::tulipImageColorScales;

ColorScaleConfigDialog::ColorScaleConfigDialog(const ColorScale& colorScale,
    QWidget *parent) : QDialog(parent), _ui(new Ui::ColorScaleDialog), colorScale(colorScale) {
  _ui->setupUi(this);
  _ui->colorsTable->setColumnWidth(0, _ui->colorsTable->width());
  _ui->colorsTable->horizontalHeader()->setHidden(true);
  QPalette palette;
  palette.setColor(QPalette::Window, Qt::white);
  _ui->savedGradientPreview->setPalette(palette);
  _ui->userGradientPreview->setPalette(palette);
  _ui->savedGradientPreview->setAutoFillBackground(true);
  _ui->userGradientPreview->setAutoFillBackground(true);
  connect(_ui->savedColorScalesList, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(displaySavedGradientPreview()));
  connect(_ui->savedColorScalesList, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this, SLOT(reeditSaveColorScale(QListWidgetItem *)));
  connect(_ui->nbColors, SIGNAL(valueChanged(int)), this, SLOT(nbColorsValueChanged(int)));
  connect(_ui->colorsTable, SIGNAL(itemDoubleClicked (QTableWidgetItem *)), this, SLOT(colorTableItemDoubleClicked(QTableWidgetItem *)));
  connect(_ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(displaySavedGradientPreview()));
  connect(_ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(displayUserGradientPreview()));
  connect(_ui->gradientCB, SIGNAL(clicked()), this, SLOT(displayUserGradientPreview()));
  connect(_ui->saveColorScaleButton, SIGNAL(clicked()), this, SLOT(saveCurrentColorScale()));
  connect(_ui->deleteColorScaleButton, SIGNAL(clicked()), this, SLOT(deleteSavedColorScale()));
  connect(_ui->importFromImgButton, SIGNAL(clicked()), this, SLOT(importColorScaleFromImageFile()));
  connect(_ui->invertColorScaleButton, SIGNAL(clicked()), this, SLOT(invertEditedColorScale()));

  if (tulipImageColorScales.empty()) {
    loadTulipImageColorScales();
  }

  loadUserSavedColorScales();
  setColorScale(colorScale);
}

ColorScaleConfigDialog::~ColorScaleConfigDialog() {
  delete _ui;
}

void ColorScaleConfigDialog::accept() {
  vector<Color> colors;
  bool gradient = true;

  if (_ui->tabWidget->currentIndex() == 1) {
    if (_ui->savedColorScalesList->count() > 0) {
      QString savedColorScaleId = _ui->savedColorScalesList->currentItem()->text();

      if (tulipImageColorScales.find(savedColorScaleId) != tulipImageColorScales.end()) {
        colors = tulipImageColorScales[savedColorScaleId];
      }
      else {
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

        std::reverse(colors.begin(), colors.end());
      }
    }
  }
  else {
    for (int i = 0; i < _ui->colorsTable->rowCount(); ++i) {
      QColor itemColor = _ui->colorsTable->item(i, 0)->backgroundColor();
      colors.push_back(Color(itemColor.red(), itemColor.green(),
                             itemColor.blue(), itemColor.alpha()));
    }

    std::reverse(colors.begin(), colors.end());
    gradient = _ui->gradientCB->isChecked();
  }

  if (!colors.empty()) {
    colorScale.setColorScale(colors, gradient);
  }

  QDialog::accept();
}

vector<Color> ColorScaleConfigDialog::getColorScaleFromImageFile(const QString &imageFilePath) {
  QImage gradientImage(imageFilePath);
  unsigned int imageHeight = gradientImage.height();

  unsigned int step = 1;

  if (imageHeight > 50)
    step = 10;

  vector<Color> colors;

  for (unsigned int i = 0; i < imageHeight; i += step) {
    QRgb pixelValue = gradientImage.pixel(0, i);
    colors.push_back(Color(qRed(pixelValue), qGreen(pixelValue), qBlue(
                             pixelValue), qAlpha(pixelValue)));
  }

  if (imageHeight % step != 0) {
    QRgb pixelValue = gradientImage.pixel(0, imageHeight - 1);
    colors.push_back(Color(qRed(pixelValue), qGreen(pixelValue), qBlue(
                             pixelValue), qAlpha(pixelValue)));
  }

  std::reverse(colors.begin(), colors.end());
  return colors;
}

ColorScale ColorScaleConfigDialog::getColorScaleFromImageFile(const std::string &imageFilePath, bool gradient) {
  return ColorScale(getColorScaleFromImageFile(QString::fromUtf8(imageFilePath.c_str())), gradient);
}

void ColorScaleConfigDialog::loadTulipImageColorScales() {
  QFileInfo colorscaleDirectory(tlpStringToQString(TulipBitmapDir)+QString("colorscales"));

  if(colorscaleDirectory.exists() && colorscaleDirectory.isDir()) {
    QDir dir(colorscaleDirectory.absoluteFilePath());
    dir.setFilter(QDir::Files | QDir::NoDot | QDir::NoDotDot);
    QFileInfoList list = dir.entryInfoList();

    for (int i = 0; i < list.size(); ++i) {
      QFileInfo fileInfo = list.at(i);
      tulipImageColorScales[fileInfo.fileName()] = getColorScaleFromImageFile(fileInfo.absoluteFilePath());
    }
  }
}

void ColorScaleConfigDialog::importColorScaleFromImageFile() {
  QString parentDirectory="./";
  QString imageFilePath = QFileDialog::getOpenFileName(this, tr("Open Image File"),
                          parentDirectory, tr("Image Files (*.png *.jpg *.bmp)"));

  if (imageFilePath.isEmpty())
    return;

  vector<Color> colorsList = getColorScaleFromImageFile(imageFilePath);

  if (!colorsList.empty()) {
    ColorScale scaleTmp(colorsList, true);
    setColorScale(scaleTmp);
    displayUserGradientPreview();
  }
}

void ColorScaleConfigDialog::pressButtonBrowse() {

  displaySavedGradientPreview();
}

void ColorScaleConfigDialog::displaySavedGradientPreview() {
  if (_ui->savedColorScalesList->count() > 0 && _ui->savedColorScalesList->currentItem()) {
    QList<QColor> colorsList;
    QString savedColorScaleId = _ui->savedColorScalesList->currentItem()->text();
    bool gradient = true;

    if (tulipImageColorScales.find(savedColorScaleId) != tulipImageColorScales.end()) {
      vector<Color> colors = tulipImageColorScales[savedColorScaleId];
      std::reverse(colors.begin(), colors.end());

      for (size_t i = 0 ; i < colors.size() ; ++i) {
        colorsList.push_back(QColor(colors[i][0], colors[i][1], colors[i][2], colors[i][3]));
      }
    }
    else {
      QSettings settings("TulipSoftware","Tulip");
      settings.beginGroup("ColorScales");
      QList<QVariant> colorsListv = settings.value(savedColorScaleId).toList();
      QString gradientScaleId = savedColorScaleId + "_gradient?";
      gradient = settings.value(gradientScaleId).toBool();
      settings.endGroup();

      for (int i = 0 ; i < colorsListv.size() ; ++i) {
        colorsList.push_back(colorsListv.at(i).value<QColor>());
      }
    }

    displayGradientPreview(colorsList, gradient, _ui->savedGradientPreview);
  }
}


void ColorScaleConfigDialog::displayUserGradientPreview() {

  QList<QColor> colorsVector;

  for (int i = 0; i < _ui->colorsTable->rowCount(); ++i) {
    colorsVector.push_back(_ui->colorsTable->item(i, 0)->backgroundColor());
  }

  displayGradientPreview(colorsVector, _ui->gradientCB->isChecked(), _ui->userGradientPreview);
}


void ColorScaleConfigDialog::invertEditedColorScale() {
  QList<QTableWidgetItem *> itemsList;
  int nbItems = _ui->colorsTable->rowCount();

  for (int i = 0; i < _ui->colorsTable->rowCount(); ++i) {
    itemsList.push_front(_ui->colorsTable->takeItem(i, 0));
  }

  for (int i = 0 ; i < nbItems ; ++i) {
    _ui->colorsTable->setItem(i, 0, itemsList.at(i));
  }

  displayUserGradientPreview();
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
  }
  else {
    float rectHeight = displayLabel->height() / colorsVector.size();

    for (int i = 0; i < colorsVector.size(); ++i) {
      painter.fillRect(0, i * rectHeight, displayLabel->width(), (i + 1)
                       * rectHeight, QBrush(colorsVector.at(i)));
    }
  }

  painter.end();
  displayLabel->setPixmap(pixmap.scaled(displayLabel->width(), displayLabel->height()));
}

void ColorScaleConfigDialog::nbColorsValueChanged(int value) {
  int lastCount = _ui->colorsTable->rowCount();
  _ui->colorsTable->setRowCount(value);

  if (lastCount < value) {
    for(int j = 0; j <= value - lastCount; ++j) {
      QTableWidgetItem *item = new QTableWidgetItem();
      item->setBackgroundColor(QColor(255, 255, 255, 255));
      item->setFlags(Qt::ItemIsEnabled);
      _ui->colorsTable->setItem(0, lastCount + j - 1, item);
    }
  }

  displayUserGradientPreview();
}

void ColorScaleConfigDialog::colorTableItemDoubleClicked(QTableWidgetItem *item) {
  QColor itemBgColor = item->backgroundColor();
  QColor newColor;

  if(getColorDialog(itemBgColor, this, "Select Color",newColor)) {
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

    for (int i = 0; i < _ui->colorsTable->rowCount(); ++i) {
      colorsVector.push_back(QVariant(_ui->colorsTable->item(i, 0)->backgroundColor()));
    }

    settings.setValue(text, colorsVector);
    QString gradientId = text + "_gradient?";
    settings.setValue(gradientId, _ui->gradientCB->isChecked());
  }

  settings.endGroup();
  loadUserSavedColorScales();
}

void ColorScaleConfigDialog::deleteSavedColorScale() {
  if (_ui->savedColorScalesList->count() > 0 && _ui->savedColorScalesList->currentItem()) {
    QString savedColorScaleId = _ui->savedColorScalesList->currentItem()->text();

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
  _ui->savedColorScalesList->clear();

  map<QString, vector<Color> >::const_iterator it = tulipImageColorScales.begin();

  for ( ; it != tulipImageColorScales.end(); ++it) {
    _ui->savedColorScalesList->addItem(it->first);
  }

  QSettings settings("TulipSoftware","Tulip");
  settings.beginGroup("ColorScales");
  QStringList savedColorScalesIdList = settings.childKeys();

  for (int i = 0 ; i < savedColorScalesIdList.size() ; ++i) {
    if (!savedColorScalesIdList.at(i).contains("_gradient?"))
      _ui->savedColorScalesList->addItem(savedColorScalesIdList.at(i));
  }

  settings.endGroup();

}

void ColorScaleConfigDialog::resizeEvent(QResizeEvent *) {
  displaySavedGradientPreview();
  displayUserGradientPreview();
  _ui->colorsTable->setColumnWidth(0, _ui->colorsTable->width());
}

void ColorScaleConfigDialog::showEvent(QShowEvent *) {
  displaySavedGradientPreview();
  displayUserGradientPreview();
  _ui->colorsTable->setColumnWidth(0, _ui->colorsTable->width());
}

void ColorScaleConfigDialog::reeditSaveColorScale(QListWidgetItem *savedColorScaleItem) {
  QString savedColorScaleId = savedColorScaleItem->text();
  vector<Color> colorsList;
  bool gradient = true;

  if (tulipImageColorScales.find(savedColorScaleId) != tulipImageColorScales.end()) {
    colorsList = tulipImageColorScales[savedColorScaleId];
  }
  else {
    QSettings settings("TulipSoftware","Tulip");
    settings.beginGroup("ColorScales");
    QList<QVariant> colorsListv = settings.value(savedColorScaleId).toList();
    QString gradientScaleId = savedColorScaleId + "_gradient?";
    gradient = settings.value(gradientScaleId).toBool();
    settings.endGroup();

    for (int i = 0 ; i < colorsListv.size() ; ++i) {
      QColor color = colorsListv.at(i).value<QColor>();
      colorsList.push_back(Color(color.red(), color.green(), color.blue(), color.alpha()));
    }

    reverse(colorsList.begin(), colorsList.end());
  }

  ColorScale scaleTmp(colorsList, gradient);
  setColorScale(scaleTmp);
  _ui->tabWidget->setCurrentIndex(0);
}

void ColorScaleConfigDialog::setColorScale(const ColorScale &colorScale) {
  disconnect(_ui->nbColors, SIGNAL(valueChanged(int)), this, SLOT(nbColorsValueChanged(int)));

  _ui->colorsTable->clear();

  if (colorScale.colorScaleInitialized()) {
    //init dialog with colors in the color Scale
    std::map<float, tlp::Color> colorMap = colorScale.getColorMap();
    unsigned int row = 0;

    if (colorScale.isGradient()) {
      _ui->colorsTable->setRowCount(colorMap.size());
      _ui->nbColors->setValue(colorMap.size());
      _ui->gradientCB->setChecked(true);
      row = colorMap.size() - 1;
    }
    else {
      _ui->colorsTable->setRowCount(colorMap.size() / 2);
      _ui->nbColors->setValue(colorMap.size() / 2);
      _ui->gradientCB->setChecked(false);
      row = (colorMap.size() / 2) - 1;
    }

    for (std::map<float, tlp::Color>::iterator it = colorMap.begin(); it
         != colorMap.end();) {
      QTableWidgetItem *item = new QTableWidgetItem();
      item->setFlags(Qt::ItemIsEnabled);
      item->setBackgroundColor(QColor(it->second.getR(), it->second.getG(),
                                      it->second.getB(), it->second.getA()));
      _ui->colorsTable->setItem(row, 0, item);
      --row;

      if (colorScale.isGradient()) {
        ++it;
      }
      else {
        ++it;
        ++it;
      }
    }

  }
  else {
    //init dialog with default colors
    _ui->colorsTable->setRowCount(5);
    QTableWidgetItem *item1 = new QTableWidgetItem();
    item1->setBackgroundColor(QColor(229, 40, 0, 200));
    item1->setFlags(Qt::ItemIsEnabled);
    QTableWidgetItem *item2 = new QTableWidgetItem();
    item2->setBackgroundColor(QColor(255, 170, 0, 200));
    item2->setFlags(Qt::ItemIsEnabled);
    QTableWidgetItem *item3 = new QTableWidgetItem();
    item3->setBackgroundColor(QColor(255, 255, 127, 200));
    item3->setFlags(Qt::ItemIsEnabled);
    QTableWidgetItem *item4 = new QTableWidgetItem();
    item4->setBackgroundColor(QColor(156, 161, 255, 200));
    item4->setFlags(Qt::ItemIsEnabled);
    QTableWidgetItem *item5 = new QTableWidgetItem();
    item5->setBackgroundColor(QColor(75, 75, 255, 200));
    item5->setFlags(Qt::ItemIsEnabled);
    _ui->colorsTable->setItem(0, 0, item1);
    _ui->colorsTable->setItem(1, 0, item2);
    _ui->colorsTable->setItem(2, 0, item3);
    _ui->colorsTable->setItem(3, 0, item4);
    _ui->colorsTable->setItem(4, 0, item5);
    _ui->nbColors->setValue(5);
    _ui->gradientCB->setChecked(true);

    accept();
  }

  connect(_ui->nbColors, SIGNAL(valueChanged(int)), this, SLOT(nbColorsValueChanged(int)));
}

const ColorScale& ColorScaleConfigDialog::getColorScale() const {
  return colorScale;
}

}
