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
#include <tulip2ogdf/OGDFLayoutPluginBase.h>

#include <vector>

#include <tulip/DrawingTools.h>

#include <ogdf/basic/basic.h>

using namespace std;
using namespace tlp;
using namespace ogdf;

OGDFLayoutPluginBase::OGDFLayoutPluginBase(const tlp::PluginContext *context,
                                           ogdf::LayoutModule *ogdfLayoutAlgo)
    : tlp::LayoutAlgorithm(context), tlpToOGDF(nullptr), ogdfLayoutAlgo(ogdfLayoutAlgo) {
  // convert Tulip Graph to OGDF Graph including attributes
  if (graph)
    tlpToOGDF = new TulipToOGDF(graph, false);
}

OGDFLayoutPluginBase::~OGDFLayoutPluginBase() {
  delete tlpToOGDF;
  delete ogdfLayoutAlgo;
}

bool OGDFLayoutPluginBase::run() {
  if (pluginProgress) {
    // user cannot interact while computing
    pluginProgress->showPreview(false);
    pluginProgress->showStops(false);
  }

  ogdf::GraphAttributes &gAttributes = tlpToOGDF->getOGDFGraphAttr();

  beforeCall();

  try {
    // run the algorithm on the OGDF Graph with attributes
    callOGDFLayoutAlgorithm(gAttributes);

  } catch (AlgorithmFailureException &afce) {
    std::string msg;

    switch (afce.exceptionCode()) {
    case AlgorithmFailureCode::IllegalParameter:
      msg = "function parameter is illegal";
      break;

    case AlgorithmFailureCode::NoFlow:
      msg = "min-cost flow could not find a legal flow";
      break;

    case AlgorithmFailureCode::Sort:
      msg = "sequence not sorted";
      break;

    case AlgorithmFailureCode::Label:
      msg = "labelling failed";
      break;

    case AlgorithmFailureCode::ExternalFace:
      msg = "external face not correct";
      break;

    case AlgorithmFailureCode::ForbiddenCrossing:
      msg = "crossing forbidden but necessary";
      break;

    case AlgorithmFailureCode::TimelimitExceeded:
      msg = "it took too long";
      break;

    case AlgorithmFailureCode::NoSolutionFound:
      msg = "couldn't solve the problem";
      break;

    default:
      msg = "unknown error";
    }

    pluginProgress->setError(std::string("Error\n") + msg);
    return false;
  }

  // retrieve nodes coordinates computed by the OGDF Layout Algorithm
  // and store them in the Tulip Layout Property
  const std::vector<tlp::node> &nodes = graph->nodes();
  size_t nbElts = nodes.size();

  for (unsigned int i = 0; i < nbElts; ++i) {
    tlp::Coord nodeCoord = tlpToOGDF->getNodeCoordFromOGDFGraphAttr(i);
    result->setNodeValue(nodes[i], nodeCoord);
  }

  // same operation as above but with edges
  const std::vector<tlp::edge> &edges = graph->edges();
  nbElts = edges.size();

  for (unsigned int i = 0; i < nbElts; ++i) {
    vector<tlp::Coord> edgeCoord = tlpToOGDF->getEdgeCoordFromOGDFGraphAttr(i);
    result->setEdgeValue(edges[i], edgeCoord);
  }

  afterCall();

  return true;
}

void OGDFLayoutPluginBase::callOGDFLayoutAlgorithm(GraphAttributes &gAttributes) {
  ogdfLayoutAlgo->call(gAttributes);
}

void OGDFLayoutPluginBase::transposeLayoutVertically() {

  const vector<tlp::node> &nodes = graph->nodes();
  const vector<tlp::edge> &edges = graph->edges();

  tlp::BoundingBox graphBB =
      tlp::computeBoundingBox(nodes, edges, result, graph->getProperty<SizeProperty>("viewSize"),
                              graph->getProperty<DoubleProperty>("viewRotation"));
  float midY = (graphBB[0][1] + graphBB[1][1]) / 2.f;

  for (auto n : nodes) {
    tlp::Coord nodeCoord = result->getNodeValue(n);
    nodeCoord[1] = midY - (nodeCoord[1] - midY);
    result->setNodeValue(n, nodeCoord);
  }

  for (auto e : edges) {
    const std::vector<tlp::Coord> &bends = result->getEdgeValue(e);

    if (!bends.empty()) {
      for (auto b : bends) {
        b[1] = midY - (b[1] - midY);
      }

      result->setEdgeValue(e, bends);
    }
  }
}
