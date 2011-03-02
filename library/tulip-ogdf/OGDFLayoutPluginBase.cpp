#include "OGDFLayoutPluginBase.h"

#include <vector>
#include <tulip/DrawingTools.h>
#include <tulip/ForEach.h>

using namespace std;

OGDFLayoutPluginBase::OGDFLayoutPluginBase(const tlp::PropertyContext &context,
		ogdf::LayoutModule *ogdfLayoutAlgo) :
	tlp::LayoutAlgorithm(context), tlpToOGDF(NULL), ogdfLayoutAlgo(ogdfLayoutAlgo) {
}

OGDFLayoutPluginBase::~OGDFLayoutPluginBase() {
	if (tlpToOGDF != NULL) {
		delete tlpToOGDF;
	}
	delete ogdfLayoutAlgo;
}

bool OGDFLayoutPluginBase::run() {

	// convert Tulip Graph to OGDF Graph including attributes
	tlpToOGDF = new TulipToOGDF(graph);
	ogdf::GraphAttributes &gAttributes = tlpToOGDF->getOGDFGraphAttr();

	beforeCall(tlpToOGDF, ogdfLayoutAlgo);
	// run the algorithm on the OGDF Graph with attributes
	ogdfLayoutAlgo->call(gAttributes);

	// retrieve nodes coordinates computed by the OGDF Layout Algorithm
	// and store it in the Tulip Layout Property
	tlp::node nodeTlp;
	forEach(nodeTlp, graph->getNodes()) {
		tlp::Coord nodeCoord = tlpToOGDF->getNodeCoordFromOGDFGraphAttr(nodeTlp.id);
		layoutResult->setNodeValue(nodeTlp, nodeCoord);
	}

	// same operation as above but with edges
	tlp::edge tlpEdge;
	forEach(tlpEdge, graph->getEdges()) {
		vector<tlp::Coord> edgeCoord =
		tlpToOGDF->getEdgeCoordFromOGDFGraphAttr(tlpEdge.id);
		layoutResult->setEdgeValue(tlpEdge, edgeCoord);
	}

	afterCall(tlpToOGDF, ogdfLayoutAlgo);

	return true;
}

void OGDFLayoutPluginBase::transposeLayoutVertically() {
	tlp::BoundingBox graphBB = tlp::computeBoundingBox(graph, layoutResult, graph->getProperty<SizeProperty>("viewSize"), graph->getProperty<DoubleProperty>("viewRotation"));
	float midY = (graphBB[0][1] + graphBB[1][1]) / 2.f;
	tlp::node n;
	forEach(n, graph->getNodes()) {
		tlp::Coord nodeCoord = layoutResult->getNodeValue(n);
		nodeCoord[1] = midY - (nodeCoord[1] - midY);
		layoutResult->setNodeValue(n, nodeCoord);
	}
	tlp::edge e;
	forEach(e, graph->getEdges()) {
		std::vector<tlp::Coord> bends = layoutResult->getEdgeValue(e);
		for (size_t i = 0 ; i < bends.size() ; ++i) {
			bends[i][1] = midY - (bends[i][1] - midY);
		}
		layoutResult->setEdgeValue(e, bends);
	}
}
