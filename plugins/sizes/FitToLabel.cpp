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

#include <tulip/TlpTools.h>
#include <tulip/GlLabel.h>

#include <cmath>
#include <tulip/TulipPluginHeaders.h>

using namespace std;
using namespace tlp;

namespace {
//file::filepath
static const char * paramHelp[] = { HTML_HELP_OPEN()
                                    HTML_HELP_DEF( "type", "StringProperty" )
                                    HTML_HELP_BODY()
                                    "Nodes labels."
                                    HTML_HELP_CLOSE(),

                                    HTML_HELP_OPEN()
                                    HTML_HELP_DEF( "type", "StringProperty" )
                                    HTML_HELP_BODY()
                                    "Font used to display the labels."
                                    HTML_HELP_CLOSE(),

                                    HTML_HELP_OPEN()
                                    HTML_HELP_DEF( "type", "IntegerProperty" )
                                    HTML_HELP_BODY()
                                    "Fonts sizes."
                                    HTML_HELP_CLOSE(),
                                  };
}

/** \addtogroup size */

/// FitToLabel.cpp - Compute size of elements according to the size of the displayed label.
/**
 *  version 1.0.0 : Tulip Team, Complete rewrite
 *  initial version  : Sebastien Carceles, Pascal Niotout, Sophie Bardet, Julien Mercadal, Bertrand Ng Sing Kwong
 *
 *  \author Maintainer : David Auber University Bordeaux I France: Email:auber@tulip-software.org
 */
class FitToLabel: public SizeAlgorithm {
public:
  PLUGININFORMATIONS("Fit to label","Tulip Team","25/01/2006","","1.0", "Size")
  //==============================================Algorithm======
  FitToLabel(const PluginContext* context) :
    SizeAlgorithm(context) {
    addInParameter<StringProperty> ("prop::label", paramHelp[0], "viewLabel");
    addInParameter<StringProperty> ("prop::font", paramHelp[1], "viewFont");
    addInParameter<IntegerProperty> ("prop::fontsize", paramHelp[2], "viewFontSize");
  }
  //====================================================
  ~FitToLabel() {
  }
  //====================================================
  bool run() {
    Observable::holdObservers();

    StringProperty *labels(graph->getProperty<StringProperty> ("viewLabel"));
    StringProperty *fonts(graph->getProperty<StringProperty> ("viewFont"));
    IntegerProperty *fontSizes(graph->getProperty<IntegerProperty> ("viewFontSize"));

    if (dataSet != NULL) {
      dataSet->get("prop::label", labels);
      dataSet->get("prop::font", fonts);
      dataSet->get("prop::fontsize", fontSizes);
    }

    GlLabel label;
    BoundingBox bb;

    node n;
    forEach(n, graph->getNodes()) {
      label.setFontNameSizeAndColor(fonts->getNodeValue(n),fontSizes->getNodeValue(n),Color());
      const string &str = labels->getNodeValue(n);

      if (str != "") {
        label.setText(str);
        bb=label.getTextBoundingBox();
        result->setNodeValue(n, Size(bb[1][0]-bb[0][0], bb[1][1]-bb[0][1], 1));
      }
    }
    result->setAllEdgeValue(Size(1, 1, 8));

    Observable::unholdObservers();
    return true;
  }
  //====================================================
};

PLUGIN(FitToLabel)
