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
     * \brief function for convex hull manipulation
     *
     * Contains functions for computing, merging, intersecting, and
     * testing convex hulls.
     *
     * \author : Daniel Archambault archam@cs.ubc.ca,
     * 
     * \version 0.0.3 26/06/2006
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

  /**
   * Intersects two convex hulls using the algorithm of
   * O'Rourke, Chien, Olson, and Naddor, 1982.  Code inspired
   * by Computational Geometry in C by Joseph O'Rourke pp 243 -- 251.
   * and code on his website: 
   * http://maven.smith.edu/~orourke/books/compgeom.html
   * Provide a full list of points involved in the convex hull computation
   * in "points".  The vectors hull1 and hull2 contain a list of indexes 
   * into the points vector that define the vertices of the convex hull.
   * The indices corresponding to the points on the intersecting hull
   * are returned in "intersection".  Node that several points may
   * be inserted into the "points" list.  These points correspond to the
   * points generated when edges of hull1 and hull2 intersect.
   */
  void intersectHulls (std::vector<Coord> &points,
		       const std::vector<unsigned int> &hull1,
		       const std::vector<unsigned int> &hull2,
		       std::vector<unsigned int> &intersection);

  /**
   * Computes the area of the convex "hull" which is a list of indexes
   * into the vector of Coord "points".
   */
  double areaOfHull (const std::vector<Coord> &points,
		     const std::vector<unsigned int> &hull);
  
  /**
   * Returns true if "point" is inside "hull".  Hull is a list of
   * indexes into the vector of Coord "points."
   */
  bool insideHull (const std::vector<Coord> &points,
		   const std::vector<unsigned int> &hull,
		   const Coord &point);
  
  /*@}*/
}
#endif
