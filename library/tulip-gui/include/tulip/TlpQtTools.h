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
#ifndef _TLPQTTOOLS_H
#define _TLPQTTOOLS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/Color.h"
#include "tulip/Reflect.h"
#include "tulip/Graph.h"
#include "tulip/GlMainWidget.h"
#include "tulip/DataSet.h"

#include <QtGui/QColor>

class QWidget;

//Declare tulip type compatible with QVariant
Q_DECLARE_METATYPE(tlp::Graph*)
Q_DECLARE_METATYPE(tlp::DataSet)
Q_DECLARE_METATYPE(tlp::Color)
Q_DECLARE_METATYPE(tlp::Coord)
Q_DECLARE_METATYPE(tlp::Size)
Q_DECLARE_METATYPE(std::vector<bool>)
Q_DECLARE_METATYPE(std::vector<tlp::Color>)
Q_DECLARE_METATYPE(std::vector<tlp::Coord>)
Q_DECLARE_METATYPE(std::vector<double>)
Q_DECLARE_METATYPE(std::vector<int>)
Q_DECLARE_METATYPE(std::vector<tlp::Size>)
Q_DECLARE_METATYPE(std::vector<std::string>)

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

  TLP_QT_SCOPE bool getColorDialog(const QColor &color,QWidget *parent,const QString &title,QColor &result);

  inline QColor colorToQColor(const Color& color) {
     return QColor(color.getR(), color.getG(), color.getB(), color.getA());
   }
  inline Color QColorToColor(const QColor& color) {
     return Color(color.red(), color.green(), color.blue(), color.alpha());
   }
  /**
    * @brief Convert a string from Tulip to QString.
    **/
  inline std::string QStringToTlpString(const QString& toConvert){
      return std::string(toConvert.toUtf8());
  }
  /**
    * @brief Convert a QString to tulip string.
    **/
  inline QString tlpStringToQString(const std::string& toConvert){
      return QString::fromUtf8(toConvert.c_str());
  }
}

#endif


