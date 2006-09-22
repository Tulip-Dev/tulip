#include <map>
#include <cmath>
#include <fstream>
#include <tulip/TulipPlugin.h>
#include <tulip/TreeTest.h>
#include <tulip/TlpTools.h>

using namespace std;
/** \addtogroup layout */
/*@{*/
/// TreeRadial.cpp - An implementation of radial drawing of trees.
/** 
 *
 **/
class TreeRadial:public Layout {
public:
  MetricProxy *leaves;

  TreeRadial(const PropertyContext &context):Layout(context) {
  }

  void dfsPlacement(node n, int depth, double alphaStart, double alphaEnd) {
    double alpha=(alphaEnd+alphaStart)/2.0;
    if (depth>0) {
      double arcCos=acos((double)depth/((double)(depth+1)));
      if ((alphaEnd-alphaStart)> 2*arcCos) {
	alphaStart=alpha-arcCos;
	alphaEnd=alpha+arcCos;
      }
    } 
    layoutProxy->setNodeValue(n,Coord(((double)depth)*cos(alpha),((double)depth)*sin(alpha),0));
    if (superGraph->outdeg(n)==0) return;
    
    double sumM = leaves->getNodeValue(n);
    double counto = 0;
    double deltaAlpha = 2;
    double newAlphaStart;
    double newAlphaEnd;
    deltaAlpha=(alphaEnd-alphaStart)/(sumM);

    Iterator<node> *itN=superGraph->getOutNodes(n);
    while (itN->hasNext()) {
      node itn=itN->next();
      newAlphaStart= alphaStart + ((double)counto)*deltaAlpha;
      newAlphaEnd  = alphaStart + ((double)(counto+leaves->getNodeValue(itn)))*deltaAlpha;
      counto += leaves->getNodeValue(itn);
      double sizeTmp=(newAlphaEnd-newAlphaStart)/2*(depth+1);
      if (sizeTmp<0.5) {
	superGraph->getLocalProperty<SizesProxy>("viewSize")->setNodeValue(itn,Size( sizeTmp , sizeTmp , sizeTmp   ));
      }
      dfsPlacement(itn,depth+1,newAlphaStart,newAlphaEnd);
    } delete itN;
  }

  bool run() {
    node startNode;
    tlp::getSource(superGraph,startNode);
    superGraph->getLocalProperty<SizesProxy>("viewSize")->setAllNodeValue( Size(0.5,0.5,0.5));
  
    bool cached,resultBool;
    string erreurMsg;
    //    leaves = superGraph->getLocalProperty<MetricProxy>("Leaf",cached,resultBool,erreurMsg);
    leaves = new MetricProxy(superGraph);
    resultBool = superGraph->computeProperty("Leaf",leaves,erreurMsg);
    assert(resultBool);
    dfsPlacement(startNode,0,0,6.283);
    delete leaves;
    return true;
  }

  bool check(string &erreurMsg) {
    if (TreeTest::isTree(superGraph)) {
      erreurMsg="";
      return true;
    }
    else {
      erreurMsg="The Graph must be a Tree";
      return false;
    }
  }
  void reset() {}
};
/*@}*/
LAYOUTPLUGINOFGROUP(TreeRadial,"Tree Radial","David Auber","03/03/2001","Ok","0","1","Tree");
