/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#ifndef _DRAWINGTOOLS_H
#define _DRAWINGTOOLS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <vector>
#include <tulip/Coord.h>
#include <tulip/BoundingBox.h>
#include <tulip/Iterator.h>

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
 * Compute the bounding box of graph elements according to node position edge bends
 * node z-rotation, and size of elements
 *
 * Iterator itN and itE will be deleted after the computations (ie. no need to delete them yourself)
 */
TLP_SCOPE BoundingBox computeBoundingBox(Iterator<node> *itN,
										 Iterator<edge> *itE,
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
