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
#include <ogdf/upward/VisibilityLayout.h>
#include <ogdf/packing/ComponentSplitterLayout.h>

#include <tulip2ogdf/OGDFLayoutPluginBase.h>

using namespace tlp;
using namespace ogdf;

static const char *paramHelp[] = {
    // minimum grid distance
    "The minimum grid distance.",

    // transpose
    "If true, transpose the layout vertically."};

class OGDFVisibility : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Visibility (OGDF)", "Hoi-Ming Wong", "12/11/2007",
                    "Implements a simple upward drawing algorithm based on visibility "
                    "representations (horizontal segments for nodes, vectical segments for edges).",
                    "1.1", "Hierarchical")
  OGDFVisibility(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ComponentSplitterLayout()) {
    addInParameter<int>("minimum grid distance", paramHelp[0], "1");
    addInParameter<bool>("transpose", paramHelp[1], "false");
  }

  ~OGDFVisibility() override {}

  void beforeCall() override {

    ogdf::ComponentSplitterLayout *csl =
        static_cast<ogdf::ComponentSplitterLayout *>(ogdfLayoutAlgo);
    // ComponentSplitterLayout takes ownership of the VisibilityLayout instance
    VisibilityLayout *visibility = new ogdf::VisibilityLayout();
    csl->setLayoutModule(visibility);

    if (dataSet != nullptr) {
      int ival = 0;

      if (dataSet->get("minimum grid distance", ival))
        visibility->setMinGridDistance(ival);
    }
  }

  void afterCall() override {
    if (dataSet != nullptr) {
      bool bval = false;

      if (dataSet->get("transpose", bval)) {
        if (bval) {
          transposeLayoutVertically();
        }
      }
    }
  }
};

PLUGIN(OGDFVisibility)
