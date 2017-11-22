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

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

#include <tulip/StringCollection.h>

// comments below have been extracted from OGDF/src/tree/TreeLayout.cpp
/** \addtogroup layout */

/**
 * Linear time layout algorithm for trees (TreeLayout)
 * based on Walker's algorithm
 *
 * \author Christoph Buchheim
 *
 * \par License:
 * This is part of the Open Graph Drawing Framework (OGDF).
 *
 * Copyright (C). All rights reserved.
 * See README.txt in the root directory of the OGDF installation for details.
 *
 * \par
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * Version 2 or 3 as published by the Free Software Foundation
 * and appearing in the files LICENSE_GPL_v2.txt and
 * LICENSE_GPL_v3.txt included in the packaging of this file.
 *
 * \par
 * In addition, as a special exception, you have permission to link
 * this software with the libraries of the COIN-OR Osi project
 * (http://www.coin-or.org/projects/Osi.xml), all libraries required
 * by Osi, and all LP-solver libraries directly supported by the
 * COIN-OR Osi project, and distribute executables, as long as
 * you follow the requirements of the GNU General Public License
 * in regard to all of the software in the executable aside from these
 * third-party libraries.
 *
 * \par
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * \par
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 * \see  http://www.gnu.org/copyleft/gpl.html
 ***************************************************************/

#define ELT_ORIENTATION "Orientation"
#define ELT_ORIENTATIONLIST "topToBottom;bottomToTop;leftToRight;rightToLeft"
#define ELT_TOPTOBOTTOM 0
#define ELT_BOTTOMTOTOP 1
#define ELT_LEFTTORIGHT 2
#define ELT_RIGHTTOLEFT 3

#define ELT_ROOTSELECTION "Root selection"
#define ELT_ROOTSELECTIONLIST "rootIsSource;rootIsSink;rootByCoord"
#define ELT_ROOTSOURCE 0
#define ELT_ROOTSINK 1
#define ELT_ROOTCOORD 2

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
    "rootIsSource <i>(Select a source in the graph)</i><br>"
    "rootIsSink <i>(Select a sink in the graph)</i><br>"
    "rootByCoord <i>(Use the coordinates, e.g., select the topmost node if orientation is "
    "topToBottom)</i>";

class OGDFTree : public OGDFLayoutPluginBase {

public:
  PLUGININFORMATION("Improved Walker (OGDF)", "Christoph Buchheim", "12/11/2007",
                    "Implements a linear-time tree layout algorithm with straight-line or "
                    "orthogonal edge routing.",
                    "1.5", "Tree")
  OGDFTree(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ogdf::TreeLayout()) {
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

  ~OGDFTree() {}

  void beforeCall() {
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
          tree->orientation(ogdf::bottomToTop);
        } else if (sc.getCurrent() == ELT_BOTTOMTOTOP) {
          // same as above
          tree->orientation(ogdf::topToBottom);
        } else if (sc.getCurrent() == ELT_LEFTTORIGHT) {
          tree->orientation(ogdf::leftToRight);
        } else {
          tree->orientation(ogdf::rightToLeft);
        }
      }

      if (dataSet->get(ELT_ROOTSELECTION, sc)) {
        if (sc.getCurrent() == ELT_ROOTSOURCE) {
          tree->rootSelection(ogdf::TreeLayout::rootIsSource);
        } else if (sc.getCurrent() == ELT_ROOTSINK) {
          tree->rootSelection(ogdf::TreeLayout::rootIsSink);
        } else {
          tree->rootSelection(ogdf::TreeLayout::rootByCoord);
        }
      }
    }
  }
};

PLUGIN(OGDFTree)
