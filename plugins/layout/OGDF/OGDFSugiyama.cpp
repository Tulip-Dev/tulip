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


#include "tulip2ogdf/OGDFLayoutPluginBase.h"

namespace {

const char * paramHelp[] = { HTML_HELP_OPEN()
                             HTML_HELP_DEF( "type", "int" )
                             HTML_HELP_BODY()
                             "Sets the option fails to nFails."
                             HTML_HELP_CLOSE(), HTML_HELP_OPEN()
                             HTML_HELP_DEF( "type", "int" )
                             HTML_HELP_BODY()
                             "Sets the option runs to nRuns."
                             HTML_HELP_CLOSE(),
                             HTML_HELP_OPEN()
                             HTML_HELP_DEF( "type", "double" )
                             HTML_HELP_BODY()
                             "The minimal distance between two nodes on the same layer."
                             HTML_HELP_CLOSE(),
                             HTML_HELP_OPEN()
                             HTML_HELP_DEF( "type", "double" )
                             HTML_HELP_BODY()
                             "The minimal distance between two layers."
                             HTML_HELP_CLOSE(),
                             HTML_HELP_OPEN()
                             HTML_HELP_DEF( "type", "bool" )
                             HTML_HELP_BODY()
                             "If false, adjust the distance of each layer to the longest edge (only for FastHierarchyLayout)."
                             HTML_HELP_CLOSE(),
                             HTML_HELP_OPEN()
                             HTML_HELP_DEF( "type", "bool" )
                             HTML_HELP_BODY()
                             "Sets the option for transposing layout vertically ."
                             HTML_HELP_CLOSE(),
                             HTML_HELP_OPEN()
                             HTML_HELP_DEF( "type", "bool" )
                             HTML_HELP_BODY()
                             "Sets the options arrangeCCs."
                             HTML_HELP_CLOSE(),
                             HTML_HELP_OPEN()
                             HTML_HELP_DEF( "type", "double" )
                             HTML_HELP_BODY()
                             "The minimal distance between connected components."
                             HTML_HELP_CLOSE(),
                             HTML_HELP_OPEN()
                             HTML_HELP_DEF( "type", "double" )
                             HTML_HELP_BODY()
                             "The page ratio used for packing connected components."
                             HTML_HELP_CLOSE(),
                             HTML_HELP_OPEN()
                             HTML_HELP_DEF( "type", "bool" )
                             HTML_HELP_BODY()
                             "Sets the option alignBaseClasses."
                             HTML_HELP_CLOSE(),
                             HTML_HELP_OPEN()
                             HTML_HELP_DEF( "type", "bool" )
                             HTML_HELP_BODY()
                             "Sets the option alignSiblings."
                             HTML_HELP_CLOSE(),
                             HTML_HELP_OPEN()
                             HTML_HELP_DEF( "type", "StringCollection")
                             HTML_HELP_DEF("values", "<FONT COLOR=\"red\"> CoffmanGrahamRanking: <FONT COLOR=\"black\"> The coffman graham ranking algorithm. <BR> <FONT COLOR=\"red\"> LongestPathRanking: <FONT COLOR=\"black\"> the well-known longest-path ranking algorithm. <BR> <FONT COLOR=\"red\"> OptimalRanking: <FONT COLOR=\"black\"> the LP-based algorithm for computing a node ranking with minimal edge lengths.")
                             HTML_HELP_DEF( "default", "LongestPathRanking " )
                             HTML_HELP_BODY()
                             "Sets the option for the node ranking (layer assignment)."
                             HTML_HELP_CLOSE(),
                             HTML_HELP_OPEN()
                             HTML_HELP_DEF( "type", "StringCollection")
                             HTML_HELP_DEF("values", "<FONT COLOR=\"red\"> BarycenterHeuristic: <FONT COLOR=\"black\"> the barycenter heuristic for 2-layer crossing minimization. <BR><FONT COLOR=\"red\"> GreedyInsertHeuristic: <FONT COLOR=\"black\"> The greedy-insert heuristic for 2-layer crossing minimization. <BR><FONT COLOR=\"red\"> GreedySwitchHeuristic: <FONT COLOR=\"black\"> The greedy-switch heuristic for 2-layer crossing minimization. <BR> <FONT COLOR=\"red\"> MedianHeuristic: <FONT COLOR=\"black\"> the median heuristic for 2-layer crossing minimization. <BR><FONT COLOR=\"red\"> SiftingHeuristic: <FONT COLOR=\"black\"> The sifting heuristic for 2-layer crossing minimization. <BR> <FONT COLOR=\"red\"> SplitHeuristic: <FONT COLOR=\"black\"> the split heuristic for 2-layer crossing minimization.")
                             HTML_HELP_DEF( "default", "BarycenterHeuristic " )
                             HTML_HELP_BODY()
                             "Sets the module option for the two-layer crossing minimization."
                             HTML_HELP_CLOSE(),
                             HTML_HELP_OPEN()
                             HTML_HELP_DEF( "type", "StringCollection")
                             HTML_HELP_DEF("values", "<FONT COLOR=\"red\"> FastHierarchyLayout: <FONT COLOR=\"black\"> Coordinate assignment phase for the Sugiyama algorithm by Buchheim et al.. <BR> <FONT COLOR=\"red\"> FastSimpleHierarchyLayout: <FONT COLOR=\"black\"> Coordinate assignment phase for the Sugiyama algorithm by Ulrik Brandes and Boris Koepf.")
                             HTML_HELP_DEF( "default", "FastHierarchyLayout" )
                             HTML_HELP_BODY()
                             "The hierarchy layout module that computes the final layout."
                             HTML_HELP_CLOSE()
                           };
}


#define ELT_RANKING "Ranking"
#define ELT_RANKINGLIST "LongestPathRanking;OptimalRanking;CoffmanGrahamRanking"
#define ELT_LONGESTPATHRANKING 0
#define ELT_OPTIMALRANKING 1
#define ELT_COFFMANGRAHAMRANKING 2

#define ELT_TWOLAYERCROSS "Two-layer crossing minimization"
#define ELT_TWOLAYERCROSSLIST "BarycenterHeuristic;MedianHeuristic;SplitHeuristic;SiftingHeuristic;GreedyInsertHeuristic;GreedySwitchHeuristic"
#define ELT_BARYCENTER 0
#define ELT_MEDIAN 1
#define ELT_SPLIT 2
#define ELT_SIFTING 3
#define ELT_GREEDYINSERT 4
#define ELT_GREEDYSWITCH 5

#define ELT_HIERARCHYLAYOUT "Layout"
#define ELT_HIERARCHYLAYOUTLIST "FastHierarchyLayout;FastSimpleHierarchyLayout"
#define ELT_FASTHIERARCHY 0
#define ELT_FASTSIMPLEHIERARCHY 1

// comments below have been extracted from OGDF/src/layered/sugiyama.cpp
/** \addtogroup layout */
/*@{*/
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
class OGDFSugiyama : public OGDFLayoutPluginBase {

public:

  OGDFSugiyama(const tlp::PropertyContext &context) :OGDFLayoutPluginBase(context, new ogdf::SugiyamaLayout()) {
    addInParameter<int>("fails", paramHelp[0], "4");
    addInParameter<int>("runs", paramHelp[1], "15");
    addInParameter<double>("node distance", paramHelp[2], "3");
    addInParameter<double>("layer distance", paramHelp[3], "3");
    addInParameter<bool>("fixed layer distance", paramHelp[4], "true");
    addInParameter<bool>("transpose", paramHelp[5], "true");
    addInParameter<bool>("arrangeCCs", paramHelp[6], "true");
    addInParameter<double>("minDistCC", paramHelp[7], "20");
    addInParameter<double>("pageRatio", paramHelp[8], "1.0");
    addInParameter<bool>("alignBaseClasses", paramHelp[9], "false");
    addInParameter<bool>("alignSiblings", paramHelp[10], "false");
    addInParameter<StringCollection>(ELT_RANKING, paramHelp[11], ELT_RANKINGLIST);
    addInParameter<StringCollection>(ELT_TWOLAYERCROSS, paramHelp[12], ELT_TWOLAYERCROSSLIST);
    addInParameter<StringCollection>(ELT_HIERARCHYLAYOUT, paramHelp[13], ELT_HIERARCHYLAYOUTLIST);
  }

  void beforeCall() {
    ogdf::SugiyamaLayout *sugiyama = static_cast<ogdf::SugiyamaLayout*>(ogdfLayoutAlgo);

    if (dataSet != NULL) {
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

      if (dataSet->get(ELT_RANKING, sc)) {
        if (sc.getCurrent() == ELT_LONGESTPATHRANKING) {
          sugiyama->setRanking(new ogdf::LongestPathRanking());
        }
        else if(sc.getCurrent() == ELT_OPTIMALRANKING) {
          sugiyama->setRanking(new ogdf::OptimalRanking());
        }
        else
          sugiyama->setRanking(new ogdf::CoffmanGrahamRanking());
      }

      if (dataSet->get(ELT_TWOLAYERCROSS, sc)) {
        if (sc.getCurrent() == ELT_BARYCENTER) {
          sugiyama->setCrossMin(new ogdf::BarycenterHeuristic());
        }
        else if (sc.getCurrent() == ELT_MEDIAN) {
          sugiyama->setCrossMin(new ogdf::MedianHeuristic());
        }
        else if(sc.getCurrent()==ELT_SPLIT) {
          sugiyama->setCrossMin(new ogdf::SplitHeuristic());
        }
        else if(sc.getCurrent()==ELT_SIFTING) {
          sugiyama->setCrossMin(new ogdf::SiftingHeuristic());
        }
        else if(sc.getCurrent()==ELT_GREEDYINSERT) {
          sugiyama->setCrossMin(new ogdf::GreedyInsertHeuristic());
        }
        else
          sugiyama->setCrossMin(new ogdf::GreedySwitchHeuristic());
      }

      if(dataSet->get(ELT_HIERARCHYLAYOUT, sc)) {
        double nodeDistance = 3;
        double layerDistance = 3;
        bool fixedLayerDistance = true;
        dataSet->get("node distance", nodeDistance);
        dataSet->get("layer distance", layerDistance);
        dataSet->get("fixed layer distance", fixedLayerDistance);

        if(sc.getCurrent()==ELT_FASTHIERARCHY) {
          ogdf::FastHierarchyLayout *fhl = new FastHierarchyLayout();
          fhl->nodeDistance(nodeDistance);
          fhl->layerDistance(layerDistance);
          fhl->fixedLayerDistance(fixedLayerDistance);
          sugiyama->setLayout(fhl);
        }
        else {
          sugiyama->setLayout(new ogdf::FastSimpleHierarchyLayout(static_cast<int>(nodeDistance), static_cast<int>(layerDistance)));
        }
      }
    }
  }

  void afterCall() {
    if (dataSet != NULL) {
      bool bval = false;

      if (dataSet->get("transpose", bval)) {
        if (bval) {
          transposeLayoutVertically();
        }
      }
    }
  }

};
/*@}*/

LAYOUTPLUGINOFGROUP(OGDFSugiyama,"Sugiyama (OGDF)","Carsten Gutwenger","12/11/2007","Ok","1.5","Hierarchical")
