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
#ifdef _OPENMP
#include <omp.h>
#endif
#include <tulip/TulipPluginHeaders.h>
#include <tulip/StringCollection.h>
#include <climits>

using namespace std;
using namespace tlp;

/** \addtogroup metric */
/*@{*/
/**
 * \file
 * \brief A metric based on the K-core decomposition of a graph.
 *
 * K-cores were first introduced in:
 *
 * S. B. Seidman, "Network structure and minimum degree",
 * Social Networks 5:269-287, 1983
 *
 * This is a method for simplifying a graph topology which helps in analysis
 * and visualization of social networks
 *
 * (see http://en.wikipedia.org/wiki/K-core for more details)
 *
 * The K-Cores metric can also be computed according to weighted degrees. See :
 *
 * C. Giatsidis, D. Thilikos, M. Vazirgiannis, \n
 * "Evaluating cooperation in communities with the k-core structure",\n
 * "Proceedings of the 2011 International Conference on Advances in Social Networks Analysis and Mining (ASONAM)",\n
 * "2011"
 *
 * \note Use the default parameters to compute simple K-Cores (undirected and unweighted)
 *
 *  <b>HISTORY</b>
 *
 *  - 2006 Version 1.0 by David Auber, LaBRI,
 *  University Bordeaux I, France
 *  - 2011 Version 2.0: Add In/Out and Weighted computation features
 *  by François Queyroi, LaBRI, University Bordeaux I, France
 *  - 2015 Performance optimization by Patrick Mary
 *
 *
 */
class KCores:public tlp::DoubleAlgorithm {
public:

  PLUGININFORMATION("K-Cores", "David Auber","28/05/2006",
                    "Node partitioning measure based on the K-core decomposition of a graph.<br/>"
                    "K-cores were first introduced in:<br/><b>Network structure and minimum degree</b>, S. B. Seidman, Social Networks 5:269-287 (1983).<br/>"
                    "This is a method for simplifying a graph topology which helps in analysis and visualization of social networks.<br>"
                    "<b>Note</b>: use the default parameters to compute simple K-Cores (undirected and unweighted).",
                    "2.0", "Graph")

  KCores(const tlp::PluginContext *context);
  ~KCores();
  bool run();
};

//========================================================================================
static const char *paramHelp[] = {
  // direction
  "This parameter indicates the direction used to compute K-Cores values.",

  // metric
  "An existing edge metric property."
};
#define DEGREE_TYPE "type"
#define DEGREE_TYPES "InOut;In;Out;"
#define INOUT 0
#define IN 1
#define OUT 2
//========================================================================================
KCores::KCores(const PluginContext *context):DoubleAlgorithm(context) {
  addInParameter<StringCollection>(DEGREE_TYPE, paramHelp[0], DEGREE_TYPES, true, "InOut <br> In <br> Out");
  addInParameter<NumericProperty*>("metric",paramHelp[1],"",false);
  addDependency("Degree","1.0");
}
//========================================================================================
KCores::~KCores() {}
//========================================================================================
// to maximize the locality of reference we will use a vector
// holding the all the nodes needed info in the structure below
struct nodeInfo {
  double k;
  bool deleted;
};

bool KCores::run() {
  NumericProperty* metric = NULL;
  StringCollection degreeTypes(DEGREE_TYPES);
  degreeTypes.setCurrent(0);

  if (dataSet!=NULL) {
    dataSet->get(DEGREE_TYPE, degreeTypes);
    dataSet->get("metric", metric);
  }

  unsigned int degree_type = degreeTypes.getCurrent();

  string errMsg="";
  graph->applyPropertyAlgorithm("Degree", result, errMsg,
                                pluginProgress, dataSet);


#ifdef _MSC_VER
  int i = 0;
#else
  unsigned int i = 0;
#endif
  // the famous k
  double k = DBL_MAX;
  // record nodes info in a vector to improve
  // the locality of reference during the multiple
  // needed nodes loop
  NodeStaticProperty<nodeInfo> nodesInfo(graph);
  const std::vector<node>& nodes = graph->nodes();
  // the number of non deleted nodes
  unsigned int nbNodes = nodes.size();

  for(unsigned int i = 0; i < nbNodes; ++i) {
    nodeInfo& nInfo = nodesInfo[i];
    nInfo.k = result->getNodeValue(nodes[i]);
    k = std::min(k, nInfo.k);
    nInfo.deleted = false;
  }
  
  bool noEdgeCheck = (graph == graph->getRoot());
  // loop on remaining nodes
  while (nbNodes) {
    bool modify = true;
    double next_k = DBL_MAX;

    while (modify) {
      modify = false;

      // finally set the values
      for (i = 0; i < nodesInfo.size(); ++i) {
        nodeInfo& nInfo = nodesInfo[i];

        // nothing to do if the node
        // is already deleted
        if (nInfo.deleted)
          continue;

        node n = nodes[i];

        unsigned int current_k = nInfo.k;

        if (current_k <= k) {
          nInfo.k = k;
          // decrease neighbours weighted degree
	  const std::vector<edge>& edges = graph->allEdges(n);
	  unsigned int nbEdges = edges.size();
	  for (unsigned int i = 0; i < nbEdges; ++i) {
	    edge ee = edges[i];
	    if (noEdgeCheck || graph->isElement(ee)) {
	      std::pair<node, node> ends = graph->ends(ee);
	      node m;
	      switch(degree_type) {
	      case IN:
		if ((m = ends.second) == n)
		  continue;
		break;
		
	      case OUT:
		if ((m = ends.first) == n)
		  continue;
		break;
		
	      default:
		m = (ends.first == n) ? ends.second : ends.first;
	      }
	      nodeInfo& mInfo = nodesInfo[graph->nodePos(m)];

	      if (mInfo.deleted)
		continue;

		mInfo.k -= metric ? metric->getEdgeDoubleValue(ee) : 1;
	    }
	  }
	  // mark node as deleted
	  nInfo.deleted = true;
	  --nbNodes;
	  modify = true;
	}
        else if (current_k < next_k)
          // update next k value
          next_k = current_k;
      }
    }

    k = next_k;
  }

  // finally set the values
#ifdef _OPENMP
  #pragma omp parallel for
#endif
  for (i = 0; i < nodesInfo.size(); ++i) {
    nodeInfo& nInfo = nodesInfo[i];
    result->setNodeValue(nodes[i], nInfo.k);
  }

  return true;
}
//========================================================================================
PLUGIN(KCores)
