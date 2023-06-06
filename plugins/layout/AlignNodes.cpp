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

#include <cfloat>

#include <tulip/PropertyAlgorithm.h>
#include <tulip/LayoutProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/StringCollection.h>

#include <set>

using namespace tlp;

static const char *paramHelp[] = {
    // selection
    "The property indicating the selected nodes to align.",

    // alignment
    "Align selected nodes on the min, max, middle or average value of their X or Y coordinates.",

    // distance
    "The distance between two consecutive aligned nodes; used only if strictly positive. The distance is computed from the node with the minimum coordinate on the chosen axis."};

#define ALIGNMENT "X min;Y min;X max;Y max;X average;Y average;X middle; Y middle"
#define X_AXIS 0
#define Y_AXIS 1
#define X_MIN 0
#define Y_MIN 1
#define X_MAX 2
#define Y_MAX 3
#define X_AVG 4
#define Y_AVG 5
#define X_MIDDLE 6
#define Y_MIDDLE 7

class NodeAlignment : public tlp::LayoutAlgorithm {
public:
  PLUGININFORMATION(
      "Align nodes", "Bruno Pinaud", "06/06/2023",
      "Align a set of selected nodes on X or Y coordinates with an optional equal distance between the nodes",
      "1.0", "Misc")

  NodeAlignment(const tlp::PluginContext *context) : LayoutAlgorithm(context, true) {
    addInParameter<BooleanProperty>("selection", paramHelp[0], "viewSelection");
    addInParameter<StringCollection>("alignment", paramHelp[1], ALIGNMENT);
    addInParameter<float>("distance", paramHelp[2], "-1");
  }

  bool run() override {
    BooleanProperty *selection = graph->getProperty<BooleanProperty>("viewSelection");
    tlp::StringCollection alignmentColl(ALIGNMENT);
    alignmentColl.setCurrent(0);
    float distance(-1);
    if (dataSet != nullptr) {
      dataSet->get("selection", selection);
      dataSet->get("alignment", alignmentColl);
      dataSet->get("distance", distance);
    }
    float new_coord, new_coord_min, new_coord_max;
    unsigned int nbNodes = 0;
    auto alignment = alignmentColl.getCurrent();
    // x coordinate is on even collection values
    // while y coordinate is on odd collection values
    auto axis = alignment % 2;
    switch (alignment) {
    case X_MIN:
    case Y_MIN:
      new_coord = FLT_MAX;
      for (auto n : selection->getNonDefaultValuatedNodes(graph))
        new_coord = std::min(new_coord, result->getNodeValue(n)[axis]);
      break;
    case X_MAX:
    case Y_MAX:
      new_coord = -FLT_MAX;
      for (auto n : selection->getNonDefaultValuatedNodes(graph))
        new_coord = std::max(new_coord, result->getNodeValue(n)[axis]);
      break;
    case X_AVG:
    case Y_AVG:
      new_coord = 0;
      for (auto n : selection->getNonDefaultValuatedNodes(graph)) {
        new_coord += result->getNodeValue(n)[axis];
        ++nbNodes;
      }
      // get average
      new_coord /= nbNodes;
      break;
    default: // X_MIDDLE, Y_MIDDLE
      new_coord_min = FLT_MAX;
      new_coord_max = -FLT_MAX;
      for (auto n : selection->getNonDefaultValuatedNodes(graph)) {
        float val(result->getNodeValue(n)[axis]);
        new_coord_min = std::min(new_coord_min, val);
        new_coord_max = std::max(new_coord_max, val);
      }
      new_coord = (new_coord_max + new_coord_min) / 2;
    }

    // set new coordinate value
    for (auto n : selection->getNonDefaultValuatedNodes(graph)) {
      Coord c(result->getNodeValue(n));
      c[axis] = new_coord;
      result->setNodeValue(n, c);
    }

    // apply distance between consecutive nodes if needed
    if (distance > 0) {
      // sort nodes on the other coordinate
      axis ^= Y_AXIS;
      auto comp_min = [this, axis](const node n1, const node n2) {
        return result->getNodeValue(n1)[axis] < result->getNodeValue(n2)[axis];
      };
      std::set<node, decltype(comp_min)> node_dist(comp_min);
      for (auto n : selection->getNonDefaultValuatedNodes(graph)) {
        node_dist.insert(n);
      }
      // get the other coordinate min value
      new_coord = result->getNodeValue(*node_dist.begin())[axis];

      for (node n : node_dist) {
        Coord c(result->getNodeValue(n));
        c[axis] = new_coord;
        result->setNodeValue(n, c);
        // add distance for next node
        new_coord += distance;
      }
    }

    return true;
  }
};

PLUGIN(NodeAlignment)
