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

#include <ogdf/energybased/NodeRespecterLayout.h>

#include <tulip2ogdf/OGDFLayoutPluginBase.h>
using namespace tlp;
using namespace ogdf;

static const char *paramHelp[] = {
    // number of iterations
    "The maximum number of iterations."

};

class OGDFNodeRespecter : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION(
      "Node Respecter (OGDF)", "Max Isen", "12/11/2017",
      "This is a force-directed layout algorithm respecting the shapes and sizes of nodes."
      "It aims to minimize the number of node overlaps as well as the number of edges crossing "
      "through non-incident nodes."
      "In order to achieve this, the algorithm adapts its forces to the node sizes and bends edges "
      "around close-by nodes."
      "The edge bends are created by introducing dummy nodes into the graph, positioning all nodes "
      "according to forces acting upon them,"
      "filtering out unnecessary dummy nodes, and then replacing the remaining dummy nodes by edge "
      "bends."
      "The algorithm is documented in and was developed for the bachelor thesis: Max Ilsen: "
      "Energy-Based Layout Algorithms for Graphs with Large Nodes. University of Osnabrueck, 2017",
      "1.0", "Force Directed")
  OGDFNodeRespecter(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ogdf::NodeRespecterLayout()) {
    addInParameter<unsigned>("number of iterations", paramHelp[0], "30000");
  }
  ~OGDFNodeRespecter() override {}

  void beforeCall() override {
    ogdf::NodeRespecterLayout *npl = static_cast<ogdf::NodeRespecterLayout *>(ogdfLayoutAlgo);
    if (dataSet != nullptr) {
      double dval = 0;
      int ival = 0;
      unsigned uval = 0;
      bool bval = 0;

      if (dataSet->get("number of iterations", uval))
        npl->setNumberOfIterations(uval);
    }
  }
};

PLUGIN(OGDFNodeRespecter)
