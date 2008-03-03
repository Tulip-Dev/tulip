#include <string>
#include <cassert>
#include <cmath> 
#include <stack> 
#include <tulip/AcyclicTest.h>
#include <tulip/TreeTest.h>
#include <tulip/MethodFactory.h>
#include <tulip/LayoutProperty.h>
#include <tulip/BooleanProperty.h>
#include <tulip/GraphTools.h>

#include "GeneralGraph3D.h"
#include "DatasetTools.h"


using namespace std;
using namespace tlp;

LAYOUTPLUGINOFGROUP(GeneralGraph3D,"Hierarchical Graph 3D","David Auber","23/05/2000","Alpha","1.0","Hierarchical")

namespace {
  const char * paramHelp[] = {
    //Orientation
    HTML_HELP_OPEN()				 \
    HTML_HELP_DEF( "type", "String Collection" ) \
    HTML_HELP_DEF( "default", "horizontal" )	 \
    HTML_HELP_BODY() \
    "This parameter enables to choose the orientation of the drawing"	\
    HTML_HELP_CLOSE(),
  };
}

#define ORIENTATION "horizontal;vertical;"

GeneralGraph3D::GeneralGraph3D(const PropertyContext &context):LayoutAlgorithm(context) {
  addNodeSizePropertyParameter(this);
  addParameter<StringCollection> ("orientation", paramHelp[0], ORIENTATION );
  addDependency<BooleanAlgorithm>("Spanning Dag", "1.0");
  addDependency<DoubleAlgorithm>("Dag Level", "1.0");
  addDependency<LayoutAlgorithm>("Cone Tree", "1.0");
}

GeneralGraph3D::~GeneralGraph3D() {}

void GeneralGraph3D::DagLevelSpanningTree(Graph* graph,node n) {
  assert(AcyclicTest::isAcyclic(graph));
  stack<edge> toDelete;
  Iterator<node> *itN=graph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    if (graph->indeg(itn)>1) {
      int tmpI=graph->indeg(itn)-1;
      Iterator<edge> *itE=graph->getInEdges(itn);
      for (;tmpI>0;--tmpI) toDelete.push(itE->next());
      delete itE;
    }
  } delete itN;
  while (!toDelete.empty()) {
    graph->delEdge(toDelete.top());
    toDelete.pop();
  }
  assert(TreeTest::isTree(graph));
}

void GeneralGraph3D::makeAcyclic(Graph* graph,set<edge> &reversed,list<SelfLoops> &selfLoops) {
  if (!AcyclicTest::isAcyclic(graph)) {
    bool resultBool;
    string erreurMsg;
    BooleanProperty spanningDag(graph);
    resultBool = graph->computeProperty("Spanning Dag", &spanningDag,erreurMsg);
    if (!resultBool) {
      cerr << __PRETTY_FUNCTION__ << endl;
      cerr << erreurMsg << endl;
    }
    assert(resultBool);
    
    //We replace self loops by three edges an two nodes.
    StableIterator<edge> itE(graph->getEdges());
    while (itE.hasNext()) {
      edge ite=itE.next();
      if ((spanningDag.getEdgeValue(ite))==false) {
	if (graph->source(ite)==graph->target(ite)) {
	  node n1=graph->addNode();
	  node n2=graph->addNode();
	  selfLoops.push_back(SelfLoops(n1 ,
					n2 , 
					graph->addEdge(graph->source(ite),n1) , 
					graph->addEdge(n1,n2) , 
					graph->addEdge(graph->source(ite),n2) , 
					ite ));
	  graph->delEdge(ite);
	}
	else {
	  reversed.insert(ite);
	  graph->reverse(ite);
	}
      }
    }
  }
  assert(AcyclicTest::isAcyclic(graph));
}

bool GeneralGraph3D::run() {
  //=======================================================================
  // Build a clone of this graph
  Graph *mySGraph=tlp::newCloneSubGraph(graph);

  //========================================================================
  //if the graph is not acyclic we reverse edges to make it acyclic
  list<SelfLoops> listSelfLoops;
  set<edge> reversedEdges;
  makeAcyclic(mySGraph,reversedEdges,listSelfLoops);

  //We add a node and edges to force the dag to have only one source.
  node startNode=makeSimpleSource(mySGraph);

  //We transform the dag in a proper dag
  list<node> properAddedNodes;
  stdext::hash_map<edge,edge> replacedEdges;
  makeProperDag(mySGraph,properAddedNodes,replacedEdges);

  //We extract a spanning tree from the proper dag.
  DagLevelSpanningTree(mySGraph,startNode);

  //We draw the tree using a tree drawing algorithm
  bool resultBool;
  string erreurMsg;
  LayoutProperty *tmpLayout= new LayoutProperty(mySGraph);
  resultBool = mySGraph->computeProperty("Cone Tree",tmpLayout,erreurMsg,0, dataSet);
  assert(resultBool);
  if (!resultBool) {
    cerr << __PRETTY_FUNCTION__ << endl;
    cerr << erreurMsg << endl;
  }
  //  cerr << "End Tree Walker box" << endl;
  Iterator<node> *itN=graph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    layoutResult->setNodeValue(itn,tmpLayout->getNodeValue(itn));
  } delete itN;
  

  //  cerr << "we compute bends on splitted edges" << endl;
  for (stdext::hash_map<edge,edge>::const_iterator it=replacedEdges.begin();it!=replacedEdges.end();++it) {
    edge toUpdate=(*it).first;
    edge start=(*it).second;
    edge end=start;
    Coord p1,p2;
    //we take the first and last point of the replaced edges
    while (graph->target(end)!=graph->target(toUpdate)) {
      Iterator<edge> *itE=mySGraph->getOutEdges(graph->target(end));end=itE->next();delete itE;
    }
    node firstN=graph->target(start);
    node endN=graph->source(end);
    LineType::RealType edgeLine;
    if (reversedEdges.find(toUpdate)!=reversedEdges.end()) {
      p1=tmpLayout->getNodeValue(endN);
      p2=tmpLayout->getNodeValue(firstN);
    }
    else {
      p1=tmpLayout->getNodeValue(firstN);
      p2=tmpLayout->getNodeValue(endN);
    }
    if (p1==p2) edgeLine.push_back(p1); else {edgeLine.push_back(p1); edgeLine.push_back(p2);}
    layoutResult->setEdgeValue(toUpdate,edgeLine);
  }
  
  //cerr << "We compute self loops" << endl;
  while (!listSelfLoops.empty()) {
    SelfLoops tmp=listSelfLoops.front();
    listSelfLoops.pop_front();
    LineType::RealType tmpLCoord;
    const LineType::RealType &edge1=tmpLayout->getEdgeValue(tmp.e1);
    const LineType::RealType &edge2=tmpLayout->getEdgeValue(tmp.e2);
    const LineType::RealType &edge3=tmpLayout->getEdgeValue(tmp.e3);
    LineType::RealType::const_iterator it;
    for (it=edge1.begin();it!=edge1.end();++it)
      tmpLCoord.push_back(*it);
    tmpLCoord.push_back(tmpLayout->getNodeValue(tmp.n1));
    for (it=edge2.begin();it!=edge2.end();++it)
      tmpLCoord.push_back(*it);
    tmpLCoord.push_back(tmpLayout->getNodeValue(tmp.n2));
    for (it=edge3.begin();it!=edge3.end();++it)
      tmpLCoord.push_back(*it);
    layoutResult->setEdgeValue(tmp.old,tmpLCoord);
    mySGraph->delAllNode(tmp.n1);
    mySGraph->delAllNode(tmp.n2);
  }
  
  //  cerr << "we clean every added nodes and edges" << endl;
  //  mySGraph->delLocalProperty("Cone Tree Extended");
  delete tmpLayout;
  //  mySGraph->delLocalProperty("viewSize");
  //  graph->getLocalProperty<SizeProperty>("viewSize")->setAllNodeValue(Size(1,1,1));
  //  graph->getLocalProperty<SizeProperty>("viewSize")->setAllEdgeValue(Size(0.125,0.125,0.5));
  for (set<edge>::const_iterator it=reversedEdges.begin();it!=reversedEdges.end();++it) {
    graph->reverse(*it);
  }
  mySGraph->delAllNode(startNode);
  for (list<node>::const_iterator it=properAddedNodes.begin();it!=properAddedNodes.end();++it)
    mySGraph->delAllNode(*it);
  graph->delSubGraph(mySGraph);
  return true;
}
