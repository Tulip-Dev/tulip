#include "tulip/GraphTools.h"
#include "tulip/GraphMeasure.h"
#include "tulip/AcyclicTest.h"
#include "tulip/TreeTest.h"
#include "tulip/Graph.h"
#include "tulip/BooleanProperty.h"
#include "tulip/StableIterator.h"
#include "tulip/IntegerProperty.h"
#include "tulip/DoubleProperty.h"
#include "tulip/ForEach.h"
#include "tulip/ConnectedTest.h"

#include <math.h>

#include <tulip/CanonicalOrdering.h>



using namespace std;

namespace tlp {
  
  //======================================================================
  void makeProperDag(Graph* graph,list<node> &addedNodes, 
			  stdext::hash_map<edge,edge> &replacedEdges, 
			  IntegerProperty *edgeLength) {
    if (TreeTest::isTree(graph)) return;
    assert(AcyclicTest::isAcyclic(graph));
    //We compute the dag level metric on resulting sg.
    bool resultBool;
    DoubleProperty dagLevel(graph);
    resultBool = computeDagLevelMetric(graph, &dagLevel);
    assert(resultBool);
    //we now transform the dag in a proper Dag, two linked nodes of a proper dag
    //must have a difference of one of dag level metric.
    node tmp1,tmp2;
    string tmpString;
    vector<edge> sgEdges(graph->numberOfEdges());
    Iterator<edge> *itE=graph->getEdges();
    for (int i=0;itE->hasNext();++i) {
      sgEdges[i]=itE->next();
    } delete itE;
    edgeLength->setAllEdgeValue(1);
    for (vector<edge>::const_iterator itEdge=sgEdges.begin();itEdge!=sgEdges.end();++itEdge) {
      edge ite=*itEdge;
      int delta=(int)rint(dagLevel.getNodeValue(graph->target(ite))-dagLevel.getNodeValue(graph->source(ite)));
      if (delta>1) {
	tmp1=graph->addNode();
	replacedEdges[ite]=graph->addEdge(graph->source(ite),tmp1);
	addedNodes.push_back(tmp1);
	dagLevel.setNodeValue(tmp1,dagLevel.getNodeValue(graph->source(ite))+1);
	if (delta>2) {
	  tmp2=graph->addNode();
	  addedNodes.push_back(tmp2);
	  edge e=graph->addEdge(tmp1,tmp2);
	  edgeLength->setEdgeValue(e,delta-2);	  
	  dagLevel.setNodeValue(tmp2,dagLevel.getNodeValue(graph->target(ite))-1);
	  tmp1=tmp2;
	}
	graph->addEdge(tmp1,graph->target(ite));
      }
    }
    for (stdext::hash_map<edge,edge>::const_iterator it=replacedEdges.begin();it!=replacedEdges.end();++it)
      graph->delEdge((*it).first);
    assert(AcyclicTest::isAcyclic(graph));
  }
  //======================================================================
  node makeSimpleSource(Graph* graph) {
    assert(AcyclicTest::isAcyclic(graph));
    node startNode=graph->addNode();
    Iterator<node> *itN=graph->getNodes();
    while (itN->hasNext()) {
      node itn=itN->next();
      if ((graph->indeg(itn)==0) && (itn!=startNode)) {
	graph->addEdge(startNode,itn);
      }
    }delete itN;
    assert(AcyclicTest::isAcyclic(graph));
    return startNode;
  }
  //======================================================================
  vector<vector<node> > computeCanonicalOrdering(PlanarConMap *carte, std::vector<edge>  *dummyEdges,PluginProgress* pluginProgress){
    Ordering o(carte, pluginProgress, 0, 100, 100); // feedback (0% -> 100%)
    if (dummyEdges!=0) 
      *dummyEdges = o.getDummyEdges();
    vector<vector<node> > res;
    int nbMax = o.size()-1;
    for (int i=nbMax;i>=0;i--) {
      res.push_back(o[i]);
    }
    return res;
  }
  //======================================================================
  std::vector<node> computeGraphCenters(Graph * graph) {
    assert(ConnectedTest::isConnected(graph));
    MutableContainer<unsigned int> _dist;
    node n;
    unsigned minD = UINT_MAX;
    forEach(n, graph->getNodes()) {
      MutableContainer<unsigned int> tmp;
      unsigned int maxD = maxDistance(graph, n, tmp, 2);
      _dist.set(n.id, maxD);
      minD = std::min(minD, maxD);
    }
    
    vector<node> result;
    forEach(n, graph->getNodes()) {
      if (_dist.get(n.id) == minD)
	result.push_back(n);
    }
    
    return result;
  }
  //======================================================================
  node graphCenterHeuristic(Graph * graph) {
    assert(ConnectedTest::isConnected(graph));
    node result;
    unsigned int cDist = UINT_MAX - 2;
    MutableContainer<bool> toTreat;
    toTreat.setAll(true);
    MutableContainer<unsigned int> dist;
    unsigned int i = 0;
    node n = graph->getOneNode();
    int nbTry = graph->numberOfNodes();
    bool stop = false;
    while (nbTry>0 && !stop) {
      --nbTry;
      if (toTreat.get(n.id)) {
	++i;
	unsigned int di = tlp::maxDistance(graph, n, dist);
	toTreat.set(n.id, false);
	if (di < cDist) {
	  result = n;
	  cDist = di;
	}
	else {
	  unsigned int delta = di - cDist;
	  node v;
	  forEach(v, graph->getNodes()) {
	    if (dist.get(v.id) < delta) { //all the nodes at distance less than delta can't be center
	      toTreat.set(v.id, false);
	    }
	  }
	}
	unsigned int nextMax = 0;
	node v;
	forEach(v, graph->getNodes()) {
	  if (dist.get(v.id) > (di/2 + di%2) ) 
	    toTreat.set(v.id, false);
	  else {
	    if (toTreat.get(v.id)) {
	      if (dist.get(v.id) > nextMax) {
		n = v;
		nextMax = dist.get(v.id);
	      }
	    }
	  }
	}
	if (nextMax == 0) stop = true;
      }
    }
    return result;
  }
}
