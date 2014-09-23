/**
 *
 * This file is part of Tulip (www.tulip-software.org)
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

#include <tulip/tuliphash.h>
#include <tulip/TulipPluginHeaders.h>

/** \addtogroup layout */


/** This plug-in implement the bubble tree drawing algorithm
 *  first published as:
 *
 *  Bubble Tree Drawing Algorithm : \n
 *  D. Auber and S. Grivet and J-P Domenger and Guy Melancon, \n
 *  In International Conference on Computer Vision and Graphics, pages 633-641, september 2004.
 *
 *  <b>HISTORY</b>
 *
 *  - 07/12/04 Version 0.0.3: Use new Tulip plugin parameter mechanism
 *  - 16/07/03 Version 0.0.2: Better management of node size
 *  - 16/05/03 Version 0.0.1: Initial release
 *
 *  \note  This algorithm only works on trees.
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
 *   auber@tulip-software.org, grivet@labri.fr
 */
class BubbleTree:public tlp::LayoutAlgorithm {
public:
  PLUGININFORMATION("Bubble Tree","D.Auber/S.Grivet","16/05/2003","Stable","1.0","Tree")
  BubbleTree(const tlp::PluginContext* context);
  ~BubbleTree();
  bool run();
private:
  double computeRelativePosition(tlp::node n, TLP_HASH_MAP< tlp::node, tlp::Vector< double, 5 > >* relativePosition);
  void calcLayout(tlp::node n, TLP_HASH_MAP<tlp::node, tlp::Vector<double, 5 > > *relativePosition);
  void calcLayout2(tlp::node n, TLP_HASH_MAP<tlp::node, tlp::Vector<double, 5 > > *relativePosition,
                   const tlp::Vector<double, 3 > &,const tlp::Vector<double, 3 > &);
  tlp::Graph *tree;
  tlp::SizeProperty *nodeSize;
  bool nAlgo;
};

#endif










