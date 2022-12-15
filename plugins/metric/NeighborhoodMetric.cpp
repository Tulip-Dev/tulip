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
#include <tulip/GraphMeasure.h>
#include <tulip/DoubleProperty.h>
#include <tulip/SimpleTest.h>

#include <unordered_set>

using namespace tlp;

static const char *paramHelp[] = {
    // type
    "Type of edges to follow (in/out/inout).",

    //loops
    "if true, reflexive edges (loops) are considered. By default, loops are not considered."
};

#define NEIGH_TYPE "type"
#define NEIGH_TYPES "InOut;In;Out"
#define NEIGH_INOUT 0
#define NEIGH_IN 1
#define NEIGH_OUT 2

//==============================================================================

class NeighborhoodMetric : public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION("Neighborhood", "Bruno Pinaud", "15/12/2022", "Compute the number of unique neighboor for each node "
                                                                  "in a graph with multipe edges between two nodes."
                                                                  "If the graph is simple, the Degree metric should be used instead.",
                    "1.0", "Graph")
NeighborhoodMetric(const tlp::PluginContext *context) : DoubleAlgorithm(context) {
  addInParameter<StringCollection>(NEIGH_TYPE, paramHelp[0], NEIGH_TYPES, true,
                                   "InOut <br> In <br> Out");
  addInParameter<bool>("consider loops", paramHelp[1], "false");
}
//==================================================================
bool run() override {
  StringCollection neighborTypes(NEIGH_TYPES);
  neighborTypes.setCurrent(0);
  bool loops(false);

  if (dataSet != nullptr) {
    dataSet->get(NEIGH_TYPE, neighborTypes);
    dataSet->get("consider loops", loops);
  }

  NodeStaticProperty<double> num(graph);
  switch (neighborTypes.getCurrent()) {
  case NEIGH_INOUT:
        TLP_PARALLEL_MAP_NODES_AND_INDICES(
            graph, [&](const node n, unsigned int i) {
            std::unordered_set<node> nd;
            for (auto n2: graph->getInOutNodes(n)) {
                if(!loops&&(n2==n)) {
                    continue;
                }
                nd.insert(n2);
            }
            num[i] = nd.size();
        });
      break;
  case NEIGH_IN:
      TLP_PARALLEL_MAP_NODES_AND_INDICES(
          graph, [&](const node n, unsigned int i) {
          std::unordered_set<node> nd;
          for (auto n2: graph->getInNodes(n)) {
              if(!loops&&(n2==n)) {
                  continue;
              }
              nd.insert(n2);
          }
          num[i] = nd.size();
      });
    break;
  case NEIGH_OUT:
      TLP_PARALLEL_MAP_NODES_AND_INDICES(
          graph, [&](const node n, unsigned int i) {
          std::unordered_set<node> nd;
          for (auto n2: graph->getOutNodes(n)) {
              if(!loops&&(n2==n)) {
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
  num.copyToProperty(result);

  return true;
}
//==================================================================
bool check(std::string &errorMsg) override {

  if (SimpleTest::isSimple(graph, false)) {
      errorMsg = "The graph is simple. Use the Degree metric plugin instead";
      return false;
  }

  return true;
}
};

PLUGIN(NeighborhoodMetric)
