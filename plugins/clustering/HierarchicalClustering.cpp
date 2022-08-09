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

#include "HierarchicalClustering.h"

using namespace std;
using namespace tlp;

PLUGIN(HierarchicalClustering)

static const char *paramHelp[] = {
    // metric
    "An existing node metric property."};

//================================================================================
HierarchicalClustering::HierarchicalClustering(PluginContext *context) : Algorithm(context) {
  addInParameter<NumericProperty *>("metric", paramHelp[0], "viewMetric");
}
//================================================================================
HierarchicalClustering::~HierarchicalClustering() {}
//================================================================================

class LessThan {
public:
  NumericProperty *metric;
  bool operator()(node n1, node n2) {
    return (metric->getNodeDoubleValue(n1) < metric->getNodeDoubleValue(n2));
  }
};

bool HierarchicalClustering::split(NumericProperty *metric, list<node> &orderedNode) {

  for (auto n : graph->nodes())
    orderedNode.push_back(n);

  LessThan comp;
  comp.metric = metric;
  orderedNode.sort(comp);

  list<node>::iterator itListNode;
  double tmpDbl;

  // We split the sorted list in two parts
  int nbElement = orderedNode.size();
  nbElement /= 2;

  if (nbElement < 10)
    return (true);

  itListNode = orderedNode.begin();
  tmpDbl = metric->getNodeDoubleValue(*itListNode);
  ++itListNode;
  --nbElement;

  while ((itListNode != orderedNode.end()) &&
         ((nbElement > 0) || (tmpDbl == metric->getNodeDoubleValue(*itListNode)))) {
    tmpDbl = metric->getNodeDoubleValue(*itListNode);
    ++itListNode;
    --nbElement;
  }

  orderedNode.erase(itListNode, orderedNode.end());
  return false;
}
//================================================================================
bool HierarchicalClustering::run() {

  string tmp1, tmp2;
  tmp1 = "Hierar Sup";
  tmp2 = "Hierar Inf";
  bool result = false;

  NumericProperty *metric = nullptr;
  if (dataSet != nullptr)
    dataSet->get("metric", metric);

  if (metric == nullptr)
    metric = graph->getProperty<DoubleProperty>("viewMetric");

  while (!result) {
    list<node> badNodeList;
    result = split(metric, badNodeList);

    if (!result) {
      BooleanProperty sel1(graph);
      BooleanProperty sel2(graph);
      BooleanProperty splitRes(graph);

      sel1.setAllNodeValue(true);
      sel1.setAllEdgeValue(true);
      sel2.setAllNodeValue(true);
      sel2.setAllEdgeValue(true);
      splitRes.setAllNodeValue(true);
      splitRes.setAllEdgeValue(true);

      list<node>::iterator itl;

      for (itl = badNodeList.begin(); itl != badNodeList.end(); ++itl)
        splitRes.setNodeValue(*itl, false);

      for (auto nit : graph->nodes()) {

        if (splitRes.getNodeValue(nit)) {
          sel2.setNodeValue(nit, false);

          for (auto ite : graph->getInOutEdges(nit)) {
            sel2.setEdgeValue(ite, false);
          }

        } else {
          sel1.setNodeValue(nit, false);

          for (auto ite : graph->getInOutEdges(nit)) {
            sel1.setEdgeValue(ite, false);
          }
        }
      }

      Graph *tmpSubGraph;
      tmpSubGraph = graph->addSubGraph(&sel1);
      tmpSubGraph->setAttribute<string>("name", tmp1);
      (graph->addSubGraph(&sel2))->setAttribute<string>("name", tmp2);
      graph = tmpSubGraph;
    }
  }

  return true;
}
