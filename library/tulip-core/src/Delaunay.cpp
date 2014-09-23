/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#include <tulip/Delaunay.h>
#include <tulip/Matrix.h>
#include <tulip/Graph.h>
#include <tulip/ForEach.h>
#include <tulip/LayoutProperty.h>
#include <tulip/BoundingBox.h>
#include <tulip/DrawingTools.h>

#include <iostream>

extern "C" {
#include <libqhull/libqhull.h>
#include <libqhull/qset.h>
#include <libqhull/geom.h>
#include <libqhull/poly.h>
#include <libqhull/io.h>
}

using namespace std;
using namespace tlp;

typedef Matrix<long double, 3> Mat3ld;
typedef Vector<long double, 3> Vec3ld;

static bool runQHull(int dim, vector<double> &points, vector<pair<unsigned int, unsigned int> >&edges,
                     vector<vector<unsigned int> > &simplices) {

  // Set default options for qhull delaunay triangulation
  // - Qt : triangulated output
  // - Qbb : scale the last coordinate to [0,m] for Delaunay
  // - Qc : keep coplanar points with nearest facet
  // - Qz - add a point-at-infinity for Delaunay triangulations
  string qhullOptions = "Qt Qbb Qc Qz";
  // build qhull command
  string qhullCommand = string("qhull d ") + qhullOptions;

  // initialize qhull
  int qhullKo = qh_new_qhull(dim, points.size()/dim, &points[0], false, const_cast<char *>(qhullCommand.c_str()), NULL, stderr);

  if (!qhullKo) {

    set<pair<unsigned int, unsigned int> > placedEdges;

    // call qhull delaunay triangulation
    qh_triangulate();

    facetT *facet;
    vertexT *vertex, **vertexp;

    // iterate over generated simplices (triangles or tetrahedra) and get corresponding delaunay edges
    FORALLfacets {

      if (!facet->upperdelaunay) {
        int pointId0=0, pointId1=0, pointId2=0, pointId3 = -1;
        int i = 0;
        FOREACHvertex_ (facet->vertices) {
          if (i == 0) {
            pointId0 = qh_pointid(vertex->point);
          }
          else if (i == 1) {
            pointId1 = qh_pointid(vertex->point);
          }
          else if (i == 2) {
            pointId2 = qh_pointid(vertex->point);
          }
          else {
            pointId3 = qh_pointid(vertex->point);
          }

          ++i;
        }

        vector<unsigned int> simplex;
        pair<unsigned int, unsigned int> edge1 = make_pair(min(pointId0, pointId1), max(pointId0, pointId1));
        pair<unsigned int, unsigned int> edge2 = make_pair(min(pointId1, pointId2), max(pointId1, pointId2));
        pair<unsigned int, unsigned int> edge3 = make_pair(min(pointId2, pointId0), max(pointId2, pointId0));

        if (placedEdges.find(edge1) == placedEdges.end()) {
          edges.push_back(edge1);
          placedEdges.insert(edge1);
        }

        if (placedEdges.find(edge2) == placedEdges.end()) {
          edges.push_back(edge2);
          placedEdges.insert(edge2);
        }

        if (placedEdges.find(edge3) == placedEdges.end()) {
          edges.push_back(edge3);
          placedEdges.insert(edge3);
        }

        simplex.reserve(3);
        simplex.push_back(pointId0);
        simplex.push_back(pointId1);
        simplex.push_back(pointId2);

        if (pointId3 != -1) {
          simplex.push_back(pointId3);
          pair<unsigned int, unsigned int> edge4 = make_pair(min(pointId0, pointId3), max(pointId0, pointId3));
          pair<unsigned int, unsigned int> edge5 = make_pair(min(pointId1, pointId3), max(pointId1, pointId3));
          pair<unsigned int, unsigned int> edge6 = make_pair(min(pointId2, pointId3), max(pointId2, pointId3));

          if (placedEdges.find(edge4) == placedEdges.end()) {
            edges.push_back(edge4);
            placedEdges.insert(edge4);
          }

          if (placedEdges.find(edge5) == placedEdges.end()) {
            edges.push_back(edge5);
            placedEdges.insert(edge5);
          }

          if (placedEdges.find(edge6) == placedEdges.end()) {
            edges.push_back(edge6);
            placedEdges.insert(edge6);
          }
        }

        simplices.push_back(simplex);
      }
    }
  }

  // free memory allocated by qhull
  qh_freeqhull(!qh_ALL);
  int curlong, totlong;
  qh_memfreeshort(&curlong, &totlong);

  return !qhullKo;
}

//================================================================================================

static void normalize(Vec3f &v) {
  if (v.norm() != 0)
    v /= v.norm();
}

//================================================================================================

// B -- C
// |    |
// A -- D
static vector<Coord> buildGrid(const Coord &A, const Coord &B,
                               const Coord &C, const Coord &D,
                               const float nbSubDiv = 10) {
  vector<Coord> ret;

  for (float i = 1 ; i < nbSubDiv - 1 ; ++i) {
    Coord start = A + (i/(nbSubDiv-1) * (B-A));
    Coord end = D + (i/(nbSubDiv-1) * (C-D));

    for (float j = 0 ; j < nbSubDiv ; ++j) {
      ret.push_back(start + (j/(nbSubDiv-1)) * (end - start));
    }
  }

  return ret;
}

//================================================================================================

static tlp::Coord computeTriangleCircumscribedCenter(const tlp::Coord &A, const tlp::Coord &B, const tlp::Coord &C) {
  Vec3ld Ad, Bd, Cd;

  for (size_t i = 0 ; i < 3 ; ++i) {
    Ad[i] = A[i];
    Bd[i] = B[i];
    Cd[i] = C[i];
  }

  Vec3ld a = Ad-Cd;
  Vec3ld b = Bd-Cd;
  Vec3ld axb = a^b;
  long double anorm = a.norm();
  long double bnorm = b.norm();
  long double axbnorm = axb.norm();

  if (axbnorm != 0) {
    Vec3ld c = Cd + (((anorm*anorm*b) - (bnorm*bnorm*a))^axb) / (2.0*axbnorm*axbnorm);
    return tlp::Coord(c[0], c[1], c[2]);
  }
  else {
    return (A+B+C)/3.0f;
  }
}

//================================================================================================

static tlp::Coord computeTetrahedronCircumscribedCenter(const tlp::Coord &A, const tlp::Coord &B,
    const tlp::Coord &C, const tlp::Coord &D) {

  Vec3ld Ad, Bd, Cd, Dd;

  for (size_t i = 0 ; i < 3 ; ++i) {
    Ad[i] = A[i];
    Bd[i] = B[i];
    Cd[i] = C[i];
    Dd[i] = D[i];
  }

  Vec3ld da = Dd-Ad;
  Vec3ld ba = Bd-Ad;
  Vec3ld ca = Cd-Ad;

  long double danorm = da.norm();
  long double banorm = ba.norm();
  long double canorm = ca.norm();

  Mat3ld m;

  for (int i = 0 ; i < 3 ; ++i) {
    m[0][i] = ba[i];
    m[1][i] = ca[i];
    m[2][i] = da[i];
  }

  long double det = m.determinant();

  if (det != 0) {
    Vec3ld c =  Ad + ((danorm*danorm*(ba^ca)) + (canorm*canorm*(da^ba)) + (banorm*banorm*(ca^da))) / (2.0*det);
    return tlp::Coord(c[0], c[1], c[2]);
  }
  else {
    return (A+B+C+D)/4.0f;
  }
}

//================================================================================================

struct Face {
  Face(unsigned int n1 = UINT_MAX, unsigned int n2 = UINT_MAX, unsigned int n3 = UINT_MAX) {
    sortedIndexes.reserve(3);
    sortedIndexes.push_back(n1);
    sortedIndexes.push_back(n2);
    sortedIndexes.push_back(n3);
    sort(sortedIndexes.begin(), sortedIndexes.end());
  }

  bool operator==(const Face &f) const {
    return sortedIndexes[0] == f.sortedIndexes[0] &&
           sortedIndexes[1] == f.sortedIndexes[1] &&
           sortedIndexes[2] == f.sortedIndexes[2];
  }

  bool isValid() const {
    return sortedIndexes[0] != UINT_MAX &&
           sortedIndexes[1] != UINT_MAX;
  }

  unsigned int size() const {
    unsigned int ret = 0;

    for (unsigned int i = 0 ; i < 3 ; ++i) {
      if (sortedIndexes[i] != UINT_MAX) {
        ++ret;
      }
    }

    return ret;
  }

  vector<unsigned int> sortedIndexes;
};

TLP_BEGIN_HASH_NAMESPACE {
  template <>
  struct hash<Face> {
    inline std::size_t operator()(const Face &f) const {
      size_t seed = 0;
      hash_combine(seed, f.sortedIndexes[0]);
      hash_combine(seed, f.sortedIndexes[1]);
      hash_combine(seed, f.sortedIndexes[2]);
      return seed;
    }
  };
} TLP_END_HASH_NAMESPACE

//================================================================================================

bool tlp::delaunayTriangulation(vector<Coord> & points,
                                vector<pair<unsigned int, unsigned int> > &edges,
                                vector<vector<unsigned int> > &simplices, bool voronoiMode) {

  vector<double> pointsQHull;
  BoundingBox bb;
  // check if the layout is coplanar
  Mat3f invTransformMatrix;
  bool coPlanarLayout = isLayoutCoPlanar(points, invTransformMatrix);
  Mat3f transformMatrix = invTransformMatrix;

  if (coPlanarLayout)
    transformMatrix.inverse();

  int dim = 3;

  // if the layout is not coplanar, use a 3d delaunay triangulation
  if (!coPlanarLayout) {
    pointsQHull.reserve(points.size()*3);

    // copy coordinates in the pointsQHull vector
    for(size_t i = 0 ; i < points.size() ; ++i) {
      bb.expand(points[i]);
      pointsQHull.push_back(points[i].getX());
      pointsQHull.push_back(points[i].getY());
      pointsQHull.push_back(points[i].getZ());
    }

    // If we compute a delaunay triangulation for then computing the dual voronoi diagram,
    // enclose the layout in a cube whose each face is a grid.
    // It ensures that each site will have an associated connected voronoi cell
    // meaning we don't have to deal with voronoi rays.
    if (voronoiMode) {

      float newWidth = bb.width() * 1.2f;
      float newHeight = bb.height() * 1.2f;
      float newDepth = bb.depth() * 1.2f;

      Coord p1 = Coord(bb.center()) + Coord(-newWidth/2, -newHeight/2, -newDepth/2);
      Coord p2 = Coord(bb.center()) + Coord(-newWidth/2, newHeight/2, -newDepth/2);
      Coord p3 = Coord(bb.center()) + Coord(newWidth/2, newHeight/2, -newDepth/2);
      Coord p4 = Coord(bb.center()) + Coord(newWidth/2, -newHeight/2, -newDepth/2);

      Coord p5 = Coord(bb.center()) + Coord(-newWidth/2, -newHeight/2, newDepth/2);
      Coord p6 = Coord(bb.center()) + Coord(-newWidth/2, newHeight/2, newDepth/2);
      Coord p7 = Coord(bb.center()) + Coord(newWidth/2, newHeight/2, newDepth/2);
      Coord p8 = Coord(bb.center()) + Coord(newWidth/2, -newHeight/2, newDepth/2);

      vector<Coord> mesh;

      vector<Coord> grid = buildGrid(p1, p2, p3, p4);
      mesh.insert(mesh.end(), grid.begin(), grid.end());
      grid = buildGrid(p5, p6, p7, p8);
      mesh.insert(mesh.end(), grid.begin(), grid.end());
      grid = buildGrid(p1, p5, p8, p4);
      mesh.insert(mesh.end(), grid.begin(), grid.end());
      grid = buildGrid(p2, p6, p7, p3);
      mesh.insert(mesh.end(), grid.begin(), grid.end());
      grid = buildGrid(p1, p5, p6, p2);
      mesh.insert(mesh.end(), grid.begin(), grid.end());
      grid = buildGrid(p4, p8, p7, p3);
      mesh.insert(mesh.end(), grid.begin(), grid.end());

      for (size_t i = 0 ; i < mesh.size() ; ++i) {
        points.push_back(mesh[i]);
        pointsQHull.push_back(mesh[i].getX());
        pointsQHull.push_back(mesh[i].getY());
        pointsQHull.push_back(mesh[i].getZ());
      }
    }

  }
  else {
    dim = 2;
    pointsQHull.reserve(points.size()*2);

    // copy coordinates in the pointsQHull vector
    for(size_t i = 0 ; i < points.size() ; ++i) {
      Coord p = Coord(invTransformMatrix * points[i]);
      pointsQHull.push_back(p.getX());
      pointsQHull.push_back(p.getY());
    }

    // If we compute a delaunay triangulation for then computing the dual voronoi diagram,
    // enclose the layout in its convex hull.
    // It ensures that each site will have an associated connected voronoi cell
    // meaning we don't have to deal with voronoi rays.
    if (voronoiMode) {

      vector<Coord> inputHullPoints;
      inputHullPoints.reserve(points.size());

      for (size_t i = 0 ; i < points.size() ; ++i) {
        Coord p = Coord(invTransformMatrix * points[i]);
        inputHullPoints.push_back(p);
      }

      vector<Coord> hullPoints = computeConvexHull(inputHullPoints);
      vector<Coord> newHullPoints;
      float nbSubdiv = 4;

      for (size_t i = 0 ; i < hullPoints.size() -1 ; ++i) {
        for (float j = 0 ; j < nbSubdiv - 1; ++j) {
          newHullPoints.push_back(hullPoints[i] + (j/(nbSubdiv-1)) * (hullPoints[i+1]-hullPoints[i]));
        }
      }

      for (float j = 0 ; j < nbSubdiv - 1; ++j) {
        newHullPoints.push_back(hullPoints.back() + (j/(nbSubdiv-1)) * (hullPoints[0]-hullPoints.back()));
      }

      hullPoints = newHullPoints;
      Coord centroid = computePolygonCentroid(hullPoints);

      for (size_t i = 0 ; i < hullPoints.size() ; ++i) {
        float dist = centroid.dist(hullPoints[i]);
        Coord dir = hullPoints[i] - centroid;
        normalize(dir);
        hullPoints[i] = centroid + dir * 1.2f * dist;
        points.push_back(Coord(transformMatrix * hullPoints[i]));
        pointsQHull.push_back(hullPoints[i].getX());
        pointsQHull.push_back(hullPoints[i].getY());
      }
    }
  }

  return runQHull(dim, pointsQHull, edges, simplices);
}

//================================================================================================

static void addVoronoiEdge(VoronoiDiagram &voronoiDiagram, const Face &face, const VoronoiDiagram::Edge &edge) {
  bool addEdge = false;

  if (face.sortedIndexes[0] < voronoiDiagram.nbSites()) {
    voronoiDiagram.siteToCellEdges[face.sortedIndexes[0]].push_back(voronoiDiagram.edges.size());
    addEdge = true;
  }

  if (face.sortedIndexes[1] < voronoiDiagram.nbSites()) {
    voronoiDiagram.siteToCellEdges[face.sortedIndexes[1]].push_back(voronoiDiagram.edges.size());
    addEdge = true;
  }

  if (face.size() > 2 && face.sortedIndexes[2] < voronoiDiagram.nbSites()) {
    voronoiDiagram.siteToCellEdges[face.sortedIndexes[2]].push_back(voronoiDiagram.edges.size());
    addEdge = true;
  }

  if (addEdge) {
    voronoiDiagram.edges.push_back(edge);
    voronoiDiagram.verticesDegree[edge.first] += 1;
    voronoiDiagram.verticesDegree[edge.second] += 1;
  }
}

//================================================================================================

bool tlp::voronoiDiagram(vector<Coord> &sites, VoronoiDiagram &voronoiDiagram) {

  vector<pair<unsigned int, unsigned int> > edges;
  vector<vector<unsigned int> > simplices;
  voronoiDiagram.sites = sites;
  unsigned int nbSites = sites.size();

  // compute the Delaunay triangulation (some dummy sites will be added in the sites vector
  // in order to have a connected voronoi cell for each input sites)
  bool ret = delaunayTriangulation(sites, edges, simplices, true);

  // now compute the dual voronoi diagram
  if (ret) {
    // Iterate over each delaunay simplex
    TLP_HASH_MAP<Face, unsigned int> faceToCircumCenter;
    map<Coord, unsigned int> circumCenterToIdx;
    tlp::Coord A, B, C, D;

    for (size_t i = 0 ; i < simplices.size() ; ++i) {
      vector<unsigned int> sitesIdx;

      for (size_t j = 0 ; j < simplices[i].size() ; ++j) {
        if (j == 0) {
          A = sites[simplices[i][j]];
          sitesIdx.push_back(simplices[i][j]);
        }
        else if (j == 1) {
          B = sites[simplices[i][j]];
          sitesIdx.push_back(simplices[i][j]);
        }
        else if (j == 2) {
          C = sites[simplices[i][j]];
          sitesIdx.push_back(simplices[i][j]);
        }
        else {
          D = sites[simplices[i][j]];
          sitesIdx.push_back(simplices[i][j]);
        }
      }

      bool treatSimplex = false;

      for (size_t j = 0 ; j < sitesIdx.size() ; ++j) {
        if (sitesIdx[j] < nbSites) {
          treatSimplex = true;
          break;
        }
      }

      // only treats simplices which have at least one original site in its vertices
      if (!treatSimplex)
        continue;

      // compute the circumscribed center of the simplex (triangle in 2d, tetrahedron in 3d)
      tlp::Coord circumCenterPos;

      if (simplices[i].size() == 3)
        circumCenterPos = computeTriangleCircumscribedCenter(A, B, C);
      else
        circumCenterPos = computeTetrahedronCircumscribedCenter(A, B, C, D);

      unsigned int circumCenterIdx = 0;

      if (circumCenterToIdx.find(circumCenterPos) != circumCenterToIdx.end()) {
        circumCenterIdx = circumCenterToIdx[circumCenterPos];
      }
      else {
        // add the voronoi vertex in the voronoi diagram
        circumCenterIdx = voronoiDiagram.vertices.size();
        circumCenterToIdx[circumCenterPos] = circumCenterIdx;
        voronoiDiagram.verticesDegree[circumCenterIdx] = 0;
        voronoiDiagram.vertices.push_back(circumCenterPos);
      }

      Face face1, face2, face3, face4;

      // try to connect two voronoi vertices of adjacent facets

      // 2d voronoi
      if (simplices[i].size() == 3) {
        face1 = Face(simplices[i][0], simplices[i][1]);
        face2 = Face(simplices[i][1], simplices[i][2]);
        face3 = Face(simplices[i][0], simplices[i][2]);
      }

      // 3d voronoi
      else {
        face1 = Face(simplices[i][0], simplices[i][1], simplices[i][2]);
        face2 = Face(simplices[i][1], simplices[i][2], simplices[i][3]);
        face3 = Face(simplices[i][0], simplices[i][2], simplices[i][3]);
        face4 = Face(simplices[i][0], simplices[i][1], simplices[i][3]);
      }

      TLP_HASH_MAP<Face, unsigned int>::const_iterator it = faceToCircumCenter.find(face1);

      if (it != faceToCircumCenter.end()) {
        VoronoiDiagram::Edge edge = make_pair(circumCenterIdx, faceToCircumCenter[face1]);
        addVoronoiEdge(voronoiDiagram, face1, edge);
      }
      else {
        faceToCircumCenter[face1] = circumCenterIdx;
      }

      it = faceToCircumCenter.find(face2);

      if (it != faceToCircumCenter.end()) {
        VoronoiDiagram::Edge edge = make_pair(circumCenterIdx, faceToCircumCenter[face2]);
        addVoronoiEdge(voronoiDiagram, face2, edge);
      }
      else {
        faceToCircumCenter[face2] = circumCenterIdx;
      }

      it = faceToCircumCenter.find(face3);

      if (it != faceToCircumCenter.end()) {
        VoronoiDiagram::Edge edge = make_pair(circumCenterIdx, faceToCircumCenter[face3]);
        addVoronoiEdge(voronoiDiagram, face3, edge);
      }
      else {
        faceToCircumCenter[face3] = circumCenterIdx;
      }

      // 3d voronoi case
      if (face4.isValid()) {

        it = faceToCircumCenter.find(face4);

        if (it != faceToCircumCenter.end()) {
          VoronoiDiagram::Edge edge = make_pair(circumCenterIdx, faceToCircumCenter[face4]);
          addVoronoiEdge(voronoiDiagram, face4, edge);
        }
        else {
          faceToCircumCenter[face4] = circumCenterIdx;
        }
      }
    }

    // compute the voronoi cells associated to sites
    voronoiDiagram.cells.reserve(nbSites);

    for (unsigned int i = 0 ; i < nbSites ; ++i) {
      VoronoiDiagram::Cell cell;
      const vector<VoronoiDiagram::Edge> &voronoiEdges = voronoiDiagram.voronoiEdgesForSite(i);

      for (size_t j = 0 ; j < voronoiEdges.size() ; ++j) {
        cell.insert(voronoiEdges[j].first);
        cell.insert(voronoiEdges[j].second);
      }

      voronoiDiagram.siteToCell[i] = voronoiDiagram.cells.size();
      voronoiDiagram.cells.push_back(cell);
    }
  }

  // restore the original sites vector (as the delaunay triangulation adds dummy sites)
  sites.resize(nbSites);
  return ret;
}
