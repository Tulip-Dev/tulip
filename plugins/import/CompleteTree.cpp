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
#include <list>
#include <tulip/TulipPluginHeaders.h>

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
    // depth
    "Depth of the tree.",

    // degree
    "The tree's degree.",

    // tree layout
    "If true, the generated tree is drawn with the 'Tree Leaf' layout algorithm."};

/** \addtogroup import */

/// Complete Tree - Import of a complete tree
/** This plugin enables to create a complete tree
 *
 *  User can specify the depth and the degree of the tree.
 */
class CompleteTree : public ImportModule {
public:
  PLUGININFORMATION("Complete Tree", "Auber", "08/09/2002", "Imports a new complete tree.", "1.1",
                    "Graph")
  CompleteTree(tlp::PluginContext *context) : ImportModule(context) {
    addInParameter<unsigned int>("depth", paramHelp[0], "5");
    addInParameter<unsigned int>("degree", paramHelp[1], "2");
    addInParameter<bool>("tree layout", paramHelp[2], "false");
    addDependency("Tree Leaf", "1.0");
  }
  ~CompleteTree() {}

  bool importGraph() {
    unsigned int degree = 2;
    unsigned int depth = 5;
    bool needLayout = false;

    if (dataSet != nullptr) {
      dataSet->get("depth", depth);
      dataSet->get("degree", degree);
      dataSet->get("tree layout", needLayout);
    }

    // reserve enough memory for nodes/edges to add
    unsigned int total = 0, previous = 1;

    for (unsigned int i = 0; i < depth; ++i) {
      previous *= degree;
      total += previous;
    }

    graph->reserveEdges(total);
    graph->addNodes(total + 1);
    const vector<node> &nodes = graph->nodes();

    unsigned int current = 0;
    unsigned int nextChild = 1;

    while (total) {
      node n = nodes[current];

      for (unsigned int i = 0; i < degree; ++i, ++nextChild, --total) {
        graph->addEdge(n, nodes[nextChild]);
      }

      ++current;
    }

    if (needLayout) {
      // apply Tree Leaf
      DataSet dSet;
      string errMsg;
      LayoutProperty *layout = graph->getProperty<LayoutProperty>("viewLayout");
      return graph->applyPropertyAlgorithm("Tree Leaf", layout, errMsg, pluginProgress, &dSet);
    }

    return true;
  }
};

PLUGIN(CompleteTree)
