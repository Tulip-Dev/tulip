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
#include "Distances.h"
#include <tulip/tuliphash.h>

#include <tulip/MutableContainer.h>

using namespace tlp;
using namespace std;

unsigned int getDist(Graph *g, node n1, node n2) {
  vector<node> nextNodes;
  TLP_HASH_MAP<node, unsigned int> nodeDepth;
  MutableContainer<bool> alreadyTreated;
  bool found = false;
  alreadyTreated.setAll(false);
  nextNodes.push_back(n1);
  nodeDepth[n1]=0;
  alreadyTreated.set(n1.id,true);

  for(unsigned int i = 0 ; !found && i < nextNodes.size(); ++i) {
    node current = nextNodes[i];
    Iterator<node> * itn = g->getInOutNodes(current);

    while(itn->hasNext()) {
      node v = itn->next();

      if(alreadyTreated.get(v.id))
        continue;

      alreadyTreated.set(v.id,true);
      nextNodes.push_back(v);
      nodeDepth[v] = nodeDepth[current]+1;

      if(v == n2) {
        found = true;
        break;
      }
    }

    delete itn;
  }

  return nodeDepth[n2];
}
