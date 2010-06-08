#include <math.h>
#include <tulip/TulipPlugin.h>
#include "DatasetTools.h"

using namespace std;
using namespace tlp;

/** \addtogroup layout */
/*@{*/
/// TreeRadial.cpp - An implementation of a radial drawing of trees.
/** 
 * This algorithm is inspired from
 * MoireGraphs: Radial Focus+Context Visualization and Interaction for Graphs with Visual Nodes
 * from T. J. Jankun-Kelly, Kwan-Liu Ma
 * published in IEEE Symposium on Information Visualization (2003)
 **/
class TreeRadial:public LayoutAlgorithm {
public:
  Graph *tree;
  vector<float> nRadii;
  vector<float> lRadii;
  vector<vector <node> > bfs;

  void dfsComputeNodeRadii(node n, unsigned depth, SizeProperty *sizes) {
    node on;
    float radius = sizes->getNodeValue(n).getW()/2;
    if (bfs.size() == depth) {
      bfs.push_back(vector<node>());
      nRadii.push_back(radius);
    } else if (radius > nRadii[depth]) {
	nRadii[depth] = radius;
    }
    bfs[depth].push_back(n);
    forEach(on, tree->getOutNodes(n)) {
      dfsComputeNodeRadii(on, depth + 1, sizes);
    }
  }

  void bfsComputeLayerRadii(float lSpacing, float nSpacing, SizeProperty *) {
    if (bfs.size() < 2) return;
    
    float lRadius = 0, lSpacingMax = 0;
    lRadii.push_back(0);
    unsigned int nbLayers = bfs.size() - 1;
    for (unsigned int i = 0; i < nbLayers; ++i) {
      float lRadiusPrev = lRadius;
      lRadius += nRadii[i] + nRadii[i + 1] + lSpacing;
      // check if there is enough space for nodes of layer i + 1
      float mRadius = (bfs[i + 1].size() * (nRadii[i + 1] + nSpacing))/(2 * M_PI);
      if (mRadius > lRadius)
	lRadius = mRadius;
      lRadii.push_back(lRadius);
      if ((lRadius - lRadiusPrev) > lSpacingMax)
	lSpacingMax = lRadius - lRadiusPrev;
    }
    ++nbLayers;
    lRadius = lSpacingMax;
    for (unsigned int i = 1; i < nbLayers; ++i, lRadius += lSpacingMax) {
      lRadii[i] = lRadius;
    }
  }
    

  double dfsComputeAngularSpread(node n, unsigned int depth,
				 SizeProperty *sizes, DoubleProperty *angles) {
    node on;
    double cAngle = 0;
    forEach(on, tree->getOutNodes(n)) {
      // compute the sum of the childs's angular spreads
      cAngle += dfsComputeAngularSpread(on, depth + 1, sizes, angles);
    }
    if (depth > 0) {
      // compute the node angular spread
      double nAngle = 2 * atan(sizes->getNodeValue(n).getW()/(2. * lRadii[depth]));
      // check if it is not greater than the sum
      if (nAngle > cAngle)
	cAngle = nAngle;
    }
    // affect the greater of the two computed angular spreads
    angles->setNodeValue(n, cAngle);
    return cAngle;
  }

  void doLayout(node n, unsigned int depth, double startAngle, double endAngle,
		DoubleProperty *angles, bool checkAngle = false) {
    double sAngle = endAngle - startAngle;
    // this will avoid crossing between the egdes from n to its children
    // and the edge from its ancestor to n
    if (checkAngle && sAngle > M_PI) {
      endAngle = startAngle + M_PI;
      sAngle = M_PI;
    }
    if (depth > 0) {
      // layout the node in the middle of the sector
      double nAngle = (startAngle + endAngle)/2.0;
      layoutResult->setNodeValue(n, Coord(lRadii[depth] * cos(nAngle),
					  lRadii[depth] * sin(nAngle),
					  0));
    } else
      layoutResult->setNodeValue(n, Coord(0, 0, 0));
    node on;
    const double& nSpread = angles->getNodeValue(n);
    checkAngle = false;
    forEach(on, tree->getOutNodes(n)) {
      endAngle = startAngle + (sAngle * (angles->getNodeValue(on)/nSpread));
      doLayout(on, depth + 1, startAngle, endAngle, angles, checkAngle);
      checkAngle = true;
      startAngle = endAngle;
    }
  }

  TreeRadial(const PropertyContext &context):LayoutAlgorithm(context) {
    addNodeSizePropertyParameter(this);
    addSpacingParameters(this);
    addDependency<LayoutAlgorithm>("Tree Leaf", "1.0");
  }

  bool run() {
    if (pluginProgress)
      pluginProgress->showPreview(false);
    tree = TreeTest::computeTree(graph, 0, false, pluginProgress);
    if (pluginProgress && pluginProgress->state() != TLP_CONTINUE)
      return false;

    float nSpacing, lSpacing;
    SizeProperty* sizes;
    if (!getNodeSizePropertyParameter(dataSet, sizes))
      sizes = graph->getProperty<SizeProperty>("viewSize");
    getSpacingParameters(dataSet, nSpacing, lSpacing);
    // ensure size updates will be kept after a pop
    preservePropertyUpdates(sizes);

    LayoutProperty tmpLayout(graph);
  
    node n;
    SizeProperty *circleSizes = new SizeProperty(graph);
    forEach(n, tree->getNodes()) {
      const Size& boundingBox = sizes->getNodeValue (n);
      double diam = 2*sqrt (boundingBox.getW() * boundingBox.getW()/4.0 +
			    boundingBox.getH() * boundingBox.getH()/4.0);
      circleSizes->setNodeValue (n, Size (diam, diam, 1.0));
    }
    sizes = circleSizes;

    bool resultBool;
    node root;
    resultBool = tlp::getSource(tree, root);
    assert(resultBool);

    dfsComputeNodeRadii(root, 0, sizes);
    bfsComputeLayerRadii(lSpacing, nSpacing, sizes);
    DoubleProperty angles(tree);
    dfsComputeAngularSpread(root, 0, sizes, &angles);
    doLayout(root, 0, 0., 2 * M_PI, &angles);

    delete sizes;

    // if not in tulip gui, ensure cleanup
    LayoutProperty* elementLayout;
    if (!graph->getAttribute("viewLayout", elementLayout))
      TreeTest::cleanComputedTree(graph, tree);

    return true;
  }
};
/*@}*/
LAYOUTPLUGINOFGROUP(TreeRadial,"Tree Radial","Patrick Mary","14/05/2007","Ok","1.0","Tree");

