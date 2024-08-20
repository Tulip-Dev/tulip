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
#include <ogdf/tree/RadialTreeLayout.h>

#include <tulip2ogdf/OGDFLayoutPluginBase.h>

#include <tulip/StringCollection.h>
#include <tulip/TreeTest.h>

#define ROOTSELECTIONLIST "source;sink;center"
#define ROOTSOURCE 0
#define ROOTSINK 1
#define ROOTCENTER 2

using namespace tlp;
using namespace ogdf;

static const char *paramHelp[] = {
    // levels distance
    "The minimal required vertical distance between levels.",

    // root selection
    "This parameter indicates how the root is selected."};

static const char *rootSelectionValuesDescription =
    "source <i>(Select a source in the graph)</i><br>"
    "sink <i>(Select a sink in the graph)</i><br>"
    "center <i>(Select the center of the tree</i><br>";

class OGDFRadialTree : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Radial Tree (OGDF)", "Carsten Gutwenger", "02/02/2020",
                    "The radial tree layout algorithm. ", "1.2", "Tree")
  OGDFRadialTree(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, context ? new ogdf::RadialTreeLayout() : nullptr) {
    addInParameter<double>("levels distance", paramHelp[0], "50");
    addInParameter<StringCollection>("root selection", paramHelp[1], ROOTSELECTIONLIST, true,
                                     rootSelectionValuesDescription);
  }

  ~OGDFRadialTree() override {}

  bool check(string &error) override {
    if (!tlp::TreeTest::isTree(graph)) {
      error += "graph is not a directed tree";
      return false;
    }

    return true;
  }

  void beforeCall() override {
    ogdf::RadialTreeLayout *layout = static_cast<ogdf::RadialTreeLayout *>(ogdfLayoutAlgo);

    if (dataSet != nullptr) {
      double dval = 0;
      StringCollection sc;

      if (dataSet->get("levels distance", dval))
        layout->levelDistance(dval);

      if (dataSet->get("root selection", sc)) {
        switch (sc.getCurrent()) {
        case ROOTSOURCE:
          layout->rootSelection(RadialTreeLayout::RootSelectionType::Source);
          break;
        case ROOTSINK:
          layout->rootSelection(RadialTreeLayout::RootSelectionType::Sink);
          break;
        default:
          layout->rootSelection(RadialTreeLayout::RootSelectionType::Center);
        }
      }
    }
  }
};

PLUGIN(OGDFRadialTree)
