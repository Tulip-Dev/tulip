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

#include <tulip/Algorithm.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/Graph.h>
#include <tulip/MethodFactory.h>
#include <tulip/MapIterator.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ForEach.h>
#include <tulip/DrawingTools.h>
#include <tulip/Matrix.h>
#include <sstream>

extern "C" {
#include <libqhull/libqhull.h>
#include <libqhull/qset.h>
#include <libqhull/geom.h>
#include <libqhull/poly.h>
#include <libqhull/io.h>
}

typedef tlp::Matrix<float, 3> Mat3f;

static void normalize(tlp::Vec3f &v) {
  if (v.norm() != 0)
    v /= v.norm();
}

static tlp::Coord computePolygonCentroid(const std::vector<tlp::Coord> &points) {
  std::vector<tlp::Coord> pointsCp(points);
  pointsCp.push_back(points[0]);
  float A = 0;
  float Cx = 0;
  float Cy = 0;

  for (size_t i = 0 ; i < pointsCp.size() - 1 ; ++i) {
    A += (pointsCp[i][0] * pointsCp[i+1][1] - pointsCp[i+1][0] * pointsCp[i][1]);
    Cx += (pointsCp[i][0] + pointsCp[i+1][0])*(pointsCp[i][0]*pointsCp[i+1][1] - pointsCp[i+1][0]*pointsCp[i][1]);
    Cy += (pointsCp[i][1] + pointsCp[i+1][1])*(pointsCp[i][0]*pointsCp[i+1][1] - pointsCp[i+1][0]*pointsCp[i][1]);
  }

  A *= 0.5f;
  Cx *= 1.0f / (6*A);
  Cy *= 1.0f / (6*A);
  return tlp::Coord(Cx, Cy);
}

static bool isLayoutCoPlanar(tlp::LayoutProperty *layout, Mat3f &invTransformMatrix) {
  tlp::node n;
  tlp::Coord A, B, C;
  int i = 0;
  // pick three points to define a plane
  forEach(n, layout->getGraph()->getNodes()) {
    if (i == 0) {
      A = layout->getNodeValue(n);
    }
    else if (i == 1) {
      B = layout->getNodeValue(n);
    }
    else {
      // pick a third point non aligned with the two others
      C = layout->getNodeValue(n);

      if (((C-A)^(B-A)).norm() > 1e-3) {
        break;
      }
    }

    ++i;
  }

  tlp::Coord a = B -A;
  tlp::Coord b = C -A;
  normalize(a);
  normalize(b);
  tlp::Coord c = a^b;
  normalize(c);
  b = c^a;
  normalize(b);

  // compute the distance of each point to the plane
  forEach(n, layout->getGraph()->getNodes()) {
    const tlp::Coord &D = layout->getNodeValue(n);

    // if the point is too far from the plane, the layout is not coplanar
    if (abs(c.dotProduct(D-A)) > 1e-3) {
      return false;
    }
  }

  // compute the inverse transform matrix for projecting the points in the z = 0 plane
  invTransformMatrix[0][0]=a[0];
  invTransformMatrix[1][0]=a[1];
  invTransformMatrix[2][0]=a[2];
  invTransformMatrix[0][1]=b[0];
  invTransformMatrix[1][1]=b[1];
  invTransformMatrix[2][1]=b[2];
  invTransformMatrix[0][2]=c[0];
  invTransformMatrix[1][2]=c[1];
  invTransformMatrix[2][2]=c[2];
  invTransformMatrix.inverse();

  return true;
}



// B -- C
// |    |
// A -- D
static std::vector<tlp::Coord> buildGrid(const tlp::Coord &A, const tlp::Coord &B,
    const tlp::Coord &C, const tlp::Coord &D,
    const float nbSubDiv = 10) {
  std::vector<tlp::Coord> ret;

  for (float i = 1 ; i < nbSubDiv - 1 ; ++i) {
    tlp::Coord start = A + (i/(nbSubDiv-1) * (B-A));
    tlp::Coord end = D + (i/(nbSubDiv-1) * (C-D));

    for (float j = 0 ; j < nbSubDiv ; ++j) {
      ret.push_back(start + (j/(nbSubDiv-1)) * (end - start));
    }
  }

  return ret;
}

class Delaunay : public tlp::Algorithm {

public :

  Delaunay(tlp::AlgorithmContext context) : Algorithm(context) {
    addParameter<bool>("simplices", "If checked, a subgraph will be added for each computed simplex (a triangle in 2d, a tetrahedron in 3d).", "false");
  }

  bool run() {

    tlp::Observable::holdObservers();

    tlp::Graph *clone = NULL;

    bool voronoiMode = false;

    if (!graph->getAttribute("voronoi mode")) {
      clone = graph->addCloneSubGraph("Original graph");
    } else {
        voronoiMode = true;
    }

    bool triangleSg = false;

    if (dataSet)
      dataSet->get("simplices", triangleSg);

    tlp::LayoutProperty* layout = graph->getProperty<tlp::LayoutProperty>("viewLayout");
    tlp::SizeProperty* size = graph->getProperty<tlp::SizeProperty>("viewSize");
    tlp::DoubleProperty* rotation = graph->getProperty<tlp::DoubleProperty>("viewRotation");

    std::vector<double> points;
    tlp::node n;
    std::vector<tlp::node> pointIdToNode;

    // check if the layout is coplanar
    Mat3f invTransformMatrix;
    bool coPlanarLayout = isLayoutCoPlanar(layout, invTransformMatrix);
    int dim = 3;

    // if the layout is not coplanar, use a 3d delaunay triangulation
    if (!coPlanarLayout) {
      points.reserve(graph->numberOfNodes()*3);

      tlp::BoundingBox bb;

      // copy nodes coordinates in the points vector
      // and build mapping between point id and node
      forEach(n, graph->getNodes()) {
        const tlp::Coord &p = layout->getNodeValue(n);
        bb.expand(p);
        points.push_back(p.getX());
        points.push_back(p.getY());
        points.push_back(p.getZ());
        pointIdToNode.push_back(n);
      }

      // If we compute a delaunay triangulation for then computing the dual voronoi diagram,
      // enclose the layout in a cube whose each face is a grid.
      // It ensures that each nodes of the original graph will have an associated connected voronoi cell
      // meaning we don't have to deal with infinite voronoi edges.
      if (voronoiMode) {

        std::vector<tlp::node> addedNodes;
        float newWidth = bb.width() * 1.2;
        float newHeight = bb.height() * 1.2;
        float newDepth = bb.depth() * 1.2;


        tlp::Coord p1 = tlp::Coord(bb.center()) + tlp::Coord(-newWidth/2, -newHeight/2, -newDepth/2);
        tlp::Coord p2 = tlp::Coord(bb.center()) + tlp::Coord(-newWidth/2, newHeight/2, -newDepth/2);
        tlp::Coord p3 = tlp::Coord(bb.center()) + tlp::Coord(newWidth/2, newHeight/2, -newDepth/2);
        tlp::Coord p4 = tlp::Coord(bb.center()) + tlp::Coord(newWidth/2, -newHeight/2, -newDepth/2);

        tlp::Coord p5 = tlp::Coord(bb.center()) + tlp::Coord(-newWidth/2, -newHeight/2, newDepth/2);
        tlp::Coord p6 = tlp::Coord(bb.center()) + tlp::Coord(-newWidth/2, newHeight/2, newDepth/2);
        tlp::Coord p7 = tlp::Coord(bb.center()) + tlp::Coord(newWidth/2, newHeight/2, newDepth/2);
        tlp::Coord p8 = tlp::Coord(bb.center()) + tlp::Coord(newWidth/2, -newHeight/2, newDepth/2);

        std::vector<tlp::Coord> mesh;

        std::vector<tlp::Coord> grid = buildGrid(p1, p2, p3, p4);
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
          tlp::node n = graph->addNode();
          layout->setNodeValue(n, mesh[i]);
          addedNodes.push_back(n);
          points.push_back(mesh[i].getX());
          points.push_back(mesh[i].getY());
          points.push_back(mesh[i].getZ());
          pointIdToNode.push_back(n);
        }

        graph->setAttribute("dummy nodes", addedNodes);
      }

      // else use a 2d delaunay triangulation
    }
    else {
      dim = 2;
      points.reserve(graph->numberOfNodes()*2);
      // project nodes coordinates in the z = 0 plane,
      // copy them in the points vector
      // and build mapping between point id and node
      tlp::LayoutProperty newLayout(graph);
      tlp::SizeProperty newSize(graph);
      forEach(n, graph->getNodes()) {
        tlp::Coord p = tlp::Coord(invTransformMatrix * layout->getNodeValue(n));
        newLayout.setNodeValue(n, tlp::Coord(p[0], p[1]));
        newSize.setNodeValue(n, size->getNodeValue(n) * 100);
        points.push_back(p.getX());
        points.push_back(p.getY());
        pointIdToNode.push_back(n);
      }

      // If we compute a delaunay triangulation for then computing the dual voronoi diagram,
      // enclose the layout in its convex hull.
      // It ensures that each nodes of the original graph will have an associated connected voronoi cell
      // meaning we don't have to deal with infinite voronoi edges.
      if (voronoiMode) {

        std::map<tlp::node, tlp::Coord> newNodeToPos;
        std::vector<tlp::node> addedNodes;
        std::vector<tlp::Coord> hullPoints = tlp::computeConvexHull(graph, &newLayout, &newSize, rotation);
        std::vector<tlp::Coord> newHullPoints;
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
        tlp::Coord centroid = computePolygonCentroid(hullPoints);

        for (size_t i = 0 ; i < hullPoints.size() ; ++i) {
          float dist = centroid.dist(hullPoints[i]);
          tlp::Coord dir = hullPoints[i] - centroid;
          normalize(dir);
          hullPoints[i] = centroid + dir * 1.2 * dist;
          tlp::node n = graph->addNode();
          addedNodes.push_back(n);
          points.push_back(hullPoints[i].getX());
          points.push_back(hullPoints[i].getY());
          pointIdToNode.push_back(n);
          newNodeToPos[n] = tlp::Coord(hullPoints[i].getX(), hullPoints[i].getY());
        }

        graph->setAttribute("dummy nodes", addedNodes);

        invTransformMatrix.inverse();

        std::map<tlp::node, tlp::Coord>::iterator it = newNodeToPos.begin();

        for(; it != newNodeToPos.end() ; ++it) {
          layout->setNodeValue(it->first, tlp::Coord(invTransformMatrix*it->second));
        }
      }
    }

    // Set default options for qhull delaunay triangulation
    // - Qt : triangulated output
    // - Qbb : scale the last coordinate to [0,m] for Delaunay
    // - Qc : keep coplanar points with nearest facet
    // - Qz - add a point-at-infinity for Delaunay triangulations
    std::string qhullOptions = "Qt Qbb Qc Qz";
    // build qhull command
    std::string qhullCommand = std::string("qhull d ") + qhullOptions;

    // initialize qhull
    int qhullOk = qh_new_qhull(dim, graph->numberOfNodes(), &points[0], false, const_cast<char *>(qhullCommand.c_str()), NULL, stderr);

    if (qhullOk) {

      tlp::Graph* delaunaySubGraph = graph->addCloneSubGraph("Delaunay");

      tlp::edge e;
      stableForEach(e, delaunaySubGraph->getEdges()) {
        delaunaySubGraph->delEdge(e);
      }

      // call qhull delaunay triangulation
      qh_triangulate();

      facetT *facet;
      vertexT *vertex, **vertexp;

      int simCpt = 0;
      std::ostringstream oss;
      std::vector<std::vector<tlp::node> > simplicesNodes;
      // iterate over generated simplices (triangles or tetrahedra) and add corresponding edges
      // in the delaunay sub-graph
      FORALLfacets {
        std::vector<tlp::node> simplexNodes;
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

          tlp::node n0 = pointIdToNode[pointId0];
          tlp::node n1 = pointIdToNode[pointId1];
          tlp::node n2 = pointIdToNode[pointId2];

          simplexNodes.push_back(n0);
          simplexNodes.push_back(n1);
          simplexNodes.push_back(n2);

          tlp::node n3;
          if (pointId3 != -1) {
            n3 = pointIdToNode[pointId3];
            simplexNodes.push_back(n3);
          }

          tlp::Graph *triSg = NULL;

          if (triangleSg) {
            oss.str("");

            if (dim == 2)
              oss << "triangle " << simCpt++;
            else
              oss << "tetrahedron " << simCpt++;

            triSg = delaunaySubGraph->addSubGraph(oss.str());
            triSg->addNode(n0);
            triSg->addNode(n1);
            triSg->addNode(n2);

            if (n3.isValid()) {
              triSg->addNode(n3);
            }
          }

          tlp::edge e1 = delaunaySubGraph->existEdge(n0, n1, false);

          if(!voronoiMode && !e1.isValid())
            e1 = delaunaySubGraph->addEdge(n0, n1);

          if (!voronoiMode && triSg)
            triSg->addEdge(e1);

          tlp::edge e2 = delaunaySubGraph->existEdge(n0, n2, false);

          if(!voronoiMode && !e2.isValid())
            e2 = delaunaySubGraph->addEdge(n0, n2);

          if (!voronoiMode && triSg)
            triSg->addEdge(e2);

          tlp::edge e3 = delaunaySubGraph->existEdge(n1, n2, false);

          if(!voronoiMode && !e3.isValid())
            e3 = delaunaySubGraph->addEdge(n1, n2);

          if (!voronoiMode && triSg)
            triSg->addEdge(e3);

          // 3d delaunay case
          if (!voronoiMode && n3.isValid()) {
            tlp::edge e4 = delaunaySubGraph->existEdge(n3, n0, false);

            if (!e4.isValid())
              e4 = delaunaySubGraph->addEdge(n3, n0);

            if (triSg)
              triSg->addEdge(e4);

            tlp::edge e5 = delaunaySubGraph->existEdge(n3, n1, false);

            if (!e5.isValid())
              e5 = delaunaySubGraph->addEdge(n3, n1);

            if (triSg)
              triSg->addEdge(e5);

            tlp::edge e6 = delaunaySubGraph->existEdge(n3, n2, false);

            if (!e6.isValid())
              e6 = delaunaySubGraph->addEdge(n3, n2);

            if (triSg)
              triSg->addEdge(e6);

          }
          simplicesNodes.push_back(simplexNodes);
        }
      }

      if (voronoiMode) {
          graph->setAttribute("delaunay simplices nodes", simplicesNodes);
      }
    }
    else if (clone) {
      graph->delSubGraph(clone);
    }



    // free memory allocated by qhull
    qh_freeqhull(!qh_ALL);
    int curlong, totlong;
    qh_memfreeshort(&curlong, &totlong);

    tlp::Observable::unholdObservers();

    return !qhullOk;
  }
};

ALGORITHMPLUGIN(Delaunay,"Delaunay triangulation","Antoine LAMBERT","","","1.0");
