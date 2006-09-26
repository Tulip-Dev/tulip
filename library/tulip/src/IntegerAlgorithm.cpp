//-*-c++-*-
#include "tulip/IntegerProperty.h"
#include "tulip/Graph.h"

tlp::IntegerAlgorithm::IntegerAlgorithm (const tlp::PropertyContext & context):tlp::PropertyAlgorithm<tlp::IntegerType , tlp::IntegerType>(context) {
  integerResult =(tlp::IntegerProperty *)context.propertyProxy;
}

