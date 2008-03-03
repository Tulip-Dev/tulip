//-*-c++-*-

#include "tulip/Types.h"
#include "tulip/SizeAlgorithm.h"
#include "tulip/Graph.h"
#include "tulip/SizeProperty.h"

tlp::SizeAlgorithm::SizeAlgorithm (const tlp::PropertyContext &context):tlp::PropertyAlgorithm(context) {
  sizeResult = (tlp::SizeProperty *)context.propertyProxy;
}
