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
#include "MISFiltering.h"
#include "Distances.h"
#include <ctime>
#include <math.h>
#include <tulip/TlpTools.h>
#include <tulip/StableIterator.h>
#include <tulip/ForEach.h>

using namespace tlp;
using namespace std;

MISFiltering::MISFiltering(Graph *g) {
  g_copy = g;
  removedVisit.setAll(false);
  removed.setAll(false);
  attendedVisit.setAll(false);
  visited.setAll(false);
}
//========================================
MISFiltering::~MISFiltering() {}
//========================================
void MISFiltering::computeFiltering() {
  DoubleProperty * metric = g_copy->getProperty<DoubleProperty>("misf");
  metric->setAllNodeValue(0);

  node firstNode= g_copy->getOneNode();

  inLastVi.setAll(true);

  node n;
  forEach(n,g_copy->getNodes())
  levelToNodes[0].insert(n);

  level = 1;
  unsigned int nb = g_copy->numberOfNodes();

  while(nb > 3) {
    nb = 0;
    visited.setAll(false);
    removedVisit.setAll(false);
    attendedVisit.setAll(false);
    attendedVisit.set(firstNode.id, true);
    toVisit.clear();
    toVisit.push_back(firstNode);
    inCurVi.setAll(false);
    inCurVi.set(firstNode.id,true);

    unsigned int depth = (unsigned int) 2 << (level - 1);

    for(unsigned int i = 0; i < toVisit.size() ; ++i) {
      node current = toVisit[i];

      if(removedVisit.get(current.id))
        continue;

      visited.set(current.id,true);
      bfsDepth(current, depth);
    }

    inLastVi.setAll(false);
    node n;
    Iterator <unsigned int> *itn = inCurVi.findAll(true);

    while(itn->hasNext()) {
      node n = node(itn->next());
      levelToNodes[level].insert(n);
      inLastVi.set(n.id, true);
      metric->setNodeValue(n, level);
      ++nb;
    }

    delete itn;

    ++level;
    inCurVi.setAll(false);
    removed.setAll(false);
  }

  updateVectors();
}
//========================================
void MISFiltering::bfsDepth(node n, unsigned int depth) {
  vector<node> nextNodes;
  TLP_HASH_MAP<node, unsigned int> nodeDepth;
  MutableContainer<bool> inNext;
  inNext.setAll(false);
  inNext.set(n.id,true);
  nextNodes.push_back(n);
  nodeDepth[n]=0;

  for(unsigned int i = 0 ; i < nextNodes.size(); ++i) {
    node current = nextNodes[i];
    node v;
    forEach(v,g_copy->getInOutNodes(current)) {
      if(visited.get(v.id) || inNext.get(v.id))
        continue;

      if(nodeDepth[current] < depth -1) {
        inNext.set(v.id, true);
        nextNodes.push_back(v);
        removed.set(v.id, true);
      }

      nodeDepth[v] = nodeDepth[current]+1;

      if(nodeDepth[v] == depth && inLastVi.get(v.id) && !removed.get(v.id)) {
        if(!attendedVisit.get(v.id)) {
          toVisit.push_back(v);
          attendedVisit.set(v.id, true);
          inCurVi.set(v.id, true);
        }
      }
      else if(nodeDepth[v] != 0 ) {
        if(attendedVisit.get(v.id)) {
          removedVisit.set(v.id, true);
          inCurVi.set(v.id, false);
          removed.set(v.id, true);
        }
      }
    }
  }
}
//========================================
void MISFiltering::updateVectors() {
  ordering.resize(g_copy->numberOfNodes());

  if(level == 1) {
    unsigned int curPos = 0;
    node n;
    forEach(n,g_copy->getNodes())
    ordering[curPos++]=n;
    return;
  }

  MutableContainer<bool> considered;
  considered.setAll(false);
  unsigned int curPos = 0;

  while(level + 1) {
    TLP_HASH_SET<node>::iterator it = levelToNodes[level].begin();

    while(it != levelToNodes[level].end()) {
      node n = *it;
      ++it;

      if(considered.get(n.id))
        continue;

      ordering[curPos] = n;
      ++curPos;
      considered.set(n.id,true);
    }

    if(level != 0)
      index.push_back(curPos);
    else
      break;

    --level;
  }

  if(index[0]==3)
    return;

  if(index.size()==1)
    index[0] = 3;
  else {
    if(index[1] > 3) {
      index[0] = 3;
    }
    else { // index[1]<=3
      index.erase(index.begin());

      if(index.size() >= 2) {
        if(index[1]>3)
          index[0] = 3;
        else
          index.erase(index.begin());
      }
      else
        index[0] = 3;
    }
  }

}
//========================================
void MISFiltering::getNearest(node n,
                              vector<node> &neighbors,
                              vector<unsigned int> &neighbors_dist,
                              unsigned int level,
                              unsigned int nbNeighbors) {
  vector<node> nextNodes;
  MutableContainer<bool> alreadyTreated;
  MutableContainer<bool> toTreat;
  TLP_HASH_MAP<node, unsigned int> nodeDepth;
  bool found = false;
  unsigned nbFound = 0;

  neighbors_dist.clear();
  neighbors.clear();
  nodeDepth[n] = 0;
  alreadyTreated.setAll(false);
  toTreat.setAll(false);
  nextNodes.push_back(n);
  alreadyTreated.set(n.id, true);

  for(unsigned int i = 0; i < index[level+1] ; ++i)
    toTreat.set(ordering[i].id, true);

  for(unsigned int i = 0; !found &&  i < nextNodes.size(); ++i) {
    node current = nextNodes[i];
    Iterator<node> * itn = g_copy->getInOutNodes(current);

    while(itn->hasNext()) {
      node v = itn->next();

      if(alreadyTreated.get(v.id))
        continue;

      alreadyTreated.set(v.id,true);
      nodeDepth[v] = nodeDepth[current] + 1;
      nextNodes.push_back(v);

      if(toTreat.get(v.id)) {
        neighbors.push_back(v);
        neighbors_dist.push_back(nodeDepth[v]);
        ++nbFound;
      }

      if(nbFound==nbNeighbors) {
        found = true;
        break;
      }
    }

    delete itn;
  }
}
//========================================
