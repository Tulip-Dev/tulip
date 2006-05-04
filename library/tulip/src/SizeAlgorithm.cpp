//-*-c++-*-

#include "tulip/Types.h"
#include "tulip/SizeAlgorithm.h"
#include "tulip/Graph.h"
#include "tulip/SizeProperty.h"

SizeAlgorithm::SizeAlgorithm (const PropertyContext &context):PropertyAlgorithm<SizeType , SizeType>(context) {
  sizeResult = (SizeProperty *)context.propertyProxy;
}
