#include "DatasetTools.h"

#define ORTHOGONAL "Orthogonal"
#define ORIENTATION "up to down;down to up;right to left;left to right;"
#define ORIENTATION_ID "Orientation"

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
  HTML_HELP_OPEN()				 \
  HTML_HELP_DEF( "type", "float" ) \
  HTML_HELP_DEF( "default", "64." )	 \
  HTML_HELP_BODY() \
  "This parameter enables to set up the minimum space between two layers in the drawing" \
  HTML_HELP_CLOSE(),
  //Node Spacing
  HTML_HELP_OPEN()				 \
  HTML_HELP_DEF( "type", "float" ) \
  HTML_HELP_DEF( "default", "18." )	 \
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
  pLayout->addParameter<StringCollection>(ORIENTATION_ID, paramHelp[0], ORIENTATION);
} 

//====================================================================	
void addOrthogonalParameters(LayoutAlgorithm* pLayout) {
  pLayout->addParameter<bool>(ORTHOGONAL,paramHelp[1],"false");
}

//====================================================================	
void addSpacingParameters(LayoutAlgorithm* pLayout) {
  pLayout->addParameter<float>("layer spacing", paramHelp[2], "64." );
  pLayout->addParameter<float>("node spacing", paramHelp[3], "18." );
}
void getSpacingParameters(DataSet* dataSet, float& nodeSpacing, float& layerSpacing) {
  if (dataSet) {
    dataSet->get("node spacing", nodeSpacing);
    dataSet->get("layer spacing", layerSpacing);
  }
}
//====================================================================	
void addNodeSizePropertyParameter(LayoutAlgorithm* pLayout) {
  pLayout->addParameter<SizeProperty>("node size", paramHelp[4], "viewSize");
}

bool getNodeSizePropertyParameter(DataSet* dataSet, SizeProperty *sizes) {
  return (dataSet != 0) && dataSet->get("node size", sizes);
}

//====================================================================
DataSet setOrientationParameters(int pOrientation) {
  DataSet dataSet;
  StringCollection stringOrientation(ORIENTATION);
  stringOrientation.setCurrent(pOrientation);
  dataSet.set<StringCollection>(ORIENTATION_ID, stringOrientation);
  return dataSet;
}

//====================================================================
orientationType getMask(DataSet* dataSet) { 
  StringCollection orientation(ORIENTATION);
  orientation.setCurrent(0);
  int current = 0;

  if (dataSet != 0) {
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
      case 0  : return ORI_DEFAULT;
      case 1  : return ORI_INVERSION_VERTICAL;
      case 2  : return ORI_ROTATION_XY;
      case 3  : return orientationType(ORI_ROTATION_XY | ORI_INVERSION_HORIZONTAL);
      default : return ORI_DEFAULT;
  }
}


//====================================================================
bool hasOrthogonalEdge(DataSet* dataSet) {
	bool orthogonalEdge = false;
	
	if (dataSet != 0) {
        dataSet->get(ORTHOGONAL, orthogonalEdge);
    }
    
    return orthogonalEdge;
}
