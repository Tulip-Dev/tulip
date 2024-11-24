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
#include <set>
#include <tulip/tuliphash.h>
#include <tulip/TriconnectedTest.h>
#include "Tutte.h"

PLUGIN(Tutte)

using namespace std;
using namespace tlp;
//====================================================
Tutte::Tutte(const tlp::PluginContext *context) : LayoutAlgorithm(context) {}
//====================================================
Tutte::~Tutte() {}
//====================================================
list<node> findCycle(Graph *sg) {
  tlp_hash_map<node, node> father;
  tlp_hash_map<node, bool> visited;
  std::list<node> bfs;
  node startNode = sg->getOneNode();
  unsigned int maxDeg = sg->deg(startNode);

  for (auto n : sg->nodes()) {
    if (sg->deg(n) > maxDeg) {
      startNode = n;
    }
  }

  node n1, n2;
  father[startNode] = startNode;
  bfs.push_front(startNode);

  while (!bfs.empty()) {
    node curNode = bfs.front();
    bfs.pop_front();

    for (auto n : sg->getInOutNodes(curNode)) {
      if (n != father[curNode]) {
        if (!visited[n]) {
          visited[n] = true;
          father[n] = curNode;
          bfs.push_back(n);
        } else {
          n1 = curNode;
          n2 = n;
          bfs.clear();
          break;
        }
      }
    }
  }

  std::list<node> result;
  result.push_back(n1);
  result.push_back(n2);

  while (n1 != n2) {
    if (father[n1] == father[n2]) {
      if ((father[n1] != n1) && (father[n2] != n2))
        result.push_back(father[n1]);

      return result;
    } else {
      if (n1 == father[n2]) {
        return result;
      } else {
        if (n2 == father[n1]) {
          return result;
        } else {
          result.push_front(father[n1]);
          result.push_back(father[n2]);
          n1 = father[n1];
          n2 = father[n2];
        }
      }
    }
  }

  result.push_back(n1);
  return result;
}
//====================================================
bool Tutte::run() {
  if (pluginProgress) {
    // user cannot interact while computing
    pluginProgress->showPreview(false);
    pluginProgress->showStops(false);
  }

  result->setAllEdgeValue(vector<Coord>(0));
  std::list<node> tmp;
  tmp = findCycle(graph);
  // We place the nodes on the outer face
  Coord tmpCoord, tmpCoord2, baseCoord;
  float gamma;
  int i = 0;
  int rayon = 100;
  gamma = 2 * M_PI / tmp.size();

  for (auto n : tmp) {
    result->setNodeValue(
        n, Coord(rayon * cos(gamma * i) + rayon * 2, rayon * sin(gamma * i) + rayon * 2, 0));
    i++;
  }

  std::list<node> toMove;

  for (auto n : graph->nodes()) {
    toMove.push_front(n);
  }

  for (auto n : tmp) {
    toMove.remove(n);
  }

  bool ok = true;

  while (ok) {
    ok = false;

    for (auto n : toMove) {
      tmpCoord.set(0, 0, 0);
      Coord baseCoord = result->getNodeValue(n);
      int i = 0;

      for (auto nn : graph->getInOutNodes(n)) {
        const Coord &tmpCoord2 = result->getNodeValue(nn);
        tmpCoord.set(tmpCoord.getX() + tmpCoord2.getX(), tmpCoord.getY() + tmpCoord2.getY(), 0);
        ++i;
      }

      result->setNodeValue(n, Coord(tmpCoord.getX() / i, tmpCoord.getY() / i, 0));

      if (fabs(baseCoord.getX() - tmpCoord.getX() / i) > 0.02)
        ok = true;

      if (fabs(baseCoord.getY() - tmpCoord.getY() / i) > 0.02)
        ok = true;
    }
  }

  return true;
}
//====================================================
bool Tutte::check(std::string &erreurMsg) {
  bool result = true;

  if (!TriconnectedTest::isTriconnected(graph))
    result = false;
  else {
    for (auto n : graph->nodes()) {
      if (graph->deg(n) < 3) {
        result = false;
        break;
      }
    }
  }

  if (!result)
    erreurMsg = "The graph must be triconnected.";
  else
    erreurMsg = "";

  return result;
}
