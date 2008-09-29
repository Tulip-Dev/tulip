#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cassert>
#include <iostream>
#include "tulip/ObservableGraph.h"

using namespace std;
using namespace tlp;

void ObservableGraph::notifyAddNode(Graph *sg, const node n) {
  set<GraphObserver *> copy(observers); //Used to preserve iterators
  for (set<GraphObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->addNode(sg, n);
}

void ObservableGraph::notifyDelNode(Graph *sg, const node n) {
  set<GraphObserver *> copy(observers); //Used to preserve iterators
  for (set<GraphObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->delNode(sg, n);
}

void ObservableGraph::notifyAddEdge(Graph *sg, const edge e) {
  set<GraphObserver *> copy(observers); //Used to preserve iterators
  for (set<GraphObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->addEdge(sg, e);
}

void ObservableGraph::notifyDelEdge(Graph *sg, const edge e) {
  set<GraphObserver *> copy(observers); //Used to preserve iterators
  for (set<GraphObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->delEdge(sg, e);
}

void ObservableGraph::notifyReverseEdge(Graph *sg, const edge e) {
  set<GraphObserver *> copy(observers); //Used to preserve iterators
  for (set<GraphObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->reverseEdge(sg, e);
}

void ObservableGraph::notifyDestroy(Graph *sg) {
  set<GraphObserver *> copy(observers); //Used to preserve iterators
  for (set<GraphObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->destroy(sg);
}

void ObservableGraph::notifyAddSubGraph(Graph *g, Graph *sg) {
  set<GraphObserver *> copy(observers); //Used to preserve iterators
  for (set<GraphObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->addSubGraph(g, sg);
}

void ObservableGraph::notifyDelSubGraph(Graph *g, Graph *sg) {
  set<GraphObserver *> copy(observers); //Used to preserve iterators
  for (set<GraphObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->delSubGraph(g, sg);
}

void ObservableGraph::notifyAddLocalProperty(Graph *g, const std::string& name) {
  set<GraphObserver *> copy(observers); //Used to preserve iterators
  for (set<GraphObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->addLocalProperty(g, name);
}

void ObservableGraph::notifyDelLocalProperty(Graph *g, const std::string& name) {
  set<GraphObserver *> copy(observers); //Used to preserve iterators
  for (set<GraphObserver *>::iterator itlObs=copy.begin();itlObs!=copy.end();++itlObs)
    (*itlObs)->delLocalProperty(g, name);
}
