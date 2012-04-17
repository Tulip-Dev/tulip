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
#include <tulip/Graph.h>
#include <tulip/MethodFactory.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/DrawingTools.h>
#include <tulip/Matrix.h>
#include <tulip/StlIterator.h>

#include <sstream>

typedef tlp::Matrix<long double, 3> Mat3ld;
typedef tlp::Vector<long double, 3> Vec3ld;

struct Face {
  Face(tlp::node n1, tlp::node n2, tlp::node n3 = tlp::node()) {
    sortedNodes.reserve(3);
    sortedNodes.push_back(n1);
    sortedNodes.push_back(n2);
    sortedNodes.push_back(n3);
    std::sort(sortedNodes.begin(), sortedNodes.end());
  }

  bool operator==(const Face &f) const {
    return sortedNodes[0] == f.sortedNodes[0] &&
           sortedNodes[1] == f.sortedNodes[1] &&
           sortedNodes[2] == f.sortedNodes[2];
  }

  std::vector<tlp::node> sortedNodes;
};

TLP_BEGIN_HASH_NAMESPACE {
  template <class T>
  inline void hash_combine(std::size_t & seed, const T & v) {
    hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
  }

  template <>
  struct hash<Face> {
    inline std::size_t operator()(const Face &f) const {
      size_t seed = 0;
      hash_combine(seed, f.sortedNodes[0]);
      hash_combine(seed, f.sortedNodes[1]);
      hash_combine(seed, f.sortedNodes[2]);
      return seed;
    }
  };
} TLP_END_HASH_NAMESPACE

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

class Voronoi : public tlp::Algorithm {

public :

  Voronoi(tlp::AlgorithmContext context) : tlp::Algorithm(context) {
    addParameter<bool>("voronoi cells", "If checked, a subgraph will be added for each computed voronoi cell.", "false");
    addParameter<bool>("connect", "If checked, original graph nodes will be connected to the vertices of their voronoi cell.", "false");
  }

  bool run() {

    tlp::Observable::holdObservers();

    bool voronoiCellSg = false;
    bool connectNodesToVoronoiCell = false;

    if (dataSet) {
      dataSet->get("voronoi cells", voronoiCellSg);
      dataSet->get("connect", connectNodesToVoronoiCell);
    }

    tlp::Graph *clone = graph->addCloneSubGraph("Original graph");

    tlp::LayoutProperty *viewLayout = graph->getProperty<tlp::LayoutProperty>("viewLayout");

    // First compute the delaunay triangulation
    graph->setAttribute("voronoi mode", true);
    std::string errMsg;

    if (graph->applyAlgorithm("Delaunay triangulation", errMsg)) {

      // Ok, now we are ready to compute the dual voronoi diagram
      tlp::Graph *delaunaySubGraph = graph->getSubGraph("Delaunay");

      tlp::Graph* voronoiSubGraph = graph->addSubGraph("Voronoi");
      tlp::Graph *cleanVoronoiSg = voronoiSubGraph->addSubGraph();

      std::vector<std::vector<tlp::node> > simplicesNodes;

      graph->getAttribute("delaunay simplices nodes", simplicesNodes);

      // Iterate over each delaunay simplex
      std::map<tlp::Coord, tlp::node> circumCenterToNode;
      TLP_HASH_MAP<Face, tlp::node> faceToCircumCenter;
      TLP_HASH_MAP<tlp::node, std::set<tlp::node> > origNodeToCellBorder;
      tlp::Coord A, B, C, D;
      tlp::node n;

      for (size_t i = 0 ; i < simplicesNodes.size() ; ++i) {
        for (size_t j = 0 ; j < simplicesNodes[i].size() ; ++j) {
          n = simplicesNodes[i][j];

          if (j == 0) {
            A = viewLayout->getNodeValue(n);
          }
          else if (j == 1) {
            B = viewLayout->getNodeValue(n);
          }
          else if (j == 2) {
            C = viewLayout->getNodeValue(n);
          }
          else {
            D = viewLayout->getNodeValue(n);
          }

        }

        // compute the circumscribed center of the simplex (triangle in 2d, tetrahedron in 3d)
        tlp::Coord circumCenterPos;

        if (simplicesNodes[i].size() == 3)
          circumCenterPos = computeTriangleCircumscribedCenter(A, B, C);
        else
          circumCenterPos = computeTetrahedronCircumscribedCenter(A, B, C, D);

        tlp::node ne;

        if (circumCenterToNode.find(circumCenterPos) != circumCenterToNode.end()) {
          ne = circumCenterToNode[circumCenterPos];
        }
        else {
          // add the voronoi vertex in the voronoi sub-graph
          ne = voronoiSubGraph->addNode();
          viewLayout->setNodeValue(ne, circumCenterPos);
          circumCenterToNode[circumCenterPos] = ne;
        }

        for (size_t j = 0 ; j < simplicesNodes[i].size() ; ++j) {
          origNodeToCellBorder[simplicesNodes[i][j]].insert(ne);
        }

        // try to connect two voronoi vertices of adjacent facets
        if (simplicesNodes[i].size() == 3) {

          Face face1(simplicesNodes[i][0], simplicesNodes[i][1]);
          Face face2(simplicesNodes[i][1], simplicesNodes[i][2]);
          Face face3(simplicesNodes[i][0], simplicesNodes[i][2]);

          TLP_HASH_MAP<Face, tlp::node>::const_iterator it = faceToCircumCenter.find(face1);

          if (it != faceToCircumCenter.end()) {
            voronoiSubGraph->addEdge(ne, it->second);
          }
          else {
            faceToCircumCenter[face1] = ne;
          }

          it = faceToCircumCenter.find(face2);

          if (it != faceToCircumCenter.end()) {
            voronoiSubGraph->addEdge(ne, it->second);
          }
          else {
            faceToCircumCenter[face2] = ne;
          }

          it = faceToCircumCenter.find(face3);

          if (it != faceToCircumCenter.end()) {
            voronoiSubGraph->addEdge(ne, it->second);
          }
          else {
            faceToCircumCenter[face3] = ne;
          }
        }
        else {

          Face face1(simplicesNodes[i][0], simplicesNodes[i][1], simplicesNodes[i][2]);
          Face face2(simplicesNodes[i][1], simplicesNodes[i][2], simplicesNodes[i][3]);
          Face face3(simplicesNodes[i][0], simplicesNodes[i][2], simplicesNodes[i][3]);
          Face face4(simplicesNodes[i][0], simplicesNodes[i][1], simplicesNodes[i][3]);

          TLP_HASH_MAP<Face, tlp::node>::const_iterator it = faceToCircumCenter.find(face1);

          if (it != faceToCircumCenter.end()) {
            voronoiSubGraph->addEdge(ne, it->second);
          }
          else {
            faceToCircumCenter[face1] = ne;
          }

          it = faceToCircumCenter.find(face2);

          if (it != faceToCircumCenter.end()) {
            voronoiSubGraph->addEdge(ne, it->second);
          }
          else {
            faceToCircumCenter[face2] = ne;
          }

          it = faceToCircumCenter.find(face3);

          if (it != faceToCircumCenter.end()) {
            voronoiSubGraph->addEdge(ne, it->second);
          }
          else {
            faceToCircumCenter[face3] = ne;
          }

          it = faceToCircumCenter.find(face4);

          if (it != faceToCircumCenter.end()) {
            voronoiSubGraph->addEdge(ne, it->second);
          }
          else {
            faceToCircumCenter[face4] = ne;
          }

        }
      }

      // compute the voronoi cells associated to nodes
      // and add corresponding sub-graphs
      int cellCpt = 0;
      std::ostringstream oss;
      forEach(n, clone->getNodes()) {
        std::set<tlp::node> cellNodes;
        tlp::node ne;

        for (std::set<tlp::node>::iterator it = origNodeToCellBorder[n].begin() ; it != origNodeToCellBorder[n].end() ; ++it) {
          ne = *it;

          if (voronoiCellSg) {
            cellNodes.insert(ne);
          }

          cleanVoronoiSg->addNode(ne);

          if (connectNodesToVoronoiCell) {
            voronoiSubGraph->addNode(n);
            cleanVoronoiSg->addNode(n);
            voronoiSubGraph->addEdge(n, ne);
          }
        }

        if (voronoiCellSg) {
          oss.str("");
          oss << "voronoi cell " << cellCpt++;
          tlp::Graph *cellSg = voronoiSubGraph->inducedSubGraph(cellNodes);
          cellSg->setName(oss.str());
        }
      }

      tlp::StableIterator<tlp::edge> delaunayEdges(delaunaySubGraph->getEdges());
      graph->delAllSubGraphs(delaunaySubGraph);
      graph->delEdges(&delaunayEdges, true);

      // remove the dummy nodes added by the delaunay triangulation
      std::vector<tlp::node> dummyNodes;
      graph->getAttribute("dummy nodes", dummyNodes);
      graph->removeAttribute("dummy nodes");

      for (size_t i = 0 ; i < dummyNodes.size() ; ++i) {
        graph->delNode(dummyNodes[i], true);
      }

      // remove voronoi vertices that are not associated to a voronoi cell
      stableForEach(n, voronoiSubGraph->getNodes()) {
        if (!cleanVoronoiSg->isElement(n)) {
          voronoiSubGraph->delNode(n, true);
        }
      }

      voronoiSubGraph->delSubGraph(cleanVoronoiSg);
    }
    else {
      graph->delSubGraph(clone);
    }

    graph->removeAttribute("voronoi mode");
    graph->removeAttribute("delaunay simplices nodes");

    tlp::Observable::unholdObservers();

    return true;
  }
};

ALGORITHMPLUGIN(Voronoi,"Voronoi diagram","Antoine LAMBERT","","","1.0");
