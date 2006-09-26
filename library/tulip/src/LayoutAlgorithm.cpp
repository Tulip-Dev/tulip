//-*-c++-*-

#include "tulip/LayoutProperty.h"
#include "tulip/Graph.h"
#include "tulip/PluginContext.h"

tlp::LayoutAlgorithm::LayoutAlgorithm (const tlp::PropertyContext & context):tlp::PropertyAlgorithm<tlp::PointType , tlp::LineType>(context) {
  layoutResult = (tlp::LayoutProperty *)context.propertyProxy;
}

tlp::LayoutAlgorithm::~LayoutAlgorithm() {}
