#include <set>
#include <climits>
#include <sstream>
#include <iomanip>
#include <ext/hash_set>
#include <tulip/Graph.h>
#include <tulip/Node.h>
#include <tulip/GraphProperty.h>
#include <tulip/StableIterator.h>
#include <tulip/ExtendedClusterOperation.h>
#include <tulip/LayoutProperty.h>
#include <tulip/SizeProperty.h>
#include <tulip/DoubleProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/DrawingTools.h>

using namespace std;
using namespace tlp;
using namespace stdext;

const string metagraphProperty = "viewMetaGraph";
const string layoutProperty = "viewLayout";
const string sizeProperty = "viewSize";
const string rotationProperty = "viewRotation";


/*
  resolve overlap,
  taille de noeud n,
  pour tous les sommets du graphe autre que n, 
  calculer la boite englobante la plus grande possible centré sur le barycentre 
  des sommets de a.
*/
/*
  void shrink(Graph *graph, node n) {
  Iterator<node> *itN = graph->getNodes();
  LayoutProperty *layout = graph->getProperty<LayoutProperty>("viewLayout");
  SizeProperty *size = graph->getProperty<SizeProperty>("viewSize");
  Coord result(DBL_MAX,DBL_MAX,DBL_MAX);
  Coord nPos  = layout->getNodeValue(n);
  Size  nSize = size->getNodeValue(n)/2.0;
  float scale = 1.0;
  while(itN->hasNext()) {
    node it = itN->next();
    if (n == it) continue;
    Size itSize = size->getNodeValue(it)/2.0;
    Coord itPos = layout->getNodeValue(it);
    float dx = fabs(itPos[0]-nPos[0]) - itSize[0] - nSize[0];
    float dy = fabs(itPos[1]-nPos[1]) - itSize[1] - nSize[1];
    //    float dz = fabs(itPos[2]-nPos[2]) - itSize[2] - nSize[2];
    // GCC 4.2: <?= is no longer supported
    // if (dx<0) scale <?= (fabs(itPos[0]-nPos[0]) - itSize[0]) / nSize[0];
    if (dy<0) scale <?= (fabs(itPos[1]-nPos[1]) - itSize[1]) / nSize[1];
    if (dx<0) scale = min(scale, (fabs(itPos[0]-nPos[0]) - itSize[0]) / nSize[0]);
    if (dy<0) scale = std::min(scale, (fabs(itPos[1]-nPos[1]) - itSize[1]) / nSize[1]);
    //    if (dz<0) scale <?= (fabs(itPos[1]-nPos[1]) - itSize[1]) / nSize[1];
  } delete itN;
  if (scale<1.0) {
    nSize *= 2.0*scale;
    size->setNodeValue(n, nSize);
  }
  }
*/

//====================================================================================
void buildMapping(Iterator<node> *it, MutableContainer<node> &mapping, GraphProperty * metaInfo, const node from = node()) {
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
void updateGroupLayout(Graph *graph, Graph *cluster, node metanode) {
  LayoutProperty *graphLayout = graph->getProperty<LayoutProperty>(layoutProperty);
  SizeProperty *graphSize = graph->getProperty<SizeProperty>(sizeProperty);
  DoubleProperty *graphRotation = graph->getProperty<DoubleProperty>(rotationProperty);
  pair<Coord, Coord> box = tlp::computeBoundingBox(cluster, graphLayout, graphSize, graphRotation);
  Coord maxL = box.first;
  Coord minL = box.second;
  graphLayout->setNodeValue(metanode, (maxL + minL) / 2.0 );
  Coord v = (maxL - minL);
  if (v[2] < 0.0001) v[2] = 0.1;
  graphSize->setNodeValue(metanode, Size(v[0],v[1],v[2]));
  LayoutProperty *clusterLayout = cluster->getProperty<LayoutProperty>(layoutProperty);
  SizeProperty  *clusterSize   = cluster->getProperty<SizeProperty>(sizeProperty);
  Iterator<node> *itN= cluster->getNodes();
  while (itN->hasNext()){
    node itn = itN->next();
    clusterLayout->setNodeValue(itn, graphLayout->getNodeValue(itn));
    clusterSize->setNodeValue(itn, graphSize->getNodeValue(itn));
  } delete itN;
  Iterator<edge> *itE= cluster->getEdges();
  while (itE->hasNext()){
    edge ite = itE->next();
    clusterLayout->setEdgeValue(ite, graphLayout->getEdgeValue(ite));
    clusterSize->setEdgeValue(ite, graphSize->getEdgeValue(ite));
  } delete itE;
  //  shrink(graph, metanode);
}
//====================================================================================
node tlp::createMetaNode(Graph *graph, set<node> &subGraph) {
  if (graph->getRoot()==graph) {
    cerr << __PRETTY_FUNCTION__ << endl;
    cerr << "\t Error: Could not group a set of nodes in the root graph" << endl;
    return node();
  }
  Observable::holdObservers();
  if (subGraph.empty()) {
    cerr << __PRETTY_FUNCTION__ << endl;
    cerr << '\t' << "Warning: Creation of an empty metagraph" << endl;
  }
  GraphProperty *metaInfo = graph->getProperty<GraphProperty>(metagraphProperty);
  Graph *metaGraph = tlp::inducedSubGraph(graph->getFather(), subGraph, "cluster");
  stringstream st;
  st << "grp_" << setfill('0') << setw(5) << metaGraph->getId(); 
  metaGraph->setAttribute("name", st.str()); 
  node metaNode = graph->addNode();
  metaInfo->setNodeValue(metaNode, metaGraph);

  updateGroupLayout(graph, metaGraph, metaNode);

  //remove nodes from graph
  StableIterator<node> itN(metaGraph->getNodes());
  while (itN.hasNext())
    graph->delNode(itN.next());

  //create new edges from nodes to metanodes
  Graph *root = graph->getRoot();
  hash_map<node, hash_set<node> > edges;
  StableIterator<edge> it(root->getEdges());
  while (it.hasNext()) {
    edge e = it.next();
    node source = graph->source(e);
    node target = graph->target(e);
    bool toDelete = (metaInfo->getNodeValue(source)!=0) || (metaInfo->getNodeValue(target)!=0);
    if (graph->isElement(source) && metaGraph->isElement(target)) {
      if ( (edges.find(source) == edges.end()) || (edges[source].find(target) == edges[source].end()) ) {
	edges[source].insert(target);
	graph->addEdge(source,metaNode);
      } 
      if (toDelete) {
	//	cerr << "delete edge e :" << e.id << endl;
	graph->delAllEdge(e);
      }
    }
    if (graph->isElement(target) && metaGraph->isElement(source)) {
      if ( (edges.find(target) == edges.end()) || (edges[target].find(source) == edges[target].end()) ) {
	edges[target].insert(source);
	graph->addEdge(metaNode, target);
      }
      if (toDelete) {
	//	cerr << "delete edge e :" << e.id << endl;
	graph->delAllEdge(e);
      }
    }
  }
  Observable::unholdObservers();
  return metaNode;
}
//====================================================================================
void updateLayoutUngroup(Graph *graph, node metanode) {
  GraphProperty *clusterInfo = graph->getProperty<GraphProperty>(metagraphProperty);
  if (clusterInfo->getNodeValue(metanode)==0) return; //The metanode is not a metanode.
  LayoutProperty *graphLayout = graph->getProperty<LayoutProperty>(layoutProperty);
  SizeProperty *graphSize = graph->getProperty<SizeProperty>(sizeProperty);
  DoubleProperty *graphRot = graph->getProperty<DoubleProperty>(rotationProperty);
  Size size = graphSize->getNodeValue(metanode);
  Coord pos = graphLayout->getNodeValue(metanode);
  double rot = graphRot->getNodeValue(metanode);
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
  Coord center = (maxL + minL) / -2.0;
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
}
//====================================================================================
void tlp::openMetaNode(Graph *graph, node n) {
  if (graph->getRoot()==graph) {
    cerr << __PRETTY_FUNCTION__ << endl;
    cerr << "\t Error: Could not ungroup a meta node in the root graph" << endl;
    return;
  }
  Observable::holdObservers();
  MutableContainer<node> mappingC;
  MutableContainer<node> mappingN;
  mappingC.setAll(node());
  mappingN.setAll(node());
  Graph *root = graph->getRoot();
  GraphProperty *metaInfo = root->getProperty<GraphProperty>(metagraphProperty);
  Graph *metaGraph = metaInfo->getNodeValue(n);
  if (metaGraph == 0) return;
  buildMapping(root->getInOutNodes(n), mappingC, metaInfo, node() );
  buildMapping(metaGraph->getNodes() , mappingN, metaInfo, node() );
  //add node from meta to graph
  Iterator<node> *itN = metaGraph->getNodes();
  while (itN->hasNext()) {
    graph->addNode(itN->next());
  } delete itN;
  Iterator<edge> *itE = metaGraph->getEdges();
  while (itE->hasNext()) {
    graph->addEdge(itE->next());
  } delete itE;
  updateLayoutUngroup(graph, n);
  //===========================
  //Remove the metagraph from the hierarchy and remove the metanode
  root->delAllNode(n);
  metaGraph->getFather()->delSubGraph(metaGraph);
  hash_map<node, hash_set<node> > edges;
  //=================================
  StableIterator<edge> it(root->getEdges());
  while(it.hasNext()) {
    edge e = it.next();
    if (graph->isElement(e)) continue;
    node sourceC = mappingC.get(root->source(e).id);
    node targetN = mappingN.get(root->target(e).id);
    node sourceN = mappingN.get(root->source(e).id);
    node targetC = mappingC.get(root->target(e).id);
    node source, target;
    if (sourceC.isValid() && targetN.isValid()) {
      source = sourceC;
      target = targetN;
    } else {
      if (sourceN.isValid() && targetC.isValid()) {
	source = sourceN;
	target = targetC;
      } else
	continue;
    }
    if (metaInfo->getNodeValue(source) == 0 && metaInfo->getNodeValue(target) == 0) {
      graph->addEdge(e);
      continue;
    }
    if ( (edges.find(source) == edges.end()) || (edges[source].find(target) == edges[source].end()) ) {
      edges[source].insert(target);
      if (!graph->existEdge(source,target).isValid())
	graph->addEdge(source,target);
      else 
	cerr << "bug exist edge 1";
    }
  }
  Observable::unholdObservers();
}
//=========================================================
Graph * tlp::inducedSubGraph(Graph *graph, const std::set<node> &nodes, string name) {
  Graph *result = graph->addSubGraph();
  set<node>::const_iterator itNodeSet = nodes.begin();
  for(;itNodeSet!=nodes.end(); ++itNodeSet) {
    result->addNode(*itNodeSet);
  }
  Iterator<node> *itN=result->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    Iterator<edge> *itE=graph->getOutEdges(itn);
    while (itE->hasNext()) {
      edge ite = itE->next();
      if (result->isElement(graph->target(ite)))
	result->addEdge(ite);
    } delete itE;
  } delete itN;
  return result;
}
//=========================================================
