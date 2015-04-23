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
#include "DatasetTools.h"
#include <tulip/StringCollection.h>

#define ORTHOGONAL "orthogonal"
#define ORIENTATION "up to down;down to up;right to left;left to right;"
#define ORIENTATION_ID "orientation"

using namespace tlp;

namespace {
const char* paramHelp[] = {
  //Orientation
  HTML_HELP_OPEN() \
  HTML_HELP_DEF("Type", "StringCollection") \
  HTML_HELP_DEF("Values", "up to down <BR> down to up <BR> right to left <BR> left to right") \
  HTML_HELP_DEF("Default", "up to down") \
  HTML_HELP_BODY() \
  "Choose your <BR> wished orientation" \
  HTML_HELP_CLOSE(),

  //Orthogonal Edge
  HTML_HELP_OPEN() \
  HTML_HELP_DEF("Type", "Boolean") \
  HTML_HELP_DEF("Values", "true <BR> false") \
  HTML_HELP_DEF("Default", "false") \
  HTML_HELP_BODY() \
  "If true then use orthogonal edges" \
  HTML_HELP_CLOSE(),

  //Layer Spacing
  HTML_HELP_OPEN()         \
  HTML_HELP_DEF( "type", "float" ) \
  HTML_HELP_DEF( "default", "64." )  \
  HTML_HELP_BODY() \
  "This parameter enables to set up the minimum space between two layers in the drawing" \
  HTML_HELP_CLOSE(),
  //Node Spacing
  HTML_HELP_OPEN()         \
  HTML_HELP_DEF( "type", "float" ) \
  HTML_HELP_DEF( "default", "18." )  \
  HTML_HELP_BODY() \
  "This parameter enables to set up the minimum space between two nodes in the same layer" \
  HTML_HELP_CLOSE(),

  // node size
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "Size" ) \
  HTML_HELP_DEF( "values", "An existing size property" ) \
  HTML_HELP_DEF( "default", "viewSize" ) \
  HTML_HELP_BODY() \
  "This parameter defines the property used for node's sizes." \
  HTML_HELP_CLOSE(),
};
}

//====================================================================
void addOrientationParameters(LayoutAlgorithm* pLayout) {
  pLayout->addInParameter<StringCollection>(ORIENTATION_ID, paramHelp[0], ORIENTATION);
}

//====================================================================
void addOrthogonalParameters(LayoutAlgorithm* pLayout) {
  pLayout->addInParameter<bool>(ORTHOGONAL,paramHelp[1],"false");
}

//====================================================================
void addSpacingParameters(LayoutAlgorithm* pLayout) {
  pLayout->addInParameter<float>("layer spacing", paramHelp[2], "64." );
  pLayout->addInParameter<float>("node spacing", paramHelp[3], "18." );
}
void getSpacingParameters(DataSet* dataSet, float& nodeSpacing, float& layerSpacing) {
  layerSpacing = 64.;
  nodeSpacing = 18.;

  if (dataSet) {
    dataSet->get("node spacing", nodeSpacing);
    dataSet->get("layer spacing", layerSpacing);
  }
}
//====================================================================
void addNodeSizePropertyParameter(LayoutAlgorithm* pLayout, bool inout) {
  if (inout)
    pLayout->addInOutParameter<SizeProperty>("node size", paramHelp[4],
        "viewSize");
  else
    pLayout->addInParameter<SizeProperty>("node size", paramHelp[4],
                                          "viewSize");
}

bool getNodeSizePropertyParameter(DataSet* dataSet, SizeProperty* &sizes) {
  return (dataSet != NULL) && dataSet->get("node size", sizes);
}

//====================================================================
DataSet setOrientationParameters(int pOrientation) {
  DataSet dataSet;
  StringCollection stringOrientation(ORIENTATION);
  stringOrientation.setCurrent(pOrientation);
  dataSet.set(ORIENTATION_ID, stringOrientation);
  return dataSet;
}

//====================================================================
orientationType getMask(DataSet* dataSet) {
  StringCollection orientation(ORIENTATION);
  orientation.setCurrent(0);
  int current = 0;

  if (dataSet != NULL) {
    StringCollection dataSetOrientation;

    if (dataSet->get(ORIENTATION_ID, dataSetOrientation)) {
      // the order of ORIENTATION items may have change
      // because the default value may have change (see DataSetDialog.cpp)
      std::string currentString = dataSetOrientation.getCurrentString();

      for (current = 0; current < 4; ++current) {
        if (currentString == orientation.at(current))
          break;
      }
    }
  }

  switch (current) {
  case 0  :
    return ORI_DEFAULT;

  case 1  :
    return ORI_INVERSION_VERTICAL;

  case 2  :
    return ORI_ROTATION_XY;

  case 3  :
    return orientationType(ORI_ROTATION_XY | ORI_INVERSION_HORIZONTAL);

  default :
    return ORI_DEFAULT;
  }
}


//====================================================================
bool hasOrthogonalEdge(DataSet* dataSet) {
  bool orthogonalEdge = false;

  if (dataSet != NULL) {
    dataSet->get(ORTHOGONAL, orthogonalEdge);
  }

  return orthogonalEdge;
}
