#include "tulip/StlFunctions.h"
#include "tulip/DoubleProperty.h"

bool LessByMetric::operator() (node n1,node n2) {
  return (metric->getNodeValue(n1) < metric->getNodeValue(n2));
}
bool LessByMetric::operator() (edge e1,edge e2) {
  return (metric->getEdgeValue(e1) < metric->getEdgeValue(e2));
}
