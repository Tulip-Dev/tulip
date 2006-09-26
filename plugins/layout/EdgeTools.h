#ifndef EDGETOOLS_H
#define EDGETOOLS_H

#include "OrientableCoord.h"

class OrientableLayout;
class OrientableSizeProxy;

namespace tlp {
class Graph;
}

void setOrthogonalEdge(OrientableLayout* oriLayout, const tlp::Graph* tree,
					   float interNodeDistance);

void setOrthogonalEdge(OrientableLayout* oriLayout,
                       OrientableSizeProxy* oriSize, const tlp::Graph* tree,
                       float interNodeDistance);
                       
void addControlPoints(OrientableLayout* oriLayout, OrientableSizeProxy* oriSize,
                      const tlp::Graph* tree, OrientableCoord fatherCoord,
                      tlp::edge e, float interNodeDistance);

#endif
