#include "tulip/StlFunctions.h"
#include "tulip/DoubleProperty.h"

bool tlp::LessByMetric::operator() (tlp::node n1, tlp::node n2) {
  return (metric->getNodeValue(n1) < metric->getNodeValue(n2));
}
bool tlp::LessByMetric::operator() (tlp::edge e1, tlp::edge e2) {
  return (metric->getEdgeValue(e1) < metric->getEdgeValue(e2));
}
