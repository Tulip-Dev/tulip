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
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <QtOpenGL/QGLPixelBuffer>
#include <QtGui/QColorDialog>

#include "tulip/TlpQtTools.h"

#include <tulip/Reflect.h>

#include "tulip/Interactor.h"
#include "tulip/View.h"
#include "tulip/Controller.h"


/**
 * For openDataSetDialog function : see OpenDataSet.cpp
 */

using namespace std;

/**
  * Init property type to property label conversion map
  **/
static map<string,QString> buildPropertyTypeToPropertyTypeLabelMap(){
    map<string,QString> propertyTypeToPropertyTypeLabel;
    propertyTypeToPropertyTypeLabel[string("bool")]=QString("Selection");
    propertyTypeToPropertyTypeLabel[string("color")]=QString("Color");
    propertyTypeToPropertyTypeLabel[string("double")]=QString("Metric");
    propertyTypeToPropertyTypeLabel[string("graph")]=QString("Graph");
    propertyTypeToPropertyTypeLabel[string("int")]=QString("Integer");
    propertyTypeToPropertyTypeLabel[string("layout")]=QString("Layout");
    propertyTypeToPropertyTypeLabel[string("size")]=QString("Size");
    propertyTypeToPropertyTypeLabel[string("string")]=QString("String");
    propertyTypeToPropertyTypeLabel[string("vector<bool>")]=QString("BooleanVector");
    propertyTypeToPropertyTypeLabel[string("vector<color>")]=QString("ColorVector");
    propertyTypeToPropertyTypeLabel[string("vector<coord>")]=QString("CoordVector");
    propertyTypeToPropertyTypeLabel[string("vector<double>")]=QString("DoubleVector");
    propertyTypeToPropertyTypeLabel[string("vector<int>")]=QString("IntegerVector");
    propertyTypeToPropertyTypeLabel[string("vector<size>")]=QString("SizeVector");
    propertyTypeToPropertyTypeLabel[string("vector<string>")]=QString("StringVector");
    return propertyTypeToPropertyTypeLabel;
}
//Property type to property label conversion map
static const map<string,QString> propertyTypeToPropertyTypeLabelMap = buildPropertyTypeToPropertyTypeLabelMap();
/**
  * Init property type label to property type conversion map
  **/
static map<QString,string> buildPropertyTypeLabelToPropertyTypeMap(){
    map<QString,string> propertyTypeLabelToPropertyType;
    propertyTypeLabelToPropertyType[QString("Selection")]=string("bool");
    propertyTypeLabelToPropertyType[QString("Color")]=string("color");
    propertyTypeLabelToPropertyType[QString("Metric")]=string("double");
    propertyTypeLabelToPropertyType[QString("Graph")]=string("graph");
    propertyTypeLabelToPropertyType[QString("Integer")]=string("int");
    propertyTypeLabelToPropertyType[QString("Layout")]=string("layout");
    propertyTypeLabelToPropertyType[QString("Size")]=string("size");
    propertyTypeLabelToPropertyType[QString("String")]=string("string");
    propertyTypeLabelToPropertyType[QString("BooleanVector")]=string("vector<bool>");
    propertyTypeLabelToPropertyType[QString("ColorVector")]=string("vector<color>");
    propertyTypeLabelToPropertyType[QString("CoordVector")]=string("vector<coord>");
    propertyTypeLabelToPropertyType[QString("DoubleVector")]=string("vector<double>");
    propertyTypeLabelToPropertyType[QString("IntegerVector")]=string("vector<int>");
    propertyTypeLabelToPropertyType[QString("SizeVector")]=string("vector<size>");
    propertyTypeLabelToPropertyType[QString("StringVector")]=string("vector<string>");
    return propertyTypeLabelToPropertyType;
}
//Property type label to property type conversion map
static const map<QString,string> propertyTypeLabelToPropertyTypeMap = buildPropertyTypeLabelToPropertyTypeMap();

namespace tlp {

  void openGraphOnGlMainWidget(Graph *graph,DataSet *dataSet,GlMainWidget *glMainWidget){
    glMainWidget->setData(graph,*dataSet);
  }

  void loadInteractorPluginsFromDir(std::string dir,PluginLoader *loader) {
    InteractorFactory::initFactory();
    loadPluginsFromDir(dir, "Interactor", loader);
  }
  void loadViewPluginsFromDir(std::string dir,PluginLoader *loader) {
    ViewFactory::initFactory();
    loadPluginsFromDir(dir, "View", loader);
  }
  void loadControllerPluginsFromDir(std::string dir,PluginLoader *loader) {
    ControllerFactory::initFactory();
    loadPluginsFromDir(dir, "Controller", loader);
  }
  bool canUseQGLPixelBuffer() {
    QGLPixelBuffer glFrameBuf(2,2);
    return glFrameBuf.isValid();
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
