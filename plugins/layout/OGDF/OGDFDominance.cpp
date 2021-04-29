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
#include <ogdf/upward/DominanceLayout.h>
#include <tulip2ogdf/OGDFLayoutPluginBase.h>
#include <tulip/ConnectedTest.h>

using namespace tlp;
using namespace std;

static const char *paramHelp[] = {
    // minimum grid distance
    "The minimum grid distance.",

    // transpose
    "If true, transpose the layout vertically."};

class OGDFDominance : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION(
      "Dominance (OGDF)", "Hoi-Ming Wong", "12/11/2007",
      "Implements a simple upward drawing algorithm based on dominance drawings of st-digraphs.",
      "1.0", "Hierarchical")
  OGDFDominance(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, context ? new ogdf::DominanceLayout() : nullptr) {
    addInParameter<int>("minimum grid distance", paramHelp[0], "1");
    addInParameter<bool>("transpose", paramHelp[1], "false");
  }
  ~OGDFDominance() override {}

  bool check(string &error) override {
    if (!tlp::ConnectedTest::isConnected(graph)) {
      error += "graph is not connected";
      return false;
    }

    return true;
  }

  void beforeCall() override {
    ogdf::DominanceLayout *dominance = static_cast<ogdf::DominanceLayout *>(ogdfLayoutAlgo);

    if (dataSet != nullptr) {
      int ival = 0;

      if (dataSet->get("minimum grid distance", ival))
        dominance->setMinGridDistance(ival);
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

PLUGIN(OGDFDominance)
