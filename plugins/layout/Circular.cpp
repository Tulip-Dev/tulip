/**
 Author: David Auber / Daniel Archambault
 Email : auber@labri.fr, archam@cs.ubc.ca
 Last modification : 25/11/2004
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/

/**
 * implementation of Circular (which is bubble circular).  For nodes, history, 
 * author and licence please see the header file Circular.h
 */

#include <math.h>
#include <tulip/ForEach.h>
#include <tulip/TlpTools.h>
#include "Circular.h"

LAYOUTPLUGINOFGROUP(Circular,"Circular","David Auber/ Daniel Archambault","25/11/2004","Ok","0","1","Basic");

using namespace std;
namespace {
  const char * paramHelp[] = {
    // nodeSize
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "SizeProxy" ) \
    HTML_HELP_DEF( "values", "An existing size property" ) \
    HTML_HELP_DEF( "default", "viewSize" ) \
    HTML_HELP_BODY() \
    "This parameter defines the property used for node's sizes." \
    HTML_HELP_CLOSE(),
    HTML_HELP_OPEN()					   \
    HTML_HELP_DEF( "type", "bool" )		   \
    HTML_HELP_DEF( "values", "true/false" ) \
    HTML_HELP_DEF( "default", "false" )		   \
    HTML_HELP_BODY()							\
    "If true search the maximum length cycle. Be careful, this problem is NP-Complete else order the node using a depth first search"	\
    HTML_HELP_CLOSE()
  };
}

Circular::Circular(const PropertyContext &context):Layout(context){
  addParameter<SizesProxy>("nodeSize", paramHelp[0], "viewSize");
  addParameter<bool>("search_cycle", paramHelp[1], "false");
}

namespace {
  //============================================================================
  void dfsRecCall(SuperGraph *graph, vector<node> &vec, MutableContainer<bool> &nodeVisited, node n) {
    nodeVisited.set(n.id, true);
    vec.push_back(n);
    node dest;
    forEach(dest, graph->getInOutNodes(n)) {
      if (!nodeVisited.get(dest.id)) {
	dfsRecCall(graph, vec, nodeVisited, dest);
      }
    }
  }
  //============================================================================
  void buildDfsOrdering(SuperGraph *graph, vector<node> &vec) {
    MutableContainer<bool> nodeVisited;
    nodeVisited.setAll(false);
    node n;
    forEach(n, graph->getNodes()) {
      if (!nodeVisited.get(n.id)) {
	dfsRecCall(graph, vec, nodeVisited, n);
      }
    }
  }
  //============================================================================
  //===============================================================================
  vector<node> extractCycle(node n, deque<node> &st) {
    // cerr << __PRETTY_FUNCTION__ << endl;
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
  void dfs(node n, const SuperGraph * graph, deque<node> &st,vector<node> & maxCycle, MutableContainer<bool> &flag,
	   unsigned int &nbCalls, PluginProgress *pluginProgress) {
    { //to enable stop of the recursion
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
    forEach(n2, graph->getInOutNodes(n)){
      dfs(n2, graph, st, maxCycle, flag, nbCalls, pluginProgress);
    }
    flag.set(n.id, false);
    st.pop_back();
  }
  

  //=======================================================================
  vector<node> findMaxCycle(SuperGraph * graph, PluginProgress *pluginProgress) {
    SuperGraph * g = tlp::newCloneSubGraph(graph);
    cerr << __PRETTY_FUNCTION__ << endl;
    MetricProxy m(g);
    string err ="";
    g->computeProperty(string("Connected Component"),&m,err);
    DataSet tmp;
    tmp.set("Metric", &m);
    tlp::clusterizeGraph(g, err, &tmp, "Equal Value");
    SuperGraph * g_tmp;
    
    MutableContainer<bool> flag;
    deque<node> st;
    vector<node> res;
    vector<node> max;
    unsigned int nbCalls = 0;
    forEach(g_tmp,g->getSubGraphs()){
      if(g_tmp->numberOfNodes() == 1)
	continue;
      st.clear();
      res.clear();
      flag.setAll(false);
      dfs(g_tmp->getOneNode( ),g_tmp,st,res,flag, nbCalls, pluginProgress);
      if(max.size() < res.size())
	max = res;
    }
    
    graph->delAllSubGraphs(g);
    return max;
  } 
}

//this inline function computes the radius size given a size
inline double computeRadius (const Size &s) {
  return sqrt (s[0]*s[0]/4.0 + s[1]*s[1]/4.0);
}//end computeRad

bool Circular::run() {
  SizesProxy *nodeSizes;
  bool searchCycle = false;
  
  if ( dataSet==0 || !dataSet->get("nodeSize", nodeSizes)) {
    if (superGraph->existProperty("viewSize"))
      nodeSizes = superGraph->getProperty<SizesProxy>("viewSize");    
    else {
      nodeSizes = superGraph->getProperty<SizesProxy>("viewSize");  
      nodeSizes->setAllNodeValue(Size(1.0,1.0,1.0));
    }
  }

  if ( dataSet != 0 )
    dataSet->get("search_cycle", searchCycle);
  
  Size resultSize;
  double sizeTm;

  //compute the sum of radii and the max radius of the graph
  double sumOfRad = 0;
  double maxRad = 0;
  node maxRadNode;
  node itn;
  forEach(itn, superGraph->getNodes()) {
    double rad = computeRadius(nodeSizes->getNodeValue(itn));
    sumOfRad += rad;
    if (maxRad < rad) {
      maxRad = rad;
      maxRadNode = itn;
    }//end if
  }

  //with two nodes, lay them on a line max rad appart
  if (superGraph->numberOfNodes() <= 2) {
    //set the (max 2) nodes maxRad appart
    double xcoord = maxRad/2.0;
    node itn;
    forEach(itn, superGraph->getNodes()) {
      layoutProxy->setNodeValue (itn, Coord (xcoord, 0, 0));
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
    cerr << "*************************" << endl;

    vector<node> cycleOrdering;
    if (searchCycle)
      cycleOrdering = findMaxCycle(superGraph, pluginProgress);
    
    vector<node> dfsOrdering;
    buildDfsOrdering(superGraph, dfsOrdering);

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
      //get the node and its size
      Size curNodeSize = nodeSizes->getNodeValue(itn);
      
      //compute the radius to ensure non overlap.  If adjustment to 
      //ensure no angle greater than pi done, detect it.
      double nodeRad = computeRadius(curNodeSize);
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
      layoutProxy->setNodeValue(itn, Coord(rayon*cos(gamma),
					   rayon*sin(gamma), 0));
      gamma += halfAngle;
    }//end while
  }//end else
  return true;
} //end run
