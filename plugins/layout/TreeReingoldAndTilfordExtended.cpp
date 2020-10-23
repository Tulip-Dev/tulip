/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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
#include <tulip/GraphTools.h>
#include <tulip/StringCollection.h>
#include <tulip/IntegerProperty.h>
#include <tulip/TreeTest.h>

#include "TreeReingoldAndTilfordExtended.h"
#include "DatasetTools.h"

PLUGIN(TreeReingoldAndTilfordExtended)

using namespace std;
using namespace tlp;

static const char *paramHelp[] = {
    // edge length
    "This parameter indicates the property used to compute the length of edges.",

    // orientation
    "This parameter enables to choose the orientation of the drawing.",

    // orthogonal
    "This parameter enables to choose if the tree is drawn orthogonally or not.",

    // bounding circles
    "Indicates if the node bounding objects are boxes or bounding circles.",

    // compact layout
    "Indicates if a compact layout is computed."};
//=============================================================================
#define ORIENTATION "vertical;horizontal;"
//=============================================================================
TreeReingoldAndTilfordExtended::TreeReingoldAndTilfordExtended(const tlp::PluginContext *context)
    : LayoutAlgorithm(context), lengthMetric(nullptr) {
  addNodeSizePropertyParameter(this);
  addInParameter<IntegerProperty>("edge length", paramHelp[0], "", false);
  addInParameter<StringCollection>("orientation", paramHelp[1], ORIENTATION, true,
                                   "vertical <br> horizontal");
  addInParameter<bool>("orthogonal", paramHelp[2], "true");
  addSpacingParameters(this);
  addInParameter<bool>("bounding circles", paramHelp[3], "false");
  addInParameter<bool>("compact layout", paramHelp[4], "true");
}
//=============================================================================
TreeReingoldAndTilfordExtended::~TreeReingoldAndTilfordExtended() {}
//=============================================================================
double TreeReingoldAndTilfordExtended::calcDecal(const std::list<LR> &arbreG,
                                                 const std::list<LR> &arbreD) {
  list<LR>::const_iterator itG, itD;
  double decal = 0;
  int iG = 0, iD = 0;
  itG = arbreG.begin();
  itD = arbreD.begin();
  decal = ((*itG).R - (*itD).L + nodeSpacing);
  iG += std::min((*itG).size, (*itD).size);
  iD += std::min((*itG).size, (*itD).size);

  if (iG == (*itG).size) {
    ++itG;
    iG = 0;
  }

  if (iD == (*itD).size) {
    ++itD;
    iD = 0;
  }

  while ((itG != arbreG.end()) && (itD != arbreD.end())) {
    decal = std::max(decal, ((*itG).R - ((*itD).L) + nodeSpacing));
    int min = std::min(((*itG).size - iG), ((*itD).size - iD));
    iG += min;
    iD += min;

    if (iG == (*itG).size) {
      ++itG;
      iG = 0;
    }

    if (iD == (*itD).size) {
      ++itD;
      iD = 0;
    }
  }

  return decal;
}
//=============================================================================
list<LR> *TreeReingoldAndTilfordExtended::mergeLRList(std::list<LR> *L, std::list<LR> *R,
                                                      double decal) {
  assert(L != nullptr);
  assert(R != nullptr);
  list<LR>::iterator itL, itR;
  int iL = 0, iR = 0;
  itL = L->begin();
  itR = R->begin();
  LR tmp;

  while ((itL != L->end()) && (itR != R->end())) {
    tmp.L = itL->L;
    tmp.R = itR->R + decal;
    int min = std::min((itL->size - iL), (itR->size - iR));
    tmp.size = min;

    if (itL->size == 1) { // start
      (*itL) = tmp;
    } else {
      if (iL == 0) {
        if (iL + min >= itL->size) // block
          (*itL) = tmp;
        else {
          L->insert(itL, tmp);
          itL->size -= min;
          iL = -min;
        }
      } else {
        if (iL + min >= itL->size) { // end
          itL->size -= min;
          ++itL;
          L->insert(itL, tmp);
          iL = -min;
        } else { // middle
          LR tmp2 = *itL;
          itL->size = iL;
          ++itL;
          L->insert(itL, tmp);
          tmp2.size -= iL + min;
          L->insert(itL, tmp2);
          --itL;
          iL = -min;
        }
      }
    }

    iL += min;
    iR += min;

    if (iL >= itL->size) {
      ++itL;
      iL = 0;
    }

    if (iR >= itR->size) {
      ++itR;
      iR = 0;
    }
  }

  if (itL != L->end()) {
    if (iL != 0) {
      tmp.L = itL->L;
      tmp.R = itL->R;
      tmp.size = itL->size - iL;
    }
  }

  if (itR != R->end()) {
    if (iR != 0) {
      tmp.L = itR->L + decal;
      tmp.R = itR->R + decal;
      tmp.size = itR->size - iR;
      L->push_back(tmp);
      ++itR;
    }

    for (; itR != R->end(); ++itR) {
      tmp.L = itR->L + decal;
      tmp.R = itR->R + decal;
      tmp.size = itR->size;
      L->push_back(tmp);
    }
  }

  return L;
}
//=============================================================================
list<LR> *TreeReingoldAndTilfordExtended::TreePlace(tlp::node n,
                                                    std::unordered_map<tlp::node, double> *p) {
  if (tree->outdeg(n) == 0) {
    list<LR> *result = new list<LR>();
    LR tmpLR;
    tmpLR.L = -sizes->getNodeValue(n).getW() / 2.;
    tmpLR.R = +sizes->getNodeValue(n).getW() / 2.;
    tmpLR.size = 1;
    (*p)[n] = 0;
    result->push_front(tmpLR);
    return (result);
  } else {
    Iterator<edge> *it;
    it = tree->getOutEdges(n);

    edge ite = it->next();
    node itn = tree->target(ite);

    list<LR> *leftTree, *rightTree;
    list<double> childPos;
    leftTree = TreePlace(itn, p);
    childPos.push_back(((*(leftTree->begin())).L + (*(leftTree->begin())).R) / 2.);

    if (useLength) {
      int tmpLength;

      if ((tmpLength = lengthMetric->getEdgeValue(ite)) > 1) {
        LR tmpLR;
        tmpLR.L = leftTree->front().L;
        tmpLR.R = leftTree->front().R;
        tmpLR.size = tmpLength - 1;
        leftTree->push_front(tmpLR);
      }
    }

    while (it->hasNext()) {
      ite = it->next();
      itn = tree->target(ite);
      rightTree = TreePlace(itn, p);

      if (useLength) {
        int tmpLength;

        if ((tmpLength = lengthMetric->getEdgeValue(ite)) > 1) {
          LR tmpLR;
          tmpLR.L = rightTree->front().L;
          tmpLR.R = rightTree->front().R;
          tmpLR.size = tmpLength - 1;
          rightTree->push_front(tmpLR);
        }
      }

      double decal = calcDecal(*leftTree, *rightTree);
      double tmpL = ((*(rightTree->begin())).L + (*(rightTree->begin())).R) / 2.;

      if (mergeLRList(leftTree, rightTree, decal) == leftTree) {
        childPos.push_back(tmpL + decal);
        delete rightTree;
      } else {
        list<double>::iterator itI = childPos.begin();

        for (; itI != childPos.end(); ++itI)
          (*itI) -= decal;

        childPos.push_back(tmpL);
        delete leftTree;
        leftTree = rightTree;
      }
    }

    delete it;
    double posFather = ((((*(leftTree->begin())).L + (*(leftTree->begin())).R) / 2.));
    LR tmpLR;
    tmpLR.L = posFather - sizes->getNodeValue(n).getW() / 2.;
    tmpLR.R = posFather + sizes->getNodeValue(n).getW() / 2.;
    tmpLR.size = 1;
    leftTree->push_front(tmpLR);

    list<double>::const_iterator itI = childPos.begin();
    for (const edge &ite : tree->getOutEdges(n)) {
      itn = tree->target(ite);
      (*p)[itn] = *itI - posFather;
      ++itI;
    }
    childPos.clear();
    (*p)[n] = 0;
    return (leftTree);
  }
}
//=============================================================================
void TreeReingoldAndTilfordExtended::TreeLevelSizing(tlp::node n,
                                                     std::unordered_map<int, double> &maxSize,
                                                     int level,
                                                     std::unordered_map<tlp::node, int> &levels) {
  levels[n] = level;

  if (maxSize.find(level) != maxSize.end()) {
    if (maxSize[level] < sizes->getNodeValue(n).getH()) {
      maxSize[level] = sizes->getNodeValue(n).getH();
    }
  } else
    maxSize[level] = sizes->getNodeValue(n).getH();

  if (useLength) {
    for (const edge &ite : tree->getOutEdges(n)) {
      node itn = tree->target(ite);
      TreeLevelSizing(itn, maxSize, level + (lengthMetric->getEdgeValue(ite)), levels);
    }
  } else {
    for (const node &itn : tree->getOutNodes(n)) {
      TreeLevelSizing(itn, maxSize, level + 1, levels);
    }
  }
}
//=============================================================================
void TreeReingoldAndTilfordExtended::calcLayout(tlp::node n,
                                                std::unordered_map<tlp::node, double> *p, double x,
                                                double y, int level,
                                                unordered_map<int, double> &maxLevelSize) {
  Coord tmpCoord;

  if (!compactLayout)
    tmpCoord.set(float(x + (*p)[n]), -float(y), 0);
  else
    tmpCoord.set(float(x + (*p)[n]), -float(y + maxLevelSize[level] / 2.f), 0);

  result->setNodeValue(n, tmpCoord);

  if (useLength) {
    for (auto ite : tree->getOutEdges(n)) {
      node itn = tree->target(ite);
      double decalY = y;
      int decalLevel = level;
      int tmp = lengthMetric->getEdgeValue(ite);

      while (tmp > 0) {
        if (!compactLayout)
          decalY += spacing;
        else
          decalY += maxLevelSize[decalLevel] + spacing;

        decalLevel++;
        tmp--;
      }

      calcLayout(itn, p, x + (*p)[n], decalY, decalLevel, maxLevelSize);
    }
  } else {
    for (auto itn : tree->getOutNodes(n)) {
      if (!compactLayout)
        calcLayout(itn, p, x + (*p)[n], y + spacing, level + 1, maxLevelSize);
      else
        calcLayout(itn, p, x + (*p)[n], y + maxLevelSize[level] + spacing, level + 1, maxLevelSize);
    }
  }
}
//===============================================================
bool TreeReingoldAndTilfordExtended::run() {
  std::unordered_map<node, double> posRelative;

  result->setAllEdgeValue(vector<Coord>(0));

  if (!getNodeSizePropertyParameter(dataSet, sizes))
    sizes = graph->getProperty<SizeProperty>("viewSize");

  getSpacingParameters(dataSet, nodeSpacing, spacing);
  orientation = "horizontal";
  lengthMetric = nullptr;
  ortho = true;
  useLength = false;
  compactLayout = true;
  bool boundingCircles = false;

  if (dataSet != nullptr) {
    useLength = dataSet->get("edge length", lengthMetric);
    dataSet->get("orthogonal", ortho);
    dataSet->get("bounding circles", boundingCircles);
    StringCollection tmp;

    if (dataSet->get("orientation", tmp)) {
      orientation = tmp.getCurrentString();
    }

    if (!dataSet->get("compact layout", compactLayout))
      compactLayout = true;
  }

  bool deleteLenghtMetric = false;

  if (lengthMetric == nullptr) {
    lengthMetric = new IntegerProperty(graph);
    lengthMetric->setAllNodeValue(1);
    lengthMetric->setAllEdgeValue(1);
    deleteLenghtMetric = true;
  }

  // use bounding circles if specified
  if (boundingCircles) {
    SizeProperty *circleSizes = new SizeProperty(graph);
    for (auto n : graph->nodes()) {
      const Size &boundCircle = sizes->getNodeValue(n);
      double diam = 2 * sqrt(boundCircle.getW() * boundCircle.getW() / 4.0 +
                             boundCircle.getH() * boundCircle.getH() / 4.0);
      circleSizes->setNodeValue(n, Size(float(diam), float(diam), 1.0f));
    }
    sizes = circleSizes;
  }

  //===========================================================

  if (pluginProgress)
    pluginProgress->showPreview(false);

  // push a temporary graph state (not redoable)
  // preserving layout updates
  std::vector<PropertyInterface *> propsToPreserve;

  if (!result->getName().empty())
    propsToPreserve.push_back(result);

  graph->push(false, &propsToPreserve);

  tree = TreeTest::computeTree(graph, pluginProgress);

  if (pluginProgress && pluginProgress->state() != TLP_CONTINUE) {
    graph->pop();

    if (deleteLenghtMetric)
      delete lengthMetric;

    return pluginProgress->state() != TLP_CANCEL;
  }

  node startNode = tree->getSource();
  assert(startNode.isValid());

  unordered_map<int, double> maxSizeLevel;
  unordered_map<node, int> levels;
  TreeLevelSizing(startNode, maxSizeLevel, 0, levels);

  // check if the specified layer spacing is greater
  // than the max of the minimum layer spacing of the tree
  if (!compactLayout) {
    for (unsigned int i = 0; i < maxSizeLevel.size() - 1; ++i) {
      float minLayerSpacing = float((maxSizeLevel[i] + maxSizeLevel[i + 1]) / 2);

      if (minLayerSpacing + nodeSpacing > spacing)
        spacing = minLayerSpacing + spacing;
    }
  }

  list<LR> *tmpList = TreePlace(startNode, &posRelative);
  delete tmpList;

  calcLayout(startNode, &posRelative, 0, 0, 0, maxSizeLevel);

  if (ortho) {
    // Edge bends
    for (auto e : tree->edges()) {
      node src = tree->source(e);
      node tgt = tree->target(e);
      const Coord &srcPos = result->getNodeValue(src);
      const Coord &tgtPos = result->getNodeValue(tgt);

      LineType::RealType tmp;
      Coord coord;
      if (srcPos[0] != tgtPos[0]) {
        if (orientation == "horizontal") {
          coord[0] = -srcPos[1];
          coord[1] = tgtPos[0];
        } else {
          coord[0] = tgtPos[0];
          coord[1] = srcPos[1];
        }
        tmp.push_back(coord);
      }
      result->setEdgeValue(e, tmp);
    }
  }

  // rotate layout and size
  if (orientation == "horizontal") {
    for (const node &n : tree->nodes()) {
      const Coord &tmpC = result->getNodeValue(n);
      result->setNodeValue(n, Coord(-tmpC[1], tmpC[0], tmpC[2]));
    }
  }

  // forget last temporary graph state
  graph->pop();

  if (boundingCircles)
    delete sizes;

  if (deleteLenghtMetric)
    delete lengthMetric;

  return true;
}
//=============================================================================
