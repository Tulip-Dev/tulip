#include <string>
#include <cassert>
#include <cmath> 
#include <stack> 
#include <tulip/AcyclicTest.h>
#include <tulip/TreeTest.h>
#include <tulip/MethodFactory.h>
#include <tulip/LayoutProperty.h>
#include <tulip/BooleanProperty.h>

#include "GeneralGraph3D.h"


using namespace std;
using namespace tlp;

LAYOUTPLUGINOFGROUP(GeneralGraph3D,"Hierarchical Graph 3D","David Auber","23/05/2000","Alpha","0","1","Hierarchical")

namespace {
  const char * paramHelp[] = {
    // nodeSize
    HTML_HELP_OPEN() \
    HTML_HELP_DEF( "type", "Size" ) \
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

#define ORIENTATION "vertical;horizontal;"

GeneralGraph3D::GeneralGraph3D(const PropertyContext &context):LayoutAlgorithm(context) {
  addParameter<SizeProperty>("nodeSize",paramHelp[0],"viewSize");
  addParameter<StringCollection> ("orientation", paramHelp[1], ORIENTATION );
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
    BooleanProperty *spanningDag= new BooleanProperty(graph);
    resultBool = graph->computeProperty("SpanningDag",spanningDag,erreurMsg);
    if (!resultBool) {
      cerr << __PRETTY_FUNCTION__ << endl;
      cerr << erreurMsg << endl;
    }
    assert(resultBool);
    
    //sauvegarde information
    vector<edge> graphEdges(graph->numberOfEdges());
    int i=0;
    Iterator<edge> *itE=graph->getEdges();
    while (itE->hasNext()) {
      graphEdges[i]=itE->next();
      i++;
    }delete itE;
    
    //We replace self loops by three edges an two nodes.
    for (vector<edge>::const_iterator itEdge=graphEdges.begin();itEdge!=graphEdges.end();++itEdge) {
      edge ite=*itEdge;
      if ((spanningDag->getEdgeValue(ite))==false) {
	if (graph->source(ite)==graph->target(ite)) {
	  node n1=graph->addNode();
	  node n2=graph->addNode();
	  selfLoops.push_back(SelfLoops(n1 ,
					n2 , 
					graph->addEdge(graph->source(ite),n1) , 
					graph->addEdge(n1,n2) , 
					graph->addEdge(graph->source(ite),n2) , 
					ite ));
	}
	else {
	  reversed.insert(ite);
	  graph->reverse(ite);
	}
      }
    }
    
    delete spanningDag;
    //We remove all self loops from the graph
    list<SelfLoops>::iterator itSelf;
    for (itSelf=selfLoops.begin();itSelf!=selfLoops.end();++itSelf) {
      graph->delEdge((*itSelf).old);
    }
  }
  assert(AcyclicTest::isAcyclic(graph));
}

node GeneralGraph3D::makeSimpleSource(Graph* graph) {
  assert(AcyclicTest::isAcyclic(graph));
  node startNode=graph->addNode();
  Iterator<node> *itN=graph->getNodes();
  while (itN->hasNext()) {
    node itn=itN->next();
    if ((graph->indeg(itn)==0) && (itn!=startNode)) {
      graph->addEdge(startNode,itn);
    }
  } delete itN;
  assert(AcyclicTest::isAcyclic(graph));
  return startNode;
}

void GeneralGraph3D::makeProperDag(Graph* graph, list<node> &addedNodes, stdext::hash_map<edge,edge> &replacedEdges) {
  assert(AcyclicTest::isAcyclic(graph));
  //We compute the dag level metric on resulting graph.
  bool resultBool;
  string erreurMsg;
  DoubleProperty *dagLevel= new DoubleProperty(graph);
  resultBool = graph->computeProperty("DagLevel",dagLevel,erreurMsg);
  assert(resultBool);
  //we now transform the dag in a proper Dag, two linked nodes of a proper dag
  //must have a difference of one of dag level metric.
  node tmp1,tmp2;
  string tmpString;
  //sauvegarde information
  vector<edge> graphEdges(graph->numberOfEdges());
  int i=0;
  Iterator<edge> *itE=graph->getEdges();
  while (itE->hasNext()) {
    graphEdges[i]=itE->next();
    i++;
  } delete itE;
  
  for (vector<edge>::const_iterator itEdge=graphEdges.begin();itEdge!=graphEdges.end();++itEdge) {
    edge ite=*itEdge;
    double delta=dagLevel->getNodeValue(graph->target(ite))-dagLevel->getNodeValue(graph->source(ite));
    double levelStartNode=dagLevel->getNodeValue(graph->source(ite))+1;
    if (delta>1) {
      tmp1=graph->addNode();
      levelStartNode++;
      replacedEdges[ite]=graph->addEdge(graph->source(ite),tmp1);
      addedNodes.push_back(tmp1);
      while (delta>2) {
	tmp2=graph->addNode();
	levelStartNode++;
	addedNodes.push_back(tmp2);
	graph->addEdge(tmp1,tmp2);
	tmp1=tmp2;
	delta--;
      }
      graph->addEdge(tmp1,graph->target(ite));
    }
  }
  delete dagLevel;
  for (stdext::hash_map<edge,edge>::iterator it=replacedEdges.begin();it!=replacedEdges.end();++it) {
    graph->delEdge((*it).first);
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
