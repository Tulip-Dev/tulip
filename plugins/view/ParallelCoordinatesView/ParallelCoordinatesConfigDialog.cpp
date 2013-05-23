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

#include "ParallelCoordinatesConfigDialog.h"
#include "ParallelTools.h"
#include "ui_ParallelCoordinatesConfigDialog.h"
#include "ParallelCoordinatesGraphProxy.h"

#include <QFileDialog>
#include <QColorDialog>

#include <algorithm>

using namespace std;

namespace tlp {

ParallelCoordinatesConfigDialog::ParallelCoordinatesConfigDialog(ParallelCoordinatesGraphProxy *data, QWidget *parent) : QDialog(parent), graphProxy(data),_ui(new Ui::ConfigDialog) {
  _ui->setupUi(this);

  propertyTypesFilter.push_back("double");
  propertyTypesFilter.push_back("int");
  propertyTypesFilter.push_back("string");
  _ui->graphPropertiesSelectionWidget->setWidgetParameters(graphProxy, propertyTypesFilter);

  // if number of data to plot is consequent, don't draw points on axis
  // by default to speed up rendering
  if (graphProxy->getDataCount() > PROGRESS_BAR_DISPLAY_NB_DATA_THRESHOLD) {
    setDrawPointOnAxis(false);
  }

  connect(_ui->browseButton,SIGNAL(clicked()),this,SLOT(pressButtonBrowse()));
  connect(_ui->userTexture, SIGNAL(toggled(bool)), this, SLOT(userTextureRbToggled(bool)));
  connect(_ui->minAxisPointSize, SIGNAL(valueChanged(int)), this, SLOT(minAxisPointSizeValueChanged(int)));
  connect(_ui->maxAxisPointSize, SIGNAL(valueChanged(int)), this, SLOT(maxAxisPointSizeValueChanged(int)));
  connect(_ui->bgColorButton,SIGNAL(clicked()),this,SLOT(pressColorButton()));
}

ParallelCoordinatesConfigDialog::~ParallelCoordinatesConfigDialog(){
    delete _ui;
}

void ParallelCoordinatesConfigDialog::updateSelectedProperties() {
  Iterator<string> *properties = graphProxy->getProperties();
  selectedProperties = graphProxy->getSelectedProperties();
  vector<string> stringList;
  vector<string>::iterator it;
  string propertyName;
  _ui->graphPropertiesSelectionWidget->clearLists();

  for (it = selectedProperties.begin() ; it != selectedProperties.end() ; ++it) {
    stringList.push_back(*it);
  }

 _ui-> graphPropertiesSelectionWidget->setOutputPropertiesList(stringList);

  stringList.clear();

  while (properties->hasNext()) {
    propertyName = properties->next();

    if (std::find(selectedProperties.begin(), selectedProperties.end(), propertyName) == selectedProperties.end()) {
      stringList.push_back(propertyName);
    }
  }

  delete properties;

  _ui->graphPropertiesSelectionWidget->setInputPropertiesList(stringList);

}

vector<string> ParallelCoordinatesConfigDialog::getSelectedProperties()  const {
  return selectedProperties;
}

void ParallelCoordinatesConfigDialog::setGraphProxy(ParallelCoordinatesGraphProxy *graphProx) {
  graphProxy = graphProx;
  _ui->graphPropertiesSelectionWidget->setWidgetParameters(graphProxy, propertyTypesFilter);
}

void ParallelCoordinatesConfigDialog::accept() {
  selectedProperties = _ui->graphPropertiesSelectionWidget->getSelectedProperties();
  graphProxy->setSelectedProperties(selectedProperties);
  graphProxy->setDataLocation(getDataLocation());
  QDialog::accept();
}

void ParallelCoordinatesConfigDialog::reject() {
  restoreBackupConfiguration();
  QDialog::reject();
}

void ParallelCoordinatesConfigDialog::pressButtonBrowse() {
  QString fileName(QFileDialog::getOpenFileName(this, tr("Open Texture File"), "./", tr("Image Files (*.png *.jpg *.bmp)")));
  _ui->userTextureFile->setText(fileName);
}

ElementType ParallelCoordinatesConfigDialog::getDataLocation() const {
  if (_ui->nodesButton->isChecked()) {
    return NODE;
  }
  else {
    return EDGE;
  }
}

void ParallelCoordinatesConfigDialog::setDataLocation(ElementType location) {
  if (location == NODE) {
    _ui->edgesButton->setChecked(false);
    _ui->nodesButton->setChecked(true);
  }
  else {
    _ui->edgesButton->setChecked(true);
    _ui->nodesButton->setChecked(false);
  }
}

unsigned int ParallelCoordinatesConfigDialog::getAxisHeight() const {
  return _ui->axisHeight->value();
}

void ParallelCoordinatesConfigDialog::setAxisHeight(const unsigned int aHeight) {
  _ui->axisHeight->setValue(aHeight);
}

bool ParallelCoordinatesConfigDialog::drawPointOnAxis() const {
  return _ui->gBoxAxisPoints->isChecked();
}

string ParallelCoordinatesConfigDialog::getLinesTextureFilename() const {
  if (_ui->gBoxLineTexture->isChecked()) {
    if (_ui->defaultTexture->isChecked()) {
      return string(TulipBitmapDir + DEFAULT_TEXTURE_FILE);
    }
    else {
      return _ui->userTextureFile->text().toUtf8().data();
    }
  }
  else {
    return "";
  }
}

void ParallelCoordinatesConfigDialog::setLinesTextureFilename(const std::string &linesTextureFileName) {
  if (linesTextureFileName != "") {
    _ui->gBoxLineTexture->setChecked(true);

    if (linesTextureFileName == string(TulipBitmapDir + DEFAULT_TEXTURE_FILE)) {
      _ui->defaultTexture->setChecked(true);
    }
    else {
     _ui-> userTexture->setChecked(true);
      _ui->userTextureFile->setText(QString::fromUtf8(linesTextureFileName.c_str()));
    }
  }
  else {
    _ui->gBoxLineTexture->setChecked(false);
  }
}

void ParallelCoordinatesConfigDialog::showEvent (QShowEvent * event) {
  updateSelectedProperties();
  backupConfiguration();
  _ui->tabWidget->setCurrentIndex(0);
  QWidget::showEvent(event);
}

Size ParallelCoordinatesConfigDialog::getAxisPointMinSize() const {
  float pointSize = _ui->minAxisPointSize->text().toFloat();
  return Size(pointSize, pointSize, pointSize);
}

Size ParallelCoordinatesConfigDialog::getAxisPointMaxSize() const {
  float pointSize = _ui->maxAxisPointSize->text().toFloat();
  return Size(pointSize, pointSize, pointSize);
}

void ParallelCoordinatesConfigDialog::setAxisPointMinSize(const unsigned int axisPointMinSize) {
  _ui->minAxisPointSize->setValue(axisPointMinSize);
}

void ParallelCoordinatesConfigDialog::setAxisPointMaxSize(const unsigned int axisPointMaxSize) {
  _ui->maxAxisPointSize->setValue(axisPointMaxSize);
}

void ParallelCoordinatesConfigDialog::userTextureRbToggled(bool checked) {
  if (checked) {
    _ui->userTextureFile->setEnabled(true);
    _ui->browseButton->setEnabled(true);
  }
  else {
    _ui->userTextureFile->setEnabled(false);
    _ui->browseButton->setEnabled(false);
  }
}

void ParallelCoordinatesConfigDialog::minAxisPointSizeValueChanged(int newValue) {
  if (_ui->maxAxisPointSize->value() < newValue) {
    _ui->maxAxisPointSize->setValue(newValue + 1);
  }
}

void ParallelCoordinatesConfigDialog::maxAxisPointSizeValueChanged(int newValue) {
  if (_ui->minAxisPointSize->value() > newValue) {
    _ui->minAxisPointSize->setValue(newValue - 1);
  }
}

void ParallelCoordinatesConfigDialog::setLinesColorAlphaValue(unsigned int value) {
  _ui->viewColorAlphaValue->setValue(value);
}

unsigned int ParallelCoordinatesConfigDialog::getLinesColorAlphaValue() const {
  if (_ui->viewColorAlphaRb->isChecked()) {
    return 300;
  }
  else {
    return (unsigned int) _ui->viewColorAlphaValue->value();
  }
}

Color ParallelCoordinatesConfigDialog::getBackgroundColor() const {
  QColor bgColor(_ui->bgColorButton->palette().color(QPalette::Button));
  return Color(bgColor.red(), bgColor.green(), bgColor.blue());
}

void ParallelCoordinatesConfigDialog::setBackgroundColor(const Color &color) {
  QPalette palette;
  palette.setColor(QPalette::Button, QColor(color.getR(), color.getG(), color.getB(), color.getA()));
  _ui->bgColorButton->setPalette(palette);
}

void ParallelCoordinatesConfigDialog::pressColorButton() {
  QColor newColor(QColorDialog::getColor(_ui->bgColorButton->palette().color(QPalette::Button), this));

  if (newColor.isValid()) {
    QPalette palette;
    palette.setColor(QPalette::Button, newColor);
   _ui-> bgColorButton->setPalette(palette);
  }
}

void ParallelCoordinatesConfigDialog::backupConfiguration() {
  if (_ui->nodesButton->isChecked()) {
    dataLocationBak = NODE;
  }
  else {
    dataLocationBak = EDGE;
  }

  bgColorBak = getBackgroundColor();
  axisHeightBak = _ui->axisHeight->value();
  drawPointOnAxisBak = _ui->gBoxAxisPoints->isChecked();
  minAxisPointSizeBak = _ui->minAxisPointSize->value();
  maxAxisPointSizeBak = _ui->maxAxisPointSize->value();
  linesColorAlphaValueBak = _ui->viewColorAlphaValue->value();
  linesTextureBak = _ui->gBoxLineTexture->isChecked();
  userTextureBak = _ui->userTexture->isChecked();
  userTextureFileBak = _ui->userTextureFile->text().toUtf8().data();
  unhighlightedEltsColorsAlphaValueBak = _ui->nonHighlightedEltsAlphaValue->value();
}

void ParallelCoordinatesConfigDialog::restoreBackupConfiguration() {
 _ui->nodesButton->setChecked(dataLocationBak == NODE);
  _ui->edgesButton->setChecked(dataLocationBak == EDGE);

  QPalette palette;
  palette.setColor(QPalette::Button, QColor(bgColorBak.getR(), bgColorBak.getG(), bgColorBak.getB()));
  _ui->bgColorButton->setPalette(palette);

  _ui->axisHeight->setValue(axisHeightBak);
  _ui->gBoxAxisPoints->setChecked(drawPointOnAxisBak);
  _ui->minAxisPointSize->setValue(minAxisPointSizeBak);
  _ui->maxAxisPointSize->setValue(maxAxisPointSizeBak);
  _ui->viewColorAlphaValue->setValue(linesColorAlphaValueBak);
  _ui->gBoxLineTexture->setChecked(linesTextureBak);
  _ui->defaultTexture->setChecked(!userTextureBak);
  _ui->userTexture->setChecked(userTextureBak);
  _ui->userTextureFile->setText(QString::fromUtf8(userTextureFileBak.c_str()));
  _ui->nonHighlightedEltsAlphaValue->setValue(unhighlightedEltsColorsAlphaValueBak);
}

unsigned int ParallelCoordinatesConfigDialog::getSpaceBetweenAxis() const {
  return _ui->spaceBetweenAxis->value();
}

void ParallelCoordinatesConfigDialog::setSpaceBetweenAxis(const unsigned int spaceBetweenAxis) {
  _ui->spaceBetweenAxis->setValue(spaceBetweenAxis);
}

void ParallelCoordinatesConfigDialog::setDrawPointOnAxis(const bool drawPointOnAxis) {
  _ui->gBoxAxisPoints->setChecked(drawPointOnAxis);
}

unsigned int ParallelCoordinatesConfigDialog::getUnhighlightedEltsColorsAlphaValue() const {
  return (unsigned int) _ui->nonHighlightedEltsAlphaValue->value();
}

void ParallelCoordinatesConfigDialog::setUnhighlightedEltsColorsAlphaValue(const unsigned int alphaValue) {
  _ui->nonHighlightedEltsAlphaValue->setValue(alphaValue);
}

}
