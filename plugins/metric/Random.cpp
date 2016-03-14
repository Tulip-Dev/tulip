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
#include <tulip/DoubleProperty.h>
#include <tulip/StringCollection.h>
#include <tulip/TlpTools.h>

using namespace tlp;

namespace {
const char * paramHelp[] = {
// target
HTML_HELP_OPEN()         \
HTML_HELP_DEF( "type", "String Collection" ) \
HTML_HELP_DEF("values", "nodes <BR> edges") \
HTML_HELP_DEF( "default", "nodes" )  \
HTML_HELP_BODY() \
"Whether metric is computed only for nodes, only for edges, or for both."  \
HTML_HELP_CLOSE(),
};
}

#define TARGET_TYPE "target"
#define TARGET_TYPES "both;nodes;edges"
#define NODES_TARGET 1
#define EDGES_TARGET 2
#define BOTH_TARGET 0

class RandomMetric: public DoubleAlgorithm {
    //===========================================
public:
    PLUGININFORMATION("Random metric","David Auber","04/10/2001","Assigns random values to nodes and edges.","1.1","Misc")
    RandomMetric(const tlp::PluginContext* context):DoubleAlgorithm(context) {
        addInParameter<StringCollection>(TARGET_TYPE, paramHelp[0], TARGET_TYPES);

        // result needs to be an inout parameter
        // in order to preserve the original values of non targetted elements
        // i.e if "target" = "nodes", the values of edges must be preserved
        // and if "target" = "edges", the values of nodes must be preserved
        parameters.setDirection("result", INOUT_PARAM);
    }

//===========================================
bool run() {
    // initialize a random sequence according the given seed
    tlp::initRandomSequence();

    bool nodes(true), edges(true);
    if ( dataSet!=NULL ) {
        StringCollection targetType;
        dataSet->get(TARGET_TYPE, targetType);
        if(targetType.getCurrent()==NODES_TARGET) {
            edges=false;
            nodes=true;
        }
        else if (targetType.getCurrent()==NODES_TARGET) {
            edges=true;
            nodes=false;
        }
        else {
            edges=true;
            nodes=true;
        }
    }
    if(nodes) {
        node n;
        forEach(n, graph->getNodes())
                result->setNodeValue(n, (double)rand() / double(RAND_MAX));
    }
    if(edges) {
        edge e;
        forEach(e, graph->getEdges())
                result->setEdgeValue(e, (double)rand() / double(RAND_MAX));
    }
    return true;
}
//===========================================
};
PLUGIN(RandomMetric)
