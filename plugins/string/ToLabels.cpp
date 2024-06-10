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

#include <tulip/PropertyAlgorithm.h>
#include <tulip/BooleanProperty.h>
#include <tulip/StringProperty.h>

using namespace tlp;

static const char *paramHelp[] = {
    // input
    "Source property",

    // selection
    "Set of elements for which to set the labels. if none is selected, the whole graph will be used.",

    // nodes
    "Sets labels on nodes.",

    // edges
    "Set labels on edges."};

class ToLabels : public tlp::StringAlgorithm {
  bool onNodes, onEdges;

public:
  PLUGININFORMATION(
      "To labels", "Ludwig Fiolka", "2012/03/16",
      "Use a string representation of the values of a given property as the labels of nodes and/or edges.",
      "1.2", "")
  ToLabels(const tlp::PluginContext *context)
      // set second parameter of the constructor below to true because
      // result needs to be an inout parameter
      // in order to preserve the original values of non targeted elements
      // i.e if "nodes" == true and "edges==false", the values of edges must be preserved
      : StringAlgorithm(context, true), onNodes(true), onEdges(true) {
    addInParameter<PropertyInterface *>("property", paramHelp[0], "viewMetric", true);
    addInParameter<BooleanProperty>("selection", paramHelp[1], "", false);
    addInParameter<bool>("nodes", paramHelp[2], "true");
    addInParameter<bool>("edges", paramHelp[3], "true");
  }

  bool check(std::string &errMsg) override {
    if (dataSet != nullptr) {
      dataSet->get("nodes", onNodes);
      dataSet->get("edges", onEdges);
      if ((onNodes == false) && (onEdges == false)) {
        errMsg = "No element selected. Please select at least nodes or edges.";
        return false;
      }
    }
    return true;
  }

  bool run() override {
    PropertyInterface *input = nullptr;
    BooleanProperty *selection = nullptr;

    if (dataSet != nullptr) {
      dataSet->get("property", input);
      dataSet->get("selection", selection);
    } else {
      input = graph->getProperty("viewMetric");
      selection = graph->getProperty<BooleanProperty>("viewSelection");
    }

    pluginProgress->showPreview(false);

    if (onNodes) {
      pluginProgress->setComment("Copying values of nodes");
      int step = 0, max_step = graph->numberOfNodes();
      for (auto n : selection ? selection->getNonDefaultValuatedNodes() : graph->getNodes()) {
        if ((++step % 100) == 0)
          pluginProgress->progress(step, max_step);

        result->setNodeValue(n, input->getNodeStringValue(n));
      }
    }

    if (onEdges) {
      pluginProgress->setComment("Copying values of edges");
      int step = 0, max_step = graph->numberOfEdges();
      for (auto e : selection ? selection->getNonDefaultValuatedEdges() : graph->getEdges()) {
        if ((++step % 100) == 0)
          pluginProgress->progress(step, max_step);

        result->setEdgeValue(e, input->getEdgeStringValue(e));
      }
    }

    return true;
  }
};

PLUGIN(ToLabels)
