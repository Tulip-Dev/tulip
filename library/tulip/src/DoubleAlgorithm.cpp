//-*-c++-*-
#include "tulip/DoubleAlgorithm.h"
#include "tulip/Graph.h"
#include "tulip/DoubleProperty.h"

DoubleAlgorithm::DoubleAlgorithm (const PropertyContext &context):PropertyAlgorithm<DoubleType , DoubleType>(context) {
  doubleResult = (DoubleProperty *)context.propertyProxy;
}

DoubleAlgorithm::~DoubleAlgorithm() {}
