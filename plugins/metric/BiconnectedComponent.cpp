#include <iostream>
#include <tulip/GraphProperty.h>
#include <stack>

using namespace std;
using namespace tlp;
//=============================================================================================
static void bicoTestAndLabeling(const Graph & graph,node v, MutableContainer<int>& compnum,
                    MutableContainer<int>& dfsnum, MutableContainer<int>& lowpt,
                    MutableContainer<node>& father,stack<node>& current,
                    int& count1,int& count2) {
  lowpt.set(v.id, dfsnum.get(v.id));
  Iterator<edge> *it = graph.getInOutEdges(v);
  while(it->hasNext()) { 
    edge e = it->next();
    node w = graph.opposite(e,v);
    if (dfsnum.get(w.id) == -1) { 
      dfsnum.set(w.id, ++count1);
      current.push(w);
      father.set(w.id, v);
      bicoTestAndLabeling(graph,w,compnum,dfsnum,lowpt,father,current,count1,count2);
      lowpt.set(v.id, std::min(lowpt.get(v.id), lowpt.get(w.id)));
    }
    else
      lowpt.set(v.id, std::min(lowpt.get(v.id), dfsnum.get(w.id)));
  } delete it;
  node w;
  if (father.get(v.id) != node(UINT_MAX) && (lowpt.get(v.id) == dfsnum.get(father.get(v.id).id) ) ) { 
    do { 
      w = current.top();
      current.pop();
      it = graph.getInOutEdges(w);
      edge e;
      while(it->hasNext()) { 
	edge e = it->next();
	if (dfsnum.get(w.id) > dfsnum.get(graph.opposite(e,w).id) ) compnum.set(e.id, count2);
      } delete it;
    } while (w != v);    
    count2++;
  }
}
//=============================================================================================
int biconnectedComponents(const Graph& graph, MutableContainer<int>& compnum) {
  stack<node> current;
  MutableContainer<int> dfsnum;
  dfsnum.setAll(-1);
  MutableContainer<int> lowpt;
  lowpt.setAll(0);
  MutableContainer<node> father;
  father.setAll(node(UINT_MAX));
  int count1 = 0; 
  int count2 = 0;
  int num_isolated = 0;
  node v;
  Iterator<node> *it = graph.getNodes();
  while(it->hasNext()) {
    v = it->next();
    if (dfsnum.get(v.id) == -1) { 
      dfsnum.set(v.id, ++count1);
      bool is_isolated = true;
      Iterator<edge> *it = graph.getInOutEdges(v);
      while (it->hasNext()) { 
	edge e = it->next();
	if ( graph.opposite(e,v) != v ) { 
	  is_isolated = false; 
	  break; 
	}
       } delete it;
      if ( is_isolated ) { 
	num_isolated++; 
      }
      else { 
	current.push(v);
	bicoTestAndLabeling(graph,v,compnum,dfsnum,lowpt,father,current,count1,count2);
	current.pop();
      }
    }
  } delete it;
  return(count2 + num_isolated);
}
//=============================================================================================
#include <tulip/TulipPlugin.h>
using namespace std;
using namespace tlp;

/** \addtogroup metric */
/*@{*/
/// BicconnectedComponent.h - An implementation of the  biconnected component decompostion algorithm.
/** This plugin is an implementation of a biconnected component decompostion algorithm. it assigns to 
 *  to all the edge in the same componnent the same value.
 *
 *  <b>HISTORY</b>
 *
 *  - 03/01/2005 Verson 0.0.1: Initial release
 *
 *
 *  \note This algorithm assigns to each node a value defined as following : If two nodes are in the same
 *  connected component they have the same value else they have a 
 *  different value.
 *
 *
 *  \author David Auber University Bordeaux I France: Email:auber@tulip-software.org
 *
 *  <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 *
 */
class BiconnectedComponnent:public DoubleAlgorithm { 
public:
  BiconnectedComponnent(const PropertyContext &context):DoubleAlgorithm(context){};
  bool run() {
    MutableContainer<int> compo;
    compo.setAll(-1);
    biconnectedComponents(*graph, compo);
    doubleResult->setAllEdgeValue(-1);
    doubleResult->setAllNodeValue(-1);
    Iterator<edge> *it = graph->getEdges();
    while(it->hasNext()) {
      edge e = it->next();
      doubleResult->setEdgeValue(e, compo.get(e.id));
    } delete it;
    return true;
  }
};
/*@}*/
//=============================================================================================
DOUBLEPLUGINOFGROUP(BiconnectedComponnent,"Biconnected Component","David Auber","03/01/2005","Alpha","0","1","Component");
