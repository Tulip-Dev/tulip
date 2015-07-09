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
namespace {
const char * paramHelp[] = {
  //direction
  HTML_HELP_OPEN()         \
  HTML_HELP_DEF( "type", "String Collection" ) \
  HTML_HELP_DEF( "default", "InOut" )  \
  HTML_HELP_BODY() \
  "This parameter indicates the direction used to compute K-Cores values."  \
  HTML_HELP_CLOSE(),
  // metric
  HTML_HELP_OPEN()              \
  HTML_HELP_DEF( "type", "NumericProperty" )       \
  HTML_HELP_DEF( "value", "An existing edge metric" )   \
  HTML_HELP_BODY()              \
  "An existing edge metric property"\
  HTML_HELP_CLOSE()
};
}
#define DEGREE_TYPE "type"
#define DEGREE_TYPES "InOut;In;Out;"
#define INOUT 0
#define IN 1
#define OUT 2
//========================================================================================
KCores::KCores(const PluginContext *context):DoubleAlgorithm(context) {
  addInParameter<StringCollection>(DEGREE_TYPE, paramHelp[0], DEGREE_TYPES);
  addInParameter<NumericProperty*>("metric",paramHelp[1],"",false);
  addDependency("Degree","1.0");
}
//========================================================================================
KCores::~KCores() {}
//========================================================================================
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

  // keep track of deleted nodes
  MutableContainer<bool> deleted;
  // the number of non deleted nodes
  unsigned int nbNodes = graph->numberOfNodes();
  // the famous k
  double k = result->getNodeMin();

  // loop on remaining nodes
  while (nbNodes > 0) {
    bool modify = true;
    double next_k = DBL_MAX;

    while (modify) {
      modify = false;
      node n;
      forEach(n, graph->getNodes()) {
        // nothing to do if the node
        // has been already deleted
        if (deleted.get(n.id))
          continue;

        double val = result->getNodeValue(n);

        if (val <= k) {
          result->setNodeValue(n, k);
          Iterator<edge>* ite;

          switch(degree_type) {
          case INOUT:
            ite = graph->getInOutEdges(n);
            break;

          case IN:
            ite = graph->getOutEdges(n);
            break;

          case OUT:
          default:
            ite = graph->getInEdges(n);
          }

          // decrease neighbours weighted degree
          while(ite->hasNext()) {
            edge ee = ite->next();
            node m = graph->opposite(ee, n);

            if (deleted.get(m.id))
              continue;

            result->setNodeValue(m, result->getNodeValue(m) -
                                 (metric ? metric->getEdgeDoubleValue(ee) : 1));
          }

          // mark node as deleted
          deleted.set(n.id, true);
          --nbNodes;
          modify = true;
        }
        else if (val < next_k)
          // update next k value
          next_k = val;
      }
    }

    k = next_k;
  }

  return true;
}
//========================================================================================
PLUGIN(KCores)
//========================================================================================
