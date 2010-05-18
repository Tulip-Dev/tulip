#include <set>
#include <climits>
#include <sstream>
#include <iomanip>
#include <tulip/Graph.h>
#include <tulip/Node.h>
#include <tulip/GraphAbstract.h>
#include <tulip/GraphProperty.h>
#include <tulip/StableIterator.h>
#include <tulip/ExtendedClusterOperation.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/ColorProperty.h>
#include <tulip/StringProperty.h>
#include <tulip/DrawingTools.h>
#include <tulip/ForEach.h>

using namespace std;
using namespace tlp;
using namespace stdext;

static const string layoutProperty = "viewLayout";
static const string sizeProperty = "viewSize";
static const string rotationProperty = "viewRotation";
static const string colorProperty = "viewColor";

static void buildMapping(Iterator<node> *it, MutableContainer<node> &mapping, GraphProperty * metaInfo, const node from = node()) {
  while(it->hasNext()) {
    node n = it->next();
    if (!from.isValid())
      mapping.set(n.id, n);
    else
      mapping.set(n.id, from);
    Graph *meta = metaInfo->getNodeValue(n);
    if ( meta != 0)
      buildMapping(meta->getNodes(), mapping, metaInfo, mapping.get(n.id));
  } delete it;
}
//====================================================================================
void updatePropertiesUngroup(Graph *graph, node metanode, 
			     GraphProperty *clusterInfo) {
  if (clusterInfo->getNodeValue(metanode)==0) return; //The metanode is not a metanode.
  LayoutProperty *graphLayout = graph->getProperty<LayoutProperty>(layoutProperty);
  SizeProperty *graphSize = graph->getProperty<SizeProperty>(sizeProperty);
  DoubleProperty *graphRot = graph->getProperty<DoubleProperty>(rotationProperty);
  const Size& size = graphSize->getNodeValue(metanode);
  const Coord& pos = graphLayout->getNodeValue(metanode);
  const double& rot = graphRot->getNodeValue(metanode);
  Graph  *cluster = clusterInfo->getNodeValue(metanode);
  LayoutProperty *clusterLayout = cluster->getProperty<LayoutProperty>(layoutProperty);
  SizeProperty  *clusterSize   = cluster->getProperty<SizeProperty>(sizeProperty);
  DoubleProperty *clusterRot = cluster->getProperty<DoubleProperty>(rotationProperty);
  pair<Coord, Coord> box = tlp::computeBoundingBox(cluster, clusterLayout, clusterSize, clusterRot);
  Coord maxL = box.first;
  Coord minL = box.second;
  double width  = maxL[0] - minL[0];
  double height = maxL[1] - minL[1];
  double depth =  maxL[2] - minL[2];
  if (width<0.0001) width=1.0;
  if (height<0.0001) height=1.0;
  if (depth<0.0001) depth=1.0;
  Coord center = (maxL + minL) / -2.0f;
  clusterLayout->translate(center, cluster);
  clusterLayout->rotateZ(graphRot->getNodeValue(metanode), cluster);
  clusterLayout->scale(Coord(size[0]/width, size[1]/height, size[2]/depth), cluster);
  clusterLayout->translate(pos, cluster);
  clusterSize->scale(Size(size[0]/width, size[1]/height, size[2]/depth), cluster);
  
  Iterator<node> *itN = cluster->getNodes();
  while(itN->hasNext()) { 
    node itn = itN->next();
    graphLayout->setNodeValue(itn, clusterLayout->getNodeValue(itn));
    graphSize->setNodeValue(itn, clusterSize->getNodeValue(itn));
    graphRot->setNodeValue(itn, clusterRot->getNodeValue(itn) + rot);
  } delete itN;
 Iterator<edge> *itE= cluster->getEdges();
  while (itE->hasNext()){
    edge ite = itE->next();
    graphLayout->setEdgeValue(ite, clusterLayout->getEdgeValue(ite));
    graphSize->setEdgeValue(ite, clusterSize->getEdgeValue(ite));
  } delete itE;
  // propagate all cluster local properties
  PropertyInterface* property;
  forEach(property, cluster->getLocalObjectProperties()) {
    if (property == graphLayout ||
	property == graphSize ||
	property == graphRot)
      continue;
    PropertyInterface *graphProp = graph->getProperty(property->getName());
    itN = cluster->getNodes();
    while(itN->hasNext()) {
      node itn = itN->next();
      graphProp->setNodeStringValue(itn, property->getNodeStringValue(itn));
    } delete itN;
    itE= cluster->getEdges();
    while (itE->hasNext()){
      edge ite = itE->next();
      graphProp->setEdgeStringValue(ite, property->getEdgeStringValue(ite));
    } delete itE;
  }
}
//====================================================================================
/*
void tlp::openMetaNode(Graph *graph, node metaNode,
		       Graph *groupUnderSubGraph, GraphProperty *metaInfo) {
  if (graph->getRoot()==graph) {
    cerr << __PRETTY_FUNCTION__ << endl;
    cerr << "\t Error: Could not ungroup a meta node in the root graph" << endl;
    return;
  }
  Graph *metaGraph = metaInfo->getNodeValue(metaNode);
  if (metaGraph == 0) return;

  Observable::holdObservers();
  //add node from meta to graph
  {
    node n;
    stableForEach(n, metaGraph->getNodes()) //stable in case of fractal graph
      graph->addNode(n);
    edge e;
    stableForEach(e, metaGraph->getEdges())
      graph->addEdge(e);
  }
  updatePropertiesUngroup(graph, metaNode, metaInfo);

  Graph* super = graph->getSuperGraph();
  Iterator<edge>* metaEdges = super->getInOutEdges(metaNode);
  if (!metaEdges->hasNext()) {
    delete metaEdges;
    if (groupUnderSubGraph != NULL)
      groupUnderSubGraph->delSubGraph(metaGraph);
    Observable::unholdObservers();
    return;
  }
  bool hasSubEdges = super->isMetaEdge(metaEdges->next());
  delete metaEdges;
  metaEdges = super->getInOutEdges(metaNode);
  ColorProperty *graphColors = 
    graph->getProperty<ColorProperty>(colorProperty);
  if (hasSubEdges) {
    while (metaEdges->hasNext()) {
      edge metaEdge = metaEdges->next();
      Color metaColor = graphColors->getEdgeValue(metaEdge);
      Iterator<edge>* subEdges = graph->getEdgeMetaInfo(metaEdge);
      while(subEdges->hasNext()) {
	edge e = subEdges->next();
	graph->addEdge(e);
	if (!graph->isElement(metaEdge))
	  graph->delEdge(e);
	graphColors->setEdgeValue(e, metaColor);
      } delete subEdges;
    }
    graph->getRoot()->delAllNode(metaNode);
  } else {
    MutableContainer<node> mappingC;
    MutableContainer<node> mappingN;
    mappingC.setAll(node());
    mappingN.setAll(node());
    Graph *root = graph->getRoot();
    buildMapping(root->getInOutNodes(metaNode), mappingC, metaInfo, node() );
    buildMapping(metaGraph->getNodes() , mappingN, metaInfo, node() );

    TLP_HASH_MAP<node, Color> metaEdgeToColor;
    while (metaEdges->hasNext()) {
      edge metaEdge = metaEdges->next();
      metaEdgeToColor[graph->opposite (metaEdge, metaNode)] =
	graphColors->getEdgeValue(metaEdge);
    }
    //Remove the metagraph from the hierarchy and remove the metanode
    root->delAllNode(metaNode);
    TLP_HASH_MAP<node, hash_set<node> > edges;
    //=================================
    //StableIterator<node> metaGraphNodes (metaGraph->getNodes());
    //while (metaGraphNodes.hasNext()) {
    StableIterator<edge> it(root->getEdges());
    while(it.hasNext()) {
      edge e = it.next();
      if (graph->isElement(e)) continue;
      node sourceC = mappingC.get(root->source(e).id);
      node targetN = mappingN.get(root->target(e).id);
      node sourceN = mappingN.get(root->source(e).id);
      node targetC = mappingC.get(root->target(e).id);
      node source, target;
      Color edgeColor;
      if (sourceC.isValid() && targetN.isValid()) {
	source = sourceC;
	target = targetN;
	edgeColor = metaEdgeToColor[source];
      } 
      else {
	if (sourceN.isValid() && targetC.isValid()) {
	  source = sourceN;
	  target = targetC;
	  edgeColor = metaEdgeToColor[target];
	} 
	else continue;
      }
      if (metaInfo->getNodeValue(source) == 0 && 
	  metaInfo->getNodeValue(target) == 0) {
	graph->addEdge(e);
	continue;
      }
      if ( (edges.find(source) == edges.end()) || 
	   (edges[source].find(target) == edges[source].end()) ) {
	edges[source].insert(target);
	if (!graph->existEdge(source,target).isValid()) {
	  edge addedEdge = graph->addEdge(source,target);
	  graphColors->setEdgeValue (addedEdge, edgeColor);
	}
	else 
	  cerr << "bug exist edge 1";
      }
      // }
    }
  }
  delete metaEdges;
  if (groupUnderSubGraph != NULL) groupUnderSubGraph->delSubGraph(metaGraph);
  Observable::unholdObservers();
}
*/
//=========================================================
Graph * Graph::inducedSubGraph(const std::set<node> &nodes) {
  Graph *result = addSubGraph();
  set<node>::const_iterator itNodeSet = nodes.begin();
  for(;itNodeSet!=nodes.end(); ++itNodeSet) {
    result->addNode(*itNodeSet);
  }
  Iterator<node> *itN=result->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    Iterator<edge> *itE=getOutEdges(itn);
    while (itE->hasNext()) {
      edge ite = itE->next();
      if (result->isElement(target(ite)))
	result->addEdge(ite);
    } delete itE;
  } delete itN;
  return result;
}
//====================================================================================
node Graph::createMetaNode (const std::set<node> &nodeSet, bool multiEdges, bool delAllEdge) {
  if (getRoot() == this) {
    cerr << __PRETTY_FUNCTION__ << endl;
    cerr << "\t Error: Could not group a set of nodes in the root graph" << endl;
    return node();
  }
  if (nodeSet.empty()) {
    cerr << __PRETTY_FUNCTION__ << endl;
    cerr << '\t' << "Warning: Creation of an empty metagraph" << endl;
  }

  Graph *subGraph = getSuperGraph()->inducedSubGraph(nodeSet);
  // all local properties
  // must be cloned in subgraph
  PropertyInterface *prop;
  forEach(prop, getLocalObjectProperties()) {
    PropertyInterface* sgProp =
      prop->clonePrototype(subGraph, prop->getName());
    set<node>::const_iterator itNodeSet = nodeSet.begin();
    for(;itNodeSet!=nodeSet.end(); ++itNodeSet) {
      node n = *itNodeSet;
      DataMem* val =  prop->getNodeDataMemValue(n);
      sgProp->setNodeDataMemValue(n, val);
      delete val;
    }
  }
  
  stringstream st;
  st << "grp_" << setfill('0') << setw(5) << subGraph->getId(); 
  subGraph->setAttribute("name", st.str());
  return createMetaNode(subGraph, multiEdges, delAllEdge);
}

//====================================================================================
node Graph::createMetaNode(Graph *subGraph, bool multiEdges, bool edgeDelAll) {
  if (getRoot() == this) {
    cerr << __PRETTY_FUNCTION__ << endl;
    cerr << "\t Error: Could not create a meta node in the root graph" << endl;
    return node();
  }
  GraphProperty* metaInfo =
    ((GraphAbstract *)getRoot())->getMetaGraphProperty();
  node metaNode = addNode();
  metaInfo->setNodeValue(metaNode, subGraph);
  Observable::holdObservers();
  //updateGroupLayout(this, subGraph, metaNode);
  // compute meta node values
  PropertyInterface *property;
  forEach(property, getObjectProperties()) {
    property->computeMetaValue(metaNode, subGraph, this);
  }

  
  /*ColorProperty *colors = getProperty<ColorProperty>(colorProperty);
    colors->setNodeValue(metaNode, Color(255, 255, 255, 127));*/

  // keep track of graph existing edges
  MutableContainer<bool> graphEdges;
  graphEdges.setAll(false);
  Iterator<edge>* itE = getEdges();
  while(itE->hasNext())
    graphEdges.set(itE->next().id, true);
  delete itE;

  //we can now Remove nodes from graph
  StableIterator<node> itN(subGraph->getNodes());
  while (itN.hasNext())
    delNode(itN.next());

  //create new meta edges from nodes to metanode
  Graph* super = getSuperGraph();
  //colors = super->getProperty<ColorProperty> (colorProperty);
  TLP_HASH_MAP<node, TLP_HASH_SET<node> > edges;
  TLP_HASH_MAP<node, edge> metaEdges;
  TLP_HASH_MAP<edge, set<edge> > subEdges;
  Iterator<node> *subGraphNodes = subGraph->getNodes();
  while (subGraphNodes->hasNext()) {
    node n = subGraphNodes->next();
    StableIterator<edge> it(getSuperGraph()->getInOutEdges(n));
    while (it.hasNext()) {
      edge e = it.next();
      pair<node, node> eEnds  = ends(e);
      node src = eEnds.first;
      node tgt = eEnds.second;
      bool toDelete =
	((metaInfo->getNodeValue(src)!=0) || 
	 (metaInfo->getNodeValue(tgt)!=0)) &&
	isElement (src) && isElement (tgt) &&
	existEdge (src, tgt).isValid();

      if (isElement(src) && subGraph->isElement(tgt)) {
	if ( (edges.find(src) == edges.end()) || (edges[src].find(tgt) == edges[src].end()) ) {
	  if (multiEdges || edges[src].empty()) {
	    // add new meta edge
	    edge metaEdge = addEdge(src, metaNode);
	    if (!graphEdges.get(e.id))
	      delEdge(metaEdge);
	    // e is a sub-edge of metaEdge
	    subEdges[metaEdge].insert(e);
	    if (!multiEdges)
	      // record metaEdge
	      metaEdges[src] = metaEdge;
	    if (!super->isElement(metaEdge))
	      super->addEdge(metaEdge);
	    //colors->setEdgeValue (metaEdge, colors->getEdgeValue(e));
	  } else if (!multiEdges)
	    // e is a sub-edge of an already created meta edge
	    subEdges[metaEdges[src]].insert(e);
	  edges[src].insert(tgt);
	} 
	if (toDelete) {
	  //	cerr << "delete edge e :" << e.id << endl;
	  if (edgeDelAll)
	    delAllEdge(e);
	  else delEdge(e);
	}
      }
      if (isElement(tgt) && subGraph->isElement(src)) {
	if ( (edges.find(tgt) == edges.end()) || (edges[tgt].find(src) == edges[tgt].end()) ) {
	  if (multiEdges || edges[tgt].empty()) {
	    // add new meta edge
	    edge metaEdge = addEdge(metaNode, tgt);
	    if (!graphEdges.get(e.id))
	      delEdge(metaEdge);
	    // e is a sub-edge of metaEdge
	    subEdges[metaEdge].insert(e);
	    if (!multiEdges)
	      // record metaEdge
	      metaEdges[src] = metaEdge;
	    if (!super->isElement(metaEdge))
	      super->addEdge(metaEdge);
	    //colors->setEdgeValue(metaEdge, colors->getEdgeValue (e));
	  } else if (!multiEdges)
	    // e is a sub-edge of an already created meta edge
	    subEdges[metaEdges[tgt]].insert(e);
	  edges[tgt].insert(src);
	}
	if (toDelete) {
	  //	cerr << "delete edge e :" << e.id << endl;
	  if (edgeDelAll)
	    delAllEdge(e);
	  else delEdge (e);
	}
      }
    }
  } delete subGraphNodes;
  // update metaInfo of new meta edges
  TLP_HASH_MAP<edge, set<edge> >::const_iterator it;
  for(it = subEdges.begin(); it != subEdges.end(); ++it) {
    edge mE = (*it).first;
    metaInfo->setEdgeValue(mE, (*it).second);
    // compute meta edge values
    forEach(property, getObjectProperties()) {
      Iterator<edge> *itE = getEdgeMetaInfo(mE);
      assert(itE->hasNext());
      property->computeMetaValue(mE, itE, this);
      delete itE;
    }
  }

  Observable::unholdObservers();
  return metaNode;
}
//====================================================================================
void Graph::openMetaNode(node metaNode) {
  if (getRoot() == this) {
    cerr << __PRETTY_FUNCTION__ << endl;
    cerr << "\t Error: Could not ungroup a meta node in the root graph" << endl;
    return;
  }
  GraphProperty* metaInfo =
    ((GraphAbstract *) getRoot())->getMetaGraphProperty();
  Graph *metaGraph = metaInfo->getNodeValue(metaNode);
  if (metaGraph == 0) return;

  Observable::holdObservers();
  MutableContainer<node> mappingM;
  //add node from meta to graph  
  {
    node n;
    //stable in case of fractal graph 
    stableForEach(n, metaGraph->getNodes()) {
      addNode(n);
      mappingM.set(n.id, n);
    }
    edge e;
    stableForEach(e, metaGraph->getEdges())
      addEdge(e);
  }
  updatePropertiesUngroup(this, metaNode, metaInfo);

  // check for edges from or to the meta node
  Graph* super = getSuperGraph();
  Iterator<edge>* metaEdges = super->getInOutEdges(metaNode);
  if (!metaEdges->hasNext()) {
    delete metaEdges;
    // no edge so just remove the meta node
    getRoot()->delAllNode(metaNode);
    Observable::unholdObservers();
    return;
  }
  bool hasSubEdges = super->isMetaEdge(metaEdges->next());
  delete metaEdges;
  metaEdges = super->getInOutEdges(metaNode);
  ColorProperty *graphColors = 
    getProperty<ColorProperty>(colorProperty);
  if (hasSubEdges) {
    node mn;
    // compute mapping for neighbour nodes
    // and their sub nodes
    forEach(mn, super->getInOutNodes(metaNode)) {
      if (isElement(mn))
	mappingM.set(mn.id, mn);
      Graph *mnGraph = metaInfo->getNodeValue(mn);
      if (mnGraph != 0) {
	Iterator<node> *it = mnGraph->getNodes();
	while(it->hasNext()) {
	  mappingM.set(it->next().id, mn);
	}
      }
    }
    while (metaEdges->hasNext()) {
      edge metaEdge = metaEdges->next();
      Color metaColor = graphColors->getEdgeValue(metaEdge);
      Iterator<edge>* subEdges = getEdgeMetaInfo(metaEdge);
      TLP_HASH_MAP<node, TLP_HASH_MAP<node, set<edge> > > newMetaEdges;
      while(subEdges->hasNext()) {
	edge e = subEdges->next();
	const std::pair<node, node>& eEnds = super->ends(e);
	if (isElement(eEnds.first)) {
	  if (isElement(eEnds.second)) {
	    addEdge(e);
	    if (!isElement(metaEdge))
	      delEdge(e);
	    graphColors->setEdgeValue(e, metaColor);
	  } else {
	    newMetaEdges[eEnds.first][mappingM.get(eEnds.second.id)].insert(e);
	  }
	} else {
	  newMetaEdges[mappingM.get(eEnds.first.id)][eEnds.second].insert(e);
	}
      } delete subEdges;
      // iterate on newMetaEdges
      TLP_HASH_MAP<node, TLP_HASH_MAP<node, set<edge> > >::iterator itme =
	newMetaEdges.begin();
      while(itme != newMetaEdges.end()) {
	node src = (*itme).first;
	TLP_HASH_MAP<node, set<edge> >::iterator itnme = (*itme).second.begin();
	TLP_HASH_MAP<node, set<edge> >::iterator itnmeEnd = (*itme).second.end();
	while(itnme != itnmeEnd) {
	  edge me = addEdge(src, (*itnme).first);
	  metaInfo->setEdgeValue(me, (*itnme).second);
	  graphColors->setEdgeValue(me, metaColor);
	  ++itnme;
	}
	++itme;
      }
    }
    getRoot()->delAllNode(metaNode);
  } else {
    MutableContainer<node> mappingC;
    MutableContainer<node> mappingN;
    mappingC.setAll(node());
    mappingN.setAll(node());
    Graph *root = getRoot();
    buildMapping(root->getInOutNodes(metaNode), mappingC, metaInfo, node() );
    buildMapping(metaGraph->getNodes() , mappingN, metaInfo, node() );

    TLP_HASH_MAP<node, Color> metaEdgeToColor;
    while (metaEdges->hasNext()) {
      edge metaEdge = metaEdges->next();
      metaEdgeToColor[opposite (metaEdge, metaNode)] =
	graphColors->getEdgeValue(metaEdge);
    }
    //Remove the metagraph from the hierarchy and remove the metanode
    root->delAllNode(metaNode);
    TLP_HASH_MAP<node, TLP_HASH_SET<node> > edges;
    //=================================
    //StableIterator<node> metaGraphNodes (metaGraph->getNodes());
    //while (metaGraphNodes.hasNext()) {
    StableIterator<edge> it(root->getEdges());
    while(it.hasNext()) {
      edge e = it.next();
      if (isElement(e)) continue;
      pair<node, node> eEnds = root->ends(e);
      unsigned int srcId = eEnds.first.id;
      unsigned int tgtId = eEnds.second.id;
      node sourceC = mappingC.get(srcId);
      node targetN = mappingN.get(tgtId);
      node sourceN = mappingN.get(srcId);
      node targetC = mappingC.get(tgtId);
      node src, tgt;
      Color edgeColor;
      if (sourceC.isValid() && targetN.isValid()) {
	src = sourceC;
	tgt = targetN;
	edgeColor = metaEdgeToColor[src];
      } 
      else {
	if (sourceN.isValid() && targetC.isValid()) {
	  src = sourceN;
	  tgt = targetC;
	  edgeColor = metaEdgeToColor[tgt];
	} 
	else continue;
      }
      if (metaInfo->getNodeValue(src) == 0 && 
	  metaInfo->getNodeValue(tgt) == 0) {
	addEdge(e);
	continue;
      }
      if ( (edges.find(src) == edges.end()) || 
	   (edges[src].find(tgt) == edges[src].end()) ) {
	edges[src].insert(tgt);
	if (!existEdge(src,tgt).isValid()) {
	  edge addedEdge = addEdge(src,tgt);
	  graphColors->setEdgeValue (addedEdge, edgeColor);
	}
	else 
	  cerr << "bug exist edge 1";
      }
      // }
    }
  }
  delete metaEdges;
  Observable::unholdObservers();
}
//====================================================================================
struct MetaEdge {
  unsigned int source,target;
  edge mE;
};

namespace std {
  template<>
  struct less<MetaEdge> {
    bool operator()(const MetaEdge &c,const MetaEdge &d) const {
      /*if (c.source<d.source) return true;
      if (c.source>d.source) return false;
      if (c.target<d.target) return true;
      if (c.target>d.target) return false;
      return false;*/
      return (c.source > d.source) ||
	((c.source == d.source) && (c.target < d.target));
    }
  };
};

void Graph::createMetaNodes(Iterator<Graph *> *itS, Graph *quotientGraph,
			    vector<node>& metaNodes) {
  GraphProperty *metaInfo =
    ((GraphAbstract*)getRoot())->getMetaGraphProperty();
  map <edge, set<edge> > eMapping;
  Observable::holdObservers();
  {
    map<node, set<node> > nMapping;
    while (itS->hasNext()) {
      Graph *its=itS->next();
      if (its!=quotientGraph) {
	// Create one metanode for each subgraph(cluster)
	node metaN = quotientGraph->addNode();
	metaNodes.push_back(metaN);
	metaInfo->setNodeValue(metaN, its);
	// compute meta node values
	string pName;
	forEach(pName, quotientGraph->getProperties()) {
	  PropertyInterface *property = quotientGraph->getProperty(pName);
	  property->computeMetaValue(metaN, its, quotientGraph);
	}
	node n;
	forEach(n, its->getNodes()) {
	  // map each subgraph's node to a set of meta nodes
	  // in order to deal consistently with overlapping clusters
	  if (nMapping.find(n) == nMapping.end())
	    nMapping[n] = set<node>();
	  nMapping[n].insert(metaN);
	}
      }
    }

    {
      set<MetaEdge> myQuotientGraph;
      edge e;
      // for each existing edge in the current graph
      // add a meta edge for the corresponding couple
      // (meta source, meta target) if it does not already exists
      // and register the edge as associated to this meta edge
      stableForEach(e, getEdges()) {
	pair<node, node> eEnds = ends(e);
	set<node>& metaSources = nMapping[eEnds.first];
	set<node>& metaTargets = nMapping[eEnds.second];
	for(set<node>::const_iterator itms = metaSources.begin();
	    itms != metaSources.end(); ++itms) {
	  node mSource = *itms;
	  for(set<node>::const_iterator itmt = metaTargets.begin();
	      itmt != metaTargets.end(); ++itmt) {
	    node mTarget = *itmt;
	    if (mSource != mTarget) {
	      MetaEdge tmp;
	      tmp.source = mSource.id, tmp.target = mTarget.id;
	      set<MetaEdge>::const_iterator itm = myQuotientGraph.find(tmp);
	      if (itm == myQuotientGraph.end()) {
		edge mE = quotientGraph->addEdge(mSource, mTarget);
		tmp.mE = mE;
		myQuotientGraph.insert(tmp);
		eMapping[mE].insert(e);
	      } else {
		// add edge
		eMapping[(*itm).mE].insert(e);
	      }
	    }
	  }
	}
      }
    }
  }
  // set viewMetaGraph for added meta edges
  map<edge, set<edge> >::const_iterator itm = eMapping.begin();
  while(itm != eMapping.end()) {
    edge mE = (*itm).first;
    metaInfo->setEdgeValue(mE, (*itm).second);
    // compute meta edge values
    string pName;
    forEach(pName, quotientGraph->getProperties()) {
      Iterator<edge> *itE = getRoot()->getEdgeMetaInfo(mE);
      PropertyInterface *property = quotientGraph->getProperty(pName);
      property->computeMetaValue(mE, itE, quotientGraph);
      delete itE;
    }
    ++itm;
  }
  //compute layout according to the layouts of subgraphs
  /*vector<node>::iterator itn = metaNodes.begin();
  while(itn != metaNodes.end()) {
    updateGroupLayout(this, quotientGraph, *itn);
    itn++;
    }*/

  Observable::unholdObservers();
}
