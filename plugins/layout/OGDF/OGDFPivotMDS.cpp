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

#include <ogdf/energybased/PivotMDS.h>
#include <ogdf/packing/ComponentSplitterLayout.h>

#include <tulip2ogdf/OGDFLayoutPluginBase.h>

using namespace tlp;
using namespace ogdf;

static const char *paramHelp[] = {
    // number of pivots
    "Sets the number of pivots. If the new value is smaller or equal 0 the default value (250) is "
    "used.",

    // use edge costs
    "Sets if the edge costs attribute has to be used.",

    // edge costs
    "Sets the desired distance between adjacent nodes. If the new value is smaller or equal 0 the "
    "default value (100) is used."};

class OGDFPivotMDS : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Pivot MDS (OGDF)", "Mark Ortmann", "29/05/2015",
                    "The Pivot MDS (multi-dimensional scaling) layout algorithm. By setting the "
                    "number of pivots to infinity this algorithm behaves just like "
                    "classical MDS. See:<br/><b>Eigensolver methods for progressive "
                    "multidimensional scaling of large data.</b> Brandes and Pich",
                    "1.0", "Force Directed")
  OGDFPivotMDS(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ogdf::ComponentSplitterLayout()) {
    addInParameter<int>("number of pivots", paramHelp[0], "250", false);
    addInParameter<bool>("use edge costs", paramHelp[1], "false", false);
    addInParameter<double>("edge costs", paramHelp[2], "100", false);
  }
  ~OGDFPivotMDS() override {}

  void beforeCall() override {
    ComponentSplitterLayout *csl = static_cast<ogdf::ComponentSplitterLayout *>(ogdfLayoutAlgo);
    PivotMDS *pivotMds(new ogdf::PivotMDS());
    csl->setLayoutModule(pivotMds);

    if (dataSet != nullptr) {
      bool bval = false;
      int ival = 0;
      double val = 0;

      if (dataSet->get("number of pivots", ival))
        pivotMds->setNumberOfPivots(ival);

      if (dataSet->get("edge costs", val))
        pivotMds->setEdgeCosts(ival);

      if (dataSet->get("use edge costs", bval))
        pivotMds->useEdgeCostsAttribute(bval);
    }
  }
};

PLUGIN(OGDFPivotMDS)
