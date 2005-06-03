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
     * Author : <a href="mailto:auber@tulip-software.org>David Auber/Daniel Archambault</A>
     * 
     * Version 0.0.1 03/06/2005
     */
  /**
   * Compute the convex hull and return a list of indexes for the
   * points on the convex hull in counterclockwise order.
   */
  void convexHull (const std::vector<Coord> &points, 
		   std::vector<unsigned int> &convexHull);
  
  /*@}*/
}
#endif
