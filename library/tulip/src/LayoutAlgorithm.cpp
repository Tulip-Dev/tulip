//-*-c++-*-

#include "tulip/LayoutProperty.h"
#include "tulip/Graph.h"
#include "tulip/PluginContext.h"

LayoutAlgorithm::LayoutAlgorithm (const PropertyContext & context):PropertyAlgorithm<PointType , LineType>(context) {
  layoutResult = (LayoutProperty *)context.propertyProxy;
}

LayoutAlgorithm::~LayoutAlgorithm() {}
