#include <math.h>
#include <qmessagebox.h>

#include <tulip/SelectionProxy.h>
#include <tulip/SuperGraph.h>
#include <tulip/TreeTest.h>

#include "ArityRecClustering.h"
#include "TableArity.h"

CLUSTERINGPLUGIN(ArityRecClustering,"Arity Dfs-Recursive","David Auber","27/01/2000","Alpha","0","1");

using namespace std;
//================================================================================
ArityRecClustering::ArityRecClustering(ClusterContext context):Clustering(context) {}
//================================================================================
ArityRecClustering::~ArityRecClustering() {}
//================================================================================
void ArityRecClustering::getRecurseChild(node nNode,SelectionProxy *resultGood,SelectionProxy *resultBad) {
  resultBad->setNodeValue(nNode,true);
  Iterator<edge> *itE=superGraph->getOutEdges(nNode);
  for (;itE->hasNext();)
    {
      edge ite=itE->next();
      resultGood->setNodeValue(superGraph->target(ite),false);
      resultGood->setEdgeValue(ite,false);
      resultBad->setEdgeValue(ite,true);
      getRecurseChild(superGraph->target(ite),resultGood,resultBad);
    }
  delete itE;
}

//================================================================================
bool ArityRecClustering::DfsClustering (node currentNode,SelectionProxy *selectGood,SelectionProxy *selectBad) {
  bool result;
  double n,c1,c2;
  double leafMax,leafMin;
  int ar;
  string tmpString;

  result=true;

  Iterator<node> *itN=superGraph->getOutNodes(currentNode);
  for (;itN->hasNext();) {
    node itn=itN->next();
    result = result & DfsClustering (itn,selectGood,selectBad);
  }

  if (result) {
    double nbLeaf=leafM->getNodeValue(currentNode);
    n=nodeM->getNodeValue(currentNode);
    ar=(int)arityM->getNodeValue(currentNode);

    if (n>10) {
      if (ar>8) {
	c1=arityValC1[8];
	c2=arityValC2[8];
      }
      else {
	c1=arityValC1[(int)ar];
	c2=arityValC2[(int)ar];
      }
      leafMin=c1*n-inter*c2*sqrt(n);
      leafMax=c1*n+inter*c2*sqrt(n);
    }
    else {
      switch (mResult) {
      case 0:
	leafMin=arityValinf10C15p100[(int)(ar*10+n-1)];
	leafMax=arityValinf10C25p100[(int)(ar*10+n-1)];
	break;
      case 1:
	leafMin=arityValinf10C110p100[(int)(ar*10+n-1)];
	leafMax=arityValinf10C210p100[(int)(ar*10+n-1)];
	break;
      case 2:
	leafMin=arityValinf10C120p100[(int)(ar*10+n-1)];
	leafMax=arityValinf10C220p100[(int)(ar*10+n-1)];
	break;
      default:
	leafMin=arityValinf10C15p100[(int)(ar*10+n-1)];
	leafMax=arityValinf10C25p100[(int)(ar*10+n-1)];
	break;
      }
    }


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

bool ArityRecClustering::run() {
  // resultat, deux ensembles disjoints
  //bool cached,resultBool;
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
  Iterator<node> *itN=superGraph->getNodes();
  for (;itN->hasNext();) {
    node nit=itN->next();
    if (superGraph->indeg(nit)==0) {
      rootNode=nit;
      break;
    }
  }

  /*  arityM=superGraph->getLocalProperty<MetricProxy>("TreeArityMax",cached,resultBool,erreurMsg);
  leafM=superGraph->getLocalProperty<MetricProxy>("Leaf",cached,resultBool,erreurMsg);
  nodeM=superGraph->getLocalProperty<MetricProxy>("Node",cached,resultBool,erreurMsg);
  */

  arityM = new MetricProxy(superGraph);
  superGraph->computeProperty("TreeArityMax", arityM, erreurMsg);
  leafM = new MetricProxy(superGraph);
  superGraph->computeProperty("Leaf", leafM, erreurMsg);
  nodeM = new MetricProxy(superGraph);
  superGraph->computeProperty("Node", nodeM, erreurMsg);

  bool result=false;
  while (!result) {
    SelectionProxy *selectGood= superGraph->getLocalProperty<SelectionProxy>("tmpSelectionGood");
    SelectionProxy *selectBad= superGraph->getLocalProperty<SelectionProxy>("tmpSelectionBad");
    SuperGraph * saveSuper=superGraph;
    selectGood->setAllNodeValue(true);
    selectGood->setAllEdgeValue(true);
    selectBad->setAllNodeValue(false);
    selectBad->setAllEdgeValue(false);
    result = DfsClustering(rootNode,selectGood,selectBad);
    if (!result) {
      SuperGraph * tmpSubGraph,*tmpSubGraph2;
      tmpSubGraph = superGraph->addSubGraph(selectGood);
      tmpSubGraph2 = superGraph->addSubGraph(selectBad);
      superGraph=tmpSubGraph;
    }
    saveSuper->delLocalProperty("tmpSelectionGood");
    saveSuper->delLocalProperty("tmpSelectionBad");
  }

  delete nodeM;
  delete leafM;
  delete arityM;
  return true;
}
//================================================================================
bool ArityRecClustering::check(string &erreurMsg) {
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
void ArityRecClustering::reset() {
}




