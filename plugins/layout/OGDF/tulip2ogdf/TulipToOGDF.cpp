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

#include <tulip2ogdf/TulipToOGDF.h>

#include <tulip/PropertyTypes.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/NumericProperty.h>

#include <vector>

using namespace std;
using namespace tlp;
using namespace ogdf;

TulipToOGDF::TulipToOGDF(tlp::Graph *g, bool importEdgeBends)
    : tulipGraph(g), ogdfNodes(g), ogdfEdges(g) {

  // needed to initialize some ogdfAttributes fields
  long attributes =
      // x, y, width, height fields
      ogdf::GraphAttributes::nodeGraphics |
      // bends field
      ogdf::GraphAttributes::edgeGraphics |
      // doubleWeight field
      ogdf::GraphAttributes::edgeDoubleWeight |
      // weight field
      ogdf::GraphAttributes::nodeWeight |
      // z coordinate
      ogdf::GraphAttributes::threeD;

  ogdfAttributes = ogdf::GraphAttributes(ogdfGraph, attributes);

  tlp::SizeProperty *sizeProp = tulipGraph->getProperty<tlp::SizeProperty>("viewSize");
  tlp::LayoutProperty *layoutProp = tulipGraph->getProperty<tlp::LayoutProperty>("viewLayout");

  const std::vector<tlp::node> &nodes = tulipGraph->nodes();
  size_t nbElts = nodes.size();

  for (size_t i = 0; i < nbElts; ++i) {
    ogdf::node nOGDF = ogdfGraph.newNode(i);
    ogdfNodes[i] = nOGDF;
    tlp::node nTlp = nodes[i];
    const tlp::Coord &c = layoutProp->getNodeValue(nTlp);
    ogdfAttributes.x(nOGDF) = c.getX();
    ogdfAttributes.y(nOGDF) = c.getY();
    ogdfAttributes.z(nOGDF) = c.getZ();
    const tlp::Size &s = sizeProp->getNodeValue(nTlp);
    ogdfAttributes.width(nOGDF) = s.getW();
    ogdfAttributes.height(nOGDF) = s.getH();
  }

  const std::vector<tlp::edge> &edges = tulipGraph->edges();
  nbElts = edges.size();

  for (size_t i = 0; i < nbElts; ++i) {
    tlp::edge eTlp = edges[i];
    auto ends = tulipGraph->ends(eTlp);
    ogdf::edge eogdf = ogdfGraph.newEdge(ogdfNodes[ends.first], ogdfNodes[ends.second], i);
    ogdfEdges[i] = eogdf;

    if (importEdgeBends) {

      const vector<tlp::Coord> &v = layoutProp->getEdgeValue(eTlp);
      ogdf::DPolyline bends;

      for (const Coord &coord : v) {
        bends.pushBack(ogdf::DPoint(coord.getX(), coord.getY()));
      }

      ogdfAttributes.bends(eogdf) = bends;
    }

    ogdfAttributes.doubleWeight(eogdf) = 1.0;
  }
}

ogdf::GraphAttributes &TulipToOGDF::getOGDFGraphAttr() {
  return ogdfAttributes;
}

ogdf::Graph &TulipToOGDF::getOGDFGraph() {
  return ogdfGraph;
}

ogdf::node TulipToOGDF::getOGDFGraphNode(unsigned int nodeIndex) {
  return ogdfNodes[nodeIndex];
}

ogdf::edge TulipToOGDF::getOGDFGraphEdge(unsigned int edgeIndex) {
  return ogdfEdges[edgeIndex];
}

tlp::Coord TulipToOGDF::getNodeCoordFromOGDFGraphAttr(unsigned int nodeIndex) {
  ogdf::node n = ogdfNodes[nodeIndex];

  double x = ogdfAttributes.x(n);
  double y = ogdfAttributes.y(n);
  double z = ogdfAttributes.z(n);

  return tlp::Coord(x, y, z);
}

vector<tlp::Coord> TulipToOGDF::getEdgeCoordFromOGDFGraphAttr(unsigned int edgeIndex) {
  ogdf::edge e = ogdfEdges[edgeIndex];
  vector<tlp::Coord> v;

  for (const DPoint &p : ogdfAttributes.bends(e)) {
    v.push_back(tlp::Coord(p.m_x, p.m_y, 0.));
  }

  return v;
}

void TulipToOGDF::copyTlpNumericPropertyToOGDFEdgeLength(tlp::NumericProperty *metric) {
  if (metric == nullptr)
    return;

  const std::vector<tlp::edge> &edges = tulipGraph->edges();
  size_t nbEdges = edges.size();

  for (size_t i = 0; i < nbEdges; ++i) {
    ogdfAttributes.doubleWeight(ogdfEdges[i]) = metric->getEdgeDoubleValue(edges[i]);
  }
}

void TulipToOGDF::copyTlpNodeSizeToOGDF(tlp::SizeProperty *size) {
  if (size == nullptr)
    return;

  const std::vector<tlp::edge> &edges = tulipGraph->edges();
  size_t nbEdges(edges.size());

  for (unsigned int i = 0; i < nbEdges; ++i) {
    const std::pair<tlp::node, tlp::node> &ends = tulipGraph->ends(edges[i]);
    ogdf::node srcOgdf = ogdfNodes[ends.first];
    const tlp::Size &s = size->getNodeValue(ends.first);
    ogdf::node tgtOgdf = ogdfNodes[ends.second];
    const tlp::Size &s2 = size->getNodeValue(ends.second);

    ogdfAttributes.width(srcOgdf) = s.getW();
    ogdfAttributes.height(srcOgdf) = s.getH();
    ogdfAttributes.width(tgtOgdf) = s2.getW();
    ogdfAttributes.height(tgtOgdf) = s2.getH();

    ogdf::edge eOgdf = ogdfEdges[i];
    ogdfAttributes.doubleWeight(eOgdf) =
        ogdfAttributes.doubleWeight(eOgdf) + s.getW() / 2. + s2.getW() / 2. - 1.;
  }
}

void TulipToOGDF::copyTlpNumericPropertyToOGDFNodeWeight(tlp::NumericProperty *metric) {
  if (metric == nullptr)
    return;

  const std::vector<tlp::node> &nodes = tulipGraph->nodes();
  size_t nbNodes = nodes.size();

  for (size_t i = 0; i < nbNodes; ++i) {
    ogdfAttributes.weight(ogdfNodes[i]) = int(metric->getNodeDoubleValue(nodes[i]));
  }
}
