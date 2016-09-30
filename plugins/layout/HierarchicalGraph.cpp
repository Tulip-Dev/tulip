/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#include <cassert>
#include <stack>

#include <tulip/SortIterator.h>
#include <tulip/StringCollection.h>

#include "HierarchicalGraph.h"
#include "DatasetTools.h"

PLUGIN(HierarchicalGraph)

using namespace std;
using namespace tlp;

static const int NB_UPDOWN_SWEEP = 4;

//================================================================================

static const char *paramHelp[] = {
    // orientation
    "This parameter enables to choose the orientation of the drawing."};

//================================================================================
#define ORIENTATION "horizontal;vertical;"
//================================================================================
HierarchicalGraph::HierarchicalGraph(const tlp::PluginContext *context) : LayoutAlgorithm(context) {
  addNodeSizePropertyParameter(this);
  addInParameter<StringCollection>("orientation", paramHelp[0], ORIENTATION, true, "horizontal <br> vertical");
  addSpacingParameters(this);
  addDependency("Dag Level", "1.0");
  addDependency("Hierarchical Tree (R-T Extended)", "1.1");
}
//================================================================================
HierarchicalGraph::~HierarchicalGraph() {
}
//================================================================================
class LessThanEdge {
public:
  DoubleProperty *metric;
  Graph *sg;
  bool operator()(edge e1, edge e2) {
    return (metric->getNodeValue(sg->source(e1)) < metric->getNodeValue(sg->source(e2)));
  }
};
//================================================================================
void HierarchicalGraph::buildGrid(tlp::Graph *sg) {
  string erreurMsg;
  DoubleProperty dagLevel(sg);

  if (!sg->applyPropertyAlgorithm("Dag Level", &dagLevel, erreurMsg)) {
    tlp::warning() << "[ERROR] : " << erreurMsg << __PRETTY_FUNCTION__ << endl;
    return;
  }

  for (node n : sg->getNodes()) {
    unsigned int level = (unsigned int)dagLevel.getNodeValue(n);

    while (level >= grid.size())
      grid.push_back(vector<node>());

    embedding->setNodeValue(n, grid[level].size());
    grid[level].push_back(n);
  }
}
//================================================================================
unsigned int HierarchicalGraph::degree(tlp::Graph *sg, tlp::node n, bool sense) {
  if (sense)
    return (sg->outdeg(n));
  else
    return (sg->indeg(n));
}
//================================================================================
// If sense==true fixed_layer is freeLayer+1 else freeLayer-1
// Compute barycenter heuristique

void HierarchicalGraph::twoLayerCrossReduction(tlp::Graph *sg, unsigned int freeLayer, bool) {
  vector<node>::const_iterator it;

  for (it = grid[freeLayer].begin(); it != grid[freeLayer].end(); ++it) {
    node n = *it;
    double sum = embedding->getNodeValue(n);
    for (node it : sg->getInOutNodes(n))
      sum += embedding->getNodeValue(it);
    embedding->setNodeValue(n, sum / (double(sg->deg(n)) + 1.0));
  }
}

//================================================================================
// Set initial position using a DFS
void HierarchicalGraph::initCross(tlp::Graph *sg, tlp::node n, tlp::MutableContainer<bool> &visited, int id) {
  if (visited.get(n.id))
    return;

  visited.set(n.id, true);
  embedding->setNodeValue(n, id);
  for (node it : sg->getOutNodes(n)) {
    initCross(sg, it, visited, id + 1);
  }
}
//================================================================================
// Do layer by layer sweep to reduce crossings in K-Layer graph
void HierarchicalGraph::crossReduction(tlp::Graph *sg) {

  node tmp = sg->addNode();
  embedding->setNodeValue(tmp, 0);
  for (node it : sg->getNodes()) {
    if (sg->outdeg(it) == 0)
      sg->addEdge(it, tmp);
  }
  grid.push_back(vector<node>());
  grid[grid.size() - 1].push_back(tmp);

  MutableContainer<bool> visited;
  visited.setAll(false);
  node root = sg->getSource();
  initCross(sg, root, visited, 1);

  for (unsigned int a = 0; a < grid.size(); ++a) {
    vector<node>::const_iterator it;
    unsigned int j = 0;
    stable_sort(grid[a].begin(), grid[a].end(), lessNode);

    for (it = grid[a].begin(); it != grid[a].end(); ++it) {
      embedding->setNodeValue(*it, j);
      j++;
    }
  }

  unsigned int maxDepth = grid.size();

  // Iterations of the sweeping
  for (int a = 0; a < NB_UPDOWN_SWEEP; ++a) {
    // Up sweeping
    for (int i = maxDepth - 1; i >= 0; --i) {
      twoLayerCrossReduction(graph, i, true);
    }

    // Down sweeping
    for (unsigned int i = 0; i < maxDepth; ++i) {
      twoLayerCrossReduction(graph, i, false);
    }
  }

  for (unsigned int a = 0; a < grid.size(); ++a) {
    vector<node>::const_iterator it;
    unsigned int j = 0;
    stable_sort(grid[a].begin(), grid[a].end(), lessNode);

    for (it = grid[a].begin(); it != grid[a].end(); ++it) {
      embedding->setNodeValue(*it, j);
      j++;
    }
  }

  sg->delNode(tmp, true);
}
//================================================================================
void HierarchicalGraph::DagLevelSpanningTree(tlp::Graph *sg, tlp::DoubleProperty *embedding) {
  assert(AcyclicTest::isAcyclic(sg));
  LessThanEdge tmpL;
  tmpL.metric = embedding;
  tmpL.sg = sg;
  for (node n : sg->getNodes()) {
    if (sg->indeg(n) > 1) {
      vector<edge> tmpVect;
      for (edge e : sg->getInEdges(n))
        tmpVect.push_back(e);
      sort(tmpVect.begin(), tmpVect.end(), tmpL);
      int toKeep = tmpVect.size() / 2;
      vector<edge>::const_iterator it;

      for (it = tmpVect.begin(); it != tmpVect.end(); ++it, --toKeep) {
        if (toKeep != 0)
          sg->delEdge(*it);
      }
    }
  }
  assert(TreeTest::isTree(sg));
}
//==============================================================================================================
void HierarchicalGraph::computeEdgeBends(const tlp::Graph *mySGraph, tlp::LayoutProperty &tmpLayout,
                                         const TLP_HASH_MAP<tlp::edge, tlp::edge> &replacedEdges, const std::vector<tlp::edge> &reversedEdges) {
  MutableContainer<bool> isReversed;
  isReversed.setAll(false);

  for (vector<edge>::const_iterator it = reversedEdges.begin(); it != reversedEdges.end(); ++it)
    isReversed.set((*it).id, true);

  for (TLP_HASH_MAP<edge, edge>::const_iterator it = replacedEdges.begin(); it != replacedEdges.end(); ++it) {
    edge toUpdate = (*it).first;
    edge start = (*it).second;
    edge end = start;
    Coord p1, p2;

    // we take the first and last point of the replaced edges
    while (graph->target(end) != graph->target(toUpdate)) {
      Iterator<edge> *itE = mySGraph->getOutEdges(graph->target(end));

      if (!itE->hasNext()) {
        delete itE;
        break;
      }

      end = itE->next();
      delete itE;
    }

    node firstN = graph->target(start);
    node endN = graph->source(end);
    LineType::RealType edgeLine;

    if (isReversed.get(toUpdate.id)) {
      p1 = tmpLayout.getNodeValue(endN);
      p2 = tmpLayout.getNodeValue(firstN);
    } else {
      p1 = tmpLayout.getNodeValue(firstN);
      p2 = tmpLayout.getNodeValue(endN);
    }

    if (p1 == p2)
      edgeLine.push_back(p1);
    else {
      edgeLine.push_back(p1);
      edgeLine.push_back(p2);
    }

    result->setEdgeValue(toUpdate, edgeLine);
  }
}
//=======================================================================
void HierarchicalGraph::computeSelfLoops(tlp::Graph *mySGraph, tlp::LayoutProperty &tmpLayout, std::vector<tlp::SelfLoops> &listSelfLoops) {
  while (!listSelfLoops.empty()) {
    tlp::SelfLoops tmp = listSelfLoops.back();
    listSelfLoops.pop_back();
    LineType::RealType tmpLCoord;
    const LineType::RealType &edge1 = tmpLayout.getEdgeValue(tmp.e1);
    const LineType::RealType &edge2 = tmpLayout.getEdgeValue(tmp.e2);
    const LineType::RealType &edge3 = tmpLayout.getEdgeValue(tmp.e3);
    LineType::RealType::const_iterator it;

    for (it = edge1.begin(); it != edge1.end(); ++it)
      tmpLCoord.push_back(*it);

    tmpLCoord.push_back(tmpLayout.getNodeValue(tmp.n1));

    for (it = edge2.begin(); it != edge2.end(); ++it)
      tmpLCoord.push_back(*it);

    tmpLCoord.push_back(tmpLayout.getNodeValue(tmp.n2));

    for (it = edge3.begin(); it != edge3.end(); ++it)
      tmpLCoord.push_back(*it);

    result->setEdgeValue(tmp.old, tmpLCoord);
    mySGraph->delNode(tmp.n1, true);
    mySGraph->delNode(tmp.n2, true);
  }
}

//=======================================================================
bool HierarchicalGraph::run() {

  orientation = "horizontal";
  spacing = 64.0;
  nodeSpacing = 18;
  SizeProperty *nodeSize = nullptr;

  if (dataSet != nullptr) {
    getNodeSizePropertyParameter(dataSet, nodeSize);
    getSpacingParameters(dataSet, nodeSpacing, spacing);
    StringCollection tmp;

    if (dataSet->get("orientation", tmp)) {
      orientation = tmp.getCurrentString();
    }
  }

  if (nodeSize == nullptr)
    nodeSize = graph->getProperty<SizeProperty>("viewSize");

  //=========================================================
  // use a temporary rotated size if necessary
  if (orientation == "horizontal") {
    SizeProperty *tmpSize = new SizeProperty(graph);

    tmpSize->copy(nodeSize);
    for (node n : graph->getNodes()) {
      const Size &tmp = tmpSize->getNodeValue(n);
      tmpSize->setNodeValue(n, Size(tmp[1], tmp[0], tmp[2]));
    }
    nodeSize = tmpSize;
  }

  // push a temporary graph state (not redoable)
  graph->push(false);
  result->setAllEdgeValue(std::vector<tlp::Coord>());

  //========================================================================
  // Build a clone of this graph
  Graph *mySGraph = graph->addCloneSubGraph("tmp clone");

  // if the graph is not acyclic we reverse edges to make it acyclic
  vector<tlp::SelfLoops> listSelfLoops;
  vector<edge> reversedEdges;
  AcyclicTest::makeAcyclic(mySGraph, reversedEdges, listSelfLoops);

  //========================================================================
  // We add a node and edges to force the dag to have only one source.
  tlp::makeSimpleSource(mySGraph);

  //========================================================================
  list<node> properAddedNodes;
  TLP_HASH_MAP<edge, edge> replacedEdges;
  IntegerProperty *edgeLength = nullptr;

  if (!TreeTest::isTree(mySGraph)) {
    // We transform the dag in a proper dag
    edgeLength = new IntegerProperty(mySGraph);
    tlp::makeProperDag(mySGraph, properAddedNodes, replacedEdges, edgeLength);
    // we compute metric for cross reduction
    DoubleProperty embed(mySGraph);
    embedding = &embed;
    lessNode.metric = embedding;
    buildGrid(mySGraph);
    crossReduction(mySGraph);

    for (node n : graph->getNodes()) {
      vector<edge> order;
      for (edge e : new SortTargetEdgeIterator(mySGraph->getOutEdges(n), mySGraph, embedding)) {
        order.push_back(e);
      }
      mySGraph->setEdgeOrder(n, order);
    }
    // We extract a spanning tree from the proper dag.
    DagLevelSpanningTree(mySGraph, &embed);
  } else {
    DoubleProperty embed(mySGraph);
    embedding = &embed;
    buildGrid(mySGraph);
  }

// We draw the tree using a tree drawing algorithm
#ifndef NDEBUG
  bool resultBool;
#endif
  string erreurMsg;
  LayoutProperty tmpLayout(graph);
  DataSet tmp;
  tmp.set("node size", nodeSize);
  tmp.set("layer spacing", spacing);
  tmp.set("node spacing", nodeSpacing);

  if (edgeLength != nullptr)
    tmp.set("edge length", edgeLength);

  tmp.set("orthogonal", true);
  StringCollection tmpS("vertical;horizontal;");
  tmpS.setCurrent("vertical");
  tmp.set("orientation", tmpS);
#ifndef NDEBUG
  resultBool =
#endif
      mySGraph->applyPropertyAlgorithm("Hierarchical Tree (R-T Extended)", &tmpLayout, erreurMsg, 0, &tmp);

  if (edgeLength)
    delete edgeLength;

  assert(resultBool);

  for (node n : graph->getNodes()) {
    result->setNodeValue(n, tmpLayout.getNodeValue(n));
  }

  computeEdgeBends(graph, tmpLayout, replacedEdges, reversedEdges);
  computeSelfLoops(graph, tmpLayout, listSelfLoops);

  // forget last temporary graph state
  graph->pop();

  // post processing
  // Prevent edge node overlaping
  std::vector<float> levelMaxSize(grid.size());
  MutableContainer<int> nodeLevel;

  for (unsigned int i = 0; i < grid.size(); ++i) {
    levelMaxSize[i] = 0;

    for (unsigned int j = 0; j < grid[i].size(); ++j) {
      node n = grid[i][j];

      if (graph->isElement(n)) {
        nodeLevel.set(n.id, i);
        const Size &tmp = nodeSize->getNodeValue(n);
        levelMaxSize[i] = std::max(levelMaxSize[i], tmp[1]);
      }
    }
  }

  for (edge e : graph->getEdges()) {
    std::pair<node, node> eEnds(graph->ends(e));
    node src = eEnds.first;
    node tgt = eEnds.second;

    if (src == tgt) {
      continue;
    }

    float rev = -1.0;

    if (nodeLevel.get(src.id) > nodeLevel.get(tgt.id)) {
      rev = 1.0;
    }

    Coord srcPos = result->getNodeValue(src);
    Coord tgtPos = result->getNodeValue(tgt);
    vector<Coord> old = result->getEdgeValue(e);

    if (old.empty()) {
      vector<Coord> pos(2);
      srcPos[1] += rev * (levelMaxSize[nodeLevel.get(src.id)] / 2.f + spacing / 4.f);
      tgtPos[1] -= rev * (levelMaxSize[nodeLevel.get(tgt.id)] / 2.f + spacing / 4.f);
      pos[0] = srcPos;
      pos[1] = tgtPos;
      result->setEdgeValue(e, pos);
    } else {
      vector<Coord> pos(4);
      srcPos[1] += rev * (levelMaxSize[nodeLevel.get(src.id)] / 2.f + spacing / 4.f);
      tgtPos[1] -= rev * (levelMaxSize[nodeLevel.get(tgt.id)] / 2.f + spacing / 4.f);
      Coord src2Pos = old.front();
      Coord tgt2Pos = old.back();
      src2Pos[1] = srcPos[1] + rev * spacing / 2.f;
      tgt2Pos[1] = tgtPos[1] - rev * spacing / 2.f;

      pos[0] = srcPos;
      pos[1] = src2Pos;
      pos[2] = tgt2Pos;
      pos[3] = tgtPos;
      result->setEdgeValue(e, pos);
    }
  }

  // post processing align nodes
  for (node n : graph->getNodes()) {
    Coord tmp = result->getNodeValue(n);
    const Size &tmpS = nodeSize->getNodeValue(n);
    tmp[1] -= (levelMaxSize[nodeLevel.get(n.id)] - tmpS[1]) / 2.f;
    result->setNodeValue(n, tmp);
  }

  // rotate layout
  if (orientation == "horizontal") {
    // delete the temporary allocated SizeProperty (see above)
    delete nodeSize;
    for (node n : graph->getNodes()) {
      const Coord &tmpC = result->getNodeValue(n);
      result->setNodeValue(n, Coord(-tmpC[1], tmpC[0], tmpC[2]));
    }

    for (edge e : graph->getEdges()) {
      LineType::RealType tmp = result->getEdgeValue(e);
      LineType::RealType tmp2;
      LineType::RealType::iterator it;

      for (it = tmp.begin(); it != tmp.end(); ++it) {
        tmp2.push_back(Coord(-(*it)[1], (*it)[0], (*it)[2]));
      }

      result->setEdgeValue(e, tmp2);
    }
  }

  return true;
}
