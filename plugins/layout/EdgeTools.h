#ifndef EDGETOOLS_H
#define EDGETOOLS_H

#include "OrientableCoord.h"

class OrientableLayout;
class OrientableSizeProxy;
class Graph;

void setOrthogonalEdge(OrientableLayout* oriLayout, const Graph* tree,
					   float interNodeDistance);

void setOrthogonalEdge(OrientableLayout* oriLayout,
                       OrientableSizeProxy* oriSize, const Graph* tree,
                       float interNodeDistance);
                       
void addControlPoints(OrientableLayout* oriLayout, OrientableSizeProxy* oriSize,
                      const Graph* tree, OrientableCoord fatherCoord,
                      edge e, float interNodeDistance);

#endif
