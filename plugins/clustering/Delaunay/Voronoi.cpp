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
#include <sstream>

typedef tlp::Matrix<float, 3> Mat3f;

static tlp::Coord computeTriangleCircumscribedCenter(const tlp::Coord &A, const tlp::Coord &B, const tlp::Coord &C) {
  tlp::Coord a = A - C;
  tlp::Coord b = B - C;
  tlp::Coord axb = a^b;
  float anorm = a.norm();
  float bnorm = b.norm();
  float axbnorm = axb.norm();
  return C + (((anorm * anorm * b) - (bnorm * bnorm * a))^axb) / (2.0f * axbnorm * axbnorm);
}

static tlp::Coord computeTetrahedronCircumscribedCenter(const tlp::Coord &A, const tlp::Coord &B,
    const tlp::Coord &C, const tlp::Coord &D) {
  tlp::Coord da = D-A;
  tlp::Coord ba = B-A;
  tlp::Coord ca = C-A;

  float danorm = da.norm();
  float banorm = ba.norm();
  float canorm = ca.norm();

  Mat3f m;

  for (int i = 0 ; i < 3 ; ++i) {
    m[0][i] = ba[i];
    m[1][i] = ca[i];
    m[2][i] = da[i];
  }

  return A + ((danorm*danorm*(ba^ca)) + (canorm*canorm*(da^ba)) + (banorm*banorm*(ca^da))) / (2.0f*m.determinant());
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
    tlp::DataSet delaunayDataSet;
    delaunayDataSet.set("simplices", true);
    std::string errMsg;

    if (graph->applyAlgorithm("Delaunay triangulation", errMsg, &delaunayDataSet)) {

      // Ok, now we are ready to compute the dual voronoi diagram
      tlp::Graph *delaunaySubGraph = graph->getSubGraph("Delaunay");

      tlp::Graph* voronoiSubGraph = graph->addSubGraph("Voronoi");
      tlp::Graph *cleanVoronoiSg = voronoiSubGraph->addSubGraph();

      // Iterate over each delaunay simplex
      TLP_HASH_MAP<tlp::edge, tlp::node> edgeToCircumCircleCenter;
      std::map<std::set<tlp::node>, tlp::node> faceToCircumSphereCenter;
      tlp::Graph *simplex = NULL;
      tlp::Coord A, B, C, D;
      forEach(simplex, delaunaySubGraph->getSubGraphs()) {
        tlp::node n;
        int i = 0;
        // get the coordinates of the simplex vertices
        forEach(n, simplex->getNodes()) {
          voronoiSubGraph->addNode(n);

          if (i == 0) {
            A = viewLayout->getNodeValue(n);
          }
          else if (i == 1) {
            B = viewLayout->getNodeValue(n);
          }
          else if (i==2) {
            C = viewLayout->getNodeValue(n);
          }
          else {
            D = viewLayout->getNodeValue(n);
          }

          ++i;
        }

        // compute the circumscribed center of the simplex (triangle in 2d, tetrahedron in 3d)
        tlp::Coord circumCenter;

        if (simplex->numberOfNodes() == 3)
          circumCenter = computeTriangleCircumscribedCenter(A, B, C);
        else
          circumCenter = computeTetrahedronCircumscribedCenter(A, B, C, D);

        // add the voronoi vertex in the voronoi sub-graph
        tlp::node ne = voronoiSubGraph->addNode();

        viewLayout->setNodeValue(ne, circumCenter);

        forEach(n, simplex->getNodes()) {
          voronoiSubGraph->addEdge(n, ne);
        }

        // try to connect two voronoi vertices of adjacent facets
        if (simplex->numberOfNodes() == 3) {

          tlp::edge e;
          forEach(e, simplex->getEdges()) {
            TLP_HASH_MAP<tlp::edge, tlp::node>::iterator it = edgeToCircumCircleCenter.find(e);

            if (it != edgeToCircumCircleCenter.end()) {
              voronoiSubGraph->addEdge(ne, it->second);
            }
            else {
              edgeToCircumCircleCenter[e] = ne;
            }
          }

        }
        else {

          tlp::node n;
          std::vector<tlp::node> tetraNodes;
          tetraNodes.reserve(4);
          forEach(n, simplex->getNodes()) {
            tetraNodes.push_back(n);
          }
          std::set<tlp::node> face1;
          std::set<tlp::node> face2;
          std::set<tlp::node> face3;
          std::set<tlp::node> face4;
          face1.insert(tetraNodes[0]);
          face1.insert(tetraNodes[1]);
          face1.insert(tetraNodes[2]);
          face2.insert(tetraNodes[1]);
          face2.insert(tetraNodes[2]);
          face2.insert(tetraNodes[3]);
          face3.insert(tetraNodes[0]);
          face3.insert(tetraNodes[2]);
          face3.insert(tetraNodes[3]);
          face4.insert(tetraNodes[0]);
          face4.insert(tetraNodes[1]);
          face4.insert(tetraNodes[3]);

          std::map<std::set<tlp::node>, tlp::node>::iterator it = faceToCircumSphereCenter.find(face1);

          if (it != faceToCircumSphereCenter.end()) {
            voronoiSubGraph->addEdge(ne, it->second);
          }
          else {
            faceToCircumSphereCenter[face1] = ne;
          }

          it = faceToCircumSphereCenter.find(face2);

          if (it != faceToCircumSphereCenter.end()) {
            voronoiSubGraph->addEdge(ne, it->second);
          }
          else {
            faceToCircumSphereCenter[face2] = ne;
          }

          it = faceToCircumSphereCenter.find(face3);

          if (it != faceToCircumSphereCenter.end()) {
            voronoiSubGraph->addEdge(ne, it->second);
          }
          else {
            faceToCircumSphereCenter[face3] = ne;
          }

          it = faceToCircumSphereCenter.find(face4);

          if (it != faceToCircumSphereCenter.end()) {
            voronoiSubGraph->addEdge(ne, it->second);
          }
          else {
            faceToCircumSphereCenter[face4] = ne;
          }

        }
      }

      // compute the voronoi cells associated to nodes
      // and add corresponding sub-graphs
      tlp::node n;
      int cellCpt = 0;
      std::ostringstream oss;
      forEach(n, clone->getNodes()) {
        std::set<tlp::node> cellNodes;
        tlp::node ne;
        forEach(ne, voronoiSubGraph->getInOutNodes(n)) {
          if (voronoiCellSg) {
            cellNodes.insert(ne);
          }

          cleanVoronoiSg->addNode(ne);
        }

        if (voronoiCellSg) {
          oss.str("");
          oss << "voronoi cell " << cellCpt++;
          tlp::Graph *cellSg = voronoiSubGraph->inducedSubGraph(cellNodes);
          cellSg->setName(oss.str());
        }

        if (!connectNodesToVoronoiCell) {
          tlp::edge e;
          stableForEach(e, voronoiSubGraph->getInOutEdges(n)) {
            if (!clone->isElement(e)) {
              voronoiSubGraph->delEdge(e, true);
            }
          }
        }

        voronoiSubGraph->delNode(n);
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

    tlp::Observable::unholdObservers();

    return true;
  }
};

ALGORITHMPLUGIN(Voronoi,"Voronoi diagram","Antoine LAMBERT","","","1.0");
