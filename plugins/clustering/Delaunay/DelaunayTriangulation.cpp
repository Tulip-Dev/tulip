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

#include <tulip/Algorithm.h>
#include <tulip/TulipPluginHeaders.h>
#include <tulip/Delaunay.h>
#include <tulip/LayoutProperty.h>

using namespace std;

static bool delaunayTriangulation(tlp::Graph *graph, bool simplicesSubGraphs) {
  vector<tlp::node> nodes;
  nodes.reserve(graph->numberOfNodes());
  vector<tlp::Coord> points;
  points.reserve(graph->numberOfNodes());
  tlp::node n;
  tlp::LayoutProperty *layout = graph->getProperty<tlp::LayoutProperty>("viewLayout");
  forEach(n, graph->getNodes()) {
    nodes.push_back(n);
    points.push_back(layout->getNodeValue(n));
  }
  vector<pair<unsigned int, unsigned int> > edges;
  vector<vector<unsigned int> > simplices;
  bool ret = tlp::delaunayTriangulation(points, edges, simplices);

  if (ret) {

    graph->addCloneSubGraph("Original graph");

    tlp::Graph *delaunaySg = graph->addCloneSubGraph("Delaunay");
    delaunaySg->delEdges(graph->getEdges());

    for (size_t i = 0 ; i < edges.size() ; ++i) {
      delaunaySg->addEdge(nodes[edges[i].first], nodes[edges[i].second]);
    }

    if (simplicesSubGraphs) {
      ostringstream oss;
      unsigned int simCpt = 0;

      for (size_t i = 0 ; i < simplices.size() ; ++i) {
        set<tlp::node> nodesSet;

        for (size_t j = 0 ; j < simplices[i].size() ; ++j) {
          nodesSet.insert(nodes[simplices[i][j]]);
        }

        oss.str("");

        if (simplices[i].size() == 3) {
          oss << "triangle " << simCpt++;
        }
        else {
          oss << "tetrahedron " << simCpt++;
        }

        tlp::Graph *simplexSg = delaunaySg->inducedSubGraph(nodesSet);
        simplexSg->setName(oss.str());
      }
    }
  }

  return ret;
}

class DelaunayTriangulation : public tlp::Algorithm {

public :

  DelaunayTriangulation(tlp::PluginContext *context) : Algorithm(context) {
    addInParameter<bool>("simplices", "If checked, a subgraph will be added for each computed simplex (a triangle in 2d, a tetrahedron in 3d).", "false");
  }

  PLUGININFORMATION("Delaunay triangulation","Antoine LAMBERT","","","1.0","Triangulation")

  bool run() {
    tlp::Observable::holdObservers();

    bool simplicesSg = false;

    if (dataSet)
      dataSet->get("simplices", simplicesSg);

    bool ret = delaunayTriangulation(graph, simplicesSg);

    tlp::Observable::unholdObservers();

    return ret;
  }

};

PLUGIN(DelaunayTriangulation)
