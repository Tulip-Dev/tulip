/*
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
///@cond DOXYGEN_HIDDEN

#ifndef TULIPTOOGDF_H_
#define TULIPTOOGDF_H_

#include <ogdf/basic/GraphAttributes.h>

#include <tulip/tulipconf.h>
#include <tulip/Graph.h>
#include <tulip/NumericProperty.h>
#include <tulip/StaticProperty.h>
#include <tulip/Coord.h>

#include <exception>
#include <string>
#include <iostream>
#include <vector>

namespace tlp {
class SizeProperty;
// namespace tlp

class TLP_OGDF_SCOPE TulipToOGDF {
public:
  TulipToOGDF(tlp::Graph *g, bool importEdgeBends = true);

  ogdf::GraphAttributes &getOGDFGraphAttr();
  ogdf::Graph &getOGDFGraph();

  ogdf::node getOGDFGraphNode(unsigned int nodeIndex);
  ogdf::edge getOGDFGraphEdge(unsigned int edgeIndex);

  tlp::Coord getNodeCoordFromOGDFGraphAttr(unsigned int nodeIndex);

  void copyTlpNumericPropertyToOGDFEdgeLength(tlp::NumericProperty *);
  void copyTlpNumericPropertyToOGDFNodeWeight(tlp::NumericProperty *);
  void copyTlpNodeSizeToOGDF(tlp::SizeProperty *);

  std::vector<tlp::Coord> getEdgeCoordFromOGDFGraphAttr(unsigned int edgeIndex);

private:
  tlp::Graph *tulipGraph;
  tlp::NodeStaticProperty<ogdf::node> ogdfNodes;
  tlp::EdgeStaticProperty<ogdf::edge> ogdfEdges;
  ogdf::Graph ogdfGraph;
  ogdf::GraphAttributes ogdfAttributes;
};
} // namespace tlp
#endif /* !TULIPTOOGDF_H_ */
///@endcond
