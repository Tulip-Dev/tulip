#ifndef TLP_GEO_DELAUNAY_H
#define TLP_GEO_DELAUNAY_H
#include <vector>
#include <tulip/Coord.h>
#include <tulip/Node.h>

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
     * \author : David Auber/Daniel Archambault : auber@tulip-software.org
     * 
     * \version 0.0.1 02/06/2005
     */
  /**
   * Compute the delaunay triangulation and return the set of edges of the 
   * triangulation in the vector edges.  Edges are defined using a pair of 
   * indexes into the original set
   * of points.
   */
  TLP_SCOPE void delaunayTriangulation (const std::vector<Coord> &points, 
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

  class VoronoiDiagram {
  public:
    //A voronoi vertex.
    typedef Coord Vertex;

    //a edge with endpoints p1 and p2
    class Edge {
    public:
      unsigned int p1;
      unsigned int p2;
    };
    
    //a ray with endpoint p and vector v.
    class Ray {
    public:
      unsigned int p;
      Coord v;
    };
    
    //A counterclockwise cell with zero or two rays.  Vertices
    //are voronoi vertices
    class Cell {
    public:
      std::vector<unsigned int> vertices;
      std::vector<unsigned int> rays;
      inline bool hasRays () { return !rays.empty(); }
    };

    //Stores listst of each of these types defining the voronoi diagram
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;
    std::vector<Ray> rays;
    std::vector<Cell> cells;
  };
  
  /**
   * Compute the voronoi diagram of a set of points.  The set of input
   * points are given in points.  The resultant voronoi diagram is returned
   * in voronoiDiagram.  It automatically computes the set of all voronoi
   * vertices, rays, and returns the set of cells in counterclockwise order.
   * If a voronoi edge list is required set returnVoronoiEdgeList to true.
   */
  void voronoiDiagram (const std::vector<Coord> &points,
		       VoronoiDiagram &voronoiDiagram, 
		       bool returnVoronoiEdgeList = false);

    /*@}*/
}
#endif
