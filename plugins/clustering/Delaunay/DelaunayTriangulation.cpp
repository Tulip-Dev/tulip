/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#ifdef _OPENMP
#include <omp.h>
#endif

#include <tulip/Algorithm.h>
#include <tulip/TulipPluginHeaders.h>
#include <tulip/Delaunay.h>
#include <tulip/LayoutProperty.h>
#include <tulip/StaticProperty.h>

using namespace std;

static bool delaunayTriangulation(tlp::Graph *graph, bool simplicesSubGraphs, bool originalClone) {
  tlp::NodeStaticProperty<tlp::Coord> points(graph);
  points.copyFromProperty(graph->getProperty<tlp::LayoutProperty>("viewLayout"));

  vector<pair<unsigned int, unsigned int> > edges;
  vector<vector<unsigned int> > simplices;
  bool ret = tlp::delaunayTriangulation(points, edges, simplices);

  if (ret) {

    if (originalClone)
      graph->addCloneSubGraph("Original graph");

    tlp::Graph *delaunaySg = graph->addSubGraph("Delaunay");
    const std::vector<tlp::node> &nodes = graph->nodes();
    delaunaySg->addNodes(nodes);

    for (size_t i = 0; i < edges.size(); ++i) {
      delaunaySg->addEdge(nodes[edges[i].first], nodes[edges[i].second]);
    }

    if (simplicesSubGraphs) {
      for (size_t i = 0; i < simplices.size(); ++i) {
        vector<tlp::node> sNodes(simplices[i].size());
#ifdef _OPENMP
#pragma omp parallel for
#endif

        for (OMP_ITER_TYPE j = 0; j < static_cast<OMP_ITER_TYPE>(simplices[i].size()); ++j) {
          sNodes[j] = nodes[simplices[i][j]];
        }

        ostringstream oss;
        oss << (simplices[i].size() == 3 ? "triangle " : "tetrahedron ") << i;
        tlp::Graph *simplexSg = delaunaySg->inducedSubGraph(sNodes);
        simplexSg->setName(oss.str());
      }
    }
  }

  return ret;
}

static const char *paramHelp[] = {
    // simplices
    "If true, a subgraph will be added for each computed simplex (a triangle in 2d, a tetrahedron "
    "in 3d).",
    // original clone
    "If true, a clone subgraph named 'Original graph' will be first added."};

class DelaunayTriangulation : public tlp::Algorithm {

public:
  DelaunayTriangulation(tlp::PluginContext *context) : Algorithm(context) {
    addInParameter<bool>("simplices", paramHelp[0], "false");
    addInParameter<bool>("original clone", paramHelp[1], "true");
  }

  PLUGININFORMATION("Delaunay triangulation", "Antoine Lambert", "",
                    "Performs a Delaunay triangulation, in considering the positions of the graph "
                    "nodes as a set of points. The building of simplices (triangles in 2D or "
                    "tetrahedrons in 3D) consists in adding edges between adjacent nodes.",
                    "1.1", "Triangulation")

  bool run() {
    if (graph->numberOfNodes() == 0)
      return true;

    bool simplicesSg = false;
    bool originalClone = true;

    if (dataSet) {
      dataSet->get("simplices", simplicesSg);
      dataSet->get("original clone", originalClone);
    }

    bool ret = delaunayTriangulation(graph, simplicesSg, originalClone);

    return ret;
  }
};

PLUGIN(DelaunayTriangulation)
