#ifndef EDGETOOLS_H
#define EDGETOOLS_H

#include "OrientableCoord.h"

class OrientableLayout;
class OrientableSizeProxy;
class SuperGraph;

void setOrthogonalEdge(OrientableLayout* oriLayout, const SuperGraph* tree,
					   float interNodeDistance);

void setOrthogonalEdge(OrientableLayout* oriLayout,
                       OrientableSizeProxy* oriSize, const SuperGraph* tree,
                       float interNodeDistance);
                       
void addControlPoints(OrientableLayout* oriLayout, OrientableSizeProxy* oriSize,
                      const SuperGraph* tree, OrientableCoord fatherCoord,
                      edge e, float interNodeDistance);

#endif
