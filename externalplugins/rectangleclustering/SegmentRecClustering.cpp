#include <math.h>
#include <qmessagebox.h>

#include <tulip/SelectionProxy.h>
#include <tulip/SuperGraph.h>
#include <tulip/TreeTest.h>

#include "SegmentRecClustering.h"
#include "TableSegment.h"

CLUSTERINGPLUGIN(SegmentRecClustering,"Segment Dfs-Recursive","David Auber","27/01/2000","Alpha","0","1");

using namespace std;
//================================================================================
SegmentRecClustering::SegmentRecClustering(ClusterContext context):Clustering(context)
{}
//================================================================================
SegmentRecClustering::~SegmentRecClustering()
{}
//================================================================================
void SegmentRecClustering::getRecurseChild(node nNode,SelectionProxy *resultGood,SelectionProxy *resultBad) {
  resultBad->setNodeValue(nNode,true);
  Iterator<edge> *itE=superGraph->getOutEdges(nNode);
  while (itE->hasNext()) {
    edge ite=itE->next();
    resultGood->setNodeValue(superGraph->target(ite),false);
    resultGood->setEdgeValue(ite,false);
    resultBad->setEdgeValue(ite,true);
    getRecurseChild(superGraph->target(ite),resultGood,resultBad);
  } delete itE;
}
//================================================================================
bool SegmentRecClustering::DfsClustering (node currentNode,SelectionProxy *selectGood,SelectionProxy *selectBad) {
  double n,c1,c2;
  double leafMax,leafMin;
  int se;
  string tmpString;
  bool result=true;
  
  Iterator<node> *itN=superGraph->getOutNodes(currentNode);
  while (itN->hasNext()) {
    node itn=itN->next();
    result = result & DfsClustering (itn,selectGood,selectBad);
  }

  if (result) {
    n=nodeM->getNodeValue(currentNode);
    se=(int)segmentM->getNodeValue(currentNode);
    if (n>10)	{
      if (se>4) {
	c1=segmentValC1[5];
	c2=segmentValC2[5];
      }
      else {
	c1=segmentValC1[se];
	c2=segmentValC2[se];
      }
      leafMin=c1*n-inter*c2*sqrt(n);
      leafMax=c1*n+inter*c2*sqrt(n);
    }
    else {
      if (se>5) se=5;
      switch (mResult) {
      case 0:
	leafMin=segmentValinf10C15p100[(int)(se*10+n-1)];
	leafMax=segmentValinf10C25p100[(int)(se*10+n-1)];
	break;
      case 1:
	leafMin=segmentValinf10C110p100[(int)(se*10+n-1)];
	leafMax=segmentValinf10C210p100[(int)(se*10+n-1)];
	break;
      case 2:
	leafMin=segmentValinf10C120p100[(int)(se*10+n-1)];
	leafMax=segmentValinf10C220p100[(int)(se*10+n-1)];
	break;
      default:
	leafMin=segmentValinf10C15p100[(int)(se*10+n-1)];
	leafMax=segmentValinf10C25p100[(int)(se*10+n-1)];
	break;
      }
    }
    double nbLeaf=leafM->getNodeValue(currentNode);
    leafMin=floor(leafMin);
    leafMax=ceil(leafMax);
    if (!( (nbLeaf>=leafMin) && (nbLeaf<=leafMax))) {
      getRecurseChild(currentNode,selectGood,selectBad);
      nodeM->setNodeValue(currentNode,1);//recompute(tmpString);
      leafM->setNodeValue(currentNode,1);//recompute(tmpString);
      //segmentM->setNodeValue(currentNode,0);//recompute(tmpString);
      result=false;
    }
  }
  return result;
}
//================================================================================
bool SegmentRecClustering::run() {
  bool resultBool;
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
  leafM = new MetricProxy(superGraph);
  resultBool = superGraph->computeProperty("Leaf",leafM,erreurMsg);
  nodeM = new MetricProxy(superGraph);
  resultBool = superGraph->computeProperty("Node",nodeM,erreurMsg);

  //Creation de la liste des sommets OK
  Iterator<node> *itN=superGraph->getNodes();
  while (itN->hasNext()) {
    node nit=itN->next();
    if (superGraph->indeg(nit)==0) {
      rootNode=nit;
      break;
    }
  }

  bool result=false;
  while (!result) {
    SelectionProxy *selectGood= superGraph->getLocalProperty<SelectionProxy>("tmpSelectionGood");
    SelectionProxy *selectBad = superGraph->getLocalProperty<SelectionProxy>("tmpSelectionBad");
    SuperGraph * saveSuper=superGraph;
    selectGood->setAllNodeValue(true);
    selectGood->setAllEdgeValue(true);
    selectBad->setAllNodeValue(false);
    selectBad->setAllEdgeValue(false);
    
    segmentM = new MetricProxy(superGraph);
    resultBool = superGraph->computeProperty("Segment", segmentM, erreurMsg);
    
    result = DfsClustering(rootNode,selectGood,selectBad);
    if (!result) {
      SuperGraph * tmpSubGraph,*tmpSubGraph2;
      tmpSubGraph = superGraph->addSubGraph(selectGood);
      tmpSubGraph2 = superGraph->addSubGraph(selectBad);
      superGraph=tmpSubGraph;
    }
    saveSuper->delLocalProperty("tmpSelectionGood");
    saveSuper->delLocalProperty("tmpSelectionBad");
    delete segmentM;
  }
  delete nodeM;
  delete leafM;
  return true;
}
//================================================================================
bool SegmentRecClustering::check(string &erreurMsg) {
  if (TreeTest::isTree(superGraph)) {
    erreurMsg="";
    return true;
  }
  else {
    erreurMsg="The Graph must be a Tree";
    return false;
  }
}
//================================================================================
void SegmentRecClustering::reset()
{
}




