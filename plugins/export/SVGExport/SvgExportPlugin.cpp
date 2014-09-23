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
#include "RepresentExport.h"
#include "ReadGraph.h"
#include "ExportSvg.h"

#include <iostream>

#include <tulip/ExportModule.h>
#include <tulip/LayoutProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/IntegerProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/StringProperty.h>

using namespace std;
using namespace tlp;

namespace {
const char * paramHelp[] = {
  // viewLayout
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "LayoutProperty" ) \
  HTML_HELP_DEF( "default", "viewLayout" ) \
  HTML_HELP_BODY() \
  "Indicates the property to use for element layout." \
  HTML_HELP_CLOSE(),
  // viewColor
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "ColorProperty" ) \
  HTML_HELP_DEF( "default", "viewColor" ) \
  HTML_HELP_BODY() \
  "Indicates the property to use for element color." \
  HTML_HELP_CLOSE(),
  // viewShape
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "IntegerProperty" ) \
  HTML_HELP_DEF( "default", "viewShape" ) \
  HTML_HELP_BODY() \
  "Indicates the property to use for defining element shape." \
  HTML_HELP_CLOSE(),
  // viewSrcAnchorShape
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "IntegerProperty" ) \
  HTML_HELP_DEF( "default", "viewSrcAnchorShape" ) \
  HTML_HELP_BODY() \
  "Indicates the property to use for defining element shape." \
  HTML_HELP_CLOSE(),
  // viewTgtAnchorShape
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "IntegerProperty" ) \
  HTML_HELP_DEF( "default", "viewTgtAnchorShape" ) \
  HTML_HELP_BODY() \
  "Indicates the property to use for defining element shape." \
  HTML_HELP_CLOSE(),
  // viewSize
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "SizeProperty" ) \
  HTML_HELP_DEF( "default", "viewSize" )
  HTML_HELP_BODY() \
  "Indicates the property to use for defining element size." \
  HTML_HELP_CLOSE(),
  // viewLabel
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "StringProperty" ) \
  HTML_HELP_DEF( "default", "viewLabel" )
  HTML_HELP_BODY() \
  "Indicates the property to use for defining element label." \
  HTML_HELP_CLOSE(),
  // viewLabelColor
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "ColorProperty" ) \
  HTML_HELP_DEF( "default", "viewLabelColor" )
  HTML_HELP_BODY() \
  "Indicates the property to use for defining element label color." \
  HTML_HELP_CLOSE(),
  // viewBorderColor
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "viewBorderColor" ) \
  HTML_HELP_DEF( "default", "viewSize" )
  HTML_HELP_BODY() \
  "Indicates the property to use for defining element border color." \
  HTML_HELP_CLOSE(),
  // viewBorderWidth
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "DoubleProperty" ) \
  HTML_HELP_DEF( "default", "viewBorderWidth" )
  HTML_HELP_BODY() \
  "Indicates the property to use for defining element border width." \
  HTML_HELP_CLOSE(),
  // viewRotation
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "DoubleProperty" ) \
  HTML_HELP_DEF( "default", "viewRotation" )
  HTML_HELP_BODY() \
  "Indicates the property to use for defining element rotation." \
  HTML_HELP_CLOSE(),
  // color interpolation
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "bool" ) \
  HTML_HELP_DEF( "default", "false" )
  HTML_HELP_BODY() \
  "Indicates if edge color interpolation has to be used" \
  HTML_HELP_CLOSE(),
  // edge extremities
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "bool" ) \
  HTML_HELP_DEF( "default", "false" )
  HTML_HELP_BODY() \
  "Indicates if edge extremities have to be exported" \
  HTML_HELP_CLOSE(),
};
}

class ExportMultiple_svg:public tlp::ExportModule {

public:
  PLUGININFORMATION("SVG Export","Sami Gasri, Charles-Antoine Lami, Bruno Pinaud","16/07/2013","Exports a graph drawing in a SVG formatted file","1.5", "File")

  std::string icon() const {
    return ":/tulip/graphperspective/icons/32/export_svg.png";
  }

  std::string fileExtension() const {
    return "svg";
  }

  ExportMultiple_svg(tlp::PluginContext* context) : tlp::ExportModule(context) {

    addInParameter<LayoutProperty>("Element's layout property", paramHelp[0], "viewLayout");
    addInParameter<ColorProperty>("Element's color property", paramHelp[1], "viewColor");
    addInParameter<IntegerProperty>("Element's shape property", paramHelp[2], "viewShape");
    addInParameter<IntegerProperty>("Element's SrcAnchorShape property", paramHelp[3], "viewSrcAnchorShape");
    addInParameter<IntegerProperty>("Element's TgtAnchorShape property", paramHelp[4], "viewTgtAnchorShape");
    addInParameter<SizeProperty>("Element's size property", paramHelp[5], "viewSize");
    addInParameter<StringProperty>("Element's label property", paramHelp[6], "viewLabel");
    addInParameter<ColorProperty>("Element's label color property", paramHelp[7], "viewLabelColor");
    addInParameter<ColorProperty>("Element's border color property", paramHelp[8], "viewBorderColor");
    addInParameter<DoubleProperty>("Element's border width property", paramHelp[9], "viewBorderWidth");
    addInParameter<DoubleProperty>("Element's rotation property", paramHelp[10], "viewRotation");
    addInParameter<bool>("Edge color interpolation", paramHelp[11], "false");
    addInParameter<bool>("Edge extremities", paramHelp[12], "false");
  }

  ~ExportMultiple_svg() {}

  bool exportGraph(ostream &os) {

    RepresentExport *svg = new ExportSvg(); // We call our first concrete builder
    ReadGraph gr1(graph, dataSet, pluginProgress, svg); // We analyse the graph
    os << gr1; // We retrieve the result

    delete svg;
    return true;

  }


};
PLUGIN(ExportMultiple_svg)
