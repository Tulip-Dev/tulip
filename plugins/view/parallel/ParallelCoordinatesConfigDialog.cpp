//-*-c++-*-
/*
 Author: Didier Bathily, Nicolas Bellino, Jonathan Dubois, Christelle Jolly, Antoine Lambert, Nicolas Sorraing

 Email : didier.bathily@etu.u-bordeaux1.fr, nicolas.bellino@etu.u-bordeaux1.fr, jonathan.dubois@etu.u-bordeaux1.fr, christelle.jolly@etu.u-bordeaux1.fr, antoine.lambert@etu.u-bordeaux1.fr, nicolas.sorraing@etu.u-bordeaux1.fr

 Last modification : 03/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#include "ParallelCoordinatesConfigDialog.h"
#include "ParallelTools.h"

#include <QtGui/QHBoxLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QVBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QFileDialog>
#include <QtGui/QPalette>
#include <QtGui/QColor>
#include <QtGui/QColorDialog>
#include <QtGui/QTabWidget>

#include <tulip/TlpTools.h>

#include <algorithm>

using namespace std;

namespace tlp {

  ParallelCoordinatesConfigDialog::ParallelCoordinatesConfigDialog(ParallelCoordinatesGraphProxy *data, QWidget *parent) : QDialog(parent), graphProxy(data) {
	  setupUi(this);

	  connect(addButton,SIGNAL(clicked()),this,SLOT(pressButtonAdd()));
	  connect(removeButton,SIGNAL(clicked()),this,SLOT(pressButtonRem()));

	  connect(upButton,SIGNAL(clicked()),this,SLOT(pressButtonUp()));
	  connect(downButton,SIGNAL(clicked()),this,SLOT(pressButtonDown()));

	  connect(browseButton,SIGNAL(clicked()),this,SLOT(pressButtonBrowse()));
	  connect(userTexture, SIGNAL(toggled(bool)), this, SLOT(userTextureRbToggled(bool)));

	  connect(minAxisPointSize, SIGNAL(valueChanged(int)), this, SLOT(minAxisPointSizeValueChanged(int)));
	  connect(maxAxisPointSize, SIGNAL(valueChanged(int)), this, SLOT(maxAxisPointSizeValueChanged(int)));

	  connect(bgColorButton,SIGNAL(clicked()),this,SLOT(pressColorButton()));

  }

  void ParallelCoordinatesConfigDialog::updateSelectedProperties() {
  	  vector<string> properties = graphProxy->getAllProperties();
  	  selectedProperties = graphProxy->getSelectedProperties();
  	  vector<string>::iterator it;
  	  string propertyName;

  	  items->clear();
  	  choices->clear();

  	  for (it = properties.begin() ; it != properties.end() ; ++it) {
  		  propertyName = *it;
  		  if (std::find(selectedProperties.begin(), selectedProperties.end(), propertyName) == selectedProperties.end()) {
  			  items->addItem(QString(propertyName.c_str()));
  		  }
  	  }

  	  for (it = selectedProperties.begin() ; it != selectedProperties.end() ; ++it) {
  		  choices->addItem(QString((*it).c_str()));
  	  }
    }

  vector<string> ParallelCoordinatesConfigDialog::getSelectedProperties()  const {
    return selectedProperties;
  }

  void ParallelCoordinatesConfigDialog::pressButtonAdd(){

    if (items->currentItem()!=NULL) {
      choices->addItemList(items->currentItem()->text());
      items->deleteItemList(items->currentItem());
    }
  }

  void ParallelCoordinatesConfigDialog::pressButtonRem(){

    if (choices->currentItem()!=NULL) {
      items->addItemList(choices->currentItem()->text());
      choices->deleteItemList(choices->currentItem());
    }
  }

  void ParallelCoordinatesConfigDialog::accept(){
    selectedProperties.clear();
    for(int i = 0; i < choices->count(); ++i) {
      selectedProperties.push_back(choices->item(i)->text().toStdString());
    }
    graphProxy->setSelectedProperties(selectedProperties);
    graphProxy->setDataLocation(getDataLocation());
    QDialog::accept();
  }

  void ParallelCoordinatesConfigDialog::reject(){
      restoreBackupConfiguration();
      QDialog::reject();
  }

  void ParallelCoordinatesConfigDialog::pressButtonUp() {
    if (choices->count() > 0) {
      int row = choices->currentRow();
      if ( row > 0) {
    	  QString s = choices->currentItem()->text();
    	  QString s2 = choices->item(row - 1)->text();
    	  choices->deleteItemList(choices->item(row - 1));
    	  choices->deleteItemList(choices->item(row - 1));
    	  choices->insertItem(row - 1, s2);
    	  choices->insertItem(row - 1, s);
    	  choices->setCurrentRow(row - 1);
      }
    }
  }

  void ParallelCoordinatesConfigDialog::pressButtonDown() {

    if (choices->count() > 0) {
      int row = choices->currentRow();
      if ( row < (choices->count() - 1)) {
    	  QString s = choices->currentItem()->text();
    	  QString s2 = choices->item(row + 1)->text();
    	  choices->deleteItemList(choices->item(row));
    	  choices->deleteItemList(choices->item(row));
    	  choices->insertItem(row, s);
    	  choices->insertItem(row, s2);
    	  choices->setCurrentRow(row + 1);
      }
    }
  }

  void ParallelCoordinatesConfigDialog::pressButtonBrowse() {
	  QString fileName = QFileDialog::getOpenFileName(this, tr("Open Texture File"), "./", tr("Image Files (*.png *.jpg *.bmp)"));
	  userTextureFile->setText(fileName);
  }

  const ElementType ParallelCoordinatesConfigDialog::getDataLocation() const {
    if (nodesButton->isChecked()) {
      return NODE;
    } else {
      return EDGE;
    }
  }

  void ParallelCoordinatesConfigDialog::setDataLocation(ElementType location) {
    if (location == NODE) {
      edgesButton->setChecked(false);
      nodesButton->setChecked(true);
    } else {
      edgesButton->setChecked(true);
      nodesButton->setChecked(false);
    }
  }

  unsigned int ParallelCoordinatesConfigDialog::getAxisHeight() const {
  	  return axisHeight->value();
  }

  void ParallelCoordinatesConfigDialog::setAxisHeight(const unsigned int aHeight) {
	  axisHeight->setValue(aHeight);
  }

  bool ParallelCoordinatesConfigDialog::drawPointOnAxis() const {
	  return gBoxAxisPoints->isChecked();
  }

  string ParallelCoordinatesConfigDialog::getLinesTextureFilename() const {
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

  void ParallelCoordinatesConfigDialog::setLinesTextureFilename(const std::string &linesTextureFileName) {
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

  void ParallelCoordinatesConfigDialog::showEvent (QShowEvent * event) {
	  updateSelectedProperties();
	  backupConfiguration();

	  // if number of data to plot is consequent, don't draw points on axis
	  // by default to speed up rendering
	  if (graphProxy->getDataCount() > PROGRESS_BAR_DISPLAY_NB_DATA_THRESHOLD) {
		  setDrawPointOnAxis(false);
	  }
	  tabWidget->setCurrentIndex(0);
	  QWidget::showEvent(event);
  }

  Size ParallelCoordinatesConfigDialog::getAxisPointMinSize() const {
	  float pointSize = minAxisPointSize->text().toFloat();
	  return Size(pointSize, pointSize, pointSize);
  }

  Size ParallelCoordinatesConfigDialog::getAxisPointMaxSize() const {
	  float pointSize = maxAxisPointSize->text().toFloat();
	  return Size(pointSize, pointSize, pointSize);
  }

  void ParallelCoordinatesConfigDialog::setAxisPointMinSize(const unsigned int axisPointMinSize) {
		  minAxisPointSize->setValue(axisPointMinSize);
  }

  void ParallelCoordinatesConfigDialog::setAxisPointMaxSize(const unsigned int axisPointMaxSize) {
	  maxAxisPointSize->setValue(axisPointMaxSize);
  }

  void ParallelCoordinatesConfigDialog::userTextureRbToggled(bool checked) {
	  if (checked) {
		  userTextureFile->setEnabled(true);
		  browseButton->setEnabled(true);
	  } else {
		  userTextureFile->setEnabled(false);
		  browseButton->setEnabled(false);
	  }
  }

  void ParallelCoordinatesConfigDialog::minAxisPointSizeValueChanged(int newValue) {
	  if (maxAxisPointSize->value() < newValue) {
		  maxAxisPointSize->setValue(newValue + 1);
	  }
  }

  void ParallelCoordinatesConfigDialog::maxAxisPointSizeValueChanged(int newValue) {
	  if (minAxisPointSize->value() > newValue) {
		  minAxisPointSize->setValue(newValue - 1);
	  }
  }

  void ParallelCoordinatesConfigDialog::setLinesColorAlphaValue(unsigned int value) {
	  linesColorAlphaValue->setValue(value);
  }

  unsigned int ParallelCoordinatesConfigDialog::getLinesColorAlphaValue() const {
	  return (unsigned int) linesColorAlphaValue->value();
  }

  Color ParallelCoordinatesConfigDialog::getBackgroundColor() const {
	  QColor bgColor = bgColorButton->palette().color(QPalette::Button);
	  return Color(bgColor.red(), bgColor.green(), bgColor.blue());
  }

  void ParallelCoordinatesConfigDialog::setBackgroundColor(const Color &color) {
	  QPalette palette;
	  palette.setColor(QPalette::Button, QColor(color.getR(), color.getG(), color.getB(), color.getA()));
	  bgColorButton->setPalette(palette);
  }

  void ParallelCoordinatesConfigDialog::pressColorButton() {
	  QColor newColor = QColorDialog::getColor(bgColorButton->palette().color(QPalette::Button), this);
	  if (newColor.isValid()) {
		  QPalette palette;
		  palette.setColor(QPalette::Button, newColor);
		  bgColorButton->setPalette(palette);
	  }
  }

  void ParallelCoordinatesConfigDialog::backupConfiguration() {
	  if (nodesButton->isChecked()) {
		  dataLocationBak = NODE;
	  } else {
		  dataLocationBak = EDGE;
	  }
	  bgColorBak = getBackgroundColor();
	  axisHeightBak = axisHeight->value();
	  drawPointOnAxisBak = gBoxAxisPoints->isChecked();
	  minAxisPointSizeBak = minAxisPointSize->value();
	  maxAxisPointSizeBak = maxAxisPointSize->value();
	  linesColorAlphaValueBak = linesColorAlphaValue->value();
	  linesTextureBak = gBoxLineTexture->isChecked();
	  userTextureBak = userTexture->isChecked();
	  userTextureFileBak = userTextureFile->text().toStdString();
  }

  void ParallelCoordinatesConfigDialog::restoreBackupConfiguration() {
	  nodesButton->setChecked(dataLocationBak == NODE);
	  edgesButton->setChecked(dataLocationBak == EDGE);

	  QPalette palette;
	  palette.setColor(QPalette::Button, QColor(bgColorBak.getR(), bgColorBak.getG(), bgColorBak.getB()));
	  bgColorButton->setPalette(palette);

	  axisHeight->setValue(axisHeightBak);
	  gBoxAxisPoints->setChecked(drawPointOnAxisBak);
	  minAxisPointSize->setValue(minAxisPointSizeBak);
	  maxAxisPointSize->setValue(maxAxisPointSizeBak);
	  linesColorAlphaValue->setValue(linesColorAlphaValueBak);
	  gBoxLineTexture->setChecked(linesTextureBak);
	  defaultTexture->setChecked(!userTextureBak);
	  userTexture->setChecked(userTextureBak);
	  userTextureFile->setText(QString(userTextureFileBak.c_str()));
  }

  unsigned int ParallelCoordinatesConfigDialog::getSpaceBetweenAxis() const {
	  return spaceBetweenAxis->value();
  }

  void ParallelCoordinatesConfigDialog::setSpaceBetweenAxis(const unsigned int spaceBetweenAxis) {
	  this->spaceBetweenAxis->setValue(spaceBetweenAxis);
  }

  void ParallelCoordinatesConfigDialog::setDrawPointOnAxis(const bool drawPointOnAxis) {
	  gBoxAxisPoints->setChecked(drawPointOnAxis);
  }

}
