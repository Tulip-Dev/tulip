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
  * add a "Circle computeBoundingCircle(...)" function (the code to compute it correctly is already available in Circle.h)
  *
  */

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
  * @todo make that fucntion return a new type of class called BoundingSphere
  * @todo test if that function correctly compute a bounding sphere.
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
