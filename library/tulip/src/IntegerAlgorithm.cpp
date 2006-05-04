//-*-c++-*-
#include "tulip/IntegerProperty.h"
#include "tulip/Graph.h"

IntegerAlgorithm::IntegerAlgorithm (const PropertyContext & context):PropertyAlgorithm<IntegerType , IntegerType>(context) {
  integerResult =(IntegerProperty *)context.propertyProxy;
}

