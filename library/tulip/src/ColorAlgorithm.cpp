//-*-c++-*-

#include "tulip/Types.h"
#include "tulip/ColorAlgorithm.h"
#include "tulip/Graph.h"
#include "tulip/ColorProperty.h"

ColorAlgorithm::ColorAlgorithm (const PropertyContext &context):PropertyAlgorithm<ColorType , ColorType>(context) {
  colorResult = (ColorProperty *)context.propertyProxy;
}

ColorAlgorithm::~ColorAlgorithm() {
}
