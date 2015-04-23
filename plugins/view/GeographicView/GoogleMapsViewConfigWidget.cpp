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

#include "GoogleMapsViewConfigWidget.h"
#include "ui_GoogleMapsViewConfigWidget.h"
#include "GoogleMapsView.h"

#include <QFileDialog>
#include <QMessageBox>

using namespace std;
using namespace tlp;

GoogleMapsViewConfigWidget::GoogleMapsViewConfigWidget(QWidget *parent) : QWidget(parent),_ui(new Ui::GoogleMapsViewConfigWidgetData), _oldPolyFileType(None),_oldFileLoaded("") {
  _ui->setupUi(this);
}

GoogleMapsViewConfigWidget::~GoogleMapsViewConfigWidget() {
  delete _ui;
}

void GoogleMapsViewConfigWidget::openCsvFileBrowser() {
  _ui->csvFile->setText(QFileDialog::getOpenFileName(NULL,tr("Open csv file"), "./", tr("cvs file (*.*)")));
}

void GoogleMapsViewConfigWidget::openPolyFileBrowser() {
  _ui->polyFile->setText(QFileDialog::getOpenFileName(NULL,tr("Open .poly file"), "./", tr("Poly file (*.poly)")));
}

void GoogleMapsViewConfigWidget::openCsvHelp() {
  QMessageBox::about(NULL,"Map csv file format","If you want to import a csv file into this view, your file must be in the format :\nid\tlng\tlat\nid\tlng\tlat\n...\nwith id : id of the polygon");
}

void GoogleMapsViewConfigWidget::openPolyHelp() {
  QMessageBox::about(NULL,"Map poly files",".poly files format are an open street map format.\nYou can donwload .poly file on :\nhttp://downloads.cloudmade.com/");
}

bool GoogleMapsViewConfigWidget::useSharedLayoutProperty() const {
  return _ui->layoutCheckBox->isChecked();
}

bool GoogleMapsViewConfigWidget::useSharedSizeProperty() const {
  return _ui->sizeCheckBox->isChecked();
}

GoogleMapsViewConfigWidget::PolyFileType GoogleMapsViewConfigWidget::polyFileType() const {
  _ui->mapToPolygon->setEnabled(false);

  if(_ui->useDefaultShape->isChecked())
    return Default;

  if(_ui->useCsvFile->isChecked())
    return CsvFile;

  if(_ui->usePolyFile->isChecked()) {
    _ui->mapToPolygon->setEnabled(true);
    return PolyFile;
  }

  return Default;
}

void GoogleMapsViewConfigWidget::setPolyFileType(PolyFileType &fileType) {
  _ui->mapToPolygon->setEnabled(false);

  if(fileType==Default)
    _ui->useDefaultShape->setChecked(true);

  if(fileType==CsvFile)
    _ui->useCsvFile->setChecked(true);

  if(fileType==PolyFile) {
    _ui->usePolyFile->setChecked(true);
    _ui->mapToPolygon->setEnabled(true);
  }
}

QString GoogleMapsViewConfigWidget::getCsvFile() const {
  return _ui->csvFile->text();
}

QString GoogleMapsViewConfigWidget::getPolyFile() const {
  return _ui->polyFile->text();
}

bool GoogleMapsViewConfigWidget::useSharedShapeProperty() const {
  return _ui->layoutCheckBox->isChecked();
}

bool GoogleMapsViewConfigWidget::polyOptionsChanged() {
  if(polyFileType()!=_oldPolyFileType) {
    _oldPolyFileType=polyFileType();

    switch(_oldPolyFileType) {
    case Default: {
      _oldFileLoaded="";
      break;
    }

    case CsvFile: {
      _oldFileLoaded=_ui->csvFile->text().toUtf8().data();
      break;
    }

    case PolyFile: {
      _oldFileLoaded=_ui->polyFile->text().toUtf8().data();
      break;
    }

    default :
      break;
    }

    return true;
  }
  else {
    switch(_oldPolyFileType) {
    case CsvFile: {
      if(_oldFileLoaded!=_ui->csvFile->text().toUtf8().data()) {
        _oldFileLoaded=_ui->csvFile->text().toUtf8().data();
        return true;
      }

      break;
    }

    case PolyFile: {
      if(_oldFileLoaded!=_ui->polyFile->text().toUtf8().data()) {
        _oldFileLoaded=_ui->polyFile->text().toUtf8().data();
        return true;
      }

      break;
    }

    default :
      break;
    }
  }

  return false;
}

void GoogleMapsViewConfigWidget::setState(const DataSet &dataSet) {
  {
    PolyFileType polyFileType;
    int type = 0;

    if (dataSet.get("polyFileType",type)) {
      polyFileType = static_cast<PolyFileType>(type);
      setPolyFileType(polyFileType);
    }
  }

  if(dataSet.exist("csvFileName")) {
    string fileName;
    dataSet.get("csvFileName",fileName);
    _ui->csvFile->setText(QString::fromUtf8(fileName.c_str()));
  }

  if(dataSet.exist("polyFileName")) {
    string fileName;
    dataSet.get("polyFileName",fileName);
    _ui->polyFile->setText(QString::fromUtf8(fileName.c_str()));
  }

  bool useShared = false;

  if (dataSet.get("useSharedLayout",useShared))
    _ui->layoutCheckBox->setChecked(useShared);

  if (dataSet.get("useSharedSize",useShared))
    _ui->sizeCheckBox->setChecked(useShared);

  if (dataSet.get("useSharedShape",useShared))
    _ui->shapeCheckBox->setChecked(useShared);
}

DataSet GoogleMapsViewConfigWidget::state() const {
  DataSet data;
  data.set("polyFileType",(int)polyFileType());
  data.set("csvFileName",std::string(_ui->csvFile->text().toUtf8().data()));
  data.set("polyFileName",std::string(_ui->polyFile->text().toUtf8().data()));
  data.set("useSharedLayout",useSharedLayoutProperty());
  data.set("useSharedSize",useSharedSizeProperty());
  data.set("useSharedShape",useSharedShapeProperty());
  return data;
}
