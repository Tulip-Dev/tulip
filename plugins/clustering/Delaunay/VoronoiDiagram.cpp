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

#include <tulip/Algorithm.h>
#include <tulip/TulipPluginHeaders.h>
#include <tulip/Delaunay.h>
#include <tulip/LayoutProperty.h>

using namespace std;

static bool voronoiDiagram(tlp::Graph *graph, bool voronoiCellsSubGraphs,
                           bool connectNodeToCellBorder, bool originalClone) {
  vector<tlp::Coord> sites;
  tlp::node n;
  tlp::VoronoiDiagram voronoiDiag;

  tlp::LayoutProperty *layout = graph->getProperty<tlp::LayoutProperty>("viewLayout");

  sites.reserve(graph->numberOfNodes());
  const std::vector<tlp::node> &nodes = graph->nodes();
  unsigned int nbNodes = nodes.size();

  for (unsigned int i = 0; i < nbNodes; ++i) {
    sites.push_back(layout->getNodeValue(nodes[i]));
  }

  bool ret = tlp::voronoiDiagram(sites, voronoiDiag);

  if (ret) {
    tlp::Graph *voronoiSg = graph->addSubGraph("Voronoi");

    if (originalClone)
      graph->addCloneSubGraph("Original graph");

    for (size_t i = 0; i < voronoiDiag.nbVertices(); ++i) {
      tlp::node n = voronoiSg->addNode();
      layout->setNodeValue(n, voronoiDiag.vertex(i));
    }

    const std::vector<tlp::node> &sgNodes = voronoiSg->nodes();

    for (size_t i = 0; i < voronoiDiag.nbEdges(); ++i) {
      voronoiSg->addEdge(sgNodes[voronoiDiag.edge(i).first], sgNodes[voronoiDiag.edge(i).second]);
    }

    if (voronoiCellsSubGraphs) {
      ostringstream oss;
      unsigned int cellCpt = 0;

      for (unsigned int i = 0; i < voronoiDiag.nbSites(); ++i) {
        oss.str("");
        oss << "voronoi cell " << cellCpt++;
        const tlp::VoronoiDiagram::Cell &cell = voronoiDiag.voronoiCellForSite(i);
        vector<tlp::node> cellSgNodes;
        cellSgNodes.reserve(cell.size());

        for (set<unsigned int>::iterator it2 = cell.begin(); it2 != cell.end(); ++it2) {
          cellSgNodes.push_back(sgNodes[*it2]);
        }

        tlp::Graph *cellSg = voronoiSg->inducedSubGraph(cellSgNodes);
        cellSg->setName(oss.str());
      }
    }

    if (connectNodeToCellBorder) {
      for (unsigned int i = 0; i < voronoiDiag.nbSites(); ++i) {
        voronoiSg->addNode(nodes[i]);

        const tlp::VoronoiDiagram::Cell &cell = voronoiDiag.voronoiCellForSite(i);

        for (set<unsigned int>::iterator it2 = cell.begin(); it2 != cell.end(); ++it2) {
          voronoiSg->addEdge(nodes[i], sgNodes[*it2]);
        }
      }
    }
  }

  return ret;
}

static const char *paramHelp[] = {

    // voronoi cells
    "If true, a subgraph will be added for each computed voronoi cell.",

    // connect
    "If true, existing graph nodes will be connected to the vertices of their voronoi cell.",

    // original clone
    "If true, a clone subgraph named 'Original graph' will be first added."};

class VoronoiDiagram : public tlp::Algorithm {

public:
  VoronoiDiagram(tlp::PluginContext *context) : tlp::Algorithm(context) {
    addInParameter<bool>("voronoi cells", paramHelp[0], "false");
    addInParameter<bool>("connect", paramHelp[1], "false");
    addInParameter<bool>("original clone", paramHelp[2], "true");
  }

  PLUGININFORMATION("Voronoi diagram", "Antoine Lambert", "",
                    "Performs a Voronoi decomposition, in considering the positions of the graph "
                    "nodes as a set of points. These points define the seeds (or sites) of the "
                    "voronoi cells. New nodes and edges are added to build the convex polygons "
                    "defining the contours of these cells.",
                    "1.1", "Triangulation")

  bool run() {
    // no nodes. Nothing to do.
    if (graph->numberOfNodes() == 0)
      return true;

    bool voronoiCellSg = false;
    bool connectNodesToVoronoiCell = false;
    bool originalClone = true;

    if (dataSet) {
      dataSet->get("voronoi cells", voronoiCellSg);
      dataSet->get("connect", connectNodesToVoronoiCell);
      dataSet->get("original clone", originalClone);
    }

    bool ret = voronoiDiagram(graph, voronoiCellSg, connectNodesToVoronoiCell, originalClone);

    return ret;
  }
};

PLUGIN(VoronoiDiagram)
