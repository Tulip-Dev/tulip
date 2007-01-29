#include <math.h>
#include <qmessagebox.h>

#include <tulip/BooleanProperty.h>
#include <tulip/Graph.h>
#include <tulip/DoubleProperty.h>
#include <tulip/TreeTest.h>

#include "AritySegmentRecClustering.h"
#include "TableAritySegment.h"

using namespace std;
using namespace tlp;

ALGORITHMPLUGIN(AritySegmentRecClustering,"Arity-Segment Bottom-Recursive","David Auber","27/01/2000","Alpha","0","1");
//================================================================================
AritySegmentRecClustering::AritySegmentRecClustering(AlgorithmContext context):Algorithm(context)
{}
//================================================================================
AritySegmentRecClustering::~AritySegmentRecClustering()
{}
//================================================================================
void AritySegmentRecClustering::getRecurseChild(node nNode,BooleanProperty *resultGood,BooleanProperty *resultBad) {
  resultBad->setNodeValue(nNode,true);
  Iterator<edge> *itE=graph->getOutEdges(nNode);
  while(itE->hasNext()) {
    edge ite=itE->next();
    resultGood->setNodeValue(graph->target(ite),false);
    resultGood->setEdgeValue(ite,false);
    resultBad->setEdgeValue(ite,true);
    getRecurseChild(graph->target(ite),resultGood,resultBad);
  } delete itE;
}
//================================================================================
bool AritySegmentRecClustering::DfsClustering (node currentNode,BooleanProperty *selectGood,BooleanProperty *selectBad) {
  bool result;
  double n,c1,c2;
  double leafMax,leafMin;
  int ar,se;
  string tmpString;

  result=true;
  Iterator<node> *itN=graph->getOutNodes(currentNode);
  while(itN->hasNext()) {
    node itn=itN->next();
    result = result & DfsClustering (itn,selectGood,selectBad);
  }

  if (result) {
    n=nodeM->getNodeValue(currentNode);
    if (n==1) return true;
    ar=(int)arityM->getNodeValue(currentNode);
    se=(int)segmentM->getNodeValue(currentNode);
    if (ar>8) ar=9;
    if (se>5) se=6;
    if (n<10) {
      ar-=1;
      se-=1;
      switch (mResult) {
      case 0:
	leafMin=aritySegmentValinf10C15p100[(int)(ar*50+se*10+n-1)];
	leafMax=aritySegmentValinf10C25p100[(int)(ar*50+se*10+n-1)];
	break;
      case 1:
	leafMin=aritySegmentValinf10C110p100[(int)(ar*50+se*10+n-1)];
	leafMax=aritySegmentValinf10C210p100[(int)(ar*50+se*10+n-1)];
	break;
      case 2:
	leafMin=aritySegmentValinf10C120p100[(int)(ar*50+se*10+n-1)];
	leafMax=aritySegmentValinf10C220p100[(int)(ar*50+se*10+n-1)];
	break;
      default:
	leafMin=aritySegmentValinf10C15p100[(int)(ar*50+se*10+n-1)];
	leafMax=aritySegmentValinf10C25p100[(int)(ar*50+se*10+n-1)];
	break;
      }
    }
    else {
      c1=aritySegmentValC1[(int)(ar*7+se)];
      c2=aritySegmentValC2[(int)(ar*7+se)];
      leafMin=c1*n-inter*c2*sqrt(n);
      leafMax=c1*n+inter*c2*sqrt(n);
    }

    double nbLeaf=leafM->getNodeValue(currentNode);
    leafMin=floor(leafMin);
    leafMax=ceil(leafMax);
    
    if (!( (nbLeaf>=leafMin) && (nbLeaf<=leafMax))) {
      //	  cerr << "leaf:" << nbLeaf << " Arity;" << ar << " node:" << n << " inter=[" << leafMin << ".." << leafMax << "]" << endl;
      getRecurseChild(currentNode,selectGood,selectBad);
      arityM->setNodeValue(currentNode,0);
      nodeM->setNodeValue(currentNode,1);
      leafM->setNodeValue(currentNode,1);
      result=false;
    }
  }
  return result;
}

//================================================================================
bool AritySegmentRecClustering::run() {
  // resultat, deux ensembles disjoints
  bool /*cached,*/ resultBool;
  string erreurMsg;
  node rootNode;
  mResult=QMessageBox::information( 0, QString("Tulip Interval"),
					QString("Select your value"),QString("5%"),QString("10%"),QString("20%"),0,0);
  switch (mResult) {
  case 0:inter=1.96;
    break;
  case 1:inter=1.66;
    break;
  case 2:inter=1.17;
    break;
  default:inter=1.96;
    break;
  }

  string tmpString;
  //=================================================
  //initialisation des metrics necessaires
  //=================================================
  //initialisation des metrics necessaires
  Iterator<node> *itN=graph->getNodes();
  while (itN->hasNext()) {
    node nit=itN->next();
    if (graph->indeg(nit)==0) {
      rootNode=nit;
      break;
    }
  }
  /*
  arityM=graph->getLocalProperty<DoubleProperty>("TreeArityMax",cached,resultBool,erreurMsg);
  leafM=graph->getLocalProperty<DoubleProperty>("Leaf",cached,resultBool,erreurMsg);
  nodeM=graph->getLocalProperty<DoubleProperty>("Node",cached,resultBool,erreurMsg);
  */
  arityM = new DoubleProperty(graph);
  graph->computeProperty("TreeArityMax", arityM, erreurMsg);
  leafM = new DoubleProperty(graph);
  graph->computeProperty("Leaf", leafM, erreurMsg);
  nodeM = new DoubleProperty(graph);
  graph->computeProperty("Node", nodeM, erreurMsg);
  
  bool result=false;
  while (!result) {
    BooleanProperty *selectGood= graph->getLocalProperty<BooleanProperty>("tmpSelectionGood");
    BooleanProperty *selectBad= graph->getLocalProperty<BooleanProperty>("tmpSelectionBad");
    Graph * saveSuper=graph;
    selectGood->setAllNodeValue(true);
    selectGood->setAllEdgeValue(true);
    selectBad->setAllNodeValue(false);
    selectBad->setAllEdgeValue(false);
      
    segmentM = new DoubleProperty(graph);
    resultBool = graph->computeProperty("Segment", segmentM, erreurMsg);

    result = DfsClustering(rootNode,selectGood,selectBad);
    if (!result) {
      Graph * tmpSubGraph,*tmpSubGraph2;
      //	  tmpSubGraph = graph->addView("ArSe-R Good",selectGood);
      tmpSubGraph = graph->addSubGraph(selectGood);
      //	  selectGood->reverse();
      //	  tmpSubGraph2 = graph->addView("ArSe-R Bad",selectBad);
      tmpSubGraph2 = graph->addSubGraph(selectBad);
      graph=tmpSubGraph;
    }
    delete segmentM;
    saveSuper->delLocalProperty("tmpSelectionGood");
    saveSuper->delLocalProperty("tmpSelectionBad");
  }
  
  delete nodeM;
  delete leafM;
  delete arityM;
  return true;
}
//================================================================================
bool AritySegmentRecClustering::check(string &erreurMsg) {
  if (TreeTest::isTree(graph)) {
    erreurMsg="";
    return true;
  }
  else {
    erreurMsg="The Graph must be a Tree";
    return false;
  }
}
//================================================================================
void AritySegmentRecClustering::reset() {
}




