#ifndef TLP_GEO_CONVEX_HULL_H
#define TLP_GEO_CONVEX_HULL_H
#include <vector>
#include <tulip/Coord.h>

namespace tlp {

  /**
   * \addtogroup basic
   */ 
  /*@{*/
    /**
     * \brief function for convex hull computation
     *
     * Used to compute the convex hull of a set of points in the plane
     *
     * \author : David Auber/Daniel Archambault auber@tulip-software.org 
     * 
     * \version 0.0.1 03/06/2005
     */
  /**
   * Compute the convex hull and return a list of indexes for the
   * points on the convex hull in counterclockwise order.  The convexHull
   * vector is automatically cleared.  The algorithm runs in O(nlgn) time.
   */
  void convexHull (const std::vector<Coord> &points, 
		   std::vector<unsigned int> &convexHull);

  /**
   * Merges two convex hulls hull1 and hull2 using the rotating calipers
   * method of Pirzadeh and Touissaint.  The mergedConvexHull
   * vector is automatically cleared and the vertices of the hulls must
   * be specified in counter clockwise order.  Provide a full list of all
   * points involved in the convex hull computation in the vector points.
   * The vectors hull1 and hull2 contain a list of indexes into the
   * points vector that define the vertices of the convex hull.  The
   * indexes into points of the verticies on the merged convex hull are 
   * returned in the vector mergedConvexHull.
   */
  void mergeHulls (const std::vector<Coord> &points,
		   const std::vector<unsigned int> &hull1,
		   const std::vector<unsigned int> &hull2,
		   std::vector<unsigned int> &mergedConvexHull);
  
  /*@}*/

}
#endif
