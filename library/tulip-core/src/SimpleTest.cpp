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

#include <vector>
#include <tulip/Graph.h>
#include <tulip/SimpleTest.h>
#include <tulip/MutableContainer.h>

using namespace std;
using namespace tlp;
//=================================================================
SimpleTest * SimpleTest::instance=NULL;
//=================================================================
SimpleTest::SimpleTest () {
}
//=================================================================
bool SimpleTest::isSimple(const tlp::Graph* graph) {
  if(instance == 0 )
    instance = new SimpleTest();

  if (instance->resultsBuffer.find(graph) == instance->resultsBuffer.end()) {
    instance->resultsBuffer[graph] = simpleTest(graph);
    graph->addListener(instance);
  }

  return instance->resultsBuffer[graph];
}
//**********************************************************************
void SimpleTest::makeSimple(Graph* graph,vector<edge> &removed) {
  if (SimpleTest::isSimple(graph)) return;

  SimpleTest::simpleTest(graph, &removed, &removed);
  vector<edge>::const_iterator it;

  for(it = removed.begin(); it!=removed.end(); ++it) {
    graph->delEdge(*it);
  }

  assert(SimpleTest::isSimple(graph));
}
//=================================================================
bool SimpleTest::simpleTest(const tlp::Graph* graph, vector< edge >* multipleEdges, vector< edge >* loops) {
  bool result = true;
  bool computeAll = (loops != NULL) || (multipleEdges != NULL);
  Iterator<node> *itNode = graph->getNodes();
  MutableContainer<bool> inserted;
  MutableContainer<bool> visited;
  inserted.setAll(false);
  visited.setAll(false);

  while (itNode->hasNext ()) {
    node current = itNode->next ();
    //Search for multiple edges and loops
    Iterator<edge> *itEdge = graph->getInOutEdges (current);
    MutableContainer<bool> targeted;
    targeted.setAll(false);

    while (itEdge->hasNext ()) {
      edge e = itEdge->next();

      // check if edge has already been visited
      if (visited.get(e.id))
        continue;

      // mark edge as already visited
      visited.set(e.id, true);
      node target = graph->opposite(e, current);

      if (target == current) { //loop
        if (!computeAll) {
          result = false;
          break;
        }

        if (loops!=NULL) {
          if (!inserted.get(e.id)) {
            loops->push_back(e);
            inserted.set(e.id, true);
          }
        }
      }

      if (targeted.get(target.id) == true) {
        if (!computeAll) {
          result = false;
          break;
        }

        if (multipleEdges != NULL)  {
          if (!inserted.get(e.id)) {
            multipleEdges->push_back(e);
            inserted.set(e.id, true);
          }
        }
      }
      else
        targeted.set(target.id, true);
    }

    delete itEdge;

    if (!computeAll && !result) break;
  }

  delete itNode;
  return result;
}
//=================================================================
void SimpleTest::deleteResult(Graph *graph) {
  resultsBuffer.erase(graph);
  graph->removeListener(this);
}
//=================================================================
void SimpleTest::treatEvent(const Event& evt) {
  const GraphEvent* gEvt = dynamic_cast<const GraphEvent*>(&evt);

  if (gEvt) {
    Graph* graph = gEvt->getGraph();

    switch(gEvt->getType()) {
    case GraphEvent::TLP_ADD_EDGE:

      if (resultsBuffer[graph])
        deleteResult(graph);

      break;

    case GraphEvent::TLP_DEL_EDGE:

      if (!resultsBuffer[graph])
        deleteResult(graph);

      break;

    case GraphEvent::TLP_REVERSE_EDGE:
      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    default:
      break;
    }
  }
  else {
    // From my point of view the use of dynamic_cast should be correct
    // but it fails, so I use reinterpret_cast (pm)
    Graph* graph = reinterpret_cast<Graph *>(evt.sender());

    if (graph && evt.type() == Event::TLP_DELETE)
      deleteResult(graph);
  }
}
