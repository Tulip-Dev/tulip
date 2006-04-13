//-*-c++-*-

#include "tulip/Layout.h"
#include "tulip/SuperGraph.h"
#include "tulip/PluginContext.h"
#include "tulip/Layout.h"

LayoutAlgorithm::LayoutAlgorithm (const PropertyContext & context):Property<PointType , LineType>(context) {
  layoutObj = (Layout *)context.propertyProxy;
}

LayoutAlgorithm::~LayoutAlgorithm() {}
