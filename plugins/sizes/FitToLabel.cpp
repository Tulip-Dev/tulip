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

#include <tulip/TextRenderer.h>
#include <tulip/TlpTools.h>

#include <cmath>
#include <tulip/TulipPlugin.h>

using namespace std;
using namespace tlp;

namespace {
//file::filepath
static const char * paramHelp[] = { HTML_HELP_OPEN()
HTML_HELP_DEF( "type", "StringProperty" )
HTML_HELP_BODY()
"The graph property containing the displayed nodes labels."
HTML_HELP_CLOSE(),

HTML_HELP_OPEN()
HTML_HELP_DEF( "type", "StringProperty" )
HTML_HELP_BODY()
"The graph property containing the font used to display the label."
HTML_HELP_CLOSE(),

HTML_HELP_OPEN()
HTML_HELP_DEF( "type", "IntegerProperty" )
HTML_HELP_BODY()
"The graph property containing the fonts sizes."
HTML_HELP_CLOSE(), };
}

/** \addtogroup size */
/*@{*/
/// FitToLabel.cpp - Compute size of elements according to the size of the displayed label.
/**
 *  version 1.0.0 : Tulip Team, Complete rewrite
 *  initial version  : Sebastien Carceles, Pascal Niotout, Sophie Bardet, Julien Mercadal, Bertrand Ng Sing Kwong
 *
 *  \author Maintainer : David Auber University Bordeaux I France: Email:auber@tulip-software.org
 */
class FitToLabel: public SizeAlgorithm {
public:
  //==============================================Algorithm======
  FitToLabel(const PropertyContext &context) :
    SizeAlgorithm(context) {
    addParameter<StringProperty> ("prop::label", paramHelp[0], "viewLabel");
    addParameter<StringProperty> ("prop::font", paramHelp[1], "viewFont");
    addParameter<IntegerProperty> ("prop::fontsize", paramHelp[2], "viewFontSize");
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

    if (dataSet != 0) {
      dataSet->get("prop::label", labels);
      dataSet->get("prop::font", fonts);
      dataSet->get("prop::fontsize", fontSizes);
    }

    TextRenderer textRender;
    textRender.setMode(TLP_POLYGON);
    sizeResult->setAllNodeValue(Size(18, 18, 1));
    node n;
    forEach(n, graph->getNodes()) {
      textRender.setContext(fonts->getNodeValue(n), fontSizes->getNodeValue(n), 255, 255, 255);
      const string &str = labels->getNodeValue(n);
      if (str != "") {
        float w_max, h, w;
        w_max = 256.0;
        textRender.setString(str, VERBATIM);
        textRender.getBoundingBox(w_max, h, w);
        sizeResult->setNodeValue(n, Size(int(w), int(h), 1));
      }
    }
    sizeResult->setAllEdgeValue(Size(1, 1, 8));

    Observable::unholdObservers();
    return true;
  }
  //====================================================
};
/*@}*/
SIZEPLUGIN(FitToLabel,"Fit to label","Tulip Team","25/01/2006","","1.0")
;
