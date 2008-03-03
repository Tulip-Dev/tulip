#include <iostream>
#include <queue>
#include <vector>
#include <set>
#include <tulip/TulipPlugin.h>
#include <tulip/TlpTools.h>
#include <tulip/Node.h>
#include <tulip/StableIterator.h>
#include <tulip/MapIterator.h>
#include <tulip/MapIterator.h>
#include <tulip/PlanarConMap.h>
#include <tulip/PlanarityTest.h>
#include <tulip/SimpleTest.h>
#include "PlanarGraph.h"
using namespace std;


struct PlanarFPP:public Layout {
  PlanarFPP(const PropertyContext &context):Layout(context){
  }
  //==========================================================================
  void accumulateOffset(node v, int delta,  MutableContainer<int> &deltaX, const MutableContainer<node> &left, const MutableContainer<node> &right) {
    if (v.isValid()) {
      deltaX.set(v.id, deltaX.get(v.id) + delta);
      accumulateOffset(left.get(v.id) , deltaX.get(v.id), deltaX, left, right);
      accumulateOffset(right.get(v.id), deltaX.get(v.id), deltaX, left, right);
    }
  }
  //==========================================================================
  void drawFPP(SuperGraph *graph, vector<node> &order) {
    MutableContainer<unsigned int> canOrder;
    for (unsigned int i=0; i<order.size(); ++i) {
      canOrder.set(order[i].id, i);
    }
    MutableContainer<int> deltaX;
    MutableContainer<int> y;
    MutableContainer<node> right;
    MutableContainer<node> left;
    deltaX.setAll(0);
    y.setAll(0);
    right.setAll(node());
    left.setAll(node());
    deltaX.set(order[0].id, 0); //v1
    deltaX.set(order[1].id, 1); //v2
    deltaX.set(order[2].id, 1); //v3
    y.set(order[0].id, 0); //v1
    y.set(order[1].id, 0); //v2
    y.set(order[2].id, 1); //v3
    right.set(order[0].id, order[2]); //right(v1) = v3
    right.set(order[2].id, order[1]); //right(v3) = v2
    
    for (unsigned int k = 3; k < order.size(); ++k) {
      
      //compute neighborhood of vk in C0(Gk-1)
      node vK = order[k];
       vector<node> w;
      { 
	Iterator<edge> *itE = graph->getInOutEdges(vK);
	edge start;
	while (itE->hasNext() && !start.isValid()) {
	  edge e = itE->next();
	  if (canOrder.get(graph->opposite(e, vK).id) > k){
	    start = e;
	  }
	}delete itE;
	Iterator<node> *it;
	if (!start.isValid()) {
	  it = new NodeMapIterator(graph, order[1] , vK);
	}
	else {
	  it = new NodeMapIterator(graph, graph->opposite(start, vK) , vK);
	}
	while (it->hasNext()) {
	  node n = it->next();
	  if (canOrder.get(n.id) < k)
	    w.push_back(n);
	}delete it;
	if (!start.isValid()) w.push_back(order[1]);
      }
      vector<node>::const_iterator it;
      unsigned int p = 0;
      unsigned int q = w.size() - 1;

      deltaX.set(w[p + 1].id, deltaX.get(w[p + 1].id) + 1);
      deltaX.set(w[q].id, deltaX.get(w[q].id) + 1);

      int deltaWpWq = 0;
      for (unsigned int i = p+1; i <= q; ++i)
	deltaWpWq += deltaX.get(w[i].id);
      deltaX.set(vK.id, ( deltaWpWq + y.get(w[q].id) - y.get(w[p].id) ) / 2  );
      y.set(vK.id, ( deltaWpWq + y.get(w[q].id) + y.get(w[p].id) ) / 2 );
      deltaX.set(w[q].id, deltaWpWq - deltaX.get(vK.id));
      if (p+1 != q) {
	deltaX.set(w[p+1].id, deltaX.get(w[p+1].id) - deltaX.get(vK.id));
      }
      right.set(w[p].id, vK);
      right.set(vK.id, w[q]);
      if (p+1 != q) {
	left.set(vK.id, w[p+1]);
	right.set(w[q-1].id, node());
	assert(!right.get(w[q-1].id).isValid());
      }
      else {
	left.set(vK.id, node());
	assert(!left.get(vK.id).isValid());
      }
    }

    accumulateOffset(order[0], 0, deltaX, left, right);
    LayoutProxy *layout = layoutProxy;
    Iterator<node> *it = graph->getNodes();
    while(it->hasNext()) {
      node n = it->next();
      layout->setNodeValue(n, Coord(deltaX.get(n.id), y.get(n.id), 0));
    } delete it;
  }
//==========================================================================
  bool run() {
    vector<edge> addedEdges;

    tlp::triangulate(superGraph, addedEdges);
    PlanarConMap mapG(superGraph);
    
    vector<node> order;
    tlp::canonicalOrdering(&mapG, order);
    drawFPP(&mapG, order);
    
    vector<edge>::const_iterator it = addedEdges.begin();
    for (; it!=addedEdges.end(); ++it)
      superGraph->delAllEdge(*it);
    
    return true;
  }
  //==========================================================================  
  bool check(string &err) {
    if (!PlanarityTest::isPlanar(superGraph) || !SimpleTest::isSimple(superGraph)) {
      err = "The graph must be planar simple";
      return false;
    }
    return true;
  }
};

LAYOUTPLUGIN(PlanarFPP, "PlanarFPP", "Bourqui Romain", "18/05/2006", "Ok", "1.0");
