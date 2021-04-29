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
#include <ogdf/tree/RadialTreeLayout.h>

#include <tulip2ogdf/OGDFLayoutPluginBase.h>

#include <tulip/StringCollection.h>

#define ELT_ROOTSELECTION "Root selection"
#define ELT_ROOTSELECTIONLIST "Source;Sink;Center"
#define ELT_ROOTSOURCE 0
#define ELT_ROOTSINK 1
#define ELT_ROOTCENTER 2

using namespace tlp;
using namespace ogdf;

static const char *paramHelp[] = {
    // levels distance
    "The minimal required vertical distance between levels.",

    // trees distance
    "The minimal required horizontal distance between trees in the forest.",

    // Root selection
    "This parameter indicates how the root is selected."};

static const char *rootSelectionValuesDescription =
    "Source <i>(Select a source in the graph)</i><br>"
    "Sink <i>(Select a sink in the graph)</i><br>"
    "Center <i>(Select the center of the tree</i><br>";

class OGDFRadialTree : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Radial Tree (OGDF)", "Carsten Gutwenger", "02/02/2020",
                    "The radial tree layout algorithm. ", "1.0", "Tree")
  OGDFRadialTree(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, context ? new ogdf::RadialTreeLayout() : nullptr) {
    addInParameter<double>("levels distance", paramHelp[0], "50");
    addInParameter<double>("trees distance", paramHelp[1], "50");
    addInParameter<StringCollection>(ELT_ROOTSELECTION, paramHelp[2], ELT_ROOTSELECTIONLIST, true,
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

      if (dataSet->get("trees distance", dval))
        layout->connectedComponentDistance(dval);

      if (dataSet->get(ELT_ROOTSELECTION, sc)) {
        if (sc.getCurrent() == ELT_ROOTSOURCE) {
          layout->rootSelection(RadialTreeLayout::RootSelectionType::Source);
        } else if (sc.getCurrent() == ELT_ROOTSINK) {
          layout->rootSelection(RadialTreeLayout::RootSelectionType::Sink);
        } else {
          layout->rootSelection(RadialTreeLayout::RootSelectionType::Center);
        }
      }
    }
  }
};

PLUGIN(OGDFRadialTree)
