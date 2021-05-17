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
#include <tulip/TulipPluginHeaders.h>

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
    // minimum size
    "Minimal number of nodes in the tree.",

    // maximum size
    "Maximal number of nodes in the tree.",

    // maximal degree
    "Maximal degree of the nodes.",

    // tree layout
    "If true, the generated tree is drawn with the 'Tree Leaf' layout algorithm."};

/** \addtogroup import */

/// Random General Tree - Import of a random general tree
/** This plugin enables to create a random general tree
 *
 *  User can specify the minimal/maximal number of nodes and the maximal degree.
 */
class RandomTreeGeneral : public ImportModule {

  bool buildNode(node n, unsigned int sizeM, int arityMax) {
    if (graph->numberOfNodes() >= sizeM)
      return true;

    bool result = true;
    int randNumber = randomInteger(RAND_MAX);
    int i = 0;

    while (randNumber < RAND_MAX / pow(2.0, 1.0 + i))
      ++i;

    i = i % arityMax;
    graph->reserveNodes(i);
    graph->reserveEdges(i);

    for (; i > 0; --i) {
      node n1;
      n1 = graph->addNode();
      graph->addEdge(n, n1);
      result = result && buildNode(n1, sizeM, arityMax);
    }

    return result;
  }

public:
  PLUGININFORMATION("Random General Tree", "Auber", "16/02/2001",
                    "Imports a new randomly generated tree.", "1.1", "Graph")
  RandomTreeGeneral(tlp::PluginContext *context) : ImportModule(context) {
    addInParameter<unsigned>("minimum size", paramHelp[0], "10");
    addInParameter<unsigned>("maximum size", paramHelp[1], "100");
    addInParameter<unsigned>("maximal node degree", paramHelp[2], "5");
    addInParameter<bool>("tree layout", paramHelp[3], "false");
    addDependency("Tree Leaf", "1.0");
  }

  bool importGraph() override {
    // initialize a random sequence according the given seed
    tlp::initRandomSequence();

    unsigned int sizeMin = 10;
    unsigned int sizeMax = 100;
    unsigned int arityMax = 5;
    bool needLayout = false;

    if (dataSet != nullptr) {
      if (!dataSet->getDeprecated("minimum size", "Minimum size", sizeMin))
        dataSet->get("minsize", sizeMin); // keep old parameter name for backward compatibility

      if (!dataSet->getDeprecated("maximum size", "Maximum size", sizeMax))
        dataSet->get("maxsize", sizeMax); // keep old parameter name for backward compatibility

      if (!dataSet->getDeprecated("maximal node degree", "Maximal node's degree", arityMax))
        dataSet->get("maxdegree", arityMax); // keep old parameter name for backward compatibility

      dataSet->get("tree layout", needLayout);
    }

    if (arityMax < 1) {
      if (pluginProgress)
        pluginProgress->setError(
            "Error: maximum node's degree must be a strictly positive integer");

      return false;
    }

    if (sizeMax < 1) {
      if (pluginProgress)
        pluginProgress->setError("Error: maximum size must be a strictly positive integer");

      return false;
    }

    if (sizeMax < sizeMin) {
      if (pluginProgress)
        pluginProgress->setError("Error: maximum size must be greater than minimum size");

      return false;
    }

    bool ok = true;
    int i = 0;
    unsigned int nbTest = 0;

    while (ok) {
      ++nbTest;

      if (nbTest % 100 == 0) {
        if (pluginProgress->progress((i / 100) % 100, 100) != TLP_CONTINUE)
          break;
      }

      ++i;
      graph->clear();
      node n = graph->addNode();
      ok = !buildNode(n, sizeMax, arityMax);

      if (graph->numberOfNodes() < sizeMin)
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

PLUGIN(RandomTreeGeneral)
