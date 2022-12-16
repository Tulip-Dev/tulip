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
#include <tulip/StringCollection.h>
#include <tulip/StaticProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/GraphMeasure.h>
#include <tulip/TreeTest.h>
#include <unordered_set>

using namespace tlp;

static const char *paramHelp[] = {
    // type
    "Type of edges to consider (in/out/inout).",

    // self loops
    "if true, self loops are considered, i.e. a node is considered as its neighbor. By default, they are ignored."};

#define NEIGH_TYPE "type"
#define NEIGH_TYPES "InOut;In;Out"
#define NEIGH_INOUT UNDIRECTED
#define NEIGH_IN INV_DIRECTED
#define NEIGH_OUT DIRECTED

//==============================================================================

class NeighborhoodMetric : public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION(
      "Unique Neighbors", "Bruno Pinaud", "15/12/2022",
      "Compute the number of unique neighbors of each node; \"unique\" means that if multiple edges exist between two nodes, they count as one. When the graph is simple, it is equivalent to the degree computation.",
      "1.0", "Graph")
  NeighborhoodMetric(const tlp::PluginContext *context) : DoubleAlgorithm(context) {
    addInParameter<StringCollection>(NEIGH_TYPE, paramHelp[0], NEIGH_TYPES, true,
                                     "InOut <br> In <br> Out");
    addInParameter<bool>("self loops", paramHelp[1], "false");
  }
  //==================================================================
  bool run() override {
    StringCollection neighTypes(NEIGH_TYPES);
    neighTypes.setCurrent(0);
    bool loops(false);
    NodeStaticProperty<double> num(graph);

    if (dataSet != nullptr) {
      dataSet->get(NEIGH_TYPE, neighTypes);
      dataSet->get("self loops", loops);
    }

    if (TreeTest::isFreeTree(graph))
      tlp::degree(graph, num, static_cast<EDGE_TYPE>(neighTypes.getCurrent()));
    else {
      switch (neighTypes.getCurrent()) {
      case NEIGH_INOUT:
        TLP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
          std::unordered_set<node> nd;
          for (auto n2 : graph->getInOutNodes(n)) {
            if (!loops && (n2 == n)) {
              continue;
            }
            nd.insert(n2);
          }
          num[i] = nd.size();
        });
        break;
      case NEIGH_IN:
        TLP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
          std::unordered_set<node> nd;
          for (auto n2 : graph->getInNodes(n)) {
            if (!loops && (n2 == n)) {
              continue;
            }
            nd.insert(n2);
          }
          num[i] = nd.size();
        });
        break;
      case NEIGH_OUT:
        TLP_PARALLEL_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
          std::unordered_set<node> nd;
          for (auto n2 : graph->getOutNodes(n)) {
            if (!loops && (n2 == n)) {
              continue;
            }
            nd.insert(n2);
          }
          num[i] = nd.size();
        });
        break;
      default:
        break;
      }
    }
    num.copyToProperty(result);

    return true;
  }
};

PLUGIN(NeighborhoodMetric)
