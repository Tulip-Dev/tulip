//-*-c++-*-
/*
 Author: Didier Bathily, Nicolas Bellino, Jonathan Dubois, Christelle Jolly, Antoine Lambert, Nicolas Sorraing

 Email : didier.bathily@etu.u-bordeaux1.fr, nicolas.bellino@etu.u-bordeaux1.fr, jonathan.dubois@etu.u-bordeaux1.fr, christelle.jolly@etu.u-bordeaux1.fr, antoine.lambert@etu.u-bordeaux1.fr, nicolas.sorraing@etu.u-bordeaux1.fr

 Last modification : 03/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
 */

#include "TulipToOGDF.h"
#include <tulip/PropertyTypes.h>
#include <tulip/LayoutProperty.h>
#include <ogdf/basic/geometry.h>
#include <tulip/SizeProperty.h>
#include <tulip/DoubleProperty.h>

#include <vector>

using namespace std;

TulipToOGDF::TulipToOGDF(tlp::Graph *g) :
	gTlp(g) {

	long attributes = ogdf::GraphAttributes::nodeGraphics | ogdf::GraphAttributes::edgeGraphics
			| ogdf::GraphAttributes::edgeDoubleWeight;
	gOGDFAttr = ogdf::GraphAttributes(gOGDF, attributes);

	tlp::SizeProperty *sizeProp = gTlp->getProperty<tlp::SizeProperty> ("viewSize");
	tlp::LayoutProperty *layoutProp = gTlp->getProperty<tlp::LayoutProperty> ("viewLayout");

	tlp::node nTlp;
	ogdf::node nOGDF;
	forEach(nTlp, gTlp->getNodes()) {
		ogdfNodes.set(nTlp.id, gOGDF.newNode(nTlp.id));
		nOGDF = ogdfNodes.get(nTlp.id);
		const tlp::Coord &c = layoutProp->getNodeValue(nTlp);
		gOGDFAttr.x(nOGDF) = c.getX();
		gOGDFAttr.y(nOGDF) = c.getY();
		const tlp::Size &s = sizeProp->getNodeValue(nTlp);
		gOGDFAttr.width(nOGDF) = s.getW();
		gOGDFAttr.height(nOGDF) = s.getH();

	}

	tlp::edge eTlp;
	forEach(eTlp, gTlp->getEdges()) {
		tlp::node srcTlp = gTlp->source(eTlp);
		tlp::node tgtTlp = gTlp->target(eTlp);
		ogdf::edge eogdf = gOGDF.newEdge(ogdfNodes.get(srcTlp.id), ogdfNodes.get(tgtTlp.id));
		ogdfEdges.set(eTlp.id, eogdf);

		const vector<tlp::Coord> &v = layoutProp->getEdgeValue(eTlp);
		ogdf::DPolyline bends;
		for (vector<tlp::Coord>::const_iterator it = v.begin(); it != v.end(); ++it) {
			bends.pushBack(ogdf::DPoint((*it).getX(), (*it).getY()));
		}
		gOGDFAttr.bends(ogdfEdges.get(eTlp.id)) = bends;

		gOGDFAttr.doubleWeight(eogdf) = 1.0;
	}

}

void TulipToOGDF::saveToGML(const char * fileName) {

	gOGDFAttr.writeGML(fileName);

}

tlp::Graph &TulipToOGDF::getTlp() {
	return *gTlp;
}

ogdf::GraphAttributes &TulipToOGDF::getOGDFGraphAttr() {
	return gOGDFAttr;
}

TulipToOGDF::~TulipToOGDF() {

}

ogdf::Graph &TulipToOGDF::getOGDFGraph() {

	return gOGDF;
}

ogdf::node TulipToOGDF::getOGDFGraphNode(unsigned int nodeIndex) {
	return ogdfNodes.get(nodeIndex);
}

ogdf::edge TulipToOGDF::getOGDFGraphEdge(unsigned int edgeIndex) {
	return ogdfEdges.get(edgeIndex);
}

tlp::Coord TulipToOGDF::getNodeCoordFromOGDFGraphAttr(unsigned int nodeIndex) {

	ogdf::node n = ogdfNodes.get(nodeIndex);

	double x = gOGDFAttr.x(n);
	double y = gOGDFAttr.y(n);
	double z = 0;

	return tlp::Coord(x, y, z);

}

vector<tlp::Coord> TulipToOGDF::getEdgeCoordFromOGDFGraphAttr(unsigned int edgeIndex) {

	ogdf::edge e = ogdfEdges.get(edgeIndex);

	ogdf::DPolyline line = gOGDFAttr.bends(e);

	vector<tlp::Coord> v;

	for (ogdf::ListIterator<ogdf::DPoint> p = line.begin(); p.valid(); ++p) {
		v.push_back(tlp::Coord((*p).m_x, (*p).m_y, 0.));
	}

	return v;
}

void TulipToOGDF::copyTlpDoublePropertyToOGDFEdgeLength(tlp::DoubleProperty *metric) {
	tlp::edge eTlp;
	forEach(eTlp, gTlp->getEdges()) {
		gOGDFAttr.doubleWeight(ogdfEdges.get(eTlp.id)) = metric->getEdgeValue(eTlp);
	}
}

void TulipToOGDF::copyTlpNodeSizeToOGDF(tlp::SizeProperty *size) {
	tlp::edge eTlp;
	forEach(eTlp, gTlp->getEdges()) {
		tlp::node srcTlp = gTlp->source(eTlp);
		tlp::node tgtTlp = gTlp->target(eTlp);
		tlp::Size s = size->getNodeValue(srcTlp);
		tlp::Size s2 = size->getNodeValue(tgtTlp);

		gOGDFAttr.width(ogdfNodes.get(srcTlp.id)) = s.getW();
		gOGDFAttr.height(ogdfNodes.get(srcTlp.id)) = s.getH();
		gOGDFAttr.width(ogdfNodes.get(tgtTlp.id)) = s2.getW();
		gOGDFAttr.height(ogdfNodes.get(tgtTlp.id)) = s2.getH();

		gOGDFAttr.doubleWeight(ogdfEdges.get(eTlp.id)) = gOGDFAttr.doubleWeight(ogdfEdges.get(
				eTlp.id)) + s.getW() / 2. + s2.getW() / 2. - 1.;
	}
}

void TulipToOGDF::copyTlpDoublePropertyToOGDFNodeWeight(tlp::DoubleProperty *metric) {
	tlp::node nTlp;
	forEach(nTlp, gTlp->getNodes()) {
		gOGDFAttr.weight(ogdfNodes.get(nTlp.id)) = static_cast<int>(metric->getNodeValue(nTlp));
	}
}

