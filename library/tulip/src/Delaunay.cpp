#include <set>
#include <map>
#include <iostream>
#include <tulip/Delaunay.h>
#define ANSI_DECLARATORS //ansi style declarations for triangle.h
#include "thirdparty/triangle/triangle.h"

using namespace std;
using namespace triangle_1_5;

//==============================================================
//a function to initialize memory before a triangle call
//triangleInput is the "triangle" algorithm input structure while
//triangleOutput is the output structure
inline void initTriangle (triangulateio &triangleInput,
			  triangulateio &triangleOutput) {
  triangleInput.pointmarkerlist = NULL;
  triangleInput.numberofpointattributes = 0;
  triangleInput.numberoftriangleattributes = 0;
  triangleOutput.pointlist = NULL;
  triangleOutput.pointmarkerlist = NULL;
  triangleOutput.numberofpointattributes = 0;
  triangleOutput.pointattributelist = NULL; 
  triangleOutput.trianglelist = NULL;
  triangleOutput.triangleattributelist = NULL;
  triangleOutput.trianglearealist = NULL;
  triangleOutput.neighborlist = NULL;
  triangleOutput.segmentlist = NULL;
  triangleOutput.segmentmarkerlist = NULL;
  triangleOutput.holelist = NULL;
  triangleOutput.regionlist = NULL;
  triangleOutput.edgelist = NULL;
  triangleOutput.edgemarkerlist = NULL;
  triangleOutput.normlist = NULL;
}//end initTriangle

//a function to clean up memory after a triangle call
inline void cleanupTriangle (triangulateio &triangleOutput) {
  if (triangleOutput.pointlist != NULL)
    free (triangleOutput.pointlist);
  if (triangleOutput.pointattributelist != NULL) 
    free (triangleOutput.pointattributelist);
  if (triangleOutput.pointmarkerlist != NULL) 
    free (triangleOutput.pointmarkerlist);
  if (triangleOutput.trianglelist != NULL)
    free (triangleOutput.trianglelist);
  if (triangleOutput.triangleattributelist != NULL)
    free (triangleOutput.triangleattributelist);
  if (triangleOutput.trianglearealist != NULL)
    free (triangleOutput.trianglearealist);
  if (triangleOutput.neighborlist != NULL)
    free (triangleOutput.neighborlist);
  if (triangleOutput.segmentlist != NULL)
    free (triangleOutput.segmentlist);
  if (triangleOutput.segmentmarkerlist != NULL)
    free (triangleOutput.segmentmarkerlist);
  if (triangleOutput.holelist != NULL)
    free (triangleOutput.holelist);
  if (triangleOutput.regionlist != NULL)
    free (triangleOutput.regionlist);
  if (triangleOutput.edgelist != NULL)
    free (triangleOutput.edgelist);
  if (triangleOutput.edgemarkerlist != NULL)
    free (triangleOutput.edgemarkerlist);
  if (triangleOutput.normlist != NULL)
    free (triangleOutput.normlist);
}//end cleanupTriangle

//A function to compute the triangulation using triangle.  The
//solution is written into triangleOutput.  The two booleans indicate
//if the edge list or the triangle list should be computed.
void runTriangle (const vector<Coord> &points, bool genEdges,
		  vector<pair<unsigned int, unsigned int> >&edgeList,
		  bool genTriangles,
		  vector< tlp::Array<unsigned int, 3> > &triangleList) {
  //set up the input for triangle
  REAL pointlist [2*points.size()];
  unsigned int pointCounter = 0;
  vector<Coord>::const_iterator it = points.begin();
  for (; it != points.end(); ++it) {
    pointlist[pointCounter++] = (*it).getX();
    pointlist[pointCounter++] = (*it).getY();
  }//end for
  
  //flags for triangle 'z' option allows 0 indexing of arrays and
  //the Q flag allows for quiet operation of triangle.
  //P, N, E, suppresses text file output
  //then make the triangle call
  triangulateio triangleInput, triangleOutput;
  initTriangle (triangleInput, triangleOutput);
  triangleInput.numberofpoints = points.size();
  triangleInput.pointlist = pointlist;
  string triangleInputFlags ("zQPB");
  if (genEdges) triangleInputFlags += string ("e");
    triangulate (const_cast<char *> (triangleInputFlags.c_str()), 
		 &triangleInput, &triangleOutput, NULL);
  
  //generate the triangle list if required
  if (genTriangles) {
    for (int i = 0; i < triangleOutput.numberoftriangles; ++i) {
      tlp::Array<unsigned int, 3> curTriangle;
      curTriangle[0] = triangleOutput.trianglelist[3*i];
      curTriangle[1] = triangleOutput.trianglelist[3*i + 1];
      curTriangle[2] = triangleOutput.trianglelist[3*i + 2];
      triangleList.push_back (curTriangle);
    }//end for
  }//end if
  
  //generate the edgelist if required
  if (genEdges) {
    for (int i = 0; i < triangleOutput.numberofedges; ++i) {
      pair<unsigned int, unsigned int> curEdge;
      curEdge.first = triangleOutput.edgelist[2*i];
      curEdge.second = triangleOutput.edgelist[2*i + 1];
      edgeList.push_back (curEdge);
    }//end for i
  }//end if
  
  //clean up memory allocated by triangle
  cleanupTriangle (triangleOutput);
}

//==============================================================
void tlp::delaunayTriangulation (const std::vector<Coord> &points, 
				 vector<pair<unsigned int, unsigned int> > &edges) {
  if (points.size() < 3) return; //cannot triangulate less than three points
  vector< tlp::Array<unsigned int, 3> > triangles;
  runTriangle (points, true, edges, false, triangles);
}
//==============================================================
void tlp::delaunayTriangulation (const std::vector<Coord> &points, 
		       vector< tlp::Array<unsigned int, 3> > &triangles) {
  if (points.size() < 3) return; //cannot triangulate less than three points
  vector<pair<unsigned int, unsigned int> > edges;
  runTriangle (points, false, edges, true, triangles);
}
//==============================================================
void tlp::delaunayTriangulation (const std::vector<Coord> &points,
		       vector<pair<unsigned int, unsigned int> > &edges,
		       vector< tlp::Array<unsigned int, 3> > &triangles) {
  if (points.size() < 3) return; //cannot triangulate less than three points
  runTriangle (points, true, edges, true, triangles);
}
