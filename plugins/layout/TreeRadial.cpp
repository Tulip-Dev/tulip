#include <tulip/TulipPlugin.h>

using namespace std;
using namespace tlp;

/** \addtogroup layout */
/*@{*/
/// TreeRadial.cpp - An implementation of radial drawing of trees.
/** 
 *
 **/
class TreeRadial:public LayoutAlgorithm {
public:
  DoubleProperty *leaves;
  Graph *tree;

  TreeRadial(const PropertyContext &context):LayoutAlgorithm(context) {
    addDependency<DoubleAlgorithm>("Leaf", "1.0");
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
    layoutResult->setNodeValue(n,Coord(((double)depth)*cos(alpha),((double)depth)*sin(alpha),0));
    if (tree->outdeg(n)==0) return;
    
    double sumM = leaves->getNodeValue(n);
    double counto = 0;
    double deltaAlpha = 2;
    double newAlphaStart;
    double newAlphaEnd;
    deltaAlpha=(alphaEnd-alphaStart)/(sumM);

    Iterator<node> *itN=tree->getOutNodes(n);
    while (itN->hasNext()) {
      node itn=itN->next();
      newAlphaStart= alphaStart + ((double)counto)*deltaAlpha;
      newAlphaEnd  = alphaStart + ((double)(counto+leaves->getNodeValue(itn)))*deltaAlpha;
      counto += leaves->getNodeValue(itn);
      double sizeTmp=(newAlphaEnd-newAlphaStart)/2*(depth+1);
      if (sizeTmp<0.5) {
	tree->getLocalProperty<SizeProperty>("viewSize")->setNodeValue(itn,Size( sizeTmp , sizeTmp , sizeTmp   ));
      }
      dfsPlacement(itn,depth+1,newAlphaStart,newAlphaEnd);
    } delete itN;
  }

  bool run() {
    vector<node> addedNodes;
    tree = computeTree(graph, addedNodes);
    node startNode;
    tlp::getSource(tree, startNode);
    graph->getLocalProperty<SizeProperty>("viewSize")->setAllNodeValue( Size(0.5,0.5,0.5));
  
    bool resultBool;
    string erreurMsg;
    leaves = new DoubleProperty(tree);
    resultBool = tree->computeProperty("Leaf",leaves,erreurMsg);
    assert(resultBool);
    dfsPlacement(startNode,0,0,6.283);
    delete leaves;

    cleanComputedTree(graph, tree, addedNodes);
    return true;
  }
};
/*@}*/
LAYOUTPLUGINOFGROUP(TreeRadial,"Tree Radial","David Auber","03/03/2001","Ok","1.0","Tree");
