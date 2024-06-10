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

#include <tulip2ogdf/OGDFLayoutPluginBase.h>

#define RANKINGLIST "longest path;optimal;coffman graham"
#define LONGESTPATHRANKING 0
#define OPTIMALRANKING 1
#define COFFMANGRAHAMRANKING 2

#define TWOLAYERCROSSLIST                                                                          \
  "barycenter;median;split;sifting;greedy insert;"                                                 \
  "greedy switch;global sifting;grid sifting"
#define BARYCENTER 0
#define MEDIAN 1
#define SPLIT 2
#define SIFTING 3
#define GREEDYINSERT 4
#define GREEDYSWITCH 5
#define GLOBALSIFTING 6
#define GRIDSIFTING 7

#define HIERARCHYLAYOUTLIST "fast;fast simple;optimal"
#define FASTHIERARCHY 0
#define FASTSIMPLEHIERARCHY 1
#define OPTIMALHIERARCHY 2

using namespace tlp;
using namespace ogdf;

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
    "fast hierarchy layout).",

    // transpose
    "If this option is set to true an additional fine tuning step is performed after each "
    "traversal, which tries to reduce the total number of crossings by switching adjacent vertices "
    "on the same layer.",

    // connected components packing
    "If set to true connected components are laid out separately and the resulting layouts are "
    "arranged afterwards using the packer module.",

    // connected components spacing
    "Specifies the spacing between connected components of the graph.",

    // page ratio
    "The page ratio used for packing connected components.",

    // align base classes
    "Determines if base classes of inheritance hierarchies shall be aligned.",

    // align siblings
    "Sets the option align siblings.",

    // ranking
    "Sets the option for the node ranking (layer assignment).",

    // two-layer crossing minimization
    "Sets the module option for the two-layer crossing minimization.",

    // hierarchy layout
    "The hierarchy layout module that computes the final layout.",

    // transpose vertically
    "Transpose the layout vertically from top to bottom.",

    // number of crossings
    "Returns the number of crossings in the computed layout.",

    // number of layers/levels
    "Returns the number of layers/levels in the computed layout."

};

static const char *eltRankingValuesDescription =
    "coffman graham <i>(The coffman graham ranking algorithm)</i><br>"
    "longest-path <i>(the well-known longest-path ranking algorithm)</i><br>"
    "optimal <i>(the LP-based algorithm for computing a node ranking with minimal edge "
    "lengths)</i>";

static const char *twoLayerCrossValuesDescription =
    "barycenter <i>(the barycenter heuristic for 2-layer crossing minimization)</i><br>"
    "greedy-insert <i>(The greedy-insert heuristic for 2-layer crossing "
    "minimization)</i><br>"
    "greedy-switch <i>(The greedy-switch heuristic for 2-layer crossing "
    "minimization)</i><br>"
    "median <i>(the median heuristic for 2-layer crossing minimization)</i><br>"
    "sifting <i>(The sifting heuristic for 2-layer crossing minimization)</i><br>"
    "split <i>(the split heuristic for 2-layer crossing minimization)</i><br>"
    "grid sifting <i>(the grid sifting heuristic for 2-layer crossing minimization)</i><br>"
    "global sifting <i>(the global sifting heuristic for 2-layer crossing "
    "minimization)</i>";

static const char *hierarchyLayoutValuesDescription =
    "fast <i>(Coordinate assignment phase for the Sugiyama algorithm by Buchheim et "
    "al.)</i><br>"
    "fast simple <i>(Coordinate assignment phase for the Sugiyama algorithm by Ulrik "
    "Brandes and Boris Koepf)</i><br>"
    "optimal <i>(The LP-based hierarchy layout algorithm)</i>";

class OGDFSugiyama : public OGDFLayoutPluginBase {
  ogdf::SugiyamaLayout *sugiyama;

public:
  OGDFSugiyama(const tlp::PluginContext *context)
      : OGDFLayoutPluginBase(context, context ? new ogdf::SugiyamaLayout() : nullptr),
        sugiyama(static_cast<ogdf::SugiyamaLayout *>(ogdfLayoutAlgo)) {
    addInParameter<int>("fails", paramHelp[0], "4");
    addInParameter<int>("runs", paramHelp[1], "15");
    addInParameter<double>("node distance", paramHelp[2], "3");
    addInParameter<double>("layer distance", paramHelp[3], "3");
    addInParameter<bool>("fixed layer distance", paramHelp[4], "false");
    addInParameter<bool>("transpose", paramHelp[5], "true");
    addInParameter<bool>("connected components packing", paramHelp[6], "true");
    addInParameter<double>("connected components spacing", paramHelp[7], "20");
    addInParameter<double>("page ratio", paramHelp[8], "1.0");
    addInParameter<bool>("align base classes", paramHelp[9], "false");
    addInParameter<bool>("align siblings", paramHelp[10], "false");
    addInParameter<StringCollection>("ranking", paramHelp[11], RANKINGLIST, true,
                                     eltRankingValuesDescription);
    addInParameter<StringCollection>("two-layer crossing minimization", paramHelp[12],
                                     TWOLAYERCROSSLIST, true, twoLayerCrossValuesDescription);
    addInParameter<StringCollection>("hierarchy layout", paramHelp[13], HIERARCHYLAYOUTLIST, true,
                                     hierarchyLayoutValuesDescription);
    addInParameter<bool>("transpose vertically", paramHelp[14], "true");
    addOutParameter<int>("number of crossings", paramHelp[15]);
    addOutParameter<int>("number of levels/layers", paramHelp[16]);
  }

  ~OGDFSugiyama() override {}

  PLUGININFORMATION("Sugiyama (OGDF)", "Carsten Gutwenger", "12/11/2007",
                    "Implements the classical layout algorithm by Sugiyama, Tagawa, and Toda. It "
                    "is a layer-based approach for producing upward drawings.",
                    "1.8", "Hierarchical")

  void beforeCall() override {
    if (dataSet != nullptr) {
      int ival = 0;
      double dval = 0;
      bool bval = false;
      StringCollection sc;

      if (dataSet->get("fails", ival))
        sugiyama->fails(ival);

      if (dataSet->get("runs", ival))
        sugiyama->runs(ival);

      if (dataSet->get("connected components packing", bval))
        sugiyama->arrangeCCs(bval);

      if (dataSet->get("connected components spacing", dval))
        sugiyama->minDistCC(dval);

      if (dataSet->get("page ratio", dval))
        sugiyama->pageRatio(dval);

      if (dataSet->get("align base classes", bval))
        sugiyama->alignBaseClasses(bval);

      if (dataSet->get("align siblings", bval))
        sugiyama->alignSiblings(bval);

      if (dataSet->get("transpose", bval))
        sugiyama->transpose(bval);

      if (dataSet->get("ranking", sc)) {
        switch (sc.getCurrent()) {
        case LONGESTPATHRANKING:
          sugiyama->setRanking(new ogdf::LongestPathRanking());
          break;
        case OPTIMALRANKING:
          sugiyama->setRanking(new ogdf::OptimalRanking());
          break;
        default:
          sugiyama->setRanking(new ogdf::CoffmanGrahamRanking());
        }
      }

      if (dataSet->get("two-layer crossing minimization", sc)) {
        switch (sc.getCurrent()) {
        case BARYCENTER:
          sugiyama->setCrossMin(new ogdf::BarycenterHeuristic());
          break;
        case MEDIAN:
          sugiyama->setCrossMin(new ogdf::MedianHeuristic());
          break;
        case SPLIT:
          sugiyama->setCrossMin(new ogdf::SplitHeuristic());
          break;
        case SIFTING:
          sugiyama->setCrossMin(new ogdf::SiftingHeuristic());
          break;
        case GREEDYINSERT:
          sugiyama->setCrossMin(new ogdf::GreedyInsertHeuristic());
          break;
        case GREEDYSWITCH:
          sugiyama->setCrossMin(new ogdf::GreedySwitchHeuristic());
          break;
        case GLOBALSIFTING:
          sugiyama->setCrossMin(new ogdf::GlobalSifting());
          break;
        default:
          sugiyama->setCrossMin(new ogdf::GridSifting());
        }
      }

      if (dataSet->get("hierarchy layout", sc)) {
        double nodeDistance = 3;
        double layerDistance = 3;
        bool fixedLayerDistance = true;
        dataSet->get("node distance", nodeDistance);
        dataSet->get("layer distance", layerDistance);
        dataSet->get("fixed layer distance", fixedLayerDistance);

        switch (sc.getCurrent()) {
        case FASTHIERARCHY: {
          ogdf::FastHierarchyLayout *fhl = new FastHierarchyLayout();
          fhl->nodeDistance(nodeDistance);
          fhl->layerDistance(layerDistance);
          fhl->fixedLayerDistance(fixedLayerDistance);
          sugiyama->setLayout(fhl);
          break;
        }
        case FASTSIMPLEHIERARCHY: {
          ogdf::FastSimpleHierarchyLayout *fshl = new ogdf::FastSimpleHierarchyLayout();
          fshl->nodeDistance(nodeDistance);
          fshl->layerDistance(layerDistance);
          sugiyama->setLayout(fshl);
          break;
        }
        default:
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

      dataSet->setDeprecated("number of crossings", "Number of crossings",
                             sugiyama->numberOfCrossings());
      dataSet->setDeprecated("number of levels/layers", "Number of levels/layers",
                             sugiyama->numberOfLevels());
    }
  }
};

PLUGIN(OGDFSugiyama)
