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

#include "tulip2ogdf/TulipToOGDF.h"
#include <tulip/PropertyTypes.h>
#include <tulip/LayoutProperty.h>
#include <ogdf/basic/geometry.h>
#include <ogdf/fileformats/GraphIO.h>
#include <tulip/SizeProperty.h>
#include <tulip/NumericProperty.h>

#include <vector>

using namespace std;

TulipToOGDF::TulipToOGDF(tlp::Graph *g, bool importEdgeBends) : tulipGraph(g) {

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
  ogdfNodes = new tlp::NodeStaticProperty<ogdf::node>(g);
  ogdfEdges = new tlp::EdgeStaticProperty<ogdf::edge>(g);

  tlp::SizeProperty *sizeProp = tulipGraph->getProperty<tlp::SizeProperty>("viewSize");
  tlp::LayoutProperty *layoutProp = tulipGraph->getProperty<tlp::LayoutProperty>("viewLayout");

  unsigned int i = 0;
  for (const tlp::node &nTlp : tulipGraph->nodes()) {
    ogdf::node nOGDF = ogdfGraph.newNode(i++);
    (*ogdfNodes)[nTlp] = nOGDF;
    const tlp::Coord &c = layoutProp->getNodeValue(nTlp);
    ogdfAttributes.x(nOGDF) = c.getX();
    ogdfAttributes.y(nOGDF) = c.getY();
    ogdfAttributes.z(nOGDF) = c.getZ();
    const tlp::Size &s = sizeProp->getNodeValue(nTlp);
    ogdfAttributes.width(nOGDF) = s.getW();
    ogdfAttributes.height(nOGDF) = s.getH();
  }

  i = 0;
  for (const tlp::edge &eTlp : tulipGraph->edges()) {
    std::pair<tlp::node, tlp::node> ends = tulipGraph->ends(eTlp);
    ogdf::edge eogdf = ogdfGraph.newEdge(ogdfNodes->getNodeValue(ends.first),
                                         ogdfNodes->getNodeValue(ends.second), i++);
    (*ogdfEdges)[eTlp] = eogdf;

    if (importEdgeBends) {

      const vector<tlp::Coord> &v = layoutProp->getEdgeValue(eTlp);
      ogdf::DPolyline bends;

      for (vector<tlp::Coord>::const_iterator it = v.begin(); it != v.end(); ++it) {
        bends.pushBack(ogdf::DPoint((*it).getX(), (*it).getY()));
      }

      ogdfAttributes.bends(eogdf) = bends;
    }

    ogdfAttributes.doubleWeight(eogdf) = 1.0;
  }
}

void TulipToOGDF::saveToGML(const char *fileName) {
  GraphIO::writeGML(ogdfAttributes, fileName);
}

tlp::Graph &TulipToOGDF::getTlp() {
  return *tulipGraph;
}

ogdf::GraphAttributes &TulipToOGDF::getOGDFGraphAttr() {
  return ogdfAttributes;
}

TulipToOGDF::~TulipToOGDF() {
  delete ogdfNodes;
  delete ogdfEdges;
}

ogdf::Graph &TulipToOGDF::getOGDFGraph() {
  return ogdfGraph;
}

ogdf::node TulipToOGDF::getOGDFGraphNode(const tlp::node &nTlp) {
  return (*ogdfNodes)[nTlp];
}

ogdf::edge TulipToOGDF::getOGDFGraphEdge(const tlp::edge &eTlp) {
  return (*ogdfEdges)[eTlp];
}

tlp::Coord TulipToOGDF::getNodeCoordFromOGDFGraphAttr(const tlp::node &nTlp) {
  ogdf::node n = (*ogdfNodes)[nTlp];

  double x = ogdfAttributes.x(n);
  double y = ogdfAttributes.y(n);
  double z = ogdfAttributes.z(n);

  return tlp::Coord(x, y, z);
}

vector<tlp::Coord> TulipToOGDF::getEdgeCoordFromOGDFGraphAttr(const tlp::edge &eTlp) {
  ogdf::edge e = (*ogdfEdges)[eTlp];
  ogdf::DPolyline line = ogdfAttributes.bends(e);
  vector<tlp::Coord> v;

  for (ogdf::ListIterator<ogdf::DPoint> p = line.begin(); p.valid(); ++p) {
    v.push_back(tlp::Coord((*p).m_x, (*p).m_y, 0.));
  }

  return v;
}

void TulipToOGDF::copyTlpNumericPropertyToOGDFEdgeLength(tlp::NumericProperty *metric) {
  if (!metric)
    return;

  for (const tlp::edge &e : tulipGraph->edges()) {
    ogdfAttributes.doubleWeight((*ogdfEdges)[e]) = metric->getEdgeDoubleValue(e);
  }
}

void TulipToOGDF::copyTlpNodeSizeToOGDF(tlp::SizeProperty *size) {
  if (!size)
    return;

  for (const tlp::edge &e : tulipGraph->edges()) {
    std::pair<tlp::node, tlp::node> ends = tulipGraph->ends(e);
    ogdf::node srcOgdf = ogdfNodes->getNodeValue(ends.first);
    tlp::Size s = size->getNodeValue(ends.first);
    ogdf::node tgtOgdf = ogdfNodes->getNodeValue(ends.second);
    tlp::Size s2 = size->getNodeValue(ends.second);

    ogdfAttributes.width(srcOgdf) = s.getW();
    ogdfAttributes.height(srcOgdf) = s.getH();
    ogdfAttributes.width(tgtOgdf) = s2.getW();
    ogdfAttributes.height(tgtOgdf) = s2.getH();

    ogdf::edge eOgdf = (*ogdfEdges)[e];
    ogdfAttributes.doubleWeight(eOgdf) =
        ogdfAttributes.doubleWeight(eOgdf) + s.getW() / 2. + s2.getW() / 2. - 1.;
  }
}

void TulipToOGDF::copyTlpNumericPropertyToOGDFNodeWeight(tlp::NumericProperty *metric) {
  if (!metric)
    return;

  for (const tlp::node &n : tulipGraph->nodes()) {
    ogdfAttributes.weight((*ogdfNodes)[n]) = int(metric->getNodeDoubleValue(n));
  }
}
