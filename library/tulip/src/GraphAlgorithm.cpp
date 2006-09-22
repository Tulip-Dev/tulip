//-*-c++-*-
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/GraphAlgorithm.h"
#include "tulip/Graph.h"
#include "tulip/GraphProperty.h"

GraphAlgorithm::GraphAlgorithm (const PropertyContext &context):PropertyAlgorithm<GraphType , GraphType>(context) {
  graphResult =(GraphProperty *)context.propertyProxy;
}
