#include <set>
#include <map>
#include <iostream>
#include <tulip/Delaunay.h>
#include <tulip/DelaunayImpl.h>


using namespace std;
using namespace delaunayImpl;

//==============================================================
namespace {
void triangulationImpl (const std::vector<Coord> &points,
			vertexSet &pointSet,
			map<vertex, unsigned int> &vertexToIndex,
			triangleSet &triangles,
			Delaunay &triangulation)
 {
  vector<Coord>::const_iterator it = points.begin();
  unsigned int index = 0;
  for (; it != points.end(); ++it, ++index) {
    vertex vertex_tmp(it->getX(), it->getY());
    pointSet.insert(vertex_tmp);
    vertexToIndex[vertex_tmp] = index;
  }//end for
  triangulation.Triangulate(pointSet, triangles);
}
}
//==============================================================
void tlp::delaunayTriangulation (const std::vector<Coord> &points, 
			   std::vector<std::pair<unsigned int, unsigned int> > &edges) {
  vertexSet pointSet;
  map<vertex, unsigned int> vertexToIndex;
  triangleSet triangles;
  Delaunay triangulation;
  triangulationImpl (points, pointSet, vertexToIndex, triangles, triangulation);
  edgeSet edge_set;
  triangulation.TrianglesToEdges(triangles, edge_set);
  set<edge>::const_iterator ite = edge_set.begin();
  for (; ite != edge_set.end(); ++ite) {
    pair<unsigned int, unsigned int> curEdge;
    curEdge.first = vertexToIndex[*(ite->m_pV0)];    
    curEdge.second = vertexToIndex[*(ite->m_pV1)];
    edges.push_back(curEdge);
  }//end for
}
//==============================================================
void tlp::delaunayTriangulation (const std::vector<Coord> &points, 
			    std::vector< tlp::Array<unsigned int, 3> > &triangles) {
  vertexSet pointSet;
  map<vertex, unsigned int> vertexToIndex;
  triangleSet triangles_set;
  Delaunay triangulation;
  triangulationImpl (points, pointSet, vertexToIndex, triangles_set, triangulation);
  triangleSet::const_iterator it= triangles_set.begin();
  triangles.resize(triangles_set.size());
  unsigned int ii;
  for (ii=0; it != triangles_set.end() ; ++it, ++ii) {
    triangle tmp = *it;
    tlp::Array<unsigned int, 3> curTriangle;
    for (int i = 0; i<3; ++i) {
      curTriangle[i] = vertexToIndex[*tmp.GetVertex(i)];
    }//end for
    triangles[ii] = curTriangle;
  }//end for
}
//==============================================================
SuperGraph *tlp::delaunayTriangulation (SuperGraph *inputGraph, LayoutProxy *inputCoord, 
				   MutableContainer<node> &nodeMapping){
  cerr << __PRETTY_FUNCTION__ << " : NOT IMPLEMENTED." << endl;
  return 0;
}
