#ifndef DATASETTOOLS_H
#define DATASETTOOLS_H

#include <tulip/TulipPlugin.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include "Orientation.h"

void addOrientationParameters(tlp::LayoutAlgorithm* pLayout);
tlp::DataSet setOrientationParameters(int orientation);
void addOrthogonalParameters(tlp::LayoutAlgorithm* pLayout);
void addSpacingParameters(tlp::LayoutAlgorithm* pLayout);
void getSpacingParameters(tlp::DataSet* dataSet, float& nodeSpacing, float& layerSpacing);
void addNodeSizePropertyParameter(tlp::LayoutAlgorithm* pLayout);
bool getNodeSizePropertyParameter(tlp::DataSet* dataSet, tlp::SizeProperty *&sizes);
	
orientationType getMask(tlp::DataSet* dataSet);
bool hasOrthogonalEdge(tlp::DataSet* dataSet);


#endif
