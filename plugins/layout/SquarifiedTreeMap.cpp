#include <cmath>
#include <float.h>
#include <algorithm>
#include <tulip/TreeTest.h>
#include "tulip/RectangleArea.h"
#include "tulip/Border.h"
#include "SquarifiedTreeMap.h"
#include "TreeTools.h"

using namespace std;
using namespace tlp;

LAYOUTPLUGINOFGROUP(SquarifiedTreeMap,"Squarified Tree Map",
                    "Tulip Team",
                    "25/05/2010", "ok", "2.0", "Tree");
//====================================================================

const double SEPARATION_Z  = 150; 
const double DEFAULT_RATIO = 1.;
const int DEFAULT_WIDTH   = 1024;
const int DEFAULT_HEIGHT  = 1024;

const int TEXTUREDGLYPHID = 101;

namespace 
{
  const char * paramHelp[] = 
    {
      // metric :
      HTML_HELP_OPEN() \
      HTML_HELP_DEF( "type", "Metric" ) \
      HTML_HELP_DEF( "values", "An existing metric property" ) \
      HTML_HELP_DEF( "default", "viewMetric if it exists" ) \
      HTML_HELP_BODY() \
      "This parameter defines the metric used to estimate the size allocated to each node." \
      HTML_HELP_CLOSE(),
      // aspect ratio
      HTML_HELP_OPEN()				 \
      HTML_HELP_DEF( "type", "float" ) \
      HTML_HELP_DEF( "default", "1." )	 \
      HTML_HELP_BODY() \
      "This parameter enables to set up the aspect ratio (height/width) for the rectangle corresponding to the root node." \
      HTML_HELP_CLOSE(),
      // texture
      HTML_HELP_OPEN() \
      HTML_HELP_DEF( "type", "bool" ) \
      HTML_HELP_DEF( "values", "[true, false]" ) \
      HTML_HELP_DEF( "default", "false" ) \
      HTML_HELP_BODY() \
      "This parameter indicates if the glyphs representing nodes are textured or not." \
      HTML_HELP_CLOSE(),
    };
}

//====================================================================
SquarifiedTreeMap::SquarifiedTreeMap(const PropertyContext& context) :LayoutAlgorithm(context){
  aspectRatio = DEFAULT_RATIO;
  addParameter<DoubleProperty>("metric", paramHelp[0], 0, false);
  addParameter<double>("Aspect Ratio", paramHelp[1], "1.");
  addParameter<bool>("Texture?", paramHelp[2], "false");
}

//====================================================================
SquarifiedTreeMap::~SquarifiedTreeMap() {
}

//====================================================================
bool SquarifiedTreeMap::check(string& errorMsg) {
  if (!TreeTest::isTree(graph)) {
    errorMsg = "The Graph must be a Tree";
    return false;
  }
    
  metric = NULL;
  if (dataSet != 0)
    dataSet->get("metric", metric);
      
  if (!metric && graph->existProperty("viewMetric")) {
    metric = graph->getProperty<DoubleProperty>("viewMetric");
    if (metric->getNodeMin() < 0.) {
      errorMsg = "Graph's nodes must have positive metric";
      return false;
    }
  }
  errorMsg = "";
  return true;
}

//====================================================================
/**
  *
  *  @todo manage correctly parameters remove texture mode, enable to choose bordersize + header size
  */
bool SquarifiedTreeMap::run() {
  double aspectRatio  = DEFAULT_RATIO;
  bool glyphTextured = false;
    
  size  = graph->getProperty<SizeProperty>("viewSize");    
  // ensure size updates will be kept after a pop
  preservePropertyUpdates(size);

  if (dataSet != 0) {
    dataSet->get("Aspect Ratio", aspectRatio);
    dataSet->get("Texture?", glyphTextured);
  }
    
  glyph = graph->getProperty<IntegerProperty>("viewShape"); 
    
  if (glyphTextured) {
    // ensure glyph updates will be kept after a pop
    //preservePropertyUpdates(glyph);
    //glyph->setAllNodeValue(TEXTUREDGLYPHID);
  }

  Rectangle<double> initialSpace(0, 0, DEFAULT_WIDTH * aspectRatio, DEFAULT_HEIGHT);

  node root;
  tlp::getSource(graph, root);
  computeNodesSize(root);

  Vec2d center = initialSpace.center();
  layoutResult->setNodeValue(root, Coord(center[0], center[1], 0));
  Size initialSpaceSize(initialSpace.width(), initialSpace.height(), 0);
  size->setNodeValue(root, initialSpaceSize);
  vector<node> toTreat(orderedChildren(root));
  if (!toTreat.empty()) {
      Rectangle<double> tmp = adjustRectangle(initialSpace);
      squarify(toTreat, tmp, 1);
  }

  return true;
}
//====================================================================
tlp::Rectangle<double> SquarifiedTreeMap::adjustRectangle(const tlp::Rectangle<double> &r) const {
    assert(r.isValid());
    Rectangle<double> result(r);
    Vec2d dist(r[1] - r[0]);
    //header size
    result[1][1] -= dist[1] * 0.1;
    //border size
    result[1][1] -= dist[1] * 0.02 * 2.;
    result[1][0] -= dist[0] * 0.02;
    result[0][0] += dist[0] * 0.02;
    result[0][1] += dist[1] * 0.02;

    assert(result.isValid());
    return result;
}
//====================================================================
void SquarifiedTreeMap::layoutRow(const vector<node> &row, const int depth, const Rectangle<double> &rectArea) {
    assert(rectArea.isValid());
    assert(!row.empty());
    vector<node>::const_iterator  it;
    double rowArea = 0;
    for (it=row.begin(); it!=row.end(); ++it)
        rowArea += nodesSize.get(it->id);

    double sum = 0;
    Vec2d dist = rectArea[1] - rectArea[0];
    for (it = row.begin(); it!=row.end(); ++it) {
        Rectangle<double> layoutRec(rectArea);
        if (rectArea.width() > rectArea.height()) {
            layoutRec[0][0] = rectArea[0][0] + (sum/rowArea) * dist[0];
            layoutRec[1][0] = layoutRec[0][0] + (nodesSize.get(it->id) / rowArea) * dist[0];
        }
        else {
            layoutRec[0][1] =  rectArea[0][1] + (sum/rowArea) * dist[1];
            layoutRec[1][1] = layoutRec[0][1] + (nodesSize.get(it->id) / rowArea) * dist[1];
        }
        assert(layoutRec.isValid());
        sum += nodesSize.get(it->id);
        Vec2d center = layoutRec.center();
        layoutResult->setNodeValue(*it, Coord(center[0], center[1], depth * SEPARATION_Z));
        size->setNodeValue(*it, Size(layoutRec.width(), layoutRec.height(), 0));

        if (graph->outdeg(*it) > 0) {
            vector<node> toTreat(orderedChildren(*it));
            Rectangle<double> newRec(adjustRectangle(layoutRec));
            squarify(toTreat, newRec, depth);
        }
    }
}
//==================================================================
class IsGreater {
public:
    IsGreater(const tlp::MutableContainer<double> &measure):measure(measure) {
    }
    bool operator()(const node a, const node b) const {
        return measure.get(a.id) > measure.get(b.id);
    }
    const tlp::MutableContainer<double> &measure;
};
//======================================
vector<node> SquarifiedTreeMap::orderedChildren(const node n) const {
    //sort children of n and store it in result
    //======================================
    vector<node> result;
    //build a list of pair <node, size>
    node child;
    forEach(child, graph->getOutNodes(n)) {
        result.push_back(child);
    }
    IsGreater sortFunctor(nodesSize);
    sort(result.begin(), result.end(), sortFunctor);
    return result;
}
//==========================================================
double SquarifiedTreeMap::evaluateRow(const vector<node> &row, node n, double width, double length, double surface) {

    double sumOfNodesSurface = nodesSize.get(n.id);
    vector<node>::const_iterator  it;
    for (it = row.begin(); it!=row.end(); ++it) {
        sumOfNodesSurface += nodesSize.get(it->id);
    }
    //====================
    double size = nodesSize.get(n.id);
    // ratio is the aspect ratio of rectangle of the considered elements
    double nodeRectangleWidth = length * sumOfNodesSurface/surface;
    double nodeRectangleHeight = width * size/sumOfNodesSurface;
    double ratio = std::min( nodeRectangleHeight, nodeRectangleWidth) /
                   std::max( nodeRectangleHeight, nodeRectangleWidth);
    double minratio = ratio;
    double maxratio = ratio;
    double sumratio = ratio;
    for (it = row.begin(); it!=row.end(); ++it) {
        double size = nodesSize.get(it->id);
        double nodeRectangleWidth  = length * sumOfNodesSurface/surface;
        double nodeRectangleHeight = width * size/sumOfNodesSurface;
        double ratio = std::min( nodeRectangleHeight, nodeRectangleWidth) /
                       std::max( nodeRectangleHeight, nodeRectangleWidth);
        sumratio += ratio;
        minratio = std::min(minratio, ratio);
        maxratio = std::max(maxratio, ratio);
    }
    // The paper formula does not give the best result
    //return std::max(surface*surface*maxratio/(sumOfNodesSurface*sumOfNodesSurface),
    //                sumOfNodesSurface*sumOfNodesSurface / (surface*surface*minratio));
    return sumratio / (row.size() + 1);
}

//====================================================================
void SquarifiedTreeMap::squarify(const vector<node> &toTreat, const Rectangle<double> &rectArea, const int depth) {
  assert(rectArea.isValid());
  assert(!toTreat.empty());

  vector<node> rowNodes;
  vector<node>::const_iterator  it;
  double surface = 0;
  for (it = toTreat.begin(); it!=toTreat.end(); ++it)
      surface += nodesSize.get(it->id);

  it = toTreat.begin();
  double length = std::max(rectArea.width(), rectArea.height());
  double width  = std::min(rectArea.width(), rectArea.height());
  double ratio = evaluateRow(rowNodes, *it, width, length, surface);
  rowNodes.push_back(*it);
  ++it;
  //build the new row
  while (it != toTreat.end()) { //add node in the current row while condition is ok
      double newRatio = evaluateRow(rowNodes, *it, width, length, surface);
      if (newRatio < ratio) break; //we finish to build that row
      ratio = newRatio;
      rowNodes.push_back(*it); //add the node to the current row
      ++it;
  }

  //Compute measure on unTreated nodes to do a recursive call
  vector<node> unTreated;
  double unTreatedSurface = 0;
  while (it != toTreat.end()) {
      unTreated.push_back(*it);
      unTreatedSurface += nodesSize.get(it->id);
      ++it;
  }
  assert(unTreated.size() + rowNodes.size() == toTreat.size());

  Vec2d dist = rectArea[1] - rectArea[0];
  assert(!rowNodes.empty());

  Rectangle<double> rowRec(rectArea); //The rectangle for that row
  if (rectArea.width() > rectArea.height())
      rowRec[1][0] -= (unTreatedSurface/surface) * dist[0];
  else
      rowRec[0][1] += (unTreatedSurface/surface) * dist[1];
  assert(rowRec.isValid());

  layoutRow(rowNodes, depth, rowRec);

  if (!unTreated.empty()) {
      Rectangle<double> subRec(rectArea); //the rectangle of unTreated nodes
      if (rectArea.width() > rectArea.height())
          subRec[0][0] = rowRec[1][0];
      else
          subRec[1][1] = rowRec[0][1];
      assert(subRec.isValid());
      squarify(unTreated, subRec, depth);
  }

}
//====================================================================
void SquarifiedTreeMap::computeNodesSize(const node n) {

    if (graph->outdeg(n) == 0) { //the node is a leaf of the tree
        double leafValue = 1.;
        if (metric != 0) { //if there is a user defined metric on leaves use it.
            if (metric->getNodeValue(n) > 0)
                leafValue = metric->getNodeValue(n);
        }
        nodesSize.set(n.id , leafValue);
        return;
    }

    double internalNodeValue = 0.;
    node child;
    forEach(child, graph->getOutNodes(n)) {
        computeNodesSize(child);
        internalNodeValue += nodesSize.get(child.id);
    }

    nodesSize.set(n.id , internalNodeValue);
}
//====================================================================
