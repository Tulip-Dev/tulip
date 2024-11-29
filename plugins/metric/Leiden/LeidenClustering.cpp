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
#include <tulip/DoubleProperty.h>

#include "GraphHelper.h"
#include "CPMVertexPartition.h"
#include "ModularityVertexPartition.h"
#include "RBConfigurationVertexPartition.h"
#include "RBERVertexPartition.h"
#include "SignificanceVertexPartition.h"
#include "SurpriseVertexPartition.h"

#include "Optimiser.h"

static const char *paramHelp[] = {
    // directed
    "Indicates if the graph should be considered as directed or not.",

    // metric
    "An existing edge weight metric property. If it is not defined all edges have a weight of 1.0.",

    // quality function
    "Specifies the method to be used to compute the quality of the partition.",

    // resolution
    "A sort of threshold: communities should have a density of at least resolution, while the density between communities should be lower than resolution. Higher values lead to more communities and lower values lead to fewer communities.<br/>Not used when quality function is Modularity, Significance or Surprise."};

#define DEFAULT_RESOLUTION 0.5
#define QUALITY_METHODS                                                                            \
  "Modularity;Constant Potts;Reichardt and Bornholdt;Erdös-Rényi;Significance;Surprise;"

/** \addtogroup clustering */
/*@{*/
/** \file
 * \brief  An implementation of the Leiden clustering algorithm
 *
 * This plugin is an implementation of the Leiden clustering algorithm
 * first published as:
 *
 * Traag, V.A., Waltman. L., Van Eck, N.-J. , \n
 * "From Louvain to Leiden: guaranteeing well-connected communities." \n
 * (2018). Scientific reports, 9(1), 5233
 *
 * This implementation used an adapted version of
 * https://github.com/vtraag/libleidenalg source code
 *
 * <b>HISTORY</b>
 * - 16/01/2024 Version 1.0: Initial release (Patrick Mary)
 *
 * \author Patrick Mary, Labri
 *
 *
 **/
class LeidenClustering : public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION(
      "Leiden", "Patrick Mary", "16/01/24",
      "Nodes partitioning measure used for community detection."
      "This is an implementation of the Leiden clustering algorithm first published in:<br/>"
      "<b>From Louvain to Leiden: guaranteeing well-connected communities.</b><br/>"
      "Traag, V.A., Waltman. L., Van Eck, N.-J. (2018). Scientific reports, 9(1), 5233.<br/>"
      "doi: <a href=\"http://doi.org/10.1088/1742-5468/2008/10/P10008\">"
      "10.1038/s41598-019-41695-z</a>.",
      "1.0", "Clustering")
  LeidenClustering(const tlp::PluginContext *context) : tlp::DoubleAlgorithm(context) {
    addInParameter<bool>("directed", paramHelp[0], "true", false);
    addInParameter<tlp::NumericProperty *>("metric", paramHelp[1], "", false);
    addInParameter<tlp::StringCollection>(
        "quality function", paramHelp[2], QUALITY_METHODS, false,
        "Modularity<br/>Constant Potts<br/>Reichardt and Bornholdt<br/>Erdös-Rényi<br/>Significance<br/>Surprise");
    addInParameter<double>("resolution", paramHelp[3], "0.5", false);
    addOutParameter<double>("quality", "The quality of the partition", "0", false);
    addOutParameter<unsigned int>("#communities", "The number of communities found", "0", false);
  }

  bool run() override {
    tlp::NumericProperty *metric = nullptr;
    bool directed = true;
    double resolution = DEFAULT_RESOLUTION;
    tlp::StringCollection qualityFunctions(QUALITY_METHODS);
    qualityFunctions.setCurrent(0);

    if (dataSet != nullptr) {
      dataSet->get("directed", directed);
      dataSet->get("metric", metric);
      dataSet->get("resolution", resolution);
      dataSet->get("quality function", qualityFunctions);
    }

    // initialize a random sequence according the given seed
    tlp::initRandomSequence();

    // compute partition
    Graph g(graph, metric, directed);
    MutableVertexPartition *part = nullptr;
    switch (qualityFunctions.getCurrent()) {
    case 1:
      part = new CPMVertexPartition(&g, resolution);
      break;
    case 2:
      part = new RBConfigurationVertexPartition(&g, resolution);
      break;
    case 3:
      part = new RBERVertexPartition(&g, resolution);
      break;
    case 4:
      part = new SignificanceVertexPartition(&g);
      break;
    case 5:
      part = new SurpriseVertexPartition(&g);
      break;
    default:
      part = new ModularityVertexPartition(&g);
    }
    Optimiser o;
    o.optimise_partition(part);

    // then set computed values
    unsigned int maxVal = 0;
    auto nodes = graph->nodes();
    for (size_t i = 0; i < g.vcount(); i++) {
      unsigned int val = part->membership(i) + 1;
      result->setNodeValue(nodes[i], val);
      if (val > maxVal)
        maxVal = val;
    }

    if (dataSet != nullptr) {
      dataSet->set("quality", part->quality());
      dataSet->set("#communities", maxVal);
    }
    delete part;

    return true;
  }
};

PLUGIN(LeidenClustering)
