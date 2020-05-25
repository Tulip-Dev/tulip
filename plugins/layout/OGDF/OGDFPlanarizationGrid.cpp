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
#include <ogdf/planarity/PlanarizationGridLayout.h>

#include <tulip2ogdf/OGDFLayoutPluginBase.h>

#include <tulip/StringCollection.h>

using namespace tlp;
using namespace ogdf;

static const char *paramHelp[] = {
    // page ratio
    "Sets the option pageRatio.",

    // number of crossings
    "Returns the number of crossings in the computed layout"};

class OGDFPlanarizationGrid : public OGDFLayoutPluginBase {

  ogdf::PlanarizationGridLayout *pgl;

public:
  PLUGININFORMATION("Planarization Grid (OGDF)", "Carsten Gutwenger", "12/11/2007",
                    "The planarization grid layout algorithm applies the planarization approach "
                    "for crossing minimization, combined with the topology-shape-metrics approach "
                    "for orthogonal planar graph drawing. It produces drawings with few crossings "
                    "and is suited for small to medium sized sparse graphs. It uses a planar grid "
                    "layout algorithm to produce a drawing on a grid.",
                    "1.0", "Planar")
  OGDFPlanarizationGrid(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ogdf::PlanarizationGridLayout()),
        pgl(static_cast<ogdf::PlanarizationGridLayout *>(ogdfLayoutAlgo)) {
    addInParameter<double>("page ratio", paramHelp[0], "1.1");
    addOutParameter<int>("number of crossings", paramHelp[1]);
  }

  ~OGDFPlanarizationGrid() override {}

  void beforeCall() override {
    if (dataSet != nullptr) {
      double dval = 0;

      if (dataSet->get("page ratio", dval))
        pgl->pageRatio(dval);
    }
  }

  void afterCall() override {
    if (dataSet != nullptr) {
      dataSet->set("number of crossings", pgl->numberOfCrossings());
    }
  }
};

PLUGIN(OGDFPlanarizationGrid)
