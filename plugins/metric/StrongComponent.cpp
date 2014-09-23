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
#include <cmath>
#include "StrongComponent.h"
#include <tulip/DoubleProperty.h>

PLUGIN(StrongComponent)

using namespace std;
using namespace tlp;

int StrongComponent::attachNumerotation(tlp::node n,
                                        TLP_HASH_MAP<tlp::node,bool> &visited,
                                        TLP_HASH_MAP<tlp::node,bool> &finished,
                                        TLP_HASH_MAP<tlp::node,int> &minAttach,
                                        int &id,
                                        std::stack<tlp::node> &renum,
                                        int &curComponent
                                       ) {
  if (visited[n]) return minAttach[n];

  visited[n]=true;
  int myId=id;
  id++;
  minAttach[n]=myId;
  renum.push(n);
  int res=myId;
  Iterator<node> *itN=graph->getOutNodes(n);

  for (; itN->hasNext();) {
    node tmpN=itN->next();

    if (!finished[tmpN]) {
      int tmp=attachNumerotation(tmpN,visited,finished,minAttach,id,renum,curComponent);

      if (res>tmp) res=tmp;
    }
  }

  delete itN;
  minAttach[n]=res;

  if (res==myId) {
    while (renum.top()!=n) {
      node tmp=renum.top();
      renum.pop();
      finished[tmp]=true;
      minAttach[tmp]=res;
      result->setNodeValue(tmp,curComponent);
    }

    finished[n]=true;
    result->setNodeValue(n,curComponent);
    curComponent++;
    renum.pop();
  }

  return res;
}

StrongComponent::StrongComponent(const tlp::PluginContext* context):DoubleAlgorithm(context) {}

StrongComponent::~StrongComponent() {}

bool StrongComponent::run() {
  TLP_HASH_MAP<node,bool> visited(graph->numberOfNodes());
  TLP_HASH_MAP<node,bool> finished(graph->numberOfNodes());
  stack<node> renum;
  TLP_HASH_MAP<node,int> cachedValues(graph->numberOfNodes());
  int id=1;
  int curComponent=0;
  Iterator<node> *itN=graph->getNodes();

  while (itN->hasNext()) {
    node itn=itN->next();

    if (!visited[itn]) {
      attachNumerotation(itn,visited,finished,cachedValues,id,renum,curComponent);
    }
  }

  delete itN;


  Iterator<edge> *itE=graph->getEdges();

  while (itE->hasNext()) {
    edge ite=itE->next();
    const std::pair<node, node>& eEnds = graph->ends(ite);

    if (result->getNodeValue(eEnds.first)==result->getNodeValue(eEnds.second))
      result->setEdgeValue(ite,result->getNodeValue(eEnds.first));
    else
      result->setEdgeValue(ite,curComponent);
  }

  delete itE;


  return true;
}
