//-*-c++-*-

#include "tulip/LayoutProperty.h"
#include "tulip/Graph.h"
#include "tulip/PluginContext.h"

tlp::LayoutAlgorithm::LayoutAlgorithm (const tlp::PropertyContext & context):tlp::PropertyAlgorithm(context) {
  layoutResult = (tlp::LayoutProperty *)context.propertyProxy;
}

tlp::LayoutAlgorithm::~LayoutAlgorithm() {}
