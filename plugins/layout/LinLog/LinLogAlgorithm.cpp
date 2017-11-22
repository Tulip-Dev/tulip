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
#include "LinLogAlgorithm.h"
#include "LinLogLayout.h"

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
    // 3D
    "If true the layout is in 3D else it is computed in 2D",

    // OctTree
    "If true, use the OctTree optimization",

    // edge weight
    "This property is used to compute the length of edges.",

    // max iterations
    "This parameter allows to limit the number of iterations. The value of 0 corresponds to a "
    "default value of 100.",

    // attraction exponent
    "This parameter allows to set the exponent of attraction.",

    // repulsion exponent
    "This parameter allows to set the exponent of repulsion.",

    // gravitation factor
    "This parameter allows to set the factor of gravitation.",

    // nodes to skip
    "This boolean property is used to skip nodes in computation when their value are set to true.",

    // initial layout
    "The layout property used to compute the initial position of the graph elements. If none is "
    "given the initial position will be computed by the algorithm."};

LinLogAlgorithm::LinLogAlgorithm(const tlp::PluginContext *context)
    : LayoutAlgorithm(context), linlog(nullptr) {
  addInParameter<bool>("3D layout", paramHelp[0], "false");
  addInParameter<bool>("octtree", paramHelp[1], "true");
  addInParameter<NumericProperty *>("edge weight", paramHelp[2], "", false);
  addInParameter<unsigned int>("max iterations", paramHelp[3], "100");
  addInParameter<float>("repulsion exponent", paramHelp[4], "0.0");
  addInParameter<float>("attraction exponent", paramHelp[5], "1.0");
  addInParameter<float>("gravitation factor", paramHelp[6], "0.05");
  addInParameter<BooleanProperty>("skip nodes", paramHelp[7], "", false);
  addInParameter<LayoutProperty>("initial layout", paramHelp[8], "", false);
}

LinLogAlgorithm::~LinLogAlgorithm() {}

bool LinLogAlgorithm::run() {
  bool is3D = false;
  bool useOctTree = false;

  unsigned int max_iter = 100;
  tlp::NumericProperty *edgeWeight = nullptr;
  tlp::BooleanProperty *skipNodes = nullptr;
  float aExp = 1.0;
  float rExp = 0.0;
  float gFac = 0.9f;
  LayoutProperty *layout = nullptr;

  if (dataSet != nullptr) {
    dataSet->get("3D layout", is3D);
    dataSet->get("octtree", useOctTree);
    dataSet->get("edge weight", edgeWeight);
    dataSet->get("max iterations", max_iter);
    dataSet->get("attraction exponent", aExp);
    dataSet->get("repulsion exponent", rExp);
    dataSet->get("gravitation factor", gFac);
    dataSet->get("skip nodes", skipNodes);
    dataSet->get("initial layout", layout);
  }

  linlog = new LinLogLayout(graph, pluginProgress);

  if (layout)
    *result = *layout;
  else {
    std::string err;

    if (graph->applyPropertyAlgorithm("Random layout", result, err) == false) {
      pluginProgress->setError(err);
      return false;
    }
  }

  // launches the lin log algorithm
  linlog->initAlgo(result, edgeWeight, aExp, rExp, gFac, max_iter, is3D, useOctTree, skipNodes);

  return linlog->startAlgo();
}

PLUGIN(LinLogAlgorithm)
