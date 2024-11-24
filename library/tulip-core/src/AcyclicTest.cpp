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
#include <stack>
#include <tulip/AcyclicTest.h>
#include <tulip/Graph.h>
#include <tulip/MutableContainer.h>

using namespace std;
using namespace tlp;

//**********************************************************************
class TestAcyclicListener : public Observable {
public:
  // override of Observable::treatEvent to remove the cached result for a graph if it is modified.
  void treatEvent(const Event &) override;

  /**
   * @brief Stored results for graphs. When a graph is updated, its entry is removed from the map.
   **/
  tlp_hash_map<const Graph *, bool> resultsBuffer;
};

void TestAcyclicListener::treatEvent(const Event &evt) {
  const GraphEvent *graphEvent = dynamic_cast<const GraphEvent *>(&evt);

  if (graphEvent) {
    Graph *graph = graphEvent->getGraph();

    switch (graphEvent->getType()) {
    case GraphEvent::TLP_ADD_EDGE:

      if (!resultsBuffer[graph])
        return;

      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    case GraphEvent::TLP_AFTER_DEL_EDGE:

      if (resultsBuffer[graph])
        return;
      [[fallthrough]];
    case GraphEvent::TLP_REVERSE_EDGE:
      graph->removeListener(this);
      resultsBuffer.erase(graph);
      break;

    default:
      // we don't care about other events
      break;
    }
  } else {

    Graph *graph = static_cast<Graph *>(evt.sender());

    if (evt.type() == Event::TLP_DELETE)
      resultsBuffer.erase(graph);
  }
}
//**********************************************************************
static TestAcyclicListener instance;
//**********************************************************************
bool AcyclicTest::isAcyclic(const Graph *graph) {
  auto it = instance.resultsBuffer.find(graph);
  if (it != instance.resultsBuffer.end())
    return it->second;

  graph->addListener(instance);
  return instance.resultsBuffer[graph] = acyclicTest(graph);
}
//**********************************************************************
void AcyclicTest::makeAcyclic(Graph *graph, vector<edge> &reversed,
                              vector<tlp::SelfLoops> &selfLoops) {
  if (AcyclicTest::isAcyclic(graph))
    return;

  std::vector<edge> edgesToDel;
  // replace self loops by three edges and two nodes.
  // do a loop on the already existing edges
  const vector<edge> &edges = graph->edges();
  // newly added edges during the loop
  // will not be taken into account
  unsigned int nbEdges = edges.size();
  for (unsigned int i = 0; i < nbEdges; ++i) {
    edge e = edges[i];
    auto eEnds = graph->ends(e);

    if (eEnds.first == eEnds.second) {
      node n1 = graph->addNode();
      node n2 = graph->addNode();
      selfLoops.emplace_back(n1, n2, graph->addEdge(eEnds.first, n1), graph->addEdge(n1, n2),
                             graph->addEdge(eEnds.first, n2), e);
      edgesToDel.push_back(e);
    }
  }
  if (!edgesToDel.empty())
    graph->delEdges(edgesToDel);

  // find obstruction edges
  reversed.clear();
  acyclicTest(graph, &reversed);

  if (reversed.size() > graph->numberOfEdges() / 2) {
    tlp::warning() << "[Warning]: " << __FUNCTION__ << ", is not efficient" << std::endl;
  }

  for (auto e : reversed)
    graph->reverse(e);

  assert(AcyclicTest::acyclicTest(graph));
}
//**********************************************************************
bool AcyclicTest::acyclicTest(const Graph *graph, vector<edge> *obstructionEdges) {
  MutableContainer<bool> visited;
  MutableContainer<bool> finished;
  visited.setAll(false);
  finished.setAll(false);
  bool result = true;
  // do a dfs traversal

  for (auto curNode : graph->nodes()) {

    if (!visited.get(curNode.id)) {
      stack<node> nodesToVisit;
      nodesToVisit.push(curNode);
      stack<Iterator<edge> *> neighboursToVisit;
      neighboursToVisit.push(graph->getOutEdges(curNode));

      while (!nodesToVisit.empty()) {
        curNode = nodesToVisit.top();
        Iterator<edge> *ite = neighboursToVisit.top();

        // check if dfs traversal of curNode neighbours is finished
        if (!ite->hasNext()) {
          // unstack curNode
          nodesToVisit.pop();
          // delete & unstack neighbours iterator
          delete neighboursToVisit.top();
          neighboursToVisit.pop();
          // mark curNode as to be skipped
          // during further exploration
          finished.set(curNode.id, true);
        } else {
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

                if (obstructionEdges != nullptr)
                  obstructionEdges->push_back(tmp);
                else {
                  // it is finished if we don't need
                  // to collect obstruction edges
                  break;
                }
              }
            } else {
              // found a new neighbour to explore
              // go deeper in our dfs traversal
              nodesToVisit.push(neighbour);
              neighboursToVisit.push(graph->getOutEdges(neighbour));
              break;
            }
          }

          // it may be finished if we don't need
          // to collect obstruction edges
          if ((!result) && (obstructionEdges == nullptr))
            break;
        }
      }

      // it may be finished if we don't need
      // to collect obstruction edges
      if ((!result) && (obstructionEdges == nullptr)) {
        // don't froget to delete remaining iterators
        while (!neighboursToVisit.empty()) {
          delete neighboursToVisit.top();
          neighboursToVisit.pop();
        }

        break;
      }
    }
  }

  return result;
}
