//-*-c++-*-
#include "tulip/DoubleAlgorithm.h"
#include "tulip/Graph.h"
#include "tulip/DoubleProperty.h"

tlp::DoubleAlgorithm::DoubleAlgorithm (const tlp::PropertyContext &context):tlp::PropertyAlgorithm(context) {
  doubleResult = (tlp::DoubleProperty *)context.propertyProxy;
}

tlp::DoubleAlgorithm::~DoubleAlgorithm() {}
