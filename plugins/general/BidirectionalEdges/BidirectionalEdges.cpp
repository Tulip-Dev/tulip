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

#include <tulip/Algorithm.h>
#include <tulip/SimpleTest.h>
#include <tulip/DoubleProperty.h>

using namespace tlp;
using namespace std;

static const char *paramHelp[] = {
    // weight
    "The property used to compute the length ratio.",
    // pairs
    "The number of bidirectional edges found."};

class BidirectionalEdges : public tlp::Algorithm {

public:
  PLUGININFORMATION(
      "Bidirectional Edges", "Bruno Pinaud", "16/11/2022",
      "When there are two weighted inverse edges between two nodes (one in each direction), called bidirectional edges, by default Tulip draw them one on top of the other. "
      "This plugin allows to compute and display only a ratio of their length based on the edge weight to enhance the visualization. "
      "The ratio of the length to display is computed by divided the weight of one edge by the sum of the weights of both edges. "
      "The ratio of the second edge is simply computed by 1 minus the ratio of the first edge.<br/>"
      "<b>Warning</b>: the computation will failed if the ratios are not into [0, 1].<br/><br/>"
      "Do not forget to display edges extremities. They will be displayed at a distance proportional to the computed length ratio. "
      "This plugin works only for edges without bends and when there are only two inverse edges between a pair of nodes.",
      "1.0", "")

  BidirectionalEdges(tlp::PluginContext *context) : tlp::Algorithm(context) {
    addInParameter<NumericProperty *>("edge weight", paramHelp[0], "viewMetric");
    addOutParameter<int>("#bidirectional edges", paramHelp[1], "0");
  }

  bool check(string &err) override {
    // get multiples edges
    SimpleTest::simpleTest(graph, &multipleEdges, nullptr, false);

    if (multipleEdges.empty()) {
      err = "The graph has no multiple edges between any pair of nodes. Nothing to do.";
      return false;
    }

    return true;
  }

  bool run() override {

    NumericProperty *weight = graph->getProperty<DoubleProperty>("viewMetric");

    if (dataSet != nullptr) {
      dataSet->get("edge weight", weight);
    }

    DoubleProperty *ratio = graph->getProperty<DoubleProperty>("viewLengthRatio");
    ratio->setAllEdgeValue(1);

    pluginProgress->showPreview(false);
    // compute ratio based on the given weight
    int step = 0, max_step = multipleEdges.size(), nb = 0;

    for (edge e : multipleEdges) {
      if ((++step % 100) == 0) {
        ProgressState state = pluginProgress->progress(step, max_step);

        if (state != TLP_CONTINUE)
          return state != TLP_CANCEL;
      }
      auto ends = graph->ends(e);
      // do the work only if there are exactly
      // two inverse edges between the pair of nodes
      if (graph->getEdges(ends.first, ends.second, false).size() == 2) {
        edge e_inv(graph->existEdge(ends.second, ends.first, true));
        if (e_inv.isValid() == false)
          continue;
        double e_w = weight->getEdgeDoubleValue(e);
        double r = e_w + weight->getEdgeDoubleValue(e_inv);
        if (r == 0) {
          std::ostringstream ess;
          ess << "Error:\nDivision by zero for edges with ids #" << e.id << " and #" << e_inv.id;
          pluginProgress->setError(ess.str());
          return false;
        } else {
          r = e_w / r;
          // check if r is really a ratio
          if (r < 0 || r > 1) {
            std::ostringstream ess;
            ess << "Error:\nRatio computed for edges with ids " << e.id << " and " << e_inv.id
                << " do not belong to [0, 1].";
            pluginProgress->setError(ess.str());
            return false;
          }
        }

        ratio->setEdgeValue(e, r);
        ratio->setEdgeValue(e_inv, 1 - r);
        ++nb;
      }
    }

    dataSet->set("#bidirectional edges", nb);
    return true;
  }

private:
  vector<edge> multipleEdges;
};

PLUGIN(BidirectionalEdges)
