#ifndef DATASETTOOLS_H
#define DATASETTOOLS_H

#include <tulip/TulipPlugin.h>
#include <tulip/LayoutProperty.h>
#include "Orientation.h"

void 	        addOrientationParameters(tlp::LayoutAlgorithm* pLayout);
tlp::DataSet         setOrientationParameters(int orientation);
void 		addOrthogonalParameters(tlp::LayoutAlgorithm* pLayout);
void 		addSpacingParameters(tlp::LayoutAlgorithm* pLayout);
	
orientationType getMask(tlp::DataSet* dataSet);
bool 		hasOrthogonalEdge(tlp::DataSet* dataSet);


#endif
