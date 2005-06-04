#ifndef TLP_GEO_DELAUNAY_H
#define TLP_GEO_DELAUNAY_H
#include <vector>
#include <tulip/Coord.h>
#include <tulip/Node.h>

class SuperGraph;
class LayoutProxy;
template<typename TYPE>
class  MutableContainer;

namespace tlp {
  /**
   * \addtogroup basic
   */ 
  /*@{*/
    /**
     * \brief functions for Delaunay Triangulations
     *
     * Used to triangulate a set of points in the plane
     *
     * Author : <a href="mailto:auber@tulip-software.org>David Auber/Daniel Archambault</A>
     * 
     * Version 0.0.1 02/06/2005
     */
  /**
   * Compute the delaunay triangulation and return the set of edges of the 
   * triangulation in the vector edges.  Edges are defined using a pair of 
   * indexes into the original set
   * of points.
   */
  void delaunayTriangulation (const std::vector<Coord> &points, 
			      std::vector<std::pair<unsigned int, unsigned int> > &edges);
  /**
   * Compute the delaunay triangulation and return the set of triangles of the 
   * triangulation in the vector triangles.  Triangles are defined using a 
   * triple indexes into the original 
   * set of points.
   */
  void delaunayTriangulation (const std::vector<Coord> &points, 
		std::vector<tlp::Array<unsigned int, 3> > &triangles);

  /**
   * Compute the delaunay triangulation and return the set of edges in the
   * vector edge and triangles of the triangulation in the vector triangles.  
   * Triangles are defined using a triple indexes into the original 
   * set of points.
   */
  
  void delaunayTriangulation (const std::vector<Coord> &points,
     std::vector<std::pair<unsigned int, unsigned int> > &edges,
     std::vector< tlp::Array<unsigned int, 3> > &triangles);

    /*@}*/
}
#endif
