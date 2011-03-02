/**
 * 
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
 * Authors: Didier Bathily, Nicolas Bellino, Jonathan Dubois, Christelle Jolly, Antoine Lambert, Nicolas Sorraing
 *
 * Email : didier.bathily@etu.u-bordeaux1.fr, nicolas.bellino@etu.u-bordeaux1.fr, jonathan.dubois@etu.u-bordeaux1.fr, christelle.jolly@etu.u-bordeaux1.fr, antoine.lambert@etu.u-bordeaux1.fr, nicolas.sorraing@etu.u-bordeaux1.fr
 *
 */

#include "TulipToOGDF.h"
#include <tulip/PropertyTypes.h>
#include <tulip/LayoutProperty.h>
#include <ogdf/basic/geometry.h>
#include <tulip/SizeProperty.h>
#include <tulip/DoubleProperty.h>

#include <vector>

using namespace std;

TulipToOGDF::TulipToOGDF(tlp::Graph *g) : tulipGraph(g) {

    long attributes = ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics
                      | ogdf::GraphAttributes::edgeDoubleWeight;
    ogdfAttributes = ogdf::GraphAttributes(ogdfGraph, attributes);

    tlp::SizeProperty *sizeProp = tulipGraph->getProperty<tlp::SizeProperty> ("viewSize");
    tlp::LayoutProperty *layoutProp = tulipGraph->getProperty<tlp::LayoutProperty> ("viewLayout");

    tlp::node nTlp;
    ogdf::node nOGDF;
    forEach(nTlp, tulipGraph->getNodes()) {
        ogdfNodes.set(nTlp.id, ogdfGraph.newNode(nTlp.id));
        nOGDF = ogdfNodes.get(nTlp.id);
        const tlp::Coord &c = layoutProp->getNodeValue(nTlp);
        ogdfAttributes.x(nOGDF) = c.getX();
        ogdfAttributes.y(nOGDF) = c.getY();
        const tlp::Size &s = sizeProp->getNodeValue(nTlp);
        ogdfAttributes.width(nOGDF) = s.getW();
        ogdfAttributes.height(nOGDF) = s.getH();
    }

    tlp::edge eTlp;
    forEach(eTlp, tulipGraph->getEdges()) {
        tlp::node srcTlp = tulipGraph->source(eTlp);
        tlp::node tgtTlp = tulipGraph->target(eTlp);
        ogdf::edge eogdf = ogdfGraph.newEdge(ogdfNodes.get(srcTlp.id), ogdfNodes.get(tgtTlp.id));
        ogdfEdges.set(eTlp.id, eogdf);

        const vector<tlp::Coord> &v = layoutProp->getEdgeValue(eTlp);
        ogdf::DPolyline bends;
        for (vector<tlp::Coord>::const_iterator it = v.begin(); it != v.end(); ++it) {
            bends.pushBack(ogdf::DPoint((*it).getX(), (*it).getY()));
        }
        ogdfAttributes.bends(ogdfEdges.get(eTlp.id)) = bends;

        ogdfAttributes.doubleWeight(eogdf) = 1.0;
    }
}

void TulipToOGDF::saveToGML(const char * fileName) {
    ogdfAttributes.writeGML(fileName);
}

tlp::Graph &TulipToOGDF::getTlp() {
    return *tulipGraph;
}

ogdf::GraphAttributes &TulipToOGDF::getOGDFGraphAttr() {
    return ogdfAttributes;
}

TulipToOGDF::~TulipToOGDF() {
}

ogdf::Graph &TulipToOGDF::getOGDFGraph() {
    return ogdfGraph;
}

ogdf::node TulipToOGDF::getOGDFGraphNode(unsigned int nodeIndex) {
    return ogdfNodes.get(nodeIndex);
}

ogdf::edge TulipToOGDF::getOGDFGraphEdge(unsigned int edgeIndex) {
    return ogdfEdges.get(edgeIndex);
}

tlp::Coord TulipToOGDF::getNodeCoordFromOGDFGraphAttr(unsigned int nodeIndex) {
    ogdf::node n = ogdfNodes.get(nodeIndex);

    double x = ogdfAttributes.x(n);
    double y = ogdfAttributes.y(n);
    double z = 0;

    return tlp::Coord(x, y, z);
}

vector<tlp::Coord> TulipToOGDF::getEdgeCoordFromOGDFGraphAttr(unsigned int edgeIndex) {
    ogdf::edge e = ogdfEdges.get(edgeIndex);
    ogdf::DPolyline line = ogdfAttributes.bends(e);
    vector<tlp::Coord> v;

    for (ogdf::ListIterator<ogdf::DPoint> p = line.begin(); p.valid(); ++p) {
        v.push_back(tlp::Coord((*p).m_x, (*p).m_y, 0.));
    }

    return v;
}

void TulipToOGDF::copyTlpDoublePropertyToOGDFEdgeLength(tlp::DoubleProperty *metric) {
    tlp::edge eTlp;
    forEach(eTlp, tulipGraph->getEdges()) {
        ogdfAttributes.doubleWeight(ogdfEdges.get(eTlp.id)) = metric->getEdgeValue(eTlp);
    }
}

void TulipToOGDF::copyTlpNodeSizeToOGDF(tlp::SizeProperty *size) {
    tlp::edge eTlp;
    forEach(eTlp, tulipGraph->getEdges()) {
        tlp::node srcTlp = tulipGraph->source(eTlp);
        tlp::node tgtTlp = tulipGraph->target(eTlp);
        tlp::Size s = size->getNodeValue(srcTlp);
        tlp::Size s2 = size->getNodeValue(tgtTlp);

        ogdfAttributes.width(ogdfNodes.get(srcTlp.id)) = s.getW();
        ogdfAttributes.height(ogdfNodes.get(srcTlp.id)) = s.getH();
        ogdfAttributes.width(ogdfNodes.get(tgtTlp.id)) = s2.getW();
        ogdfAttributes.height(ogdfNodes.get(tgtTlp.id)) = s2.getH();

        ogdfAttributes.doubleWeight(ogdfEdges.get(eTlp.id)) = ogdfAttributes.doubleWeight(ogdfEdges.get(
                    eTlp.id)) + s.getW() / 2. + s2.getW() / 2. - 1.;
    }
}

void TulipToOGDF::copyTlpDoublePropertyToOGDFNodeWeight(tlp::DoubleProperty *metric) {
    tlp::node nTlp;
    forEach(nTlp, tulipGraph->getNodes()) {
        ogdfAttributes.weight(ogdfNodes.get(nTlp.id)) = static_cast<int>(metric->getNodeValue(nTlp));
    }
}
