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
#ifndef _BUBBLETREELAYOUT_H
#define _BUBBLETREELAYOUT_H

#include <tulip/LayoutProperty.h>
#include <tulip/StaticProperty.h>

/** \addtogroup layout */

/** This plug-in implement the bubble tree drawing algorithm
 *  first published as:
 *
 *  Bubble Tree Drawing Algorithm : \n
 *  D. Auber and S. Grivet and J-P Domenger and Guy Melancon, \n
 *  Computer Vision and Graphics. Computational Imaging and Vision, vol 32, 2006. \n
 *  Springer, Dordrecht <a
 * href=\"https://doi.org/10.1007/1-4020-4179-9_91\">10.1007/1-4020-4179-9_91</a>",
 *
 *
 *  Let n be the number of nodes, the algorithm complexity is in O(n) or O(nlog(n)),
 *  By default O(nlog(n)) algorithm is used, but one can choose the complexity by using
 *  the argument (bool)"complexity" (true means  O(nlog(n), false  O(n)).
 *  The algorithm can manage nodes of different size. The SizeProperty "viewSize"
 *  is used by default if no parameters are given to the plug-in.
 *  The parameter is (SizeProperty*) "node size".
 *
 *
 *
 *  \author David Auber, S. Grivet  University Bordeaux I France:
 *   auber@labri.fr, grivet@labri.fr
 */
class BubbleTree : public tlp::LayoutAlgorithm {
public:
  PLUGININFORMATION(
      "Bubble Tree", "D.Auber/S.Grivet", "16/05/2003",
      "Implement the bubble tree drawing algorithm first published as:<br>"
      "<b>Bubble Tree Drawing Algorithm</b>, "
      "S. Grivet, D. Auber, J-P Domenger and Guy Melancon, "
      "Computer Vision and Graphics. Computational Imaging and Vision, vol 32, 2006. Springer, "
      "Dordrecht, "
      "doi: <a href=\"https://doi.org/10.1007/1-4020-4179-9_91\">10.1007/1-4020-4179-9_91</a>",
      "1.1", "Tree")
  BubbleTree(const tlp::PluginContext *context);
  ~BubbleTree() override;
  bool run() override;

private:
  double computeRelativePosition(tlp::node n,
                                 tlp::NodeStaticProperty<tlp::Vector<double, 5>> &relativePosition);
  void calcLayout(tlp::node n, tlp::NodeStaticProperty<tlp::Vector<double, 5>> &relativePosition);
  void calcLayout2(tlp::node n, tlp::Vector<double, 5> &nrPos,
                   tlp::NodeStaticProperty<tlp::Vector<double, 5>> &relativePosition,
                   const tlp::Vector<double, 3> &, const tlp::Vector<double, 3> &);
  tlp::Graph *tree;
  tlp::SizeProperty *nodeSize;
  bool nAlgo;
};

#endif
