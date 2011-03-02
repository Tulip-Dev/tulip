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
 *
 * Authors: Didier Bathily, Nicolas Bellino, Jonathan Dubois, Christelle Jolly, Antoine Lambert, Nicolas Sorraing
 *
 * Email : didier.bathily@etu.u-bordeaux1.fr, nicolas.bellino@etu.u-bordeaux1.fr, jonathan.dubois@etu.u-bordeaux1.fr, christelle.jolly@etu.u-bordeaux1.fr, antoine.lambert@etu.u-bordeaux1.fr, nicolas.sorraing@etu.u-bordeaux1.fr
 *
 */

#ifndef         TULIPTOOGDF_H_
#define         TULIPTOOGDF_H_

#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/fileformats/GmlParser.h>

#include <tulip/Graph.h>
#include <tulip/TlpTools.h>
#include <tulip/StringProperty.h>
#include <tulip/ForEach.h>

#include <exception>
#include <string>
#include <iostream>
#include <vector>

using namespace tlp;
using namespace ogdf;
using namespace std;

namespace tlp {
  class DoubleProperty;
  class SizeProperty;
}

class TulipToOGDF {
public:

    TulipToOGDF(tlp::Graph *g);
    ~TulipToOGDF();

    void saveToGML(const char * fileName);

    tlp::Graph &getTlp();
    ogdf::GraphAttributes &getOGDFGraphAttr();
    ogdf::Graph &getOGDFGraph();

    ogdf::node getOGDFGraphNode(unsigned int nodeIndex);
    ogdf::edge getOGDFGraphEdge(unsigned int edgeIndex);

    tlp::Coord getNodeCoordFromOGDFGraphAttr(unsigned int nodeIndex);

    void copyTlpDoublePropertyToOGDFEdgeLength(tlp::DoubleProperty *);
    void copyTlpDoublePropertyToOGDFNodeWeight(tlp::DoubleProperty *);
    void copyTlpNodeSizeToOGDF(tlp::SizeProperty *);

    vector<tlp::Coord> getEdgeCoordFromOGDFGraphAttr(unsigned int edgeIndex);

private:
    tlp::Graph *tulipGraph;
    ogdf::Graph ogdfGraph;
    ogdf::GraphAttributes ogdfAttributes;

    MutableContainer<ogdf::node> ogdfNodes;
    MutableContainer<ogdf::edge> ogdfEdges;
};

#endif      /* !TULIPTOOGDF_H_ */
