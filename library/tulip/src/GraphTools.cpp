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

  void selectSpanningForest(Graph* graph, BooleanProperty *selectionProperty) {
    list<node> fifo;

    BooleanProperty *nodeFlag= graph->getLocalProperty<BooleanProperty>("selectionNodeFlag");
    
    // get previously selected nodes 
    Iterator<node> *itN=graph->getNodes();
    for (;itN->hasNext();) { 
      node itn=itN->next();
      if (selectionProperty->getNodeValue(itn)==true) {
	fifo.push_back(itn);
	nodeFlag->setNodeValue(itn,true);
      }
    } delete itN;

    selectionProperty->setAllEdgeValue(true);
    selectionProperty->setAllNodeValue(true);

    bool ok=true;
    node tmp1;
    while (ok) {
      while (!fifo.empty()) {
	tmp1=fifo.front();
	fifo.pop_front();
	Iterator<edge> *itE=graph->getOutEdges(tmp1);
	for(;itE->hasNext();) {
	  edge adjit=itE->next();
	  if (!nodeFlag->getNodeValue(graph->target(adjit)))	{
	    nodeFlag->setNodeValue(graph->target(adjit),true);	    
	    fifo.push_back(graph->target(adjit));
	  }
	  else
	    selectionProperty->setEdgeValue(adjit,false);
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
      }
    }
    graph->delLocalProperty("selectionNodeFlag");
  }

  //====================================================================
  static Graph *computeTreeFromConnectedComponents(Graph *graph, vector<node> &addedNodes,
						   Graph* rGraph) {
    // graph is not connected
    // compute the connected components's subgraph
    string err;
    DoubleProperty connectedComponent(graph);
    graph->computeProperty(string("Connected Component"), &connectedComponent, err);
    DataSet tmp;
    tmp.set("Property", &connectedComponent);
    bool result = tlp::applyAlgorithm(graph, err, &tmp, "Equal Value");
    assert(result);
  
    // create a new subgraph for the tree
    Graph *tree = rGraph->addSubGraph();
    node root = tree->addNode();
    addedNodes.push_back(root);
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
      Graph *sTree = computeTree(gConn, addedNodes, rGraph, true);
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

#define CLONE_NAME "CloneForTree"
  Graph *computeTree(Graph *graph, vector<node> &addedNodes, Graph *rGraph, bool isConnected) {
    // nothing todo if the graph is already a tree
    if (TreeTest::isTree(graph))
      return graph;
  
    // if needed, create a clone of the graph
    // as a working copy
    Graph *gClone = graph;
    if (!rGraph)
      rGraph = gClone = tlp::newCloneSubGraph(graph, CLONE_NAME);

    // if the graph is topologically a tree, make it directed
    // using a 'center' of the graph as root
    if (TreeTest::isFreeTree(gClone)) {
      TreeTest::makeDirectedTree(gClone, graphCenterHeuristic(gClone));
      return gClone;
    }

    // if the graph is connected,
    // make it acyclic,
    // extract a spanning forest,
    // add a new node as a tree root
    // and connect it to each root of the forest's trees
    if (isConnected || ConnectedTest::isConnected(gClone)) {
      vector<edge> reversed;
      vector<SelfLoops> selfLoops;
      AcyclicTest::makeAcyclic(gClone, reversed, selfLoops);
      vector<SelfLoops>::iterator itSelf;
      for (itSelf=selfLoops.begin();itSelf!=selfLoops.end();++itSelf) {
	addedNodes.push_back((*itSelf).n1);
	addedNodes.push_back((*itSelf).n2);
      }
      BooleanProperty spanningForestSelection(gClone);
      selectSpanningForest(gClone, &spanningForestSelection);
      gClone = gClone->addSubGraph(&spanningForestSelection);
      if (ConnectedTest::isConnected(gClone))
	return computeTree(gClone, addedNodes, rGraph, true);
      node root = gClone->addNode(), n;
      addedNodes.push_back(root);
      forEach(n, gClone->getNodes()) {
	if (n == root)
	  continue;
	if (gClone->indeg(n) == 0)
	  gClone->addEdge(root, n);
      }
      assert(TreeTest::isTree(gClone));
      return gClone;
    }

    return computeTreeFromConnectedComponents(gClone, addedNodes, rGraph);
  }

  void cleanComputedTree(tlp::Graph *graph, tlp::Graph *tree, vector<node> &addedNodes) {
    if (graph == tree)
      return;

    // remove all added nodes
    vector<node>::iterator it;
    for (it = addedNodes.begin(); it != addedNodes.end(); ++it)
      graph->delNode(*it);

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
