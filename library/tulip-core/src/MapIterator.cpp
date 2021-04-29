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
#include <tulip/MapIterator.h>
#include <tulip/Graph.h>
using namespace std;
using namespace tlp;

edge tlp::nextFaceEdge(Graph *g, edge e, node n) {
  EdgeMapIterator it(g, e, n);
  edge result;

  if (it.hasNext())
    result = it.next();

  return result;
}

NodeMapIterator::NodeMapIterator(Graph *sg, node source, node target) {
  assert(cloneIt.empty());
  bool start = true;

  for (auto tmp : sg->getInOutNodes(target)) {

    if ((start) && (tmp == source)) {
      start = false;
      itStl = cloneIt.begin();
    } else {
      if (start)
        cloneIt.push_back(tmp);
      else
        cloneIt.insert(itStl, tmp);
    }
  }

  itStl = cloneIt.begin();
}

NodeMapIterator::~NodeMapIterator() {}

node NodeMapIterator::next() {
  node tmp = *itStl;
  ++itStl;
  return tmp;
}

bool NodeMapIterator::hasNext() {
  return (itStl != cloneIt.end());
}

//=========================================
EdgeMapIterator::EdgeMapIterator(const Graph *sg, edge source, node target) {
  adj.resize(sg->deg(target));
  finished = false;
  treat = 0;
  pos = 0;

  for (auto e : sg->getInOutEdges(target)) {
    if (e == source)
      pos = treat + 1;

    adj[treat++] = e;
  }
}
//=========================================

edge EdgeMapIterator::next() {
  --treat;
  pos %= adj.size();
  return adj[pos++];
}
//=========================================
bool EdgeMapIterator::hasNext() {
  return (treat > 0);
}
//=========================================
