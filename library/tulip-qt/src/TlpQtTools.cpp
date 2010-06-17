/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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

}
