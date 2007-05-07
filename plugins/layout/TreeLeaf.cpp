#include <tulip/GraphTools.h>
#include "TreeLeaf.h"
#include "DatasetTools.h"
#include "Orientation.h"


LAYOUTPLUGINOFGROUP(TreeLeaf,"Tree Leaf","David Auber","01/12/1999","ok","1.0","Tree");

using namespace std;
using namespace tlp;

static int dfsPlacement(Graph* graph, node n, int &curPos, int depth,
			OrientableLayout *oriLayout) {
  int resultMin=0;
  int resultMax=0;
  int result=0;
  if (graph->outdeg(n)==0) {
    curPos+=2;
    oriLayout->setNodeValue(n,OrientableCoord(oriLayout, curPos,depth,0));
    return curPos;
  }
  Iterator<node> *itN=graph->getOutNodes(n);
  if (itN->hasNext()) {
    node itn=itN->next();
    result=dfsPlacement(graph, itn, curPos, depth+2, oriLayout);
    resultMin=result;
    resultMax=result;
  }
  for (;itN->hasNext();) {
    node itn=itN->next();
    result=dfsPlacement(graph, itn, curPos, depth+2, oriLayout);
    if (result>resultMax)
      resultMax=result;
    if (result<resultMin)
      resultMin=result;
  }
  delete itN;
  result=(resultMin+resultMax)/2;
  oriLayout->setNodeValue(n, OrientableCoord(oriLayout, result,depth,0));
  return result;
}

TreeLeaf::TreeLeaf(const PropertyContext &context):LayoutAlgorithm(context){
  addOrientationParameters(this);
}

TreeLeaf::~TreeLeaf() {}

bool TreeLeaf::run() {
  orientationType mask = getMask(dataSet);
  OrientableLayout oriLayout(layoutResult, mask);

  graph->getLocalProperty<SizeProperty>("viewSize")->setAllNodeValue(Size(1,1,1));
  graph->getLocalProperty<SizeProperty>("viewSize")->setAllEdgeValue(Size(0.125,0.125,0.5));

  Graph *tree = computeTree(graph);
  node tmpNode;
  tlp::getSource(tree, tmpNode);
  int x=0;

  dfsPlacement(tree, tmpNode, x, 0, &oriLayout);

  cleanComputedTree(graph, tree);
  return true;
}
