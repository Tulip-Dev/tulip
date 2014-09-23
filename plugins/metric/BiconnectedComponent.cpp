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
#include <iostream>
#include <stack>

#include <tulip/TulipPluginHeaders.h>

using namespace std;
using namespace tlp;
//=============================================================================================
// This is the original dfs recursive implementation
// I don't remove it because it corresponds to the original algorithm
/*static void bicoTestAndLabeling(const Graph & graph,node v, MutableContainer<int>& compnum,
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
  }*/
// simple structure to implement
// the dfs biconnected component loop
struct dfsBicoTestStruct {
  node v;
  node opp;
  Iterator<edge>* ite;

  dfsBicoTestStruct(node n, node o, Iterator<edge> *it):
    v(n), opp(o), ite(it) {}
};
// dfs biconnected component loop
static void bicoTestAndLabeling(const Graph & graph, node v,
                                MutableContainer<int>& compnum,
                                MutableContainer<int>& dfsnum,
                                MutableContainer<int>& lowpt,
                                MutableContainer<node>& father,
                                stack<node>& current,
                                int& count1, int& count2) {
  Iterator<edge> *it = graph.getInOutEdges(v);
  stack<dfsBicoTestStruct> dfsLevels;
  dfsBicoTestStruct dfsParams(v, node(), it);
  dfsLevels.push(dfsParams);
  lowpt.set(v.id, dfsnum.get(v.id));

  while(!dfsLevels.empty()) {
    dfsParams = dfsLevels.top();
    v = dfsParams.v;
    it = dfsParams.ite;

    if (it->hasNext()) {
      edge e = it->next();
      node w = graph.opposite(e, v);

      if (dfsnum.get(w.id) == -1) {
        dfsnum.set(w.id, ++count1);
        current.push(w);
        father.set(w.id, v);
        dfsParams.v = w;
        dfsParams.opp = v;
        dfsParams.ite = graph.getInOutEdges(w);
        dfsLevels.push(dfsParams);
        lowpt.set(w.id, dfsnum.get(w.id));
      }
      else
        lowpt.set(v.id, std::min(lowpt.get(v.id), dfsnum.get(w.id)));
    }
    else {
      delete it;
      dfsLevels.pop();
      node opp = dfsParams.opp;

      if (opp.isValid())
        lowpt.set(opp.id, std::min(lowpt.get(opp.id), lowpt.get(v.id)));

      if (father.get(v.id).isValid() &&
          (lowpt.get(v.id) == dfsnum.get(father.get(v.id).id) ) ) {
        node w;

        do {
          w = current.top();
          current.pop();
          it = graph.getInOutEdges(w);
          edge e;

          while(it->hasNext()) {
            edge e = it->next();

            if (dfsnum.get(w.id) > dfsnum.get(graph.opposite(e,w).id) )
              compnum.set(e.id, count2);
          }

          delete it;
        }
        while (w != v);

        count2++;
      }
    }
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
  father.setAll(node());
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
      }

      delete it;

      if ( is_isolated ) {
        num_isolated++;
      }
      else {
        current.push(v);
        bicoTestAndLabeling(graph,v,compnum,dfsnum,lowpt,father,current,count1,count2);
        current.pop();
      }
    }
  }

  delete it;
  return(count2 + num_isolated);
}
//=============================================================================================
#include <tulip/TulipPluginHeaders.h>
using namespace std;
using namespace tlp;

/** \addtogroup metric */

/** This plugin is an implementation of a biconnected component decomposition algorithm. It assigns
 *  the same value to all the edges in the same component.
 *
 *  \note This algorithm assigns to each node a value defined as following : If two nodes are in the same
 *  connected component they have the same value else they have a
 *  different value.
 *
 */
class BiconnectedComponent:public DoubleAlgorithm {
public:
  PLUGININFORMATION("Biconnected Component","David Auber","03/01/2005","Implements a biconnected component decomposition.","1.0","Component")
  BiconnectedComponent(const tlp::PluginContext* context):DoubleAlgorithm(context) {}
  bool run() {
    MutableContainer<int> compo;
    compo.setAll(-1);
    biconnectedComponents(*graph, compo);
    result->setAllEdgeValue(-1);
    result->setAllNodeValue(-1);
    Iterator<edge> *it = graph->getEdges();

    while(it->hasNext()) {
      edge e = it->next();
      result->setEdgeValue(e, compo.get(e.id));
    }

    delete it;
    return true;
  }
};

//=============================================================================================
PLUGIN(BiconnectedComponent)
