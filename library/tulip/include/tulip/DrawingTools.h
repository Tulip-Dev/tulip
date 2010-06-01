//-*-c++-*-
#ifndef _DRAWINGTOOLS_H
#define _DRAWINGTOOLS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <vector>
#include <tulip/Coord.h>
#include <tulip/BoundingBox.h>

namespace tlp {

class Graph;
class LayoutProperty;
class SizeProperty;
class DoubleProperty;
class BooleanProperty;

/**
  *
  * Compute the bounding box of a graph according to node position edge bends
  * node z-rotation, and size of elements
  *
  */
TLP_SCOPE   BoundingBox computeBoundingBox(const Graph *graph,
                                           const LayoutProperty *layout,
                                           const SizeProperty *size,
                                           const DoubleProperty *rotation,
                                           const BooleanProperty *selection = 0);
/**
  *
  * Compute a bounding sphere of a graph according to node position edge bends
  * node z-rotation, and size of elements
  *
  */
TLP_SCOPE   std::pair<Coord, Coord> computeBoundingRadius (const Graph *graph,
                                                           const LayoutProperty *layout,
                                                           const SizeProperty *size,
                                                           const DoubleProperty *rotation,
                                                           const BooleanProperty *selection = 0);
/**
  *
  * Compute a convexHull of a graph according to node position edge bends
  * node z-rotation, and size of elements. Only works in 2D.
  * @todo refactor code from GlConvexHull here
  *
  */
TLP_SCOPE   std::vector<Coord> computeConvexHull (const Graph *graph,
                                                  const LayoutProperty *layout,
                                                  const SizeProperty *size,
                                                  const DoubleProperty *rotation,
                                                  const BooleanProperty *selection = 0);

}


#endif
