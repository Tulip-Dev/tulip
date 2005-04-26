#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cassert>
#include <iostream>
#include "tulip/ObservableGraph.h"

using namespace std;

void ObservableGraph::notifyAddNode(SuperGraph *graph, const node n) {
  set<GraphObserver *> copy(observers); //Used to preserve iteratros
  for (set<GraphObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->addNode(graph, n);
}

void ObservableGraph::notifyDelNode(SuperGraph *graph, const node n) {
  set<GraphObserver *> copy(observers); //Used to preserve iteratros
  for (set<GraphObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->delNode(graph, n);
}

void ObservableGraph::notifyAddEdge(SuperGraph *graph, const edge e) {
  set<GraphObserver *> copy(observers); //Used to preserve iteratros
  for (set<GraphObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->addEdge(graph, e);
}

void ObservableGraph::notifyDelEdge(SuperGraph *graph, const edge e) {
  set<GraphObserver *> copy(observers); //Used to preserve iteratros
  for (set<GraphObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->delEdge(graph, e);
}

void ObservableGraph::notifyReverseEdge(SuperGraph *graph, const edge e) {
  set<GraphObserver *> copy(observers); //Used to preserve iteratros
  for (set<GraphObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->reverseEdge(graph, e);
}

void ObservableGraph::notifyDestroy(SuperGraph *graph) {
  set<GraphObserver *> copy(observers); //Used to preserve iteratros
  for (set<GraphObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->destroy(graph);
}

