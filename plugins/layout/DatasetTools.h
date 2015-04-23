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
#ifndef DATASETTOOLS_H
#define DATASETTOOLS_H

#include <tulip/TulipPluginHeaders.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include "Orientation.h"

void addOrientationParameters(tlp::LayoutAlgorithm* pLayout);
tlp::DataSet setOrientationParameters(int orientation);
void addOrthogonalParameters(tlp::LayoutAlgorithm* pLayout);
void addSpacingParameters(tlp::LayoutAlgorithm* pLayout);
void getSpacingParameters(tlp::DataSet* dataSet, float& nodeSpacing, float& layerSpacing);
void addNodeSizePropertyParameter(tlp::LayoutAlgorithm* pLayout,
                                  bool inout = false);
bool getNodeSizePropertyParameter(tlp::DataSet* dataSet, tlp::SizeProperty *&sizes);

orientationType getMask(tlp::DataSet* dataSet);
bool hasOrthogonalEdge(tlp::DataSet* dataSet);


#endif
