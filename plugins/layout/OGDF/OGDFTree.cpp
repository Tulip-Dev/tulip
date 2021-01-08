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
#include <ogdf/tree/TreeLayout.h>

#include <tulip2ogdf/OGDFLayoutPluginBase.h>

#include <tulip/StringCollection.h>

#define ELT_ORIENTATION "Orientation"
#define ELT_ORIENTATIONLIST "topToBottom;bottomToTop;leftToRight;rightToLeft"
#define ELT_TOPTOBOTTOM 0
#define ELT_BOTTOMTOTOP 1
#define ELT_LEFTTORIGHT 2
#define ELT_RIGHTTOLEFT 3

#define ELT_ROOTSELECTION "Root selection"
#define ELT_ROOTSELECTIONLIST "Source;Sink;ByCoord"
#define ELT_ROOTSOURCE 0
#define ELT_ROOTSINK 1
#define ELT_ROOTCOORD 2

using namespace tlp;
using namespace ogdf;

static const char *paramHelp[] = {
    // siblings distance
    "The minimal required horizontal distance between siblings.",

    // subtrees distance
    "The minimal required horizontal distance between subtrees.",

    // levels distance
    "The minimal required vertical distance between levels.",

    // trees distance
    "The minimal required horizontal distance between trees in the forest.",

    // orthogonal layout
    "Indicates whether orthogonal edge routing style is used or not.",

    // Orientation
    "This parameter indicates the orientation of the layout.",

    // Root selection
    "This parameter indicates how the root is selected."};

static const char *orientationValuesDescription =
    "topToBottom <i>(Edges are oriented from top to bottom)</i><br>"
    "bottomToTop <i>(Edges are oriented from bottom to top)</i><br>"
    "leftToRight <i>(Edges are oriented from left to right)</i><br>"
    "rightToLeft <i>(Edges are oriented from right to left)</i>";

static const char *rootSelectionValuesDescription =
    "Source <i>(Select a source in the graph)</i><br>"
    "Sink <i>(Select a sink in the graph)</i><br>"
    "ByCoord <i>(Use the coordinates, e.g., select the topmost node if orientation is "
    "topToBottom)</i>";

class OGDFTree : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Improved Walker (OGDF)", "Christoph Buchheim", "12/11/2007",
                    "Implements a linear-time tree layout algorithm with straight-line or "
                    "orthogonal edge routing.",
                    "1.5", "Tree")
  OGDFTree(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, context ? new ogdf::TreeLayout() : nullptr) {
    addInParameter<double>("siblings distance", paramHelp[0], "20");
    addInParameter<double>("subtrees distance", paramHelp[1], "20");
    addInParameter<double>("levels distance", paramHelp[2], "50");
    addInParameter<double>("trees distance", paramHelp[3], "50");
    addInParameter<bool>("orthogonal layout", paramHelp[4], "false");
    addInParameter<StringCollection>(ELT_ORIENTATION, paramHelp[5], ELT_ORIENTATIONLIST, true,
                                     orientationValuesDescription);
    addInParameter<StringCollection>(ELT_ROOTSELECTION, paramHelp[6], ELT_ROOTSELECTIONLIST, true,
                                     rootSelectionValuesDescription);
  }

  ~OGDFTree() override {}

  bool check(string &error) override {
    if (!tlp::TreeTest::isTree(graph)) {
      error += "graph is not a directed tree";
      return false;
    }

    return true;
  }

  void beforeCall() override {
    ogdf::TreeLayout *tree = static_cast<ogdf::TreeLayout *>(ogdfLayoutAlgo);

    if (dataSet != nullptr) {
      double dval = 0;
      bool bval = false;
      StringCollection sc;

      if (dataSet->get("siblings distance", dval))
        tree->siblingDistance(dval);

      if (dataSet->get("subtrees distance", dval))
        tree->subtreeDistance(dval);

      if (dataSet->get("levels distance", dval))
        tree->levelDistance(dval);

      if (dataSet->get("trees distance", dval))
        tree->treeDistance(dval);

      if (dataSet->get("orthogonal layout", bval))
        tree->orthogonalLayout(bval);

      if (dataSet->get(ELT_ORIENTATION, sc)) {
        if (sc.getCurrent() == ELT_TOPTOBOTTOM) {
          // because of an ununderstanding fix
          // in thirdparty/OGDF/src/ogdf/tree/TreeLayout.cpp
          tree->orientation(Orientation::bottomToTop);
        } else if (sc.getCurrent() == ELT_BOTTOMTOTOP) {
          // same as above
          tree->orientation(Orientation::topToBottom);
        } else if (sc.getCurrent() == ELT_LEFTTORIGHT) {
          tree->orientation(Orientation::leftToRight);
        } else {
          tree->orientation(Orientation::rightToLeft);
        }
      }

      if (dataSet->get(ELT_ROOTSELECTION, sc)) {
        if (sc.getCurrent() == ELT_ROOTSOURCE) {
          tree->rootSelection(TreeLayout::RootSelectionType::Source);
        } else if (sc.getCurrent() == ELT_ROOTSINK) {
          tree->rootSelection(TreeLayout::RootSelectionType::Sink);
        } else {
          tree->rootSelection(TreeLayout::RootSelectionType::ByCoord);
        }
      }
    }
  }
};

PLUGIN(OGDFTree)
