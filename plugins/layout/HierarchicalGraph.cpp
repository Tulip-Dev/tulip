/**
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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
  addInParameter<StringCollection>("orientation", paramHelp[0], ORIENTATION, true,
                                   "horizontal <br> vertical");
  addSpacingParameters(this);
  addDependency("Hierarchical Tree (R-T Extended)", "1.1");
}
//================================================================================
HierarchicalGraph::~HierarchicalGraph() {}
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
  tlp::NodeStaticProperty<unsigned int> levels(sg);
  dagLevel(graph, levels);

  TLP_MAP_NODES_AND_INDICES(graph, [&](const node n, unsigned int i) {
    unsigned int level = levels[i];

    if (level >= grid.size())
      grid.resize(level + 1);

    embedding->setNodeValue(n, grid[level].size());
    grid[level].push_back(n);
  });
}
//================================================================================
inline unsigned int HierarchicalGraph::degree(tlp::Graph *sg, tlp::node n, bool directed) {
  return directed ? sg->outdeg(n) : sg->indeg(n);
}
//================================================================================
void HierarchicalGraph::twoLayerCrossReduction(tlp::Graph *sg, unsigned int freeLayer) {
  for (auto n : grid[freeLayer]) {
    double sum = embedding->getNodeValue(n);
    unsigned int deg = 1;
    for (auto itn : sg->getInOutNodes(n)) {
      sum += embedding->getNodeValue(itn);
      ++deg;
    }
    embedding->setNodeValue(n, sum / deg);
  }
}
//================================================================================
// Set initial position using a DFS
void HierarchicalGraph::initCross(tlp::Graph *sg, tlp::node n, tlp::MutableContainer<bool> &visited,
                                  int id) {
  if (visited.get(n.id))
    return;

  visited.set(n.id, true);
  embedding->setNodeValue(n, id);
  for (auto it : sg->getOutNodes(n)) {
    initCross(sg, it, visited, id + 1);
  }
}
//================================================================================
// Do layer by layer sweep to reduce crossings in K-Layer graph
void HierarchicalGraph::crossReduction(tlp::Graph *sg) {

  node tmp = sg->addNode();
  embedding->setNodeValue(tmp, 0);
  for (auto it : sg->nodes()) {
    if (sg->outdeg(it) == 0)
      sg->addEdge(it, tmp);
  }
  grid.push_back(vector<node>(1, tmp));
  {
    MutableContainer<bool> visited;
    visited.setAll(false);
    node root = sg->getSource();
    initCross(sg, root, visited, 1);
  }

  unsigned int maxDepth = grid.size();

  for (unsigned int i = 0; i < maxDepth; ++i) {
    vector<node> &igrid = grid[i];
    stable_sort(igrid.begin(), igrid.end(), lessNode);

    for (unsigned int j = 0; j < igrid.size(); ++j)
      embedding->setNodeValue(igrid[j], j);
  }

  // Iterations of the sweeping
  for (unsigned int a = 0; a < NB_UPDOWN_SWEEP; ++a) {
    // Up sweeping
    for (int i = maxDepth - 1; i >= 0; --i) {
      twoLayerCrossReduction(graph, i);
    }

    // Down sweeping
    for (unsigned int i = 0; i < maxDepth; ++i) {
      twoLayerCrossReduction(graph, i);
    }
  }

  for (unsigned int i = 0; i < maxDepth; ++i) {
    vector<node> &igrid = grid[i];
    stable_sort(igrid.begin(), igrid.end(), lessNode);

    for (unsigned int j = 0; j < igrid.size(); ++j)
      embedding->setNodeValue(igrid[j], j);
  }

  sg->delNode(tmp, true);
}
//================================================================================
void HierarchicalGraph::DagLevelSpanningTree(tlp::Graph *sg, tlp::DoubleProperty *embedding) {
  assert(AcyclicTest::isAcyclic(sg));
  LessThanEdge tmpL;
  tmpL.metric = embedding;
  tmpL.sg = sg;
  for (auto n : sg->nodes()) {
    if (sg->indeg(n) > 1) {
      vector<edge> tmpVect;
      for (auto e : sg->getInEdges(n))
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
                                         const tlp_hash_map<tlp::edge, tlp::edge> &replacedEdges,
                                         const std::vector<tlp::edge> &reversedEdges) {
  MutableContainer<bool> isReversed;
  isReversed.setAll(false);

  for (auto e : reversedEdges)
    isReversed.set(e.id, true);

  for (auto &it : replacedEdges) {
    edge toUpdate = it.first;
    edge start = it.second;
    edge end = start;
    node tgt;

    // we take the first and last point of the replaced edges
    while ((tgt = graph->target(end)) != graph->target(toUpdate)) {
      Iterator<edge> *itE = mySGraph->getOutEdges(tgt);

      if (!itE->hasNext()) {
        delete itE;
        break;
      }

      end = itE->next();
      delete itE;
    }

    node firstN = graph->target(start);
    node endN = graph->source(end);
    Coord p1, p2;

    if (isReversed.get(toUpdate.id)) {
      p1 = tmpLayout.getNodeValue(endN);
      p2 = tmpLayout.getNodeValue(firstN);
    } else {
      p1 = tmpLayout.getNodeValue(firstN);
      p2 = tmpLayout.getNodeValue(endN);
    }

    LineType::RealType edgeLine;

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
void HierarchicalGraph::computeSelfLoops(tlp::Graph *mySGraph, tlp::LayoutProperty &tmpLayout,
                                         std::vector<tlp::SelfLoops> &listSelfLoops) {
  // tlp::warning() << "We compute self loops" << endl;
  while (!listSelfLoops.empty()) {
    tlp::SelfLoops tmp = listSelfLoops.back();
    listSelfLoops.pop_back();
    const LineType::RealType &edge1 = tmpLayout.getEdgeValue(tmp.e1);
    const LineType::RealType &edge2 = tmpLayout.getEdgeValue(tmp.e2);
    const LineType::RealType &edge3 = tmpLayout.getEdgeValue(tmp.e3);
    LineType::RealType tmpLCoord;
    tmpLCoord.reserve(edge1.size() + edge2.size() + edge3.size() + 2);

    tmpLCoord.insert(tmpLCoord.end(), edge1.begin(), edge1.end());
    tmpLCoord.push_back(tmpLayout.getNodeValue(tmp.n1));
    tmpLCoord.insert(tmpLCoord.end(), edge2.begin(), edge2.end());
    tmpLCoord.push_back(tmpLayout.getNodeValue(tmp.n2));
    tmpLCoord.insert(tmpLCoord.end(), edge3.begin(), edge3.end());

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
    for (auto n : graph->nodes()) {
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
  tlp_hash_map<edge, edge> replacedEdges;
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
    for (auto n : graph->nodes()) {
      vector<edge> order =
          iteratorVector(new SortTargetEdgeIterator(mySGraph->getOutEdges(n), mySGraph, embedding));
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
      mySGraph->applyPropertyAlgorithm("Hierarchical Tree (R-T Extended)", &tmpLayout, erreurMsg,
                                       &tmp);

  if (edgeLength)
    delete edgeLength;

  assert(resultBool);

  for (auto n : graph->nodes()) {
    result->setNodeValue(n, tmpLayout.getNodeValue(n));
  }

  computeEdgeBends(graph, tmpLayout, replacedEdges, reversedEdges);
  computeSelfLoops(graph, tmpLayout, listSelfLoops);

  // forget last temporary graph state
  graph->pop();

  // post processing
  // Prevent edge node overlapping
  unsigned int nbGrids = grid.size();
  std::vector<float> levelMaxSize(nbGrids);
  MutableContainer<unsigned int> nodeLevel;

  for (unsigned int i = 0; i < nbGrids; ++i) {
    float levelMax = levelMaxSize[i] = 0;
    std::vector<node> &igrid = grid[i];
    unsigned int nbNodes = igrid.size();

    for (unsigned int j = 0; j < nbNodes; ++j) {
      node n = igrid[j];

      if (graph->isElement(n)) {
        nodeLevel.set(n.id, i);
        const Size &tmp = nodeSize->getNodeValue(n);
        levelMax = levelMaxSize[i] = std::max(levelMax, tmp[1]);
      }
    }
  }

  float spacing_4 = spacing / 4.f;
  for (auto e : graph->edges()) {
    std::pair<node, node> eEnds(graph->ends(e));
    node src = eEnds.first;
    node tgt = eEnds.second;

    if (src == tgt) {
      continue;
    }

    unsigned int srcLevel = nodeLevel.get(src.id);
    unsigned int tgtLevel = nodeLevel.get(tgt.id);
    Coord srcPos = result->getNodeValue(src);
    Coord tgtPos = result->getNodeValue(tgt);
    float curSpacing;

    if (srcLevel > tgtLevel) {
      srcPos[1] += (levelMaxSize[srcLevel] / 2.f + spacing_4);
      tgtPos[1] -= (levelMaxSize[tgtLevel] / 2.f + spacing_4);
      curSpacing = spacing / 2.f;
    } else {
      srcPos[1] -= (levelMaxSize[srcLevel] / 2.f + spacing_4);
      tgtPos[1] += (levelMaxSize[tgtLevel] / 2.f + spacing_4);
      curSpacing = -spacing / 2.f;
    }

    const vector<Coord> &old = result->getEdgeValue(e);

    if (old.empty()) {
      vector<Coord> pos(2);
      pos[0] = srcPos;
      pos[1] = tgtPos;
      result->setEdgeValue(e, pos);
    } else {
      vector<Coord> pos(4);
      Coord src2Pos = old.front();
      Coord tgt2Pos = old.back();
      src2Pos[1] = srcPos[1] + curSpacing;
      tgt2Pos[1] = tgtPos[1] - curSpacing;
      pos[0] = srcPos;
      pos[1] = src2Pos;
      pos[2] = tgt2Pos;
      pos[3] = tgtPos;
      result->setEdgeValue(e, pos);
    }
  }

  // post processing align nodes
  for (auto n : graph->nodes()) {
    Coord tmp = result->getNodeValue(n);
    const Size &tmpS = nodeSize->getNodeValue(n);
    tmp[1] -= (levelMaxSize[nodeLevel.get(n.id)] - tmpS[1]) / 2.f;
    result->setNodeValue(n, tmp);
  }

  // rotate layout
  if (orientation == "horizontal") {
    // delete the temporary allocated SizeProperty (see above)
    delete nodeSize;
    for (auto n : graph->nodes()) {
      const Coord &tmpC = result->getNodeValue(n);
      result->setNodeValue(n, Coord(-tmpC[1], tmpC[0], tmpC[2]));
    }
    for (auto e : graph->edges()) {
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
