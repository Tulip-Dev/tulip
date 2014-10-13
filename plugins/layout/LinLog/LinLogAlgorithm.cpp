/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

static const char * paramHelp[] = {
  // 3D
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "Boolean" ) \
  HTML_HELP_BODY() \
  "If true the layout is in 3D else it is computed in 2D" \
  HTML_HELP_CLOSE(),
  // OctTree
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "Boolean" ) \
  HTML_HELP_DEF( "default", "true" ) \
  HTML_HELP_BODY() \
  "If true, use the OctTree optimization" \
  HTML_HELP_CLOSE(),
  // edge weight
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "NumericProperty" ) \
  HTML_HELP_BODY() \
  "This property is used to compute the length of edges." \
  HTML_HELP_CLOSE(),
  // max iterations
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "unsigned integer" ) \
  HTML_HELP_DEF( "default", "100" ) \
  HTML_HELP_BODY() \
  "This parameter allows to limit the number of iterations. The value of 0 corresponds to a default value of 100." \
  HTML_HELP_CLOSE(),
  // attraction exponent
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "float" ) \
  HTML_HELP_DEF( "default", "0.0" ) \
  HTML_HELP_BODY() \
  "This parameter allows to set the exponent of attraction." \
  HTML_HELP_CLOSE(),
  // repulsion exponent
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "float" ) \
  HTML_HELP_DEF( "default", "1.0" ) \
  HTML_HELP_BODY() \
  "This parameter allows to set the exponent of repulsion." \
  HTML_HELP_CLOSE(),
  // gravitation factor
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "float" ) \
  HTML_HELP_DEF( "default", "0.05" ) \
  HTML_HELP_BODY() \
  "This parameter allows to set the factor of gravitation." \
  HTML_HELP_CLOSE(),
  // nodes to skip
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "BooleanProperty" ) \
  HTML_HELP_BODY() \
  "This BooleanProperty is used to skip nodes in computation when set to True." \
  HTML_HELP_CLOSE(),
  // initial layout
  HTML_HELP_OPEN() \
  HTML_HELP_DEF( "type", "LayoutProperty" ) \
  HTML_HELP_BODY() \
  "The layout property used to compute the initial position of the graph elements. If none is given the initial position will be computed by the algorithm." \
  HTML_HELP_CLOSE(),
};

PLUGIN(LinLogAlgorithm)

LinLogAlgorithm::LinLogAlgorithm(const tlp::PluginContext *context) : LayoutAlgorithm(context), linlog(NULL) {
  addInParameter<bool>("3D layout", paramHelp[0], "false");
  addInParameter<bool>("octtree", paramHelp[1], "true");
  addInParameter<NumericProperty*>("edge weight", paramHelp[2], "", false);
  addInParameter<unsigned int>("max iterations", paramHelp[3], "100");
  addInParameter<float>("repulsion exponent", paramHelp[4], "0.0");
  addInParameter<float>("attraction exponent", paramHelp[5], "1.0");
  addInParameter<float>("gravitation factor", paramHelp[6], "0.05");
  addInParameter<BooleanProperty>("skip nodes", paramHelp[7], "", false);
  addInParameter<LayoutProperty>("initial layout", paramHelp[8], "", false);
}


LinLogAlgorithm::~LinLogAlgorithm() {
}


bool LinLogAlgorithm::run() {
  bool is3D = false;
  bool useOctTree = false;

  unsigned int max_iter = 100;
  tlp::NumericProperty* edgeWeight = NULL;
  tlp::BooleanProperty* skipNodes = NULL;
  float aExp = 1.0;
  float rExp = 0.0;
  float gFac = 0.9f;
  LayoutProperty* layout = NULL;

  if ( dataSet!=NULL ) {
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

  //launches the lin log algorithm
  linlog->initAlgo(result, edgeWeight,
                   aExp, rExp,  gFac,
                   max_iter,
                   is3D, useOctTree, skipNodes);

  return linlog->startAlgo();
}
