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
#ifndef _TREE3DLAYOUT_H
#define _TREE3DLAYOUT_H

#include <unordered_map>
#include <tulip/LayoutProperty.h>
/** \addtogroup layout */

/** This plugin is an extension of the Cone tree layout
 *  algorithm first published as:
 *
 *  A. FJ. Carriere and R. Kazman, \n
 *  "Interacting with Huge Hierarchies: Beyond Cone Trees", \n
 *  In Proceedings of InfoViz'95, \n
 *  IEEE Symposium on Information Visualization \n
 *  pages 74-78, 1995,\n
 * doi: <a href=\"https://dx.doi.org/10.1109/INFVIS.1995.528689\">10.1109/INFVIS.1995.528689</a>
 *
 *  Information about the extension can be found in :
 *
 *  D.Auber, PhD Thesis, "Outils de visualisation de larges structures de donnees",
 *  University Bordeaux, december 2002.
 *
 *  Let n be the number of nodes, the algorithm complexity is in O(n).
 *
 *  \author David Auber University Bordeaux I France: Email:auber@labri.fr
 */
class ConeTreeExtended : public tlp::LayoutAlgorithm {
public:
  PLUGININFORMATION(
      "Cone Tree", "David Auber", "01/04/1999",
      "Implements an extension of the Cone tree layout algorithm first published as:<br/>"
      "<b>Interacting with Huge Hierarchies: Beyond Cone Trees </b>, A. FJ. Carriere and R. "
      "Kazman, InfoViz'95, IEEE Symposium on Information Visualization pages 74--78 (1995),"
      "doi: <a "
      "href=\"https://dx.doi.org/10.1109/INFVIS.1995.528689\">10.1109/INFVIS.1995.528689</a>",
      "1.0", "Tree")
  ConeTreeExtended(const tlp::PluginContext *context);
  ~ConeTreeExtended() override;
  bool run() override;

private:
  double treePlace3D(tlp::node n, std::unordered_map<tlp::node, double> *posRelX,
                     std::unordered_map<tlp::node, double> *posRelY);
  void calcLayout(tlp::node n, std::unordered_map<tlp::node, double> *px,
                  std::unordered_map<tlp::node, double> *py, double x, double y, int level);
  void computeLayerSize(tlp::node n, unsigned int level);
  void computeYCoodinates(tlp::node root);
  tlp::Graph *tree;
  tlp::SizeProperty *nodeSize;
  std::vector<float> yCoordinates;
  std::vector<float> levelSize;
  float spaceBetweenLevels;
};

#endif
