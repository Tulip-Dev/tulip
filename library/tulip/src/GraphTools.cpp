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

#include <tulip/Ordering.h>



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
	pluginProgress->setComment("Computing minimim spanning tree...");
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

  //====================================================================
  Graph *computeTree(Graph *graph, Graph *rGraph, bool isConnected,
		     PluginProgress *pluginProgress) {
    // nothing todo if the graph is already a tree
    if (TreeTest::isTree(graph))
      return graph;
  
    // if needed, create a clone of the graph
    // as a working copy
    Graph *gClone = graph;
    if (!rGraph)
      // the name used for subgraph clone when computing a tree
      #define CLONE_NAME "CloneForTree"
      rGraph = gClone = tlp::newCloneSubGraph(graph, CLONE_NAME);

    // if the graph is topologically a tree, make it directed
    // using a 'center' of the graph as root
    if (TreeTest::isFreeTree(gClone)) {
      TreeTest::makeDirectedTree(gClone, graphCenterHeuristic(gClone));
      return gClone;
    }

    // if the graph is connected,
    // extract a minimum spanning tree,
    // and make it directed
    if (isConnected || ConnectedTest::isConnected(gClone)) {
      BooleanProperty treeSelection(gClone);
      selectMinimumSpanningTree(gClone, &treeSelection, 0, pluginProgress);
      if (pluginProgress && pluginProgress->state() !=TLP_CONTINUE)
	return 0;
      return computeTree(gClone->addSubGraph(&treeSelection), rGraph, true, pluginProgress);
    }

    // graph is not connected
    // compute the connected components's subgraph
    string err;
    DoubleProperty connectedComponent(graph);
    graph->computeProperty(string("Connected Component"), &connectedComponent, err, pluginProgress);
    if (pluginProgress && pluginProgress->state() !=TLP_CONTINUE)
      return 0;
    DataSet tmp;
    tmp.set("Property", &connectedComponent);
    bool result = tlp::applyAlgorithm(graph, err, &tmp, "Equal Value", pluginProgress);
    if (pluginProgress && pluginProgress->state() !=TLP_CONTINUE)
      return 0;
    assert(result);
  
    // create a new subgraph for the tree
    Graph *tree = rGraph->addSubGraph();
    node root = tree->addNode();
    Graph *gConn;

    // connected components subgraphs loop
    forEach(gConn, graph->getSubGraphs()) {
      if (tree == gConn)
	continue;
      // compute a tree for each subgraph
      // add each element of that tree
      // to our main tree
      // and connect the main root to each
      // subtree root
      Graph *sTree = computeTree(gConn, rGraph, true, pluginProgress);
      if (pluginProgress && pluginProgress->state() !=TLP_CONTINUE)
	return 0;
      node n;
      forEach(n, sTree->getNodes()) {
	tree->addNode(n);
	if (sTree->indeg(n) == 0)
	  tree->addEdge(root, n);
      }
      edge e;
      forEach(e, sTree->getEdges())
	tree->addEdge(e);
    }
    assert (TreeTest::isTree(tree));
    return tree;
  }

  void cleanComputedTree(tlp::Graph *graph, tlp::Graph *tree) {
    if (graph == tree)
      return;

    if (!ConnectedTest::isConnected(graph)) {
      // graph is not connected, so remove the tree root
      node root;
      getSource(tree, root);
      graph->delNode(root);
    }

    // then remove the subgraph clone
    Graph *sg = tree;
    string nameAtt("name");
    string name = sg->getAttribute<string>(nameAtt);
    while(name != CLONE_NAME) {
      sg = sg->getSuperGraph();
      name = sg->getAttribute<string>(nameAtt);
    }
    graph->delAllSubGraphs(sg);
  }
}
