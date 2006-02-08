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

static const int NB_UPDOWN_SWEEP = 4;

//================================================================================
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
    HTML_HELP_CLOSE(),
    //Spacing
    HTML_HELP_OPEN()				 \
    HTML_HELP_DEF( "type", "float" ) \
    HTML_HELP_DEF( "default", "64." )	 \
    HTML_HELP_BODY() \
    "This parameter enables to set up the minimum space between two layers in the drawing" \
    HTML_HELP_CLOSE(),
    //Spacing
    HTML_HELP_OPEN()				 \
    HTML_HELP_DEF( "type", "float" ) \
    HTML_HELP_DEF( "default", "18." )	 \
    HTML_HELP_BODY() \
    "This parameter enables to set up the minimum space between two nodes in the same layer" \
    HTML_HELP_CLOSE()
  };
}
//================================================================================
const std::string ORIENTATION("horizontal;vertical;");
//================================================================================
HierarchicalGraph::HierarchicalGraph(const PropertyContext &context):Layout(context) {
  addParameter<SizesProxy>("nodeSize",paramHelp[0],"viewSize");
  addParameter<StringCollection> ("orientation", paramHelp[1], ORIENTATION );
  addParameter<float> ("layer spacing", paramHelp[2], "64." );
  addParameter<float> ("node spacing", paramHelp[5], "18." );
}
//================================================================================
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
  if(!graph->computeProperty("DagLevel", &dagLevel,erreurMsg)) {
    cerr << "[ERROR] : " << erreurMsg << __PRETTY_FUNCTION__ << endl;
    return;
  }
  node n;
  forEach(n, graph->getNodes()) {
    unsigned int level=(unsigned int)dagLevel.getNodeValue(n);
    while (level>=grid.size()) grid.push_back(vector<node>());
    embedding->setNodeValue(n, grid[level].size());
    grid[level].push_back(n);
  }
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
  for (it = grid[freeLayer].begin(); it!=grid[freeLayer].end(); ++it) {
    node n = *it;
    double sum = embedding->getNodeValue(n);
    node it;
    forEach(it, superGraph->getInOutNodes(n))
      sum += embedding->getNodeValue(it);
    embedding->setNodeValue(n, sum / (double(superGraph->deg(n)) + 1.0 ) );
  }
  /*
  stable_sort(grid[freeLayer].begin(), grid[freeLayer].end(), lessNode);
  unsigned int j = 0;
  for (it=grid[freeLayer].begin();it!=grid[freeLayer].end();++it) {
    embedding->setNodeValue(*it,j);
    j++;
  }
  */
}

/*
void HierarchicalGraph::twoLayerCrossReduction(SuperGraph *superGraph,unsigned int freeLayer,bool sense){
  vector<node>::const_iterator it;
  for (it = grid[freeLayer].begin(); it!=grid[freeLayer].end(); ++it) {
    node n = *it;
    if (degree(superGraph,  n, sense)>0) {
      double sum = embedding->getNodeValue(n);
      Iterator<node>*itN;
      if (sense) 
	itN=superGraph->getOutNodes(n);
      else
	itN=superGraph->getInNodes(n);
      while(itN->hasNext()) {
	node itn=itN->next();
	sum += embedding->getNodeValue(itn);
      } delete itN;
      embedding->setNodeValue(n,sum/((double)degree(superGraph,n,sense) + 1.0));
    }
    else {
      //      embedding->setNodeValue(n, 10000);
    }
  }

  stable_sort(grid[freeLayer].begin(),grid[freeLayer].end(),lessNode);
  unsigned int j=0;
  for (it=grid[freeLayer].begin();it!=grid[freeLayer].end(); ++it) {
    embedding->setNodeValue(*it, j);
    j++;
  }

}
*/
//================================================================================
//Set initial position using a DFS
void HierarchicalGraph::initCross(SuperGraph *superGraph, node n, MutableContainer<bool> &visited, int id) {
  if (visited.get(n.id)) return;
  visited.set(n.id, true);
  embedding->setNodeValue(n, id);
  node it;
  forEach(it, superGraph->getOutNodes(n)) {
    initCross(superGraph, it, visited, id + 1);
  }
}
//================================================================================
// Do layer by layer sweep to reduce crossings in K-Layer graph
void HierarchicalGraph::crossReduction(SuperGraph *graph){

  node tmp = graph->addNode();
  embedding->setNodeValue(tmp, 0);
  node it;
  forEach(it, graph->getNodes()){
    if (graph->outdeg(it) == 0)
      graph->addEdge(it, tmp);
  }
  grid.push_back(vector<node>());
  grid[grid.size()-1].push_back(tmp);
  //  cerr << __PRETTY_FUNCTION__  << endl;
  MutableContainer<bool> visited;
  visited.setAll(false);
  node root;
  tlp::getSource(graph,root);
  initCross(graph, root, visited, 1);

  for (int a=0; a<grid.size(); ++a) {
    vector<node>::const_iterator it;
    unsigned int j=0;
    stable_sort(grid[a].begin(), grid[a].end(), lessNode);
    for (it=grid[a].begin(); it!=grid[a].end(); ++it) {
      embedding->setNodeValue(*it,j);
      j++;
    }
  }

  unsigned int maxDepth = grid.size();
  //Iterations of the sweeping
  for (int a=0; a<NB_UPDOWN_SWEEP; ++a) {
    //Up sweeping
    for (int i = maxDepth-1; i>=0; --i) {
      twoLayerCrossReduction(superGraph,i,true);
    }
    //Down sweeping
    for (int i = 0; i<maxDepth; ++i) {
      twoLayerCrossReduction(superGraph,i,false);
    }
  }

  for (int a=0; a<grid.size(); ++a) {
    vector<node>::const_iterator it;
    unsigned int j=0;
    stable_sort(grid[a].begin(), grid[a].end(), lessNode);
    for (it=grid[a].begin(); it!=grid[a].end(); ++it) {
      embedding->setNodeValue(*it,j);
      j++;
    }
  }
  graph->delAllNode(tmp);
  //  cerr << __PRETTY_FUNCTION__  << endl;
}
//================================================================================
void HierarchicalGraph::DagLevelSpanningTree(SuperGraph* graph, MetricProxy *embedding) {
  //  cerr << __PRETTY_FUNCTION__  << endl;
  assert(AcyclicTest::isAcyclic(graph));
  LessThanEdge tmpL;
  tmpL.metric = embedding;
  tmpL.sg = graph;
  node n;
  forEach(n, graph->getNodes()) {
    if (graph->indeg(n) > 1) {
      vector<edge> tmpVect;
      edge e;
      forEach(e, graph->getInEdges(n)) 
	tmpVect.push_back(e);
      sort(tmpVect.begin(), tmpVect.end(), tmpL);
      int toKeep = tmpVect.size()/2;
      vector<edge>::const_iterator it;
      for (it=tmpVect.begin(); it!=tmpVect.end(); ++it, --toKeep) {
	if (toKeep!=0) 
	  graph->delEdge(*it);
      }
    }
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
  orientation = "horizontal";
  float spacing = 64.0;
  float nodeSpacing = 18;
  if (dataSet!=0) {
    dataSet->get("nodeSize", nodeSize);
    dataSet->get("layer spacing", spacing);
    dataSet->get("node spacing", nodeSpacing);
    StringCollection tmp;
    if (dataSet->get("orientation", tmp)) {
      orientation = tmp.getCurrentString();
    }
  }
  //=========================================================
  //rotate size if necessary
  if (orientation == "horizontal") {
    node n;
    forEach(n, superGraph->getNodes()) {
      Size tmp = nodeSize->getNodeValue(n);
      nodeSize->setNodeValue(n, Size(tmp[1], tmp[0], tmp[2]));
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
    DagLevelSpanningTree(mySGraph, &embed);
  } else {
    MetricProxy embed(mySGraph);
    embedding = &embed;
    buildGrid(mySGraph);
  }
  
  //We draw the tree using a tree drawing algorithm
  bool resultBool;
  string erreurMsg;
  LayoutProxy tmpLayout(superGraph);
  DataSet tmp;
  tmp.set("nodeSize", nodeSize);
  tmp.set("edgeLength", edgeLength);
  tmp.set("layer spacing", spacing);
  tmp.set("node spacing", nodeSpacing);
  if (edgeLength!=0)
    tmp.set("use length", true);
  else
    tmp.set("use length", false);
  tmp.set("ortogonal", false);
  StringCollection tmpS("vertical;horizontal;");
  tmpS.setCurrent("vertical");
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

  //post processing 
  //Prevent edge node overlaping
  std::vector< float > levelMaxSize(grid.size());
  MutableContainer<int> nodeLevel;

  for (unsigned int i = 0; i<grid.size();++i) {
    levelMaxSize[i] = 0;
    for (unsigned int j= 0; j < grid[i].size(); ++ j) {
      node n = grid[i][j];
      if(superGraph->isElement(n)) {
	nodeLevel.set(n.id, i);
	Size tmp = nodeSize->getNodeValue(n);
	levelMaxSize[i] = std::max(levelMaxSize[i], tmp[1]);
      }
    }
  }
  
  edge e;
  forEach(e, superGraph->getEdges()) {
    node src = superGraph->source(e);
    node tgt = superGraph->target(e);
    if (src == tgt) {
      continue;
    }
    float rev = 1.0;
    if (nodeLevel.get(src.id)>nodeLevel.get(tgt.id)) {
      rev = -1.0;
    }
    Coord srcPos = layoutProxy->getNodeValue(src);
    Coord tgtPos = layoutProxy->getNodeValue(tgt);
    vector<Coord> old = layoutProxy->getEdgeValue(e);
    if (old.size() == 0) {
      vector<Coord> pos(2);
      srcPos[1] += rev*(levelMaxSize[nodeLevel.get(src.id)]/2.0 + spacing/4.);
      tgtPos[1] -= rev*(levelMaxSize[nodeLevel.get(tgt.id)]/2.0 + spacing/4.);
      pos[0] = srcPos;
      pos[1] = tgtPos;
      layoutProxy->setEdgeValue(e, pos);
    }
    else {
      vector<Coord> pos(4);
      srcPos[1] += rev*(levelMaxSize[nodeLevel.get(src.id)]/2.0 + spacing/4.);
      tgtPos[1] -= rev*(levelMaxSize[nodeLevel.get(tgt.id)]/2.0 + spacing/4.);
      Coord src2Pos = old.front();
      Coord tgt2Pos = old.back();
      src2Pos[1] -= rev*(levelMaxSize[nodeLevel.get(src.id) + 1]/2.0 + spacing/4.);
      tgt2Pos[1] += rev*(levelMaxSize[nodeLevel.get(tgt.id) - 1]/2.0 + spacing/4.);
      pos[0] = srcPos;
      pos[1] = src2Pos;
      pos[2] = tgt2Pos;
      pos[3] = tgtPos;
      layoutProxy->setEdgeValue(e, pos);
    }
  }
  
  //post processing align nodes
  forEach(n, superGraph->getNodes()) {
    Coord tmp = layoutProxy->getNodeValue(n);
    Size tmpS = nodeSize->getNodeValue(n);
    tmp[1] -= (levelMaxSize[nodeLevel.get(n.id)] - tmpS[1]) / 2.0;
    layoutProxy->setNodeValue(n, tmp);
  }

  //rotate layout and size
  if (orientation == "horizontal") {
    node n;
    forEach(n, superGraph->getNodes()) {
      Size  tmp = nodeSize->getNodeValue(n);
      nodeSize->setNodeValue(n, Size(tmp[1], tmp[0], tmp[2]));
      Coord tmpC = layoutProxy->getNodeValue(n);
      layoutProxy->setNodeValue(n, Coord(-tmpC[1], tmpC[0], tmpC[2]));
    }
    edge e;
    forEach(e, superGraph->getEdges()) {
      LineType::RealType tmp = layoutProxy->getEdgeValue(e);
      LineType::RealType tmp2;
      LineType::RealType::iterator it;
      for (it = tmp.begin(); it!= tmp.end(); ++it) {
	tmp2.push_back(Coord(-(*it)[1], (*it)[0], (*it)[2]));
      }
      layoutProxy->setEdgeValue(e, tmp2);
    }
  }
  return true;
}

