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
#include <float.h>
#include <tulip/Delaunay.h>
#include <iostream>
// Triangle from Jonathan Richard Shewchuk
// is no longer used by Tulip
//#define ANSI_DECLARATORS //ansi style declarations for triangle.h
//#include "thirdparty/triangle/triangle.h"
#ifdef _WIN32
#include <float.h>
#endif
#ifdef LINUX
#include <fpu_control.h> //assume compiling for linux
#endif


using namespace std;
using namespace tlp;
// Triangle from Jonathan Richard Shewchuk
// is no longer used by Tulip
//using namespace triangle_1_5;

#define FLOAT_ERROR (1e-6) 
#define TRIANGLE_NULL (-1)

//==============================================================
//the following inline function of cross product ensures we
//ignore the z-coordinate. 
inline double cross (Coord v1, Coord v2) {
  v1.setZ (0.0);
  v2.setZ (0.0);
  return ((v1^v2)[2]);
}//end cross

//==============================================================
//the following inline function of dot product ensures we
//ignore the z-coordinate. 
inline double dot (Coord v1, Coord v2) {
  return v1.getX()*v2.getX() + v1.getY()*v2.getY();
}//end cross

//==============================================================
//A fuction to copy an edgelist from an integer list of edge indexes
//src to a destination vector of pair objects
inline void copyToEdgeList (const int *src, int numberofedges,
			    vector<pair<unsigned int, unsigned int> > *dest) {
  for (int i = 0; i < numberofedges; ++i) {
    pair<unsigned int, unsigned int> curEdge;
    curEdge.first = src[2*i];
    curEdge.second = src[2*i + 1];
    dest->push_back (curEdge);
  }//end for i
}//end copyEdgeList

//==============================================================
//A function to copy a triangle list from an integer list of indexes src
//to a vector of arrays of three vertex indexes
inline void copyToTriangleList (const int *src, int numberoftriangles,
				vector<tlp::Array<unsigned int, 3> > *dest) {
  for (int i = 0; i < numberoftriangles; ++i) {
    tlp::Array<unsigned int, 3> curTriangle;
    curTriangle[0] = src[3*i];
    curTriangle[1] = src[3*i + 1];
    curTriangle[2] = src[3*i + 2];
    dest->push_back (curTriangle);
  }//end for
}//end copyTriangleList

//==============================================================
//a function to copy the vertices of of points to the 
//voronoi diagram structure
inline void copyVorVertexToVorDiagram (const double *points, int numberofpoints,
				       tlp::VoronoiDiagram *voronoiDiagram) {
  //load the voronoi vertices of the diagram
  for (int i = 0; i < numberofpoints; ++i) {
    tlp::VoronoiDiagram::Vertex vertex;
    vertex.setX (points[2*i]);
    vertex.setY (points[2*i + 1]);
    vertex.setZ (0.0);
    voronoiDiagram->vertices.push_back (vertex);
  }//end for
}//end copyToVoronoiLists

//==============================================================
//A fuction to copy a voronoi edge list src to the voronoi diagram
inline void  copyVorEdgeAndRaysToVorDiagram (const int *edgelist,
					     const double *normlist,
					     const int numberofedges,
					     const bool computeVoronoiEdgeList,
					     tlp::VoronoiDiagram *voronoiDiagram) {
  for (int i = 0; i < numberofedges; ++i) {
    //load any rays into the ray list
    if ((edgelist[2*i] == TRIANGLE_NULL) || (edgelist[2*i + 1] == TRIANGLE_NULL)) {
      tlp::VoronoiDiagram::Ray curRay;
      curRay.p = (edgelist[2*i] != TRIANGLE_NULL) ? 
	edgelist[2*i] : edgelist[2*i + 1];
      curRay.v.setX (normlist[2*i]);
      curRay.v.setY (normlist[2*i + 1]);
      curRay.v.setZ (0.0);
      if (curRay.v.norm() > FLOAT_ERROR) curRay.v /= curRay.v.norm();
      voronoiDiagram->rays.push_back (curRay);
    }//end if
    //load edges into the edge list if required
    else if (computeVoronoiEdgeList) {
      tlp::VoronoiDiagram::Edge curEdge;
      curEdge.p1 = edgelist[2*i];
      curEdge.p2 = edgelist[2*i + 1];
      voronoiDiagram->edges.push_back (curEdge);
    }//end if
  }//end for i
}//end copyVorEdgeToVorDiagram

//==============================================================
//a function to reverse the order of the vertices and the rays in the cell
void reverseCellOrder (tlp::VoronoiDiagram::Cell &voronoiCell) {
  const vector<unsigned int> reverse = voronoiCell.vertices;
  voronoiCell.vertices.clear();
  for (vector<unsigned int>::const_reverse_iterator reverseit = reverse.rbegin();
       reverseit != reverse.rend(); ++reverseit)
    voronoiCell.vertices.push_back (*reverseit);
  if (voronoiCell.hasRays()) {
    unsigned int tempRay = voronoiCell.rays[0];
    voronoiCell.rays[0] = voronoiCell.rays[1];
    voronoiCell.rays[1] = tempRay;
  }//end if
}//end reverseCellOrder

//==============================================================
//An inline function to make the voronoi diagram of two vertices.
inline void voronoiOfTwo (const vector<Coord> &points,
			  tlp::VoronoiDiagram *voronoiDiagram) {
  voronoiDiagram->vertices.push_back ((points[0] + points[1])/2.0f);
  tlp::VoronoiDiagram::Ray ray;
  ray.p = 0;
  Coord edge = points[1] - points[0];
  if (edge.norm() > FLOAT_ERROR) edge /= edge.norm();
  ray.v.setX (edge.getY()); 
  ray.v.setY (-edge.getX());
  voronoiDiagram->rays.push_back (ray);
  ray.v *= -1.0;
  voronoiDiagram->rays.push_back (ray);
  tlp::VoronoiDiagram::Cell cell;
  cell.vertices.push_back (0);
  cell.rays.push_back(0);
  cell.rays.push_back(1);
  voronoiDiagram->cells.push_back (cell);
  cell.rays[0] = 1;
  cell.rays[1] = 0;
  voronoiDiagram->cells.push_back (cell);
}//end voronoiOfTwo

//==============================================================
// Triangle from Jonathan Richard Shewchuk
// is no longer used by Tulip
/*
//==============================================================
//inits the tritoRay datastructure, which given a triangle returns
//the ray associated with each edge or TRIANGLE_NULL
void initTriToRay (int *triToRay, const vector<Coord> &points, 
		   const triangulateio &triangleOutput, 
		   tlp::VoronoiDiagram *voronoiDiagram) {
  for (int i = 0; i < 3*triangleOutput.numberoftriangles; i++)
    triToRay[i] = TRIANGLE_NULL;
  unsigned int rayCounter = 0;
  for (vector<tlp::VoronoiDiagram::Ray>::iterator 
	 rayit = voronoiDiagram->rays.begin(); 
       rayit != voronoiDiagram->rays.end(); ++rayit, ++rayCounter) {
    const int a = triangleOutput.trianglelist[3*rayit->p];
    const int b = triangleOutput.trianglelist[3*rayit->p + 1];
    const int c = triangleOutput.trianglelist[3*rayit->p + 2];
    const int bc = triangleOutput.neighborlist[3*rayit->p];
    const int ca = triangleOutput.neighborlist[3*rayit->p + 1];
    const int ab = triangleOutput.neighborlist[3*rayit->p + 2];
    
    //look for the edge of the triangle that is perpendicular to the ray
    unsigned int offset = 0;
    double minDot = DBL_MAX;
    if ((bc == TRIANGLE_NULL) && 
	(fabs (dot (rayit->v, points[c] - points[b])) < minDot)) {
      offset = 0;
      minDot = fabs (dot (rayit->v, points[c] - points[b]));
    }//end if
    if ((ca == TRIANGLE_NULL) && 
	(fabs (dot (rayit->v, points[a] - points[c])) < minDot)) {
      offset = 1;
      minDot = fabs (dot (rayit->v, points[a] - points[c]));
    }//end else if
    if ((ab == TRIANGLE_NULL) && 
	(fabs (dot (rayit->v, points[b] - points[a])) < minDot))
      offset = 2;
    triToRay[3*rayit->p + offset] = rayCounter;
  }//end for  
}//end initTriToRay

//==============================================================
//a function to get the next the adjacent unvisited voronoi vertex
int getUnvisited (const int voronoiCentre,
		  int visitedVoronoiVertex,
		  int curVoronoiVertex,
		  const triangulateio &triangleOutput) {
  const int a = triangleOutput.trianglelist[3*curVoronoiVertex];
  const int b = triangleOutput.trianglelist[3*curVoronoiVertex + 1];
  const int c = triangleOutput.trianglelist[3*curVoronoiVertex + 2];
  const int bc = triangleOutput.neighborlist[3*curVoronoiVertex];
  const int ca = triangleOutput.neighborlist[3*curVoronoiVertex + 1];
  const int ab = triangleOutput.neighborlist[3*curVoronoiVertex + 2];

  //visit the voronoi vertex we are not currently at
  if ((voronoiCentre == a) && (ab == visitedVoronoiVertex)) return ca;
  else if ((voronoiCentre == a) && (ca == visitedVoronoiVertex)) return ab;
  else if ((voronoiCentre == b) && (ab == visitedVoronoiVertex)) return bc;
  else if ((voronoiCentre == b) && (bc == visitedVoronoiVertex)) return ab;
  else if ((voronoiCentre == c) && (bc == visitedVoronoiVertex)) return ca;
  else return bc; //the c, ca option
}//end getUnvisited

//==============================================================
//sets up the infinite ray 
void setInfiniteEdges (const int voronoiCentre,
		       int curVoronoiVertex,
		       const triangulateio &triangleOutput, 
		       tlp::VoronoiDiagram::Cell &voronoiCell,
		       const int *triToRay,
		       tlp::VoronoiDiagram *voronoiDiagram) {
  const int a = triangleOutput.trianglelist[3*curVoronoiVertex];
  const int b = triangleOutput.trianglelist[3*curVoronoiVertex + 1];
  const int c = triangleOutput.trianglelist[3*curVoronoiVertex + 2];
  const int bc = triangleOutput.neighborlist[3*curVoronoiVertex];
  const int ca = triangleOutput.neighborlist[3*curVoronoiVertex + 1];
  const int ab = triangleOutput.neighborlist[3*curVoronoiVertex + 2];
  unsigned int rayIndices[2];
  
  //get the index into the rays vector for the first ray
  unsigned int numRays = 0;
  if ((voronoiCentre != c) && (ab == TRIANGLE_NULL))
    rayIndices[numRays++] = triToRay[3*curVoronoiVertex + 2];
  if ((voronoiCentre != a) && (bc == TRIANGLE_NULL))
    rayIndices[numRays++] = triToRay[3*curVoronoiVertex];
  if ((voronoiCentre != b) && (ca == TRIANGLE_NULL))
    rayIndices[numRays++] = triToRay[3*curVoronoiVertex + 1];
  voronoiCell.rays.push_back (rayIndices[0]);

  //if we have detected multiple rays in a triangle
  if (numRays > 1) {
    voronoiCell.rays.push_back (rayIndices[1]);
    //single vertex unbounded polygon travels through voronoi vertex first 
    if (cross (voronoiDiagram->rays[rayIndices[0]].v, 
	       voronoiDiagram->rays[rayIndices[1]].v) >= -FLOAT_ERROR) {
      voronoiCell.rays[0] = rayIndices[1];
      voronoiCell.rays[1] = rayIndices[0];
    }//end if
  }//end if
}//end setInfiniteEdge

//==============================================================
//a function to walk the boundary of a cell.  It returns true
//if it traversed the boundary counterclockwise.  It will update
//the pointers to curVoronoiVertex and nextVoronoiVertex to indicate
//if we have discovered an unbounded polygon.
bool counterclockwiseTraversal (const int voronoiCentre,
				int &curVoronoiVertex,
				int &nextVoronoiVertex,
				const triangulateio &triangleOutput,
				vector<unsigned int> &vertices,
				tlp::VoronoiDiagram *voronoiDiagram) {
  bool counterclockwise = true;
  const int startingVoronoiVertex = curVoronoiVertex;
  while ((nextVoronoiVertex != startingVoronoiVertex) &&
 	 (nextVoronoiVertex != TRIANGLE_NULL)) {
    int unvisitedVoronoiVertex = 
      getUnvisited (voronoiCentre, curVoronoiVertex,
		    nextVoronoiVertex, triangleOutput);
    if (unvisitedVoronoiVertex != TRIANGLE_NULL) {
      Coord visited = voronoiDiagram->vertices[curVoronoiVertex];
      Coord next = voronoiDiagram->vertices[nextVoronoiVertex];
      Coord unvisited = voronoiDiagram->vertices[unvisitedVoronoiVertex];
      counterclockwise &= cross (next - visited, 
				 unvisited - next) >= -FLOAT_ERROR;
    }//end if
    curVoronoiVertex = nextVoronoiVertex;
    nextVoronoiVertex = unvisitedVoronoiVertex;
    vertices.push_back (curVoronoiVertex);
  }//end while
  return counterclockwise;
}//end counterclockwiseBoundaryTraversal

//==============================================================
//a function to trace out the voronoi cell in counterclockwise order
void traceCell (const int voronoiCentre,
		const int startingVoronoiVertex,
		const int startingNextVoronoiVertex,
		const triangulateio &triangleOutput,
		tlp::VoronoiDiagram::Cell &voronoiCell,
		const int *triToRay,
		tlp::VoronoiDiagram *voronoiDiagram) {
  bool counterclockwise;
  int curVoronoiVertex = startingVoronoiVertex;
  int nextVoronoiVertex = startingNextVoronoiVertex;
  voronoiCell.vertices.push_back (curVoronoiVertex);
  counterclockwise = counterclockwiseTraversal (voronoiCentre,
						curVoronoiVertex,
						nextVoronoiVertex,
						triangleOutput,
						voronoiCell.vertices,
						voronoiDiagram);

  //if this is an unbounded cell
  if (nextVoronoiVertex == TRIANGLE_NULL) {
    //record the first ray we have detected
    setInfiniteEdges (voronoiCentre, curVoronoiVertex, triangleOutput, 
		      voronoiCell, triToRay, voronoiDiagram);
    curVoronoiVertex = startingVoronoiVertex;
    nextVoronoiVertex =
      getUnvisited (voronoiCentre, startingNextVoronoiVertex, 
		    curVoronoiVertex, triangleOutput);
    
    //traverse in the other direction to the other ray
    vector<unsigned int> reverse;
    counterclockwise &= !counterclockwiseTraversal (voronoiCentre,
						    curVoronoiVertex,
						    nextVoronoiVertex,
						    triangleOutput,
						    reverse,
						    voronoiDiagram);

    //if we have not recorded the other ray of this unbounded cell
    if (voronoiCell.rays.size() == 1)
      setInfiniteEdges (voronoiCentre, curVoronoiVertex, triangleOutput, 
			voronoiCell, triToRay, voronoiDiagram);
    vector<unsigned int> cellBackup = voronoiCell.vertices;
    voronoiCell.vertices = reverse;
    reverseCellOrder (voronoiCell);
    for (vector<unsigned int>::iterator otherhalfit = cellBackup.begin();
	 otherhalfit != cellBackup.end(); ++otherhalfit)
      voronoiCell.vertices.push_back (*otherhalfit);

    //check the infinite ray against the first vector to make sure 
    //the vertex chain is counterclockwise
    if (voronoiCell.vertices.size() > 1) {
      Coord firstVec = voronoiDiagram->vertices[voronoiCell.vertices[1]] -
	voronoiDiagram->vertices[voronoiCell.vertices[0]];
      Coord voronoiRayVec = voronoiDiagram->rays[voronoiCell.rays[0]].v;
      voronoiRayVec *= -1.0;
      counterclockwise = cross (voronoiRayVec, firstVec) >= -FLOAT_ERROR;
    }//end if
  }//end if
  
  //if clocwise order is detected, reverse the order of the vertices
  if (!counterclockwise) reverseCellOrder (voronoiCell);
}//end trace cell
  
  //==============================================================
//a function to compute the counterclockwise voronoi cells as 
//pointers into the voronoi edge list.
void computeVoronoiCells (const vector<Coord> &points,
			  const triangulateio &triangleOutput,
			  const int *triToRay,
			  tlp::VoronoiDiagram *voronoiDiagram) {
  //initialize all of the cells of the voronoi diagram cell list
  voronoiDiagram->cells.resize (points.size());
  bool cellDone [points.size()];
  for (unsigned int i = 0; i < points.size(); i++) cellDone[i] = false; 

  for (int tri = 0; tri < triangleOutput.numberoftriangles; tri++) {
    //get the three adjacent vertices and triangles
    const int a = triangleOutput.trianglelist[3*tri];
    const int b = triangleOutput.trianglelist[3*tri + 1];
    const int c = triangleOutput.trianglelist[3*tri + 2];
    const int bc = triangleOutput.neighborlist[3*tri];
    const int ca = triangleOutput.neighborlist[3*tri + 1];
    const int ab = triangleOutput.neighborlist[3*tri + 2];

    if (!cellDone[a])
      traceCell (a, tri, ca, triangleOutput, 
		 voronoiDiagram->cells[a], triToRay, voronoiDiagram);
    if (!cellDone[b])
      traceCell (b, tri, ab, triangleOutput, 
		 voronoiDiagram->cells[b], triToRay, voronoiDiagram);
    
    if (!cellDone[c])
      traceCell (c, tri, bc, triangleOutput,
		 voronoiDiagram->cells[c], triToRay, voronoiDiagram);
    cellDone[a] = cellDone[b] = cellDone[c] = true;
  }//end for
}//end computeVoronoiCells

//==============================================================
//a function to initialize memory of a triangle input structure
//before a triangle call triangleInput is the "triangle" algorithm input 
//structure
inline void inputInitTriangle (triangulateio &triangleInput) {
  triangleInput.pointmarkerlist = NULL;
  triangleInput.numberofpointattributes = 0;
  triangleInput.numberoftriangleattributes = 0;
}//end initTriangle

//==============================================================
//a function to initialize memory of a triangle output structure
//before a triangle call triangleOutput is the "triangle" algorithm input 
//structure
inline void outputInitTriangle (triangulateio &triangleOutput) {
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
}//end outputInitTriangle

//==============================================================
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

//==============================================================
//A function to compute the triangulation using triangle.  The
//solution is written into triangleOutput.  The two booleans indicate
//if the edge list or the triangle list should be computed.
void runTriangle (const vector<Coord> &points,
		  vector<pair<unsigned int, unsigned int> > *edgeList = NULL,
		  vector<tlp::Array<unsigned int, 3> > *triangleList = NULL,
		  bool computeVoronoiEdgeList = false,
		  tlp::VoronoiDiagram *voronoiDiagram = NULL) {

  //cannot triangulate less than three points
  if (points.size() < 3)  {
    if (points.size() == 2) {
      if (edgeList != NULL)
	edgeList->push_back (pair<unsigned int, unsigned int> (0, 1));
      if (voronoiDiagram != NULL) voronoiOfTwo (points, voronoiDiagram);
    }//end if
    return; 
  }//end if

  //set up the input for triangle
  double pointlist [2*points.size()];
  unsigned int pointCounter = 0;
  vector<Coord>::const_iterator it = points.begin();
  for (; it != points.end(); ++it) {
    pointlist[pointCounter++] = it->getX();
    pointlist[pointCounter++] = it->getY();
  }//end for
  
  //flags for triangle 'z' option allows 0 indexing of arrays and
  //the Q flag allows for quiet operation of triangle.
  //P, N, E, suppresses text file output
  //then make the triangle call
  triangulateio triangleInput, triangleOutput, voronoiOutput;
  inputInitTriangle (triangleInput);
  outputInitTriangle (triangleOutput);
  outputInitTriangle (voronoiOutput);
  triangleInput.numberofpoints = points.size();
  triangleInput.pointlist = pointlist;
  string triangleInputFlags ("zQPB");

  //e flag generates edge list
  if (edgeList != NULL) triangleInputFlags += string ("e");
  //e for edge list n for neighbour triangles, v for voronoi diagrams
  if (voronoiDiagram != NULL) triangleInputFlags += string ("env");

  //The fpu mode is changed by triangle.  Backup the current fpu mode
  //If we don't reset, floating point exceptions may occur later on in
  //Tulip execution (and they are very hard to debug).
  unsigned int fpu_mode = 0;
#ifdef _WIN32
  fpu_mode = _control87 (0, 0);
#endif
#ifdef LINUX
  _FPU_GETCW (fpu_mode);
#endif
  triangulate (const_cast<char *> (triangleInputFlags.c_str()), 
	       &triangleInput, &triangleOutput, &voronoiOutput);
  //reset the fpu mode to what it was before
#ifdef _WIN32
  _control87 (fpu_mode, 0x000fffff);
#endif
#ifdef LINUX
  _FPU_SETCW (fpu_mode);
#endif

  //generate the triangle list if required
  if (triangleList != NULL)
    copyToTriangleList (triangleOutput.trianglelist, 
			triangleOutput.numberoftriangles,
			triangleList);
  
  //generate the edgelist if required
  if (edgeList != NULL) 
    copyToEdgeList (triangleOutput.edgelist, triangleOutput.numberofedges,
		    edgeList);
				      

  //generate the voronoi diagram if required
  if (voronoiDiagram != NULL) {
    copyVorVertexToVorDiagram (voronoiOutput.pointlist, 
			       voronoiOutput.numberofpoints,
			       voronoiDiagram); 
    copyVorEdgeAndRaysToVorDiagram (voronoiOutput.edgelist,
				    voronoiOutput.normlist,
				    voronoiOutput.numberofedges,
				    computeVoronoiEdgeList,
				    voronoiDiagram);
    int triToRay[3*triangleOutput.numberoftriangles];
    initTriToRay (triToRay, points, triangleOutput, voronoiDiagram);
    computeVoronoiCells (points, triangleOutput, triToRay, voronoiDiagram);
  }//end if
  
  //clean up memory allocated by triangle
  cleanupTriangle (triangleOutput);
  cleanupTriangle (voronoiOutput);
}//end runTriangle
*/

//==============================================================
void tlp::delaunayTriangulation (const vector<Coord> &, 
	    vector<pair<unsigned int, unsigned int> > &) {
  std::cerr << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
  //runTriangle (points, &edges);
}//end delaunayTriangulation
//==============================================================
void tlp::delaunayTriangulation (const vector<Coord> &, 
	    vector< tlp::Array<unsigned int, 3> > &) {
  std::cerr << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
  //runTriangle (points, NULL, &triangles);
}//end delaunayTriangulation
//==============================================================
void tlp::delaunayTriangulation (const vector<Coord> &,
		       vector<pair<unsigned int, unsigned int> > &,
		       vector< tlp::Array<unsigned int, 3> > &) {
  //runTriangle (points, &edges, &triangles);
}//end delaunayTriangulation
//==============================================================
void tlp::voronoiDiagram (const vector<Coord> &,
			  VoronoiDiagram &,
			  bool) {
  std::cerr << __PRETTY_FUNCTION__ << " not yet implemented" << std::endl;
  //runTriangle (points, NULL, NULL, returnVoronoiEdgeList, &voronoiDiagram);
}//end voronoiDiagram
