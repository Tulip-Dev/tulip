//-*-c++-*-
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/GraphAlgorithm.h"
#include "tulip/Graph.h"
#include "tulip/GraphProperty.h"

tlp::GraphAlgorithm::GraphAlgorithm (const tlp::PropertyContext &context):tlp::PropertyAlgorithm<tlp::GraphType , tlp::GraphType>(context) {
  graphResult =(tlp::GraphProperty *)context.propertyProxy;
}
