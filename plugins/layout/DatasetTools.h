#ifndef DATASETTOOLS_H
#define DATASETTOOLS_H

#include <tulip/TulipPlugin.h>
#include <tulip/Layout.h>
#include "Orientation.h"

void 	        addOrientationParameters(Layout* pLayout);
DataSet         setOrientationParameters(int orientation);
void 		    addOrthogonalParameters(Layout* pLayout);
	
orientationType getMask(DataSet* dataSet);
bool 		    hasOrthogonalEdge(DataSet* dataSet);


#endif
