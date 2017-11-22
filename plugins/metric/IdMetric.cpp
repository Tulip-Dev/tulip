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
#include "IdMetric.h"

#include <tulip/ForEach.h>
#include <tulip/StringCollection.h>

PLUGIN(IdMetric)

using namespace tlp;

static const char *paramHelp[] = {
    // target
    "Whether the id is copied only for nodes, only for edges, or for both."};

#define TARGET_TYPE "target"
#define TARGET_TYPES "both;nodes;edges"
#define NODES_TARGET 1
#define EDGES_TARGET 2
#define BOTH_TARGET 0

//==================================================================
IdMetric::IdMetric(const tlp::PluginContext *context) : DoubleAlgorithm(context) {
  addInParameter<StringCollection>(TARGET_TYPE, paramHelp[0], TARGET_TYPES, true,
                                   "both <br> nodes <br> edges");

  // result needs to be an inout parameter
  // in order to preserve the original values of non targetted elements
  // i.e if "target" = "nodes", the values of edges must be preserved
  // and if "target" = "edges", the values of nodes must be preserved
  parameters.setDirection("result", INOUT_PARAM);
}
//==================================================================
bool IdMetric::run() {
  bool nodes(true), edges(true);

  if (dataSet != nullptr) {
    StringCollection targetType;
    dataSet->get(TARGET_TYPE, targetType);

    if (targetType.getCurrent() == NODES_TARGET) {
      edges = false;
      nodes = true;
    } else if (targetType.getCurrent() == EDGES_TARGET) {
      edges = true;
      nodes = false;
    } else {
      edges = true;
      nodes = true;
    }
  }

  if (nodes) {
    node n;
    forEach(n, graph->getNodes()) result->setNodeValue(n, n.id);
  }

  if (edges) {
    edge e;
    forEach(e, graph->getEdges()) result->setEdgeValue(e, e.id);
  }

  return true;
}
//=================================================================
