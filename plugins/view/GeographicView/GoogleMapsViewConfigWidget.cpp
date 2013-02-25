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

#include "GoogleMapsViewConfigWidget.h"

#include "GoogleMapsView.h"

using namespace std;

GoogleMapsViewConfigWidget::GoogleMapsViewConfigWidget(QWidget *parent) : QWidget(parent),_oldPolyFileType(None),_oldFileLoaded("") {
	setupUi(this);
}

bool GoogleMapsViewConfigWidget::polyOptionsChanged() {
  if(polyFileType()!=_oldPolyFileType){
    _oldPolyFileType=polyFileType();
    switch(_oldPolyFileType){
    case Default: {
      _oldFileLoaded="";
      break;
    }
    case CsvFile: {
      _oldFileLoaded=csvFile->text().toStdString();
      break;
    }
    case PolyFile: {
      _oldFileLoaded=polyFile->text().toStdString();
      break;
    }
    default :
      break;
    }
    return true;
  }else{
    switch(_oldPolyFileType){
    case CsvFile: {
      if(_oldFileLoaded!=csvFile->text().toStdString()){
        _oldFileLoaded=csvFile->text().toStdString();
        return true;
      }
      break;
    }
    case PolyFile: {
      if(_oldFileLoaded!=polyFile->text().toStdString()){
        _oldFileLoaded=polyFile->text().toStdString();
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
    PolyFileType type;
    if (dataSet.get("polyFileType",(int&)type))
    setPolyFileType(type);
  }
  if(dataSet.exist("csvFileName")){
    string fileName;
    dataSet.get("csvFileName",fileName);
    csvFile->setText(fileName.c_str());
  }
  if(dataSet.exist("polyFileName")){
    string fileName;
    dataSet.get("polyFileName",fileName);
    polyFile->setText(fileName.c_str());
  }

  bool useShared = false;
  if (dataSet.get("useSharedLayout",useShared))
    layoutCheckBox->setChecked(useShared);

  if (dataSet.get("useSharedSize",useShared))
    sizeCheckBox->setChecked(useShared);

  if (dataSet.get("useSharedShape",useShared))
    shapeCheckBox->setChecked(useShared);
}

DataSet GoogleMapsViewConfigWidget::state() const {
  DataSet data;
  data.set("polyFileType",(int)polyFileType());
  data.set("csvFileName",csvFile->text().toStdString());
  data.set("polyFileName",polyFile->text().toStdString());
  data.set("useSharedLayout",useSharedLayoutProperty());
  data.set("useSharedSize",useSharedSizeProperty());
  data.set("useSharedShape",useSharedShapeProperty());
  return data;
}
