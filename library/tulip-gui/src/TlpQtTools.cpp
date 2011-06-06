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
#include <QtOpenGL/QGLPixelBuffer>
#include <QtGui/QColorDialog>

#include "tulip/TlpQtTools.h"

#include <tulip/Reflect.h>

#include "tulip/Interactor.h"
#include "tulip/View.h"
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/GraphProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>
/**
 * For openDataSetDialog function : see OpenDataSet.cpp
 */
using namespace std;
using namespace tlp;

/**
  * Init property type to property label conversion map
  **/
static map<string,QString> buildPropertyTypeToPropertyTypeLabelMap(){
    map<string,QString> propertyTypeToPropertyTypeLabel;
    propertyTypeToPropertyTypeLabel[BooleanProperty::propertyTypename]=QString("Selection");
    propertyTypeToPropertyTypeLabel[ColorProperty::propertyTypename]=QString("Color");
    propertyTypeToPropertyTypeLabel[DoubleProperty::propertyTypename]=QString("Metric");
    propertyTypeToPropertyTypeLabel[GraphProperty::propertyTypename]=QString("Graph");
    propertyTypeToPropertyTypeLabel[IntegerProperty::propertyTypename]=QString("Integer");
    propertyTypeToPropertyTypeLabel[LayoutProperty::propertyTypename]=QString("Layout");
    propertyTypeToPropertyTypeLabel[SizeProperty::propertyTypename]=QString("Size");
    propertyTypeToPropertyTypeLabel[StringProperty::propertyTypename]=QString("String");
    propertyTypeToPropertyTypeLabel[BooleanVectorProperty::propertyTypename]=QString("BooleanVector");
    propertyTypeToPropertyTypeLabel[ColorVectorProperty::propertyTypename]=QString("ColorVector");
    propertyTypeToPropertyTypeLabel[CoordVectorProperty::propertyTypename]=QString("CoordVector");
    propertyTypeToPropertyTypeLabel[DoubleVectorProperty::propertyTypename]=QString("DoubleVector");
    propertyTypeToPropertyTypeLabel[IntegerVectorProperty::propertyTypename]=QString("IntegerVector");
    propertyTypeToPropertyTypeLabel[SizeVectorProperty::propertyTypename]=QString("SizeVector");
    propertyTypeToPropertyTypeLabel[StringVectorProperty::propertyTypename]=QString("StringVector");
    return propertyTypeToPropertyTypeLabel;
}
//Property type to property label conversion map
static const map<string,QString> propertyTypeToPropertyTypeLabelMap = buildPropertyTypeToPropertyTypeLabelMap();
/**
  * Init property type label to property type conversion map
  **/
static map<QString,string> buildPropertyTypeLabelToPropertyTypeMap(){
    map<QString,string> propertyTypeLabelToPropertyType;
    propertyTypeLabelToPropertyType[QString("Selection")]=BooleanProperty::propertyTypename;
    propertyTypeLabelToPropertyType[QString("Color")]=ColorProperty::propertyTypename;
    propertyTypeLabelToPropertyType[QString("Metric")]=DoubleProperty::propertyTypename;
    propertyTypeLabelToPropertyType[QString("Graph")]=GraphProperty::propertyTypename;
    propertyTypeLabelToPropertyType[QString("Integer")]=IntegerProperty::propertyTypename;
    propertyTypeLabelToPropertyType[QString("Layout")]=LayoutProperty::propertyTypename;
    propertyTypeLabelToPropertyType[QString("Size")]=SizeProperty::propertyTypename;
    propertyTypeLabelToPropertyType[QString("String")]=StringProperty::propertyTypename;
    propertyTypeLabelToPropertyType[QString("BooleanVector")]=BooleanVectorProperty::propertyTypename;
    propertyTypeLabelToPropertyType[QString("ColorVector")]=ColorVectorProperty::propertyTypename;
    propertyTypeLabelToPropertyType[QString("CoordVector")]=CoordVectorProperty::propertyTypename;
    propertyTypeLabelToPropertyType[QString("DoubleVector")]=DoubleVectorProperty::propertyTypename;
    propertyTypeLabelToPropertyType[QString("IntegerVector")]=IntegerVectorProperty::propertyTypename;
    propertyTypeLabelToPropertyType[QString("SizeVector")]=SizeVectorProperty::propertyTypename;
    propertyTypeLabelToPropertyType[QString("StringVector")]=StringVectorProperty::propertyTypename;
    return propertyTypeLabelToPropertyType;
}
//Property type label to property type conversion map
static const map<QString,string> propertyTypeLabelToPropertyTypeMap = buildPropertyTypeLabelToPropertyTypeMap();

namespace tlp {

  void openGraphOnGlMainWidget(Graph *graph,DataSet *dataSet,GlMainWidget *glMainWidget){
    glMainWidget->setData(graph,*dataSet);
  }

  bool getColorDialog(const QColor &color,QWidget *parent,const QString &title,QColor &result){

#if (QT_VERSION >= QT_VERSION_CHECK(4, 5, 0))

    QColor newColor = QColorDialog::getColor(color, parent, title, QColorDialog::ShowAlphaChannel | QColorDialog::DontUseNativeDialog);
    if(newColor.isValid()){
      result=newColor;
      return true;
    }else
      return false;

#else

    bool ok = true;
    QRgb newColor = QColorDialog::getRgba(qRgba(color.red(),color.green(), color.blue(), color.alpha()), &ok, parent);
    if(ok){
      result=QColor(qRed(newColor), qGreen(newColor), qBlue(newColor), qAlpha(newColor));
      return true;
    }else
      return false;
#endif
  }

  QString propertyTypeToPropertyTypeLabel(const string& typeName){
        map<string,QString>::const_iterator  it = propertyTypeToPropertyTypeLabelMap.find(typeName);
        return it != propertyTypeToPropertyTypeLabelMap.end()?it->second:QString();
    }

    string propertyTypeLabelToPropertyType(const QString& typeNameLabel){
        map<QString,string>::const_iterator  it = propertyTypeLabelToPropertyTypeMap.find(typeNameLabel);
        return it != propertyTypeLabelToPropertyTypeMap.end()?it->second:string();
    }
}
