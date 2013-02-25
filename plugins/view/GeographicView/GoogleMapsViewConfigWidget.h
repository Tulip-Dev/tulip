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

#ifndef GOOGLEMAPSVIEWCONFIGWIDGET_H_
#define GOOGLEMAPSVIEWCONFIGWIDGET_H_

#include <tulip/Graph.h>
#include <tulip/DataSet.h>

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

#include "ui_GoogleMapsViewConfigWidget.h"

using namespace tlp;

class GoogleMapsViewConfigWidget : public QWidget, public Ui::GoogleMapsViewConfigWidgetData {

	Q_OBJECT

public :

  enum PolyFileType {
    None=-1,
    Default=0,
    CsvFile,
    PolyFile
  };

  GoogleMapsViewConfigWidget(QWidget *parent = 0);

  bool useSharedLayoutProperty() const {
    return layoutCheckBox->isChecked();
  }

  bool useSharedSizeProperty() const {
    return sizeCheckBox->isChecked();
  }

  bool useSharedShapeProperty() const {
    return layoutCheckBox->isChecked();
  }

  PolyFileType polyFileType() const{
    mapToPolygon->setEnabled(false);
    if(useDefaultShape->isChecked())
      return Default;
    if(useCsvFile->isChecked())
      return CsvFile;
    if(usePolyFile->isChecked()){
      mapToPolygon->setEnabled(true);
      return PolyFile;
    }

    return Default;
  }

  void setPolyFileType(PolyFileType &fileType){
    mapToPolygon->setEnabled(false);
    if(fileType==Default)
      useDefaultShape->setChecked(true);
    if(fileType==CsvFile)
      useCsvFile->setChecked(true);
    if(fileType==PolyFile){
      usePolyFile->setChecked(true);
      mapToPolygon->setEnabled(true);
    }
  }

  QString getCsvFile() {
    return csvFile->text();
  }

  QString getPolyFile() {
    return polyFile->text();
  }

  bool polyOptionsChanged();

  void setState(const DataSet &dataSet);

  DataSet state() const;

signals :

  void mapToPolygonSignal();

public slots:

  void openCsvFileBrowser(){
    csvFile->setText(QFileDialog::getOpenFileName(NULL,tr("Open csv file"), "./", tr("cvs file (*.*)")));
  }

  void openPolyFileBrowser() {
    polyFile->setText(QFileDialog::getOpenFileName(NULL,tr("Open .poly file"), "./", tr("Poly file (*.poly)")));
  }

  void openCsvHelp() {
    QMessageBox::about(NULL,"Map csv file format","If you want to import a csv file into this view, your file must be in the format :\nid\tlng\tlat\nid\tlng\tlat\n...\nwith id : id of the polygon");
  }

  void openPolyHelp() {
    QMessageBox::about(NULL,"Map poly files",".poly files format are an open street map format.\nYou can donwload .poly file on :\nhttp://downloads.cloudmade.com/");
  }

  void mapToPolygonSlot() {
    emit mapToPolygonSignal();
  }

protected :

  PolyFileType _oldPolyFileType;
  std::string _oldFileLoaded;

};


#endif /* GOOGLEMAPSVIEWCONFIGWIDGET_H_ */
