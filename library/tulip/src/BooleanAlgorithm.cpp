//-*-c++-*-
#include "tulip/BooleanAlgorithm.h"
#include "tulip/Graph.h"
#include "tulip/BooleanProperty.h"

BooleanAlgorithm::BooleanAlgorithm (const PropertyContext & context):PropertyAlgorithm<BooleanType , BooleanType>(context) {
  booleanResult = (BooleanProperty *)context.propertyProxy;
}
