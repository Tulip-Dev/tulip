#include <tulip/Layout.h>
#include <tulip/Types.h>
#include "DatasetTools.h"

const std::string ORTHOGONAL("Orthogonal");
const std::string ORIENTATION("up to down;down to up;right to left;left to right;");
const std::string ORIENTATION_ID("Orientation");

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
HTML_HELP_CLOSE()};
}                     

//====================================================================	
void addOrientationParameters(Layout* pLayout) {
    pLayout->addParameter<StringCollection>(ORIENTATION_ID, paramHelp[0], ORIENTATION);
} 

//====================================================================	
void addOrthogonalParameters(Layout* pLayout) {
	pLayout->addParameter<bool>(ORTHOGONAL,paramHelp[1],"false");
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

  if (dataSet != 0)
    dataSet->get(ORIENTATION_ID, orientation);

  switch (orientation.getCurrent()) {
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
