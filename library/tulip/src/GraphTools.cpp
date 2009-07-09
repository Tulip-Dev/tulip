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
#include "tulip/ExtendedClusterOperation.h"
#include "tulip/Ordering.h"
#include "tulip/hash_string.h"

#include <math.h>

using namespace std;

namespace stdext {
  template<>
  struct hash<double> {
    size_t operator()(const double s) const { return (size_t) s; }
  };
};

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
    if (edgeLength)
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
	  if (edgeLength)
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

  //======================================================================
  void selectSpanningForest(Graph* graph, BooleanProperty *selectionProperty,
			    PluginProgress *pluginProgress) {
    list<node> fifo;

    BooleanProperty *nodeFlag= graph->getLocalProperty<BooleanProperty>("selectionNodeFlag");
    
    unsigned int nbSelectedNodes = 0;
    unsigned int nbNodes = 0;
    // get previously selected nodes 
    Iterator<node> *itN=graph->getNodes();
    for (;itN->hasNext();) {
      ++ nbNodes;
      node itn=itN->next();
      if (selectionProperty->getNodeValue(itn)==true) {
	fifo.push_back(itn);
	nodeFlag->setNodeValue(itn,true);
	++nbSelectedNodes;
      }
    } delete itN;

    selectionProperty->setAllEdgeValue(true);
    selectionProperty->setAllNodeValue(true);

    bool ok=true;
    node tmp1;
    unsigned int edgeCount = 0;
    while (ok) {
      while (!fifo.empty()) {
	tmp1=fifo.front();
	fifo.pop_front();
	Iterator<edge> *itE=graph->getOutEdges(tmp1);
	for(;itE->hasNext();) {
	  edge adjit=itE->next();
	  if (!nodeFlag->getNodeValue(graph->target(adjit))) {
	    nodeFlag->setNodeValue(graph->target(adjit),true);
	    ++nbSelectedNodes;
	    fifo.push_back(graph->target(adjit));
	  }
	  else
	    selectionProperty->setEdgeValue(adjit,false);
	  if (pluginProgress) {
	    pluginProgress->setComment("Computing a spanning forest...");
	    ++edgeCount;
	    if (edgeCount == 200 ) {
	      if (pluginProgress->progress(nbSelectedNodes*100/nbNodes, 100) != TLP_CONTINUE) {
		graph->delLocalProperty("selectionNodeFlag");
		return;
	      }
	      edgeCount = 0;
	    }
	  }	  
	} delete itE;
      }
      ok=false;
      bool degZ=false;
      node goodNode;
      Iterator<node> *itN=graph->getNodes();
      for(;itN->hasNext();) {
	node itn=itN->next();
	if (!nodeFlag->getNodeValue(itn)) {
	  if (!ok) {
	    goodNode=itn;
	    ok=true;
	  }
	  if (graph->indeg(itn)==0) {
	    fifo.push_back(itn);
	    nodeFlag->setNodeValue(itn,true);
	    ++nbSelectedNodes;
	    degZ=true;
	  }
	  if (!degZ) {
	    if (graph->indeg(itn)<graph->indeg(goodNode))
	      goodNode=itn;
	    else {
	      if (graph->indeg(itn)==graph->indeg(goodNode))
		if (graph->outdeg(itn)>graph->outdeg(goodNode))
		  goodNode=itn;
	    }
	  }
	}
      } delete itN;
      if (ok && (!degZ)) {
	fifo.push_back(goodNode);
	nodeFlag->setNodeValue(goodNode,true);
	++nbSelectedNodes;
      }
    }
    graph->delLocalProperty("selectionNodeFlag");
  }

  //======================================================================
  struct ltEdge {
    DoubleProperty *m;
    ltEdge(DoubleProperty *metric) : m(metric) {}
    bool operator()(const edge &e1, const edge &e2) const
    {
      return (m->getEdgeValue(e1) < m->getEdgeValue(e2));
    } 
  };

  void selectMinimumSpanningTree(Graph* graph, BooleanProperty *selection,
				 DoubleProperty *edgeWeight,
				 PluginProgress *pluginProgress) {
    assert(ConnectedTest::isConnected(graph));
    selection->setAllNodeValue(true);
    selection->setAllEdgeValue(false);

    unsigned int numClasses = 0;
    map<int, int> classes;
  
    Iterator<node> *itN = graph->getNodes();
    while (itN->hasNext()) { 
      node n=itN->next();
      classes[n.id] = numClasses;
      ++numClasses;
    } delete itN;

    unsigned int maxCount = numClasses;
    unsigned int edgeCount = 0;
    
    std::list<edge> sortedEdges;
    Iterator<edge> *itE = graph->getEdges();
    while (itE->hasNext()) { 
      edge e=itE->next();
      sortedEdges.push_back(e);
    } delete itE;

    if (edgeWeight)
      sortedEdges.sort<ltEdge>(ltEdge(edgeWeight));
    while(numClasses > 1) {
      edge cur;
      do {
	cur = sortedEdges.front();
	sortedEdges.pop_front();
      } while(!(classes[graph->source(cur).id] !=  classes[graph->target(cur).id]));
    
      selection->setEdgeValue(cur, true);
      if (pluginProgress) {
	pluginProgress->setComment(edgeWeight ?
				   "Computing minimum spanning tree..." :
				   "Computing spanning tree...");
	++edgeCount;
	if (edgeCount == 200 ) {
	  if (pluginProgress->progress((maxCount - numClasses)*100/maxCount, 100) != TLP_CONTINUE)
	    return;
	  edgeCount = 0;
	}
      }	  

      int x = classes[graph->source(cur).id];
      int y = classes[graph->target(cur).id];

      Iterator<node> *itN = graph->getNodes();
      while (itN->hasNext()) { 
	node n=itN->next();
	if(classes[n.id] == y)
	  classes[n.id] = x;
      } delete itN;
      numClasses--;
    }
  }

  //=======================================================================
  // should be removed in 3.1
  bool computeEqualValueClustering(Graph *graph, PropertyInterface* property,
				   bool onNodes,
				   PluginProgress *pluginProgress) {
    return computeEqualValueClustering(graph, property, onNodes, false, pluginProgress);
  }

  //=======================================================================
  bool computeEqualValueClustering(Graph *graph, PropertyInterface* property,
					bool onNodes, bool connected,
					PluginProgress *pluginProgress) {
    StableIterator<node> itN(graph->getNodes());
    StableIterator<edge> itE(graph->getEdges());
    int step = 0, maxSteps = graph->numberOfNodes();

    if (maxSteps < 100)
      maxSteps = 100;
    if (pluginProgress)
      pluginProgress->setComment(onNodes ? "Partitioning nodes..." : "Partitioning edges");

    vector<Graph *> subGraphs;

    // try to work with double value if it's a DoubleProperty
    if (typeid(*property) == typeid(DoubleProperty)) {
      stdext::hash_map<double, Graph *> partitions;
      DoubleProperty *metric = (DoubleProperty *) property;
      if (onNodes) {
	while (itN.hasNext()) {
	  Graph *sg;
	  node n = itN.next();
	  const double& tmp = metric->getNodeValue(n);
	  if (partitions.find(tmp) == partitions.end()) {
	    sg = graph->addSubGraph();
	    stringstream sstr;
	    sstr << "value = " << tmp;
	    sg->setAttribute("name", sstr.str());
	    partitions[tmp]=sg;
	    subGraphs.push_back(sg);
	  } else
	    sg = partitions[tmp];
	  sg->addNode(n);
	  if ((++step % (maxSteps/100)) == 0) {
	    pluginProgress->progress(step, maxSteps);
	    if (pluginProgress->state() !=TLP_CONTINUE)
	      return pluginProgress->state()!= TLP_CANCEL;
	  }
	}

	step = 0;
	maxSteps = graph->numberOfEdges();
	if (maxSteps < 100)
	  maxSteps = 100;
	pluginProgress->setComment("Partitioning edges...");
	while(itE.hasNext()) {
	  edge ite = itE.next();
	  const double& tmp = metric->getNodeValue(graph->source(ite));
	  if (tmp == metric->getNodeValue(graph->target(ite))) {
	    partitions[tmp]->addEdge(ite);
	  }
	  if ((++step % (maxSteps/100)) == 0) {
	    pluginProgress->progress(step, maxSteps);
	    if (pluginProgress->state() !=TLP_CONTINUE)
	      return pluginProgress->state()!= TLP_CANCEL;
	  }
	}
      } else {
	while (itE.hasNext()) {
	  Graph *sg;
	  edge e = itE.next();
	  const double& tmp = metric->getEdgeValue(e);
	  if (partitions.find(tmp) == partitions.end()) {
	    sg = graph->addSubGraph();
	    stringstream sstr;
	    sstr << "value = " << tmp;
	    sg->setAttribute("name", sstr.str());
	    partitions[tmp]=sg;
	    subGraphs.push_back(sg);
	  } else
	    sg = partitions[tmp];
	  sg->addNode(graph->source(e));
	  sg->addNode(graph->target(e));
	  sg->addEdge(e);
	  if ((++step % (maxSteps/100)) == 0) {
	    pluginProgress->progress(step, maxSteps);
	    if (pluginProgress->state() !=TLP_CONTINUE)
	      return pluginProgress->state()!= TLP_CANCEL;
	  }
	}
      }
    } else {
      stdext::hash_map<string, Graph *> partitions;
      if (onNodes) {
	while (itN.hasNext()) {
	  Graph *sg;
	  node n = itN.next();
	  string tmp=property->getNodeStringValue(n);
	  if (partitions.find(tmp)==partitions.end()) {
	    sg = graph->addSubGraph();
	    sg->setAttribute("name", string("value = ") + tmp);
	    partitions[tmp]=sg;
	    subGraphs.push_back(sg);
	  } else
	    sg = partitions[tmp];
	  sg->addNode(n);
	  if ((++step % (maxSteps/100)) == 0) {
	    pluginProgress->progress(step, maxSteps);
	    if (pluginProgress->state() !=TLP_CONTINUE)
	      return pluginProgress->state()!= TLP_CANCEL;
	  }
	}

	step = 0;
	maxSteps = graph->numberOfEdges();
	if (maxSteps < 100)
	  maxSteps = 100;
	pluginProgress->setComment("Partitioning edges...");
	while(itE.hasNext()) {
	  edge ite = itE.next();
	  string tmp = property->getNodeStringValue(graph->source(ite));
	  if (tmp == property->getNodeStringValue(graph->target(ite))) {
	    partitions[tmp]->addEdge(ite);
	  }
	  if ((++step % (maxSteps/100)) == 0) {
	    pluginProgress->progress(step, maxSteps);
	    if (pluginProgress->state() !=TLP_CONTINUE)
	      return pluginProgress->state()!= TLP_CANCEL;
	  }
	}
      } else {
	while (itE.hasNext()) {
	  Graph *sg;
	  edge e = itE.next();
	  string tmp = property->getEdgeStringValue(e);
	  if (partitions.find(tmp) == partitions.end()) {
	    sg = graph->addSubGraph();
	    sg->setAttribute("name", string("value = ") + tmp);
	    partitions[tmp]=sg;
	    subGraphs.push_back(sg);
	  } else
	    sg = partitions[tmp];
	  sg->addNode(graph->source(e));
	  sg->addNode(graph->target(e));
	  sg->addEdge(e);
	  if ((++step % (maxSteps/100)) == 0) {
	    pluginProgress->progress(step, maxSteps);
	    if (pluginProgress->state() !=TLP_CONTINUE)
	      return pluginProgress->state()!= TLP_CANCEL;
	  }
	}
      }
    }
    if (connected) {
      // loop on subgraphs to only have connected components
      for (unsigned int i = 0; i < subGraphs.size(); ++i) {
	Graph* sg = subGraphs[i];
	std::vector<std::set<node> > components;
	// compute the connected components's subgraph
	ConnectedTest::computeConnectedComponents(sg, components);
	if (components.size() > 1) {
	  string name = sg->getAttribute<string>("name");
	  // remove the orginal subgraph
	  graph->delSubGraph(sg);
	  // create new subgraphs with same name
	  for (unsigned int i = 0; i < components.size(); ++i) {
	    sg = graph->inducedSubGraph(components[i]);
	    stringstream sstr;
	    sstr << name << " [" << i << ']';
	    sg->setAttribute("name", sstr.str());
	  }
	}
      }
    }
    return true;
  }
}
