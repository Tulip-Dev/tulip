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
#include "Circular.h"
#include "DatasetTools.h"

PLUGIN(Circular)

using namespace std;
using namespace tlp;
namespace {
const char * paramHelp[] = {
  HTML_HELP_OPEN()             \
  HTML_HELP_DEF( "type", "bool" )      \
  HTML_HELP_DEF( "values", "true/false" ) \
  HTML_HELP_DEF( "default", "false" )      \
  HTML_HELP_BODY()              \
  "If true, search first for the maximum length cycle (be careful, this problem is NP-Complete). If false, nodes are ordered using a depth first search." \
  HTML_HELP_CLOSE()
};
}

Circular::Circular(const tlp::PluginContext* context):LayoutAlgorithm(context) {
  addNodeSizePropertyParameter(this);
  addInParameter<bool>("search cycle", paramHelp[0], "false");
}

namespace {
//============================================================================
void visitNode(Graph *sg, node n, vector<node> &vec,
               MutableContainer<bool> &nodeVisited,
               std::list<node> &toVisit) {
  nodeVisited.set(n.id, true);
  vec.push_back(n);
  node neighbour;
  forEach(neighbour, sg->getInOutNodes(n)) {
    if (!nodeVisited.get(neighbour.id))
      toVisit.push_back(neighbour);
  }
}

//============================================================================
void buildDfsOrdering(Graph *sg, vector<node> &vec) {
  MutableContainer<bool> nodeVisited;
  nodeVisited.setAll(false);
  node n;
  forEach(n, sg->getNodes()) {
    std::list<node> toVisit;

    if (!nodeVisited.get(n.id)) {
      visitNode(sg, n, vec, nodeVisited, toVisit);
      // toVisit loop
      std::list<node>::iterator itl = toVisit.begin();

      while(itl != toVisit.end()) {
        node current = *itl;

        if (!nodeVisited.get(current.id))
          visitNode(sg, current, vec, nodeVisited, toVisit);

        ++itl;
      }
    }
  }
}
//===============================================================================
vector<node> extractCycle(node n, deque<node> &st) {
  // tlp::warning() << __PRETTY_FUNCTION__ << endl;
  vector<node> result;
  deque<node>::const_reverse_iterator it = st.rbegin();

  while( (*it) != n ) {
    result.push_back(*it);
    ++it;
  }

  result.push_back(*it);
  return result;
}
//===============================================================================
void dfs(node n, const Graph * sg, deque<node> &st,vector<node> & maxCycle, MutableContainer<bool> &flag,
         unsigned int &nbCalls, PluginProgress *pluginProgress) {
  {
    //to enable stop of the recursion
    ++nbCalls;

    if (nbCalls % 10000 == 0) {
      pluginProgress->progress(rand()%100, 100);
      nbCalls = 0;
    }

    if (pluginProgress->state()!=TLP_CONTINUE) return;
  }

  if(flag.get(n.id)) {
    vector<node> cycle(extractCycle(n, st));

    if(cycle.size() > maxCycle.size())
      maxCycle = cycle;

    return;
  }

  st.push_back(n);
  flag.set(n.id,true);
  node n2;
  forEach(n2, sg->getInOutNodes(n)) {
    dfs(n2, sg, st, maxCycle, flag, nbCalls, pluginProgress);
  }
  flag.set(n.id, false);
  st.pop_back();
}


//=======================================================================
vector<node> findMaxCycle(Graph * sg, PluginProgress *pluginProgress) {
  Graph * g = sg->addCloneSubGraph();
  tlp::warning() << __PRETTY_FUNCTION__ << endl;

  // compute the connected components's subgraphs
  std::vector<std::set<node> > components;
  ConnectedTest::computeConnectedComponents(g, components);

  for (unsigned int i = 0; i < components.size(); ++i) {
    g->inducedSubGraph(components[i]);
  }

  Graph * g_tmp;

  MutableContainer<bool> flag;
  deque<node> st;
  vector<node> res;
  vector<node> max;
  unsigned int nbCalls = 0;
  forEach(g_tmp,g->getSubGraphs()) {
    if(g_tmp->numberOfNodes() == 1)
      continue;

    st.clear();
    res.clear();
    flag.setAll(false);
    dfs(g_tmp->getOneNode( ),g_tmp,st,res,flag, nbCalls, pluginProgress);

    if(max.size() < res.size())
      max = res;
  }

  sg->delAllSubGraphs(g);
  return max;
}
}

//this inline function computes the radius size given a size
inline double computeRadius (const Size &s) {
  return std::max(1E-3, sqrt (s[0]*s[0]/4.0 + s[1]*s[1]/4.0));
}//end computeRad

bool Circular::run() {
  SizeProperty *nodeSize;
  bool searchCycle = false;

  if (!getNodeSizePropertyParameter(dataSet, nodeSize)) {
    if (graph->existProperty("viewSize"))
      nodeSize = graph->getProperty<SizeProperty>("viewSize");
    else {
      nodeSize = graph->getProperty<SizeProperty>("viewSize");
      nodeSize->setAllNodeValue(Size(1.0,1.0,1.0));
    }
  }

  if ( dataSet != NULL )
    dataSet->get("search cycle", searchCycle);

  //compute the sum of radii and the max radius of the graph
  double sumOfRad = 0;
  double maxRad = 0;
  node maxRadNode;
  node itn;
  forEach(itn, graph->getNodes()) {
    double rad = computeRadius(nodeSize->getNodeValue(itn));
    sumOfRad += rad;

    if (maxRad < rad) {
      maxRad = rad;
      maxRadNode = itn;
    }//end if
  }

  //with two nodes, lay them on a line max rad appart
  if (graph->numberOfNodes() <= 2) {
    //set the (max 2) nodes maxRad appart
    double xcoord = maxRad/2.0;
    node itn;
    forEach(itn, graph->getNodes()) {
      result->setNodeValue (itn, Coord (xcoord, 0, 0));
      xcoord *= -1;
    }
  }//end if
  else {
    //this is the current angle
    double gamma = 0;
    //if the ratio of maxRad/sumOfRad > .5, we need to ajust angles
    bool angleAdjust = false;

    if (maxRad/sumOfRad > 0.5) {
      sumOfRad -= maxRad;
      angleAdjust = true;
    }//end if

    //tlp::warning() << "*************************" << endl;

    vector<node> cycleOrdering;

    if (searchCycle)
      cycleOrdering = findMaxCycle(graph, pluginProgress);

    vector<node> dfsOrdering;
    buildDfsOrdering(graph, dfsOrdering);

    MutableContainer<bool> inCir;
    inCir.setAll(false);

    for(unsigned int i =0; i < cycleOrdering.size(); ++i)
      inCir.set(cycleOrdering[i].id, true);

    for(unsigned int i =0; i < dfsOrdering.size(); ++i)
      if(!inCir.get(dfsOrdering[i].id))
        cycleOrdering.push_back(dfsOrdering[i]);

    vector<node>::const_iterator it = cycleOrdering.begin();

    for(; it!=cycleOrdering.end(); ++it) {
      node itn = *it;
      //compute the radius to ensure non overlap.  If adjustment to
      //ensure no angle greater than pi done, detect it.
      double nodeRad = computeRadius(nodeSize->getNodeValue(itn));
      double halfAngle =
        (nodeRad/sumOfRad)*((angleAdjust) ? M_PI/2.0 : M_PI);
      double rayon = nodeRad/sin(halfAngle);

      //if this node was the node that took up more than half the circle,
      //we complet the adjustment to make sure that it does not.
      if (angleAdjust && (maxRadNode == itn)) {
        halfAngle = M_PI/2.0;
        rayon = nodeRad;
      }//end if

      //compute the position of the node.
      gamma += halfAngle;
      result->setNodeValue(itn, Coord(rayon*cos(gamma),
                                      rayon*sin(gamma), 0));
      gamma += halfAngle;
    }//end while
  }//end else

  return true;
} //end run
