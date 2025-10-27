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
#ifndef _STRAHLERGRAPH2METRIC_H
#define _STRAHLERGRAPH2METRIC_H

#include <unordered_map>
#include <tulip/TulipPluginHeaders.h>
struct Strahler {
  Strahler(int stra = 1, int sta = 0, int used = 0)
      : strahler(stra), stacks(sta), usedStack(used) {}
  int strahler;
  int stacks;
  int usedStack;
};

/** \addtogroup metric */

/** This plugin is an implementation of the Strahler numbers computation,
 *  first published as:
 *
 *  A.N. Strahler, \n
 *  "Hypsomic analysis of erosional topography",\n
 *  "Bulletin Geological Society of America 63,pages 1117-1142.",\n
 *   1952.\n
 *
 *  Extended to graphs in:
 *
 *  D. Auber, \n
 *  Using Strahler numbers for real time visual exploration of huge graphs, \n
 *  ICCVG, International Conference on Computer Vision and Graphics, \n
 *  pages 56-69, \n
 *  2002, September.
 *
 *  \note This algorithm works on general graphs, if no node is selected,
 *  the algorithm automatically choose the nodes with higher outdegree as
 *  starting nodes.
 *
 *  The algorithm use the "viewSelection" boolean property to determine
 *  the starting nodes.
 *
 */
class StrahlerMetric : public tlp::DoubleAlgorithm {
public:
  PLUGININFORMATION(
      "Strahler", "David Auber", "06/04/2000",
      "Computes the Strahler numbers."
      "This is an implementation of the Strahler numbers computation, first published as:<br/>"
      "<b>Hypsomic analysis of erosional topography</b>,<br/>A.N. Strahler, Bulletin Geological "
      "Society of America 63,pages 1117-1142 (1952).<br/>"
      "Extended to graphs in:<br/><b>Using Strahler numbers for real time visual exploration of "
      "huge graphs</b>,<br/>D. Auber, ICCVG, International Conference on Computer Vision and Graphics, "
      "pages 56-69 (2002)",
      "1.1", "Graph")
  StrahlerMetric(const tlp::PluginContext *context);
  bool run() override;

private:
  struct SortInfos {
    int toFree, prefix;
    bool visited, finished;
    SortInfos() : toFree(0), prefix(0), visited(false), finished(false) {}
  };

  Strahler topSortStrahler(tlp::node n, SortInfos &nInfos, int &curPref,
                           std::unordered_map<tlp::node, SortInfos> &sortInfos,
                           std::unordered_map<tlp::node, Strahler> &cachedValues);
  bool allNodes;
};

#endif
