/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
 *
 * Authors: Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * This plug-in is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * This plug-in is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#ifndef EDGE_BUNDLING_H
#define EDGE_BUNDLING_H
#include <climits>
#include <vector>
#include <tulip/TulipPluginHeaders.h>
#include <tulip/LayoutProperty.h>

/** \addtogroup selection */
/*@{*/

/**
 * This plugin is an implementation of the intuitive Edge Bundling technique published in :
 * "Winding Roads: Routing edges into bundles"
 * Antoine Lambert, Romain Bourqui and David Auber
 * Computer Graphics Forum special issue on 12th Eurographics/IEEE-VGTC Symposium on Visualization,
 * June 2010, pages 853-862,
 * doi: 10.1111/j.1467-8659.2009.01700.x
 *
 * Visualizations of large graphs usually suffer from visual clutter due to a large amount of edge
 * crossings and node-edge overlaps.
 * Edge bundling is a technique aiming to reduce such clutter by routing edges and grouping them
 * into bundles.
 * It also helps to uncover high level edge patterns to show how information flows between different
 * region of a graph.
 *
 * To apply this edge bundling technique, all nodes must have different positions.
 * If you encounter some crashes, try to apply the "Fast Overlap Removal" plugin before
 * executing this edge bundling plugin.
 *
 *  \author David Auber, Romain Bourqui, Antoine Lambert, LaBRI University Bordeaux I France:
 *   auber@labri.fr
 */
class EdgeBundling : public tlp::Algorithm {
public:
  EdgeBundling(const tlp::PluginContext *);
  PLUGININFORMATION("Edge bundling", "David Auber/ Romain Bourqui / Antoine Lambert", "2010-...",
                    "Edges routing algorithm, implementing the intuitive Edge Bundling technique "
                    "published as:<br/>"
                    "<b>Winding Roads: Routing edges into bundles</b><br>,"
                    "Antoine Lambert, Romain Bourqui and David Auber, "
                    "Computer Graphics Forum special issue on 12th Eurographics/IEEE-VGTC "
                    "Symposium on Visualization, "
                    "pages 853-862 (2010), doi: <a "
                    "href=\"https://doi.org/10.1111/j.1467-8659.2009.01700.x\">10.1111/"
                    "j.1467-8659.2009.01700.x</a>",
                    "1.4", "")
  bool run() override;

private:
  void fixEdgeType(tlp::EdgeStaticProperty<unsigned int> &);
  void computeDistances();
  void computeDistance(tlp::node, unsigned int = UINT_MAX);
  double longEdges;
  double splitRatio;
  unsigned int optimizationLevel;
  unsigned int maxThread;
  bool forceEdgeTest;
  bool edgeNodeOverlap;
  bool layout3D;
  tlp::LayoutProperty *layout;
  tlp::Graph *vertexCoverGraph, *oriGraph, *gridGraph;
};
/*@}*/

#endif // EDGE_BUNDLING
