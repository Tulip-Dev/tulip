#include <string>
#include <cassert>
#include <math.h> 
#include <stack>
#include <tulip/AcyclicTest.h>
#include <tulip/TreeTest.h>
#include <tulip/MethodFactory.h>
#include <tulip/LayoutProxy.h>
#include <tulip/SelectionProxy.h>
#include <tulip/GraphTools.h>
#include <tulip/TlpTools.h>
#include <tulip/ForEach.h>
#include <tulip/SortIterator.h>
#include "HierarchicalGraph.h"


LAYOUTPLUGIN(HierarchicalGraph,"Hierarchical Graph","David Auber","23/05/2000","Alpha","0","1");

using namespace std;

static const int NB_UPDOWN_SWEEP = 10;

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
    //Orientation
    HTML_HELP_OPEN()				 \
    HTML_HELP_DEF( "type", "String Collection" ) \
    HTML_HELP_DEF( "default", "horizontal" )	 \
    HTML_HELP_BODY() \
    "This parameter enables to choose the orientation of the drawing"	\
    HTML_HELP_CLOSE()
  };
}

const std::string ORIENTATION("vertical;horizontal;");

HierarchicalGraph::HierarchicalGraph(const PropertyContext &context):Layout(context) {
  addParameter<SizesProxy>("nodeSize",paramHelp[0],"viewSize");
  addParameter<StringCollection> ("orientation", paramHelp[1], ORIENTATION );
}
HierarchicalGraph::~HierarchicalGraph() {}
//================================================================================
class LessThanEdge {
public:
  MetricProxy *metric;
  SuperGraph *sg;
  bool operator() (edge e1, edge e2) {
    return (metric->getNodeValue(sg->source(e1)) < metric->getNodeValue(sg->source(e2)));
  }
};
//================================================================================
void HierarchicalGraph::buildGrid(SuperGraph *graph){
  //  cerr << __PRETTY_FUNCTION__  << endl;
  bool resultBool;
  string erreurMsg;
  MetricProxy dagLevel(graph);
  resultBool = graph->computeProperty("DagLevel", &dagLevel,erreurMsg);
  Iterator<node> *itN=graph->getNodes();  
  while(itN->hasNext()){
    node itn=itN->next();
    unsigned int level=(unsigned int)dagLevel.getNodeValue(itn);
    while (level>=grid.size()) grid.push_back(vector<node>());
    embedding->setNodeValue(itn, grid[level].size());
    grid[level].push_back(itn);
  } delete itN;
  //  cerr << __PRETTY_FUNCTION__  << endl;
}
//================================================================================
unsigned int HierarchicalGraph::degree(SuperGraph *superGraph,node n,bool sense) {
  if (sense)
    return (superGraph->outdeg(n));
  else
    return (superGraph->indeg(n));
}
//================================================================================
//If sense==true fixed_layer is freeLayer+1 else freeLayer-1
//Compute barycenter heuristique
void HierarchicalGraph::twoLayerCrossReduction(SuperGraph *superGraph,unsigned int freeLayer,bool sense){
  vector<node>::const_iterator it;
  for (it=grid[freeLayer].begin();it!=grid[freeLayer].end();++it) {
    node n=*it;
    if (degree(superGraph,n,sense)>0) {
      double sum=0;
      Iterator<node>*itN;
      if (sense) 
	itN=superGraph->getOutNodes(n);
      else
	itN=superGraph->getInNodes(n);
      while(itN->hasNext()) {
	node itn=itN->next();
	sum += embedding->getNodeValue(itn);
      } delete itN;
      embedding->setNodeValue(n,sum/(double)degree(superGraph,n,sense));
    }
  }
  stable_sort(grid[freeLayer].begin(),grid[freeLayer].end(),lessNode);
  unsigned int j=0;
  for (it=grid[freeLayer].begin();it!=grid[freeLayer].end();++it) {
    embedding->setNodeValue(*it,j);
    j++;
  }
}
//================================================================================
//Set initial position using a DFS
void HierarchicalGraph::initCross(SuperGraph *superGraph,node n, stdext::hash_map<node,bool> &visited,int &id) {
  if (visited[n]) return;
  visited[n]=true;
  embedding->setNodeValue(n,id);
  id++;
  Iterator<node> *itN=superGraph->getOutNodes(n);
  while (itN->hasNext()){
    node itn=itN->next();
    initCross(superGraph,itn,visited,id);
  } delete itN;
}
//================================================================================
// Do layer by layer sweep to reduce crossings in K-Layer graph
void HierarchicalGraph::crossReduction(SuperGraph *graph){
  //  cerr << __PRETTY_FUNCTION__  << endl;
  stdext::hash_map<node,bool> visited(graph->numberOfNodes());
  int id=1;
  Iterator<node> *itN=graph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    if (graph->indeg(itn)==0) initCross(graph,itn, visited,id);
  } delete itN;
  unsigned int maxDepth=grid.size();
  //Iterations of the sweeping
  for (int a=0;a<NB_UPDOWN_SWEEP;++a) {
    //Down sweeping
    for (unsigned int i=0;i<maxDepth;++i) {
      twoLayerCrossReduction(superGraph,i,false);
    }
    //Up sweeping
    for (int i=maxDepth-1;i>=0;--i) {
      twoLayerCrossReduction(superGraph,i,true);
    }
  }
  //  cerr << __PRETTY_FUNCTION__  << endl;
}
//================================================================================
void HierarchicalGraph::DagLevelSpanningTree(SuperGraph* superGraph,node n) {
  //  cerr << __PRETTY_FUNCTION__  << endl;
  assert(AcyclicTest::isAcyclic(superGraph));
  stack<edge> toDelete;
  MetricProxy *barycenter = embedding;
  Iterator<node> *itN=superGraph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    if (superGraph->indeg(itn)>1) {
      vector<edge> tmpList;
      Iterator<edge> *itE = superGraph->getInEdges(itn);
      while (itE->hasNext()) {
	edge ite=itE->next();
	tmpList.push_back(ite);
      } delete itE;
      LessThanEdge tmpL;
      tmpL.metric = barycenter;
      tmpL.sg = superGraph;
      sort(tmpList.begin(), tmpList.end(), tmpL);
      int toKeep = tmpList.size()/2;
      vector<edge>::const_iterator it;
      for (it=tmpList.begin(); it!=tmpList.end(); ++it) {
	if (toKeep!=0) toDelete.push(*it);
	toKeep--;
      }
    }
  } delete itN;
  while (!toDelete.empty()) {
    superGraph->delEdge(toDelete.top());
    toDelete.pop();
  }
  assert(TreeTest::isTree(superGraph));
  //  cerr << __PRETTY_FUNCTION__  << endl;
}
//==============================================================================================================
void HierarchicalGraph::computeEdgeBends(const SuperGraph *mySGraph, LayoutProxy &tmpLayout, 
					 const stdext::hash_map<edge,edge> &replacedEdges, const vector<edge> &reversedEdges) {
  //  cerr << "we compute bends on splitted edges" << endl;
  MutableContainer<bool> isReversed;
  isReversed.setAll(false);
  for (vector<edge>::const_iterator it = reversedEdges.begin(); it != reversedEdges.end(); ++it)
    isReversed.set((*it).id, true);
  
  for (stdext::hash_map<edge,edge>::const_iterator it = replacedEdges.begin(); it!=replacedEdges.end(); ++it) {
    edge toUpdate = (*it).first;
    edge start = (*it).second;
    edge end = start;
    Coord p1,p2;
    //we take the first and last point of the replaced edges
    while (superGraph->target(end) != superGraph->target(toUpdate)) {
      Iterator<edge> *itE = mySGraph->getOutEdges(superGraph->target(end));
      end = itE->next();
      delete itE;
    }
    node firstN = superGraph->target(start);
    node endN = superGraph->source(end);

    LineType::RealType edgeLine;
    if (isReversed.get(toUpdate.id)) {
      p1 = tmpLayout.getNodeValue(endN);
      p2 = tmpLayout.getNodeValue(firstN);
    }
    else {
      p1 = tmpLayout.getNodeValue(firstN);
      p2 = tmpLayout.getNodeValue(endN);
    }
    if (p1==p2) 
      edgeLine.push_back(p1); 
    else {
      edgeLine.push_back(p1); 
      edgeLine.push_back(p2);
    }
    layoutProxy->setEdgeValue(toUpdate,edgeLine);
  }
}
//=======================================================================
void HierarchicalGraph::computeSelfLoops(SuperGraph *mySGraph, LayoutProxy &tmpLayout, std::vector<tlp::SelfLoops> &listSelfLoops) {
  //cerr << "We compute self loops" << endl;
  while (!listSelfLoops.empty()) {
    tlp::SelfLoops tmp = listSelfLoops.back();
    listSelfLoops.pop_back();
    LineType::RealType tmpLCoord;
    const LineType::RealType &edge1 = tmpLayout.getEdgeValue(tmp.e1);
    const LineType::RealType &edge2 = tmpLayout.getEdgeValue(tmp.e2);
    const LineType::RealType &edge3 = tmpLayout.getEdgeValue(tmp.e3);
    LineType::RealType::const_iterator it;
    for (it = edge1.begin(); it!=edge1.end(); ++it)
      tmpLCoord.push_back(*it);
    tmpLCoord.push_back(tmpLayout.getNodeValue(tmp.n1));
    for (it = edge2.begin(); it!=edge2.end(); ++it)
      tmpLCoord.push_back(*it);
    tmpLCoord.push_back(tmpLayout.getNodeValue(tmp.n2));
    for (it = edge3.begin(); it!=edge3.end(); ++it)
      tmpLCoord.push_back(*it);
    layoutProxy->setEdgeValue(tmp.old,tmpLCoord);
    mySGraph->delAllNode(tmp.n1);
    mySGraph->delAllNode(tmp.n2);
  }  
  //  cerr << "we clean every added nodes and edges" << endl;
}
  //=======================================================================
bool HierarchicalGraph::run() {
  //=======================================================================
  // Build a clone of this graph
  SuperGraph *mySGraph = tlp::newCloneSubGraph(superGraph,"tmp clone");

  nodeSize = superGraph->getProperty<SizesProxy>("viewSize");
  orientation = "vertical";
  if (dataSet!=0) {
    dataSet->get("nodeSize", nodeSize);
    StringCollection tmp;
    if (dataSet->get("orientation", tmp)) {
      orientation = tmp.getCurrentString();
    }
  }
  //========================================================================
  //if the graph is not acyclic we reverse edges to make it acyclic
  vector<tlp::SelfLoops> listSelfLoops;
  vector<edge> reversedEdges;
  AcyclicTest::makeAcyclic(mySGraph, reversedEdges, listSelfLoops);

  //========================================================================
  //We add a node and edges to force the dag to have only one source.
  node startNode = tlp::makeSimpleSource(mySGraph);
  
  //========================================================================
  list<node> properAddedNodes;
  stdext::hash_map<edge,edge> replacedEdges;
  IntProxy *edgeLength = 0;
  if (!TreeTest::isTree(mySGraph)) {
    //We transform the dag in a proper dag
    edgeLength = new IntProxy(mySGraph);
    tlp::makeProperDag(mySGraph,properAddedNodes,replacedEdges,edgeLength);
    //we compute metric for cross reduction
    MetricProxy embed(mySGraph);
    embedding = &embed;
    lessNode.metric = embedding;
    buildGrid(mySGraph);
    crossReduction(mySGraph);
    node n;
    forEach(n, superGraph->getNodes()) {
      vector<edge> order;
      edge e;
      forEach(e, new SortTargetEdgeIterator(mySGraph->getOutEdges(n),mySGraph, embedding)) {
	order.push_back(e);
      }
      mySGraph->setEdgeOrder(n, order);
    }  
    //We extract a spanning tree from the proper dag.
    DagLevelSpanningTree(mySGraph,startNode);
  }
  
  //We draw the tree using a tree drawing algorithm
  bool resultBool;
  string erreurMsg;
  LayoutProxy tmpLayout(superGraph);
  DataSet tmp;
  tmp.set("nodeSize", nodeSize);
  tmp.set("edgeLength", edgeLength);
  if (edgeLength!=0)
    tmp.set("use length", true);
  else
    tmp.set("use length", false);
  tmp.set("ortogonal", false);
  StringCollection tmpS;
  if (dataSet!=0) {
    dataSet->get("orientation", tmpS);
  }
  tmp.set("orientation", tmpS);
  resultBool = mySGraph->computeProperty("Hierarchical Tree (R-T Extended)", &tmpLayout, erreurMsg, 0, &tmp);
  assert(resultBool);
  
  
  node n;
  forEach(n, superGraph->getNodes()) {
    layoutProxy->setNodeValue(n, tmpLayout.getNodeValue(n));
  }

  computeEdgeBends(mySGraph, tmpLayout, replacedEdges, reversedEdges);
  computeSelfLoops(mySGraph, tmpLayout, listSelfLoops);
  
  for (vector<edge>::const_iterator it=reversedEdges.begin(); it!=reversedEdges.end(); ++it) {
    superGraph->reverse(*it);
  }
  mySGraph->delAllNode(startNode);
  while(!properAddedNodes.empty()) {
    mySGraph->delAllNode(properAddedNodes.back());
    properAddedNodes.pop_back();
  }
  superGraph->delSubGraph(mySGraph);
  return true;
}

