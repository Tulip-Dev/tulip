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



#include <tulip/Bfs.h>
#include <tulip/BooleanProperty.h>

using namespace std;
using namespace tlp;

Bfs::Bfs(Graph * G, BooleanProperty * resultatAlgoSelection): graph(G->addCloneSubGraph()),selectedNodes(), selectedEdges() {
  selectedNodes.setAll(false);
  selectedEdges.setAll(false);
  nbNodes = 0;

  node root;;
  bool unselected = false;
  Iterator<node> * itn = resultatAlgoSelection->getNodesEqualTo(true);

  if(itn->hasNext()) {
    root = itn->next();

    if(!G->isElement(root))
      unselected = true;
  }
  else
    unselected = true;

  delete itn;

  if(unselected)
    root = graph->getOneNode();

  resultatAlgoSelection->setNodeValue(root, true);
  selectedNodes.set(root.id, true);
  ++nbNodes;
  computeBfs(G,resultatAlgoSelection,root);
}

void Bfs::computeBfs(Graph * G,BooleanProperty * resultatAlgoSelection, node root) {
  unsigned int taille = G->numberOfNodes();
  unsigned int nb_n = 1;
  unsigned int i = 0;
  vector<node> next_roots;
  next_roots.push_back(root);

  while(taille != nbNodes) {
    node r = next_roots[i];
    nb_n++;

    if(!G->isElement(r))
      tlp::error() << __PRETTY_FUNCTION__ << ": ERROR NODE R NOT IN G" << std::endl;

    Iterator<edge> * ite = G->getInOutEdges(r);

    while(ite->hasNext()) {
      edge e = ite->next();

      if(!selectedEdges.get(e.id)) {
        node tmp = G->opposite(e,r);

        if(!selectedNodes.get(tmp.id)) {
          selectedNodes.set(tmp.id,true);
          selectedEdges.set(e.id, true);
          next_roots.push_back(tmp);
          nbNodes++;
          resultatAlgoSelection->setNodeValue(tmp, true);
          resultatAlgoSelection->setEdgeValue(e, true);
        }
      }
    }

    delete ite;
    i++;
  }
}

Bfs::~Bfs() {}
