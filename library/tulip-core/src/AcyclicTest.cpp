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
#include <stack>
#include <tulip/Graph.h>
#include <tulip/AcyclicTest.h>
#include <tulip/StableIterator.h>

using namespace std;
using namespace tlp;

AcyclicTest * AcyclicTest::instance = NULL;
//**********************************************************************
AcyclicTest::AcyclicTest() {
}
//**********************************************************************
bool AcyclicTest::isAcyclic(const Graph *graph) {
  if (instance == NULL)
    instance = new AcyclicTest();

  if (instance->resultsBuffer.find(graph) == instance->resultsBuffer.end()) {
    instance->resultsBuffer[graph] = acyclicTest(graph);
    graph->addListener(instance);
  }

  return instance->resultsBuffer[graph];
}
//**********************************************************************
void AcyclicTest::makeAcyclic(Graph* graph,vector<edge> &reversed, vector<tlp::SelfLoops> &selfLoops) {
  if (AcyclicTest::isAcyclic(graph)) return;

  //replace self loops by three edges and two nodes.
  StableIterator<edge> itE(graph->getEdges());

  while (itE.hasNext()) {
    edge e = itE.next();
    const pair<node, node>& eEnds = graph->ends(e);

    if (eEnds.first == eEnds.second) {
      node n1 = graph->addNode();
      node n2 = graph->addNode();
      selfLoops.push_back(tlp::SelfLoops(n1 , n2 ,
                                         graph->addEdge(eEnds.first, n1) ,
                                         graph->addEdge(n1,n2) ,
                                         graph->addEdge(eEnds.first, n2) ,
                                         e ));
      graph->delEdge(e);
    }
  }

  //find obstruction edges
  reversed.clear();
  acyclicTest(graph, &reversed);

  if (reversed.size() > graph->numberOfEdges() / 2) {
    tlp::warning() << "[Warning]: " << __FUNCTION__ << ", is not efficient" << std::endl;
  }

  vector<edge>::const_iterator it = reversed.begin();

  for (; it != reversed.end(); ++it)
    graph->reverse(*it);

  assert(AcyclicTest::isAcyclic(graph));
}
//**********************************************************************
bool AcyclicTest::acyclicTest(const Graph *graph, vector<edge> *obstructionEdges) {
  MutableContainer<bool> visited;
  MutableContainer<bool> finished;
  visited.setAll(false);
  finished.setAll(false);
  bool result = true;
  // do a dfs traversal
  Iterator<node> *it = graph->getNodes();

  while (it->hasNext()) {
    node curNode = it->next();

    if (!visited.get(curNode.id)) {
      stack<node> nodesToVisit;
      nodesToVisit.push(curNode);
      stack<Iterator<edge>*> neighboursToVisit;
      neighboursToVisit.push(graph->getOutEdges(curNode));

      while(!nodesToVisit.empty()) {
        curNode = nodesToVisit.top();
        Iterator<edge> *ite = neighboursToVisit.top();

        // check if dfs traversal of curNode neighbours is finished
        if (!ite->hasNext()) {
          // unstack curNode
          nodesToVisit.pop();
          // delete & unstack neightbours iterator
          delete neighboursToVisit.top();
          neighboursToVisit.pop();
          // mark curNode as to be skipped
          // during further exploration
          finished.set(curNode.id, true);
        }
        else {
          visited.set(curNode.id, true);

          // loop on remaining neighbours
          while (ite->hasNext()) {
            edge tmp = ite->next();
            node neighbour = graph->target(tmp);

            // check if we are already in the exploration
            // of the neighbours of neighbour
            if (visited.get(neighbour.id)) {
              if (!finished.get(neighbour.id)) {
                // found a cycle
                result = false;

                if (obstructionEdges != NULL)
                  obstructionEdges->push_back(tmp);
                else {
                  // it is finished if we don't need
                  // to collect obstruction edges
                  break;
                }
              }
            }
            else {
              // found a new neighbour to explore
              // go deeper in our dfs traversal
              nodesToVisit.push(neighbour);
              neighboursToVisit.push(graph->getOutEdges(neighbour));
              break;
            }
          }

          // it may be finished if we don't need
          // to collect obstruction edges
          if ((!result) && (obstructionEdges == NULL))
            break;
        }
      }

      // it may be finished if we don't need
      // to collect obstruction edges
      if ((!result) && (obstructionEdges == NULL)) {
        // don't froget to delete remaining iterators
        while(!neighboursToVisit.empty()) {
          delete neighboursToVisit.top();
          neighboursToVisit.pop();
        }

        break;
      }
    }
  }

  delete it;
  return result;
}
//**********************************************************************
void AcyclicTest::treatEvent(const Event& evt) {
  const GraphEvent* graphEvent = dynamic_cast<const GraphEvent*>(&evt);

  if (graphEvent) {
    Graph* graph = graphEvent->getGraph();

    switch(graphEvent->getType()) {
    case GraphEvent::TLP_ADD_EDGE:

      if (resultsBuffer[graph]==false)
        return;

      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_DEL_EDGE:

      if (resultsBuffer[graph]==true) return;

      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_REVERSE_EDGE:
      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    default:
      //we don't care about other events
      break;
    }
  }
  else {
    // From my point of view the use of dynamic_cast should be correct
    // but it fails, so I use reinterpret_cast (pm)
    Graph* graph = reinterpret_cast<Graph *>(evt.sender());

    if (graph && evt.type() == Event::TLP_DELETE)
      resultsBuffer.erase(graph);
  }
}
