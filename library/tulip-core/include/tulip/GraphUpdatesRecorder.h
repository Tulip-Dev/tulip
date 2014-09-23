/*
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
///@cond DOXYGEN_HIDDEN

#ifndef DOXYGEN_NOTFOR_DEVEL
#ifndef TLPGRAPHRECORDER_H
#define TLPGRAPHRECORDER_H


#include <string>
#include <set>
#include <vector>

//#include <tulip/tuliphash.h>
#include <tulip/Node.h>
//#include <tulip/Edge.h>
#include <tulip/Graph.h>
//#include <tulip/Observable.h>
#include <tulip/MutableContainer.h>

namespace std {
template<>
struct less<tlp::Graph*> {
  size_t  operator()(const tlp::Graph* g1, const tlp::Graph* g2) const {
    // the id order corresponds to the creation order
    // so when dealing with a set<Graph*> this will ensure that
    // we encounter a supergraph before its subgraphs
    return g1->getId() < g2->getId();
  }
};
}

namespace tlp {

struct EdgeRecord {
  node source;
  node target;
  std::set<tlp::Graph*> graphs;

  EdgeRecord() {}
  EdgeRecord(Graph* g, node s, node t) : source(s), target(t) {
    graphs.insert(g);
  }
};

struct GraphEltsRecord {
  Graph* graph;
  MutableContainer<bool> elts;

  GraphEltsRecord(Graph* g): graph(g) {}
};

} // end of namespace tlp

namespace std {
template<>
struct less<tlp::GraphEltsRecord*> {
  size_t  operator()(const tlp::GraphEltsRecord* g1, const tlp::GraphEltsRecord* g2) const {
    return g1->graph->getId() < g2->graph->getId();
  }
};
}

namespace tlp {
class GraphImpl;
class GraphStorageIdsMemento;

class GraphUpdatesRecorder :public Observable {
  friend class GraphImpl;
  //
#if !defined(NDEBUG)
  bool recordingStopped;
#endif
  bool updatesReverted;
  bool restartAllowed;
  bool newValuesRecorded;

  // one 'set' of added nodes per graph
  MutableContainer<GraphEltsRecord*> graphAddedNodes;
  // the whole 'set' of added nodes
  MutableContainer<bool> addedNodes;
  // one 'set' of deleted nodes per graph
  MutableContainer<GraphEltsRecord*> graphDeletedNodes;
  // one 'set' of added edges per graph
  MutableContainer<GraphEltsRecord*> graphAddedEdges;
  // ends of all added edges
  MutableContainer<std::pair<node, node>*> addedEdgesEnds;
  // one 'set' of deleted edges per graph
  MutableContainer<GraphEltsRecord*> graphDeletedEdges;
  // ends of all deleted edges
  MutableContainer<std::pair<node, node>*> deletedEdgesEnds;
  // one set of reverted edges
  std::set<edge> revertedEdges;
  // source + target per updated edge
  TLP_HASH_MAP<edge, std::pair<node, node> > oldEdgesEnds;
  // source + target per updated edge
  TLP_HASH_MAP<edge, std::pair<node, node> > newEdgesEnds;
  // one set for old edge containers
  MutableContainer<std::vector<edge>*> oldContainers;
  // one set for new edge containers
  MutableContainer<std::vector<edge>*> newContainers;

  // copy of nodes/edges id manager state at start time
  const GraphStorageIdsMemento* oldIdsState;
  // copy of nodes/edges id manager state at stop time
  const GraphStorageIdsMemento* newIdsState;

  // one list of (parent graph, added sub-graph)
  std::list<std::pair<Graph*, Graph*> > addedSubGraphs;
  // one list of (parent graph, deleted sub-graph)
  std::list<std::pair<Graph*, Graph*> > deletedSubGraphs;

  // one set of added properties per graph
  TLP_HASH_MAP<Graph*,  std::set<PropertyInterface*> > addedProperties;
  // one set of deleted properties per graph
  TLP_HASH_MAP<Graph*,  std::set<PropertyInterface*> > deletedProperties;
  // one set of old attribute values per graph
  TLP_HASH_MAP<Graph*, DataSet> oldAttributeValues;
  // one set of new attribute values per graph
  TLP_HASH_MAP<Graph*, DataSet> newAttributeValues;

  // one set of updated addNodes per property
  TLP_HASH_MAP<PropertyInterface*, std::set<node> > updatedPropsAddedNodes;

  // one set of updated addEdges per property
  TLP_HASH_MAP<PropertyInterface*, std::set<edge> > updatedPropsAddedEdges;

  // the old default node value for each updated property
  TLP_HASH_MAP<PropertyInterface*, DataMem*> oldNodeDefaultValues;
  // the new default node value for each updated property
  TLP_HASH_MAP<PropertyInterface*, DataMem*> newNodeDefaultValues;
  // the old default edge value for each updated property
  TLP_HASH_MAP<PropertyInterface*, DataMem*> oldEdgeDefaultValues;
  // the new default edge value for each updated property
  TLP_HASH_MAP<PropertyInterface*, DataMem*> newEdgeDefaultValues;
  // the old name for each renamed property
  TLP_HASH_MAP<PropertyInterface*, std::string> renamedProperties;

  struct RecordedValues {
    PropertyInterface* values;
    MutableContainer<bool>* recordedNodes;
    MutableContainer<bool>* recordedEdges;

    RecordedValues(PropertyInterface* prop = NULL,
                   MutableContainer<bool>* rn = NULL,
                   MutableContainer<bool>* re = NULL):
      values(prop), recordedNodes(rn), recordedEdges(re) {}
  };

  // the old nodes/edges values for each updated property
  TLP_HASH_MAP<PropertyInterface*, RecordedValues> oldValues;
  // the new node value for each updated property
  TLP_HASH_MAP<PropertyInterface*, RecordedValues> newValues;

  // real deletion of deleted objects (properties, sub graphs)
  // during the recording of updates these objects are removed from graph
  // structures but not really 'deleted'
  void deleteDeletedObjects();
  // deletion of recorded values
  void deleteValues(TLP_HASH_MAP<PropertyInterface*, RecordedValues>& values);
  // deletion of DataMem default values
  void deleteDefaultValues(TLP_HASH_MAP<PropertyInterface*, DataMem*>& values);
  // deletion of various containers
  template<typename T>
  void deleteContainerValues(MutableContainer<T>& ctnr) {
    IteratorValue* it = ctnr.findAllValues(NULL, false);

    while(it->hasNext()) {
      TypedValueContainer<T> tvc;
      it->nextValue(tvc);
      delete tvc.value;
    }

    delete it;
  }
  // record of a node's edges container before/after modification
  void recordEdgeContainer(MutableContainer<std::vector<edge>*>&,
                           GraphImpl*, node);
  // remove an edge from a node's edges container
  void removeFromEdgeContainer(MutableContainer<std::vector<edge>*>& containers,
                               edge e, node n);

  void removeGraphData(Graph *);

  // record new values for all updated properties
  // restartAllowed must be true
  void recordNewValues(GraphImpl*);
  void recordNewNodeValues(PropertyInterface* p);
  void recordNewEdgeValues(PropertyInterface* p);

  // start of recording (push)
  void startRecording(GraphImpl*);
  // end of recording
  // push an other recorder or pop this one
  void stopRecording(Graph*);
  // restart of recording (unpop)
  void restartRecording(Graph*);
  // perform undo/redo updates
  void doUpdates(GraphImpl*, bool undo);
  // remove a property from the observed ones
  // only if nothing is yet recorded for that property
  bool dontObserveProperty(PropertyInterface *);
  // check if the property is newly added or deleted
  bool isAddedOrDeletedProperty(Graph*, PropertyInterface *);

public:
  GraphUpdatesRecorder(bool allowRestart = true);
  ~GraphUpdatesRecorder();

  // old GraphObserver interface
  // graphAddedNodes
  void addNode(Graph* g, const node n);

  // graphAddedEdges
  void addEdge(Graph* g, const edge e);

  // graphDeletedNodes
  void delNode(Graph* g, const node n);

  // graphDeletedEdges
  void delEdge(Graph* g, const edge e);

  // revertedEdges
  void reverseEdge(Graph* g, const edge e);

  // oldEdgesEnds
  void beforeSetEnds(Graph* g, const edge e);

  // newEdgesEnds
  void afterSetEnds(Graph* g, const edge e);

  // addedSubGraphs
  void addSubGraph(Graph* g, Graph* sg);

  // deletedSubGraphs
  void delSubGraph(Graph* g, Graph* sg);

  // addedProperties
  void addLocalProperty(Graph* g, const std::string& name);

  // deletedProperties
  void delLocalProperty(Graph* g, const std::string& name);

  // beforeSetAttribute
  void beforeSetAttribute(Graph* g, const std::string& name);

  // removeAttribute
  void removeAttribute(Graph* g, const std::string& name);

  // old PropertyObserver Interface
  // oldValues
  void beforeSetNodeValue(PropertyInterface* p, const node n);

  // oldNodeDefaultValues
  void beforeSetAllNodeValue(PropertyInterface* p);

  // oldValues
  void beforeSetEdgeValue(PropertyInterface* p, const edge e);

  // oldEdgeDefaultValues
  void beforeSetAllEdgeValue(PropertyInterface* p);

  // renamedProperties
  void propertyRenamed(PropertyInterface* p);

protected:
  // override Observable::treatEvent
  virtual void treatEvent(const Event& ev);
};
}


#endif // TLPGRAPHRECORDER_H
#endif // DOXYGEN_NOTFOR_DEVEL
///@endcond
