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
#include <ogdf/upward/UpwardPlanarizationLayout.h>
#include <ogdf/packing/ComponentSplitterLayout.h>
#include <tulip2ogdf/OGDFLayoutPluginBase.h>

using namespace tlp;
using namespace ogdf;

static const char *paramHelp[] = {
    // transpose
    "If true, transpose the layout vertically.",

    // number of crossings
    "Returns the number of crossings",

    // number of layers
    "Returns the number of layers/levels"};

class OGDFUpwardPlanarization : public OGDFLayoutPluginBase {

  UpwardPlanarizationLayout *upl;

public:
  PLUGININFORMATION("Upward Planarization (OGDF)", "Hoi-Ming Wong", "12/11/2007",
                    "Implements an alternative to the classical Sugiyama approach. It adapts the "
                    "planarization approach for hierarchical graphs and produces significantly "
                    "less crossings than Sugiyama layout.",
                    "1.1", "Hierarchical")
  OGDFUpwardPlanarization(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ogdf::ComponentSplitterLayout()) {
    addInParameter<bool>("transpose", paramHelp[0], "false");
    addOutParameter<int>("number of crossings", paramHelp[1]);
    addOutParameter<int>("number of layers", paramHelp[2]);
  }

  void beforeCall() override {
    ComponentSplitterLayout *csl = static_cast<ogdf::ComponentSplitterLayout *>(ogdfLayoutAlgo);
    upl = new UpwardPlanarizationLayout();
    csl->setLayoutModule(upl);
  }

  void afterCall() override {
    if (dataSet != nullptr) {
      bool bval = false;

      if (dataSet->get("transpose", bval)) {
        if (bval) {
          transposeLayoutVertically();
        }
      }
      dataSet->set("number of crossings", upl->numberOfCrossings());
      dataSet->set("number of layers", upl->numberOfLevels());
    }
  }
};

PLUGIN(OGDFUpwardPlanarization)
