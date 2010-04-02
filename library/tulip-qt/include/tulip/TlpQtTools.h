//-*-c++-*-
#ifndef _TLPQTTOOLS_H
#define _TLPQTTOOLS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/Color.h"
#include "tulip/Reflect.h"
#include "tulip/Graph.h"
#include "tulip/GlMainWidget.h"

#include <QtGui/QColor>

class QWidget;

namespace tlp {

  /**
   * Open a QT Dialog Box to fill-up an user DataSet according to a required
   * StructDef
   * inDef		: Input-variable informations
   * outSet		: Output (filled) set
   * [inSet		: Input (optional) set, containing some default values]
   */

  TLP_QT_SCOPE bool openDataSetDialog	(DataSet & outSet,
					 const StructDef *sysDef,
					 StructDef *inDef,
					 const DataSet *inSet	= NULL,
					 const char *inName = NULL,
					 Graph *inG = NULL,
					 QWidget *parent = NULL);

  /**
   * Create a main layout in GlMainWidget and display the graph on it
   */

  TLP_QT_SCOPE void openGraphOnGlMainWidget(Graph *graph,DataSet *dataSet,GlMainWidget *glMainWidget);

  TLP_QT_SCOPE void loadInteractorPluginsFromDir(std::string dir,PluginLoader *loader=0);
  TLP_QT_SCOPE void loadViewPluginsFromDir(std::string dir,PluginLoader *loader=0);
  TLP_QT_SCOPE void loadControllerPluginsFromDir(std::string dir,PluginLoader *loader=0);

  TLP_QT_SCOPE bool canUseQGLPixelBuffer();

  TLP_QT_SCOPE QColor colorToQColor(const Color& color) {
     return QColor(color.getR(), color.getG(), color.getB(), color.getA());
   }
  TLP_QT_SCOPE Color QColorToColor(const QColor& color) {
     return Color(color.red(), color.green(), color.blue(), color.alpha());
   }

}

#endif


