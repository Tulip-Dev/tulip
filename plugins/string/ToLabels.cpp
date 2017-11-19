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

using namespace tlp;

static const char *paramHelp[] = {
    // input
    "Property to stringify values on labels.",

    // selection
    "Set of elements for which to set the labels.",

    // nodes
    "Sets labels on nodes.",

    // edges
    "Set labels on edges."};

class ToLabels : public tlp::StringAlgorithm {
public:
  PLUGININFORMATION("To labels", "Ludwig Fiolka", "2012/03/16",
                    "Maps the labels of the graph elements onto the values of a given property.",
                    "1.0", "")
  ToLabels(const tlp::PluginContext *context) : StringAlgorithm(context) {
    addInParameter<PropertyInterface *>("input", paramHelp[0], "viewMetric", true);
    addInParameter<BooleanProperty>("selection", paramHelp[1], "", false);
    addInParameter<bool>("nodes", paramHelp[2], "true");
    addInParameter<bool>("edges", paramHelp[3], "true");
  }

  bool run() override {
    PropertyInterface *input = nullptr;
    BooleanProperty *selection = nullptr;
    bool onNodes = true;
    bool onEdges = true;

    if (dataSet != nullptr) {
      dataSet->get("input", input);
      dataSet->get("selection", selection);
      dataSet->get("nodes", onNodes);
      dataSet->get("edges", onEdges);
    }

    pluginProgress->showPreview(false);

    if (onNodes) {
      pluginProgress->setComment("Copying nodes values");
      int step = 0, max_step = graph->numberOfNodes();
      node n;
      forEach(n, selection ? selection->getNonDefaultValuatedNodes() : graph->getNodes()) {
        if ((++step % 100) == 0)
          pluginProgress->progress(step, max_step);

        result->setNodeValue(n, input->getNodeStringValue(n));
      }
    }

    if (onEdges) {
      pluginProgress->setComment("Copying edges values");
      int step = 0, max_step = graph->numberOfEdges();
      edge e;
      forEach(e, selection ? selection->getNonDefaultValuatedEdges() : graph->getEdges()) {
        if ((++step % 100) == 0)
          pluginProgress->progress(step, max_step);

        result->setEdgeValue(e, input->getEdgeStringValue(e));
      }
    }

    return true;
  }
};

PLUGIN(ToLabels)
