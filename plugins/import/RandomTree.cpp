/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
#include <ctime>
#include <tulip/TulipPluginHeaders.h>

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
    // min size
    "Minimum number of nodes in the tree.",

    // max size
    "Maximum number of nodes in the tree.",

    // tree layout
    "If true, the generated tree is drawn with the 'Tree Leaf' layout algorithm."};

/** \addtogroup import */

/// Random Tree - Import of a random uniform binary tree
/** This plugin enables to create a random tree
 *
 *  User can specify the minimal/maximal numbers of nodes used to build of the tree.
 */
class RandomTree : public ImportModule {

  bool buildNode(node n, unsigned int sizeM) {
    if (graph->numberOfNodes() >= sizeM - 1)
      return false;

    bool result = true;
    int randNumber = randomInteger(RAND_MAX);

    if (randNumber > RAND_MAX / 2) {
      node n1, n2;
      n1 = graph->addNode();
      graph->addEdge(n, n1);
      result = buildNode(n1, sizeM);

      if (result) {
        n2 = graph->addNode();
        graph->addEdge(n, n2);
        result = buildNode(n2, sizeM);
      }
    }

    return result;
  }

public:
  PLUGININFORMATION("Uniform Random Binary Tree", "Auber", "16/02/2001",
                    "Imports a new randomly generated uniform binary tree.", "1.2", "Graph")
  RandomTree(tlp::PluginContext *context) : ImportModule(context) {
    addInParameter<unsigned int>("min size", paramHelp[0], "50");
    addInParameter<unsigned int>("max size", paramHelp[1], "60");
    addInParameter<bool>("tree layout", paramHelp[2], "false");
    addDependency("Tree Leaf", "1.1");
  }

  bool importGraph() override {
    // initialize a random sequence according the given seed
    tlp::initRandomSequence();

    unsigned int minSize = 100;
    unsigned int maxSize = 1000;
    bool needLayout = false;

    if (dataSet != nullptr) {
      dataSet->get("min size", minSize);
      dataSet->get("max size", maxSize);
      dataSet->get("tree layout", needLayout);
    }

    if (maxSize < 1) {
      if (pluginProgress)
        pluginProgress->setError("Error: \"max size\" must be a strictly positive integer");

      return false;
    }

    if (maxSize < minSize) {
      if (pluginProgress)
        pluginProgress->setError("Error: \"max size\" must be greater than \"min size\"");

      return false;
    }

    bool ok = true;
    int i = 0;

    while (ok) {
      if (pluginProgress->progress(i % 100, 100) != TLP_CONTINUE)
        break;

      ++i;
      graph->clear();
      node n = graph->addNode();
      ok = !buildNode(n, maxSize);

      if (graph->numberOfNodes() < minSize)
        ok = true;
    }

    if (pluginProgress->progress(100, 100) == TLP_CANCEL)
      return false;

    if (needLayout) {
      // apply Tree Leaf
      string errMsg;
      LayoutProperty *layout = graph->getProperty<LayoutProperty>("viewLayout");
      return graph->applyPropertyAlgorithm("Tree Leaf", layout, errMsg, nullptr, pluginProgress);
    }

    return true;
  }
};

PLUGIN(RandomTree)
