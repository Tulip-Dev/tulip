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

#include <ogdf/layered/SugiyamaLayout.h>
#include <ogdf/layered/LongestPathRanking.h>
#include <ogdf/layered/OptimalRanking.h>
#include <ogdf/layered/BarycenterHeuristic.h>
#include <ogdf/layered/MedianHeuristic.h>
#include <ogdf/layered/SplitHeuristic.h>
#include <ogdf/layered/FastHierarchyLayout.h>
#include <ogdf/layered/CoffmanGrahamRanking.h>
#include <ogdf/layered/SiftingHeuristic.h>
#include <ogdf/layered/GreedyInsertHeuristic.h>
#include <ogdf/layered/GreedySwitchHeuristic.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>
#include <ogdf/layered/FastSimpleHierarchyLayout.h>
#include <ogdf/layered/GridSifting.h>
#include <ogdf/layered/OptimalHierarchyLayout.h>

#include <tulip/StringCollection.h>

#include "tulip2ogdf/OGDFLayoutPluginBase.h"

// comments below have been extracted from OGDF/src/layered/sugiyama.cpp
/** \addtogroup layout */

/**
 * Implementation of Sugiyama algorithm (classes Hierarchy,
 * Level, SugiyamaLayout)
 *
 * \author Carsten Gutwenger
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

#define ELT_RANKING "Ranking"
#define ELT_RANKINGLIST "LongestPathRanking;OptimalRanking;CoffmanGrahamRanking"
#define ELT_LONGESTPATHRANKING 0
#define ELT_OPTIMALRANKING 1
#define ELT_COFFMANGRAHAMRANKING 2

#define ELT_TWOLAYERCROSS "Two-layer crossing minimization"
#define ELT_TWOLAYERCROSSLIST                                                                      \
  "BarycenterHeuristic;MedianHeuristic;SplitHeuristic;SiftingHeuristic;GreedyInsertHeuristic;"     \
  "GreedySwitchHeuristic;GlobalSiftingHeuristic;GridSiftingHeuristic"
#define ELT_BARYCENTER 0
#define ELT_MEDIAN 1
#define ELT_SPLIT 2
#define ELT_SIFTING 3
#define ELT_GREEDYINSERT 4
#define ELT_GREEDYSWITCH 5
#define ELT_GLOBALSIFTING 6
#define ELT_GRIDSIFTING 7

#define ELT_HIERARCHYLAYOUT "Layout"
#define ELT_HIERARCHYLAYOUTLIST                                                                    \
  "FastHierarchyLayout;FastSimpleHierarchyLayout;OptimalHierarchyLayout"
#define ELT_FASTHIERARCHY 0
#define ELT_FASTSIMPLEHIERARCHY 1
#define ELT_OPTIMALHIERARCHY 2

static const char *paramHelp[] = {
    // fails
    "The number of times that the number of crossings may not decrease after a complete top-down "
    "bottom-up traversal, before a run is terminated.",

    // runs
    "Determines, how many times the crossing minimization is repeated. Each repetition (except for "
    "the first) starts with randomly permuted nodes on each layer. Deterministic behaviour can be "
    "achieved by setting runs to 1.",

    // node distance
    "The minimal horizontal distance between two nodes on the same layer.",

    // layer distance
    "The minimal vertical distance between two nodes on neighboring layers.",

    // fixed layer distance
    "If true, the distance between neighboring layers is fixed, otherwise variable (only for "
    "FastHierarchyLayout).",

    // transpose
    "If this option is set to true an additional fine tuning step is performed after each "
    "traversal, which tries to reduce the total number of crossings by switching adjacent vertices "
    "on the same layer.",

    // arrangeCCs
    "If set to true connected components are laid out separately and the resulting layouts are "
    "arranged afterwards using the packer module.",

    // minDistCC
    "Specifies the spacing between connected components of the graph.",

    // pageRatio
    "The page ratio used for packing connected components.",

    // alignBaseClasses
    "Determines if base classes of inheritance hierarchies shall be aligned.",

    // alignSiblings
    "Sets the option alignSiblings.",

    // Ranking
    "Sets the option for the node ranking (layer assignment).",

    // Two-layer crossing minimization
    "Sets the module option for the two-layer crossing minimization.",

    // Layout
    "The hierarchy layout module that computes the final layout.",

    // transpose vertically
    "Transpose the layout vertically from top to bottom."};

static const char *eltRankingValuesDescription =
    "CoffmanGrahamRanking <i>(The coffman graham ranking algorithm)</i><br>"
    "LongestPathRanking <i>(the well-known longest-path ranking algorithm)</i><br>"
    "OptimalRanking <i>(the LP-based algorithm for computing a node ranking with minimal edge "
    "lengths)</i>";

static const char *twoLayerCrossValuesDescription =
    "BarycenterHeuristic <i>(the barycenter heuristic for 2-layer crossing minimization)</i><br>"
    "GreedyInsertHeuristic <i>(The greedy-insert heuristic for 2-layer crossing "
    "minimization)</i><br>"
    "GreedySwitchHeuristic <i>(The greedy-switch heuristic for 2-layer crossing "
    "minimization)</i><br>"
    "MedianHeuristic <i>(the median heuristic for 2-layer crossing minimization)</i><br>"
    "SiftingHeuristic <i>(The sifting heuristic for 2-layer crossing minimization)</i><br>"
    "SplitHeuristic <i>(the split heuristic for 2-layer crossing minimization)</i><br>"
    "GridSiftingHeuristic <i>(the grid sifting heuristic for 2-layer crossing minimization)</i><br>"
    "GlobalSiftingHeuristic <i>(the global sifting heuristic for 2-layer crossing "
    "minimization)</i>";

static const char *hierarchyLayoutValuesDescription =
    "FastHierarchyLayout <i>(Coordinate assignment phase for the Sugiyama algorithm by Buchheim et "
    "al.)</i><br>"
    "FastSimpleHierarchyLayout <i>(Coordinate assignment phase for the Sugiyama algorithm by Ulrik "
    "Brandes and Boris Koepf)</i><br>"
    "OptimalHierarchyLayout <i>(The LP-based hierarchy layout algorithm)</i>";

class OGDFSugiyama : public OGDFLayoutPluginBase {

public:
  OGDFSugiyama(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, new ogdf::SugiyamaLayout()) {
    addInParameter<int>("fails", paramHelp[0], "4");
    addInParameter<int>("runs", paramHelp[1], "15");
    addInParameter<double>("node distance", paramHelp[2], "3");
    addInParameter<double>("layer distance", paramHelp[3], "3");
    addInParameter<bool>("fixed layer distance", paramHelp[4], "false");
    addInParameter<bool>("transpose", paramHelp[5], "true");
    addInParameter<bool>("arrangeCCs", paramHelp[6], "true");
    addInParameter<double>("minDistCC", paramHelp[7], "20");
    addInParameter<double>("pageRatio", paramHelp[8], "1.0");
    addInParameter<bool>("alignBaseClasses", paramHelp[9], "false");
    addInParameter<bool>("alignSiblings", paramHelp[10], "false");
    addInParameter<StringCollection>(ELT_RANKING, paramHelp[11], ELT_RANKINGLIST, true,
                                     eltRankingValuesDescription);
    addInParameter<StringCollection>(ELT_TWOLAYERCROSS, paramHelp[12], ELT_TWOLAYERCROSSLIST, true,
                                     twoLayerCrossValuesDescription);
    addInParameter<StringCollection>(ELT_HIERARCHYLAYOUT, paramHelp[13], ELT_HIERARCHYLAYOUTLIST,
                                     true, hierarchyLayoutValuesDescription);
    addInParameter<bool>("transpose vertically", paramHelp[14], "true");
  }

  ~OGDFSugiyama() override {}

  PLUGININFORMATION("Sugiyama (OGDF)", "Carsten Gutwenger", "12/11/2007",
                    "Implements the classical layout algorithm by Sugiyama, Tagawa, and Toda. It "
                    "is a layer-based approach for producing upward drawings.",
                    "1.7", "Hierarchical")

  void beforeCall() override {
    ogdf::SugiyamaLayout *sugiyama = static_cast<ogdf::SugiyamaLayout *>(ogdfLayoutAlgo);

    if (dataSet != nullptr) {
      int ival = 0;
      double dval = 0;
      bool bval = false;
      StringCollection sc;

      if (dataSet->get("fails", ival))
        sugiyama->fails(ival);

      if (dataSet->get("runs", ival))
        sugiyama->runs(ival);

      if (dataSet->get("arrangeCCS", bval))
        sugiyama->arrangeCCs(bval);

      if (dataSet->get("minDistCC", dval))
        sugiyama->minDistCC(dval);

      if (dataSet->get("pageRatio", dval))
        sugiyama->pageRatio(dval);

      if (dataSet->get("alignBaseClasses", bval))
        sugiyama->alignBaseClasses(bval);

      if (dataSet->get("alignSiblings", bval))
        sugiyama->alignSiblings(bval);

      if (dataSet->get("transpose", bval))
        sugiyama->transpose(bval);

      if (dataSet->get(ELT_RANKING, sc)) {
        if (sc.getCurrent() == ELT_LONGESTPATHRANKING) {
          sugiyama->setRanking(new ogdf::LongestPathRanking());
        } else if (sc.getCurrent() == ELT_OPTIMALRANKING) {
          sugiyama->setRanking(new ogdf::OptimalRanking());
        } else
          sugiyama->setRanking(new ogdf::CoffmanGrahamRanking());
      }

      if (dataSet->get(ELT_TWOLAYERCROSS, sc)) {
        if (sc.getCurrent() == ELT_BARYCENTER) {
          sugiyama->setCrossMin(new ogdf::BarycenterHeuristic());
        } else if (sc.getCurrent() == ELT_MEDIAN) {
          sugiyama->setCrossMin(new ogdf::MedianHeuristic());
        } else if (sc.getCurrent() == ELT_SPLIT) {
          sugiyama->setCrossMin(new ogdf::SplitHeuristic());
        } else if (sc.getCurrent() == ELT_SIFTING) {
          sugiyama->setCrossMin(new ogdf::SiftingHeuristic());
        } else if (sc.getCurrent() == ELT_GREEDYINSERT) {
          sugiyama->setCrossMin(new ogdf::GreedyInsertHeuristic());
        } else if (sc.getCurrent() == ELT_GREEDYSWITCH) {
          sugiyama->setCrossMin(new ogdf::GreedySwitchHeuristic());
        } else if (sc.getCurrent() == ELT_GLOBALSIFTING) {
          sugiyama->setCrossMin(new ogdf::GlobalSifting());
        } else {
          sugiyama->setCrossMin(new ogdf::GridSifting());
        }
      }

      if (dataSet->get(ELT_HIERARCHYLAYOUT, sc)) {
        double nodeDistance = 3;
        double layerDistance = 3;
        bool fixedLayerDistance = true;
        dataSet->get("node distance", nodeDistance);
        dataSet->get("layer distance", layerDistance);
        dataSet->get("fixed layer distance", fixedLayerDistance);

        if (sc.getCurrent() == ELT_FASTHIERARCHY) {
          ogdf::FastHierarchyLayout *fhl = new FastHierarchyLayout();
          fhl->nodeDistance(nodeDistance);
          fhl->layerDistance(layerDistance);
          fhl->fixedLayerDistance(fixedLayerDistance);
          sugiyama->setLayout(fhl);
        } else if (sc.getCurrent() == ELT_FASTSIMPLEHIERARCHY) {
          ogdf::FastSimpleHierarchyLayout *fshl = new ogdf::FastSimpleHierarchyLayout();
          fshl->nodeDistance(nodeDistance);
          fshl->layerDistance(layerDistance);
          sugiyama->setLayout(fshl);
        } else {
          ogdf::OptimalHierarchyLayout *ohl = new ogdf::OptimalHierarchyLayout();
          ohl->nodeDistance(nodeDistance);
          ohl->layerDistance(layerDistance);
          sugiyama->setLayout(ohl);
        }
      }
    }
  }

  void callOGDFLayoutAlgorithm(ogdf::GraphAttributes &gAttributes) override {
    ogdf::SugiyamaLayout *sugiyama = static_cast<ogdf::SugiyamaLayout *>(ogdfLayoutAlgo);

    if (sugiyama->alignBaseClasses() || sugiyama->alignSiblings())
      sugiyama->callUML(gAttributes);
    else
      ogdfLayoutAlgo->call(gAttributes);
  }

  void afterCall() override {
    if (dataSet != nullptr) {
      bool bval = false;

      if (dataSet->get("transpose vertically", bval)) {
        if (bval) {
          transposeLayoutVertically();
        }
      }
    }
  }
};

PLUGIN(OGDFSugiyama)
