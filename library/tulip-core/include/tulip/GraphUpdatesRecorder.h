/*
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
///@cond DOXYGEN_HIDDEN

#ifndef TLPGRAPHRECORDER_H
#define TLPGRAPHRECORDER_H

#include <string>
#include <set>
#include <unordered_set>
#include <vector>

#include <tulip/Graph.h>
#include <tulip/MutableContainer.h>
#include <tulip/tuliphash.h>

namespace std {
template <>
struct less<tlp::Graph *> {
  size_t operator()(const tlp::Graph *g1, const tlp::Graph *g2) const {
    // the id order corresponds to the creation order
    // so when dealing with a set<Graph*> this will ensure that
    // we encounter a supergraph before its subgraphs
    return g1->getId() < g2->getId();
  }
};
} // namespace std

namespace tlp {
class GraphImpl;
class GraphStorageIdsMemento;

class GraphUpdatesRecorder : public Observable {
  friend class GraphImpl;
//
#if !defined(NDEBUG)
  bool recordingStopped;
#endif
  bool updatesReverted;
  bool restartAllowed;
  bool newValuesRecorded;
  const bool oldIdsStateRecorded;

  // one 'set' of added nodes per graph
  tlp_hash_map<Graph *, std::unordered_set<node>> graphAddedNodes;
  // the whole 'set' of added nodes
  std::unordered_set<node> addedNodes;
  // one 'set' of deleted nodes per graph
  tlp_hash_map<Graph *, std::unordered_set<node>> graphDeletedNodes;
  // one 'set' of added edges per graph
  std::map<Graph *, std::unordered_set<edge>> graphAddedEdges;
  // ends of all added edges
  tlp_hash_map<edge, std::pair<node, node>> addedEdgesEnds;
  // one 'set' of deleted edges per graph
  std::map<Graph *, std::unordered_set<edge>> graphDeletedEdges;
  // ends of all deleted edges
  tlp_hash_map<edge, std::pair<node, node>> deletedEdgesEnds;
  // one set of reverted edges
  std::unordered_set<edge> revertedEdges;
  // source + target per updated edge
  tlp_hash_map<edge, std::pair<node, node>> oldEdgesEnds;
  // source + target per updated edge
  tlp_hash_map<edge, std::pair<node, node>> newEdgesEnds;
  // one 'set' for old edge containers
  tlp_hash_map<node, std::vector<edge>> oldContainers;
  // one 'set' for new edge containers
  tlp_hash_map<node, std::vector<edge>> newContainers;

  // copy of nodes/edges id manager state at start time
  const GraphStorageIdsMemento *oldIdsState;
  // copy of nodes/edges id manager state at stop time
  const GraphStorageIdsMemento *newIdsState;

  // one list of (parent graph, added subgraph)
  std::list<std::pair<Graph *, Graph *>> addedSubGraphs;
  // one list of (parent graph, deleted subgraph)
  std::list<std::pair<Graph *, Graph *>> deletedSubGraphs;

  // one set of added properties per graph
  tlp_hash_map<Graph *, std::set<PropertyInterface *>> addedProperties;
  // one set of deleted properties per graph
  tlp_hash_map<Graph *, std::set<PropertyInterface *>> deletedProperties;
  // one set of old attribute values per graph
  tlp_hash_map<Graph *, DataSet> oldAttributeValues;
  // one set of new attribute values per graph
  tlp_hash_map<Graph *, DataSet> newAttributeValues;

  // one set of updated addNodes per property
  tlp_hash_map<PropertyInterface *, std::set<node>> updatedPropsAddedNodes;

  // one set of updated addEdges per property
  tlp_hash_map<PropertyInterface *, std::set<edge>> updatedPropsAddedEdges;

  // the old default node value for each updated property
  tlp_hash_map<PropertyInterface *, DataMem *> oldNodeDefaultValues;
  // the new default node value for each updated property
  tlp_hash_map<PropertyInterface *, DataMem *> newNodeDefaultValues;
  // the old default edge value for each updated property
  tlp_hash_map<PropertyInterface *, DataMem *> oldEdgeDefaultValues;
  // the new default edge value for each updated property
  tlp_hash_map<PropertyInterface *, DataMem *> newEdgeDefaultValues;
  // the old name for each renamed property
  tlp_hash_map<PropertyInterface *, std::string> renamedProperties;

  struct RecordedValues {
    PropertyInterface *values;
    MutableContainer<bool> *recordedNodes;
    MutableContainer<bool> *recordedEdges;

    RecordedValues(PropertyInterface *prop = nullptr, MutableContainer<bool> *rn = nullptr,
                   MutableContainer<bool> *re = nullptr)
        : values(prop), recordedNodes(rn), recordedEdges(re) {}
  };

  // the old nodes/edges values for each updated property
  tlp_hash_map<PropertyInterface *, RecordedValues> oldValues;
  // the new node value for each updated property
  tlp_hash_map<PropertyInterface *, RecordedValues> newValues;

  // real deletion of deleted objects (properties, sub graphs)
  // during the recording of updates these objects are removed from graph
  // structures but not really 'deleted'
  void deleteDeletedObjects();
  // deletion of recorded values
  void deleteValues(tlp_hash_map<PropertyInterface *, RecordedValues> &values);
  // deletion of DataMem default values
  void deleteDefaultValues(tlp_hash_map<PropertyInterface *, DataMem *> &values);
  // record of a node's edges container before/after modification
  void recordEdgeContainer(tlp_hash_map<node, std::vector<edge>> &, GraphImpl *, node,
                           edge e = edge(), bool loop = false);
  void recordEdgeContainer(tlp_hash_map<node, std::vector<edge>> &, GraphImpl *, node,
                           const std::vector<edge> &, unsigned int);
  // remove an edge from a node's edges container
  void removeFromEdgeContainer(tlp_hash_map<node, std::vector<edge>> &containers, edge e, node n);

  void removeGraphData(Graph *);

  // record new values for all updated properties
  // restartAllowed must be true
  void recordNewValues(GraphImpl *);
  void recordNewNodeValues(PropertyInterface *p);
  void recordNewEdgeValues(PropertyInterface *p);

  // start of recording (push)
  void startRecording(GraphImpl *);
  // end of recording
  // push an other recorder or pop this one
  void stopRecording(Graph *);
  // restart of recording (unpop)
  void restartRecording(Graph *);
  // perform undo/redo updates
  void doUpdates(GraphImpl *, bool undo);
  // check for recorded updates
  bool hasUpdates();
  // remove a property from the observed ones
  // only if nothing is yet recorded for that property
  bool dontObserveProperty(PropertyInterface *);
  // check if the property is newly added or deleted
  bool isAddedOrDeletedProperty(Graph *, PropertyInterface *);

public:
  GraphUpdatesRecorder(bool allowRestart = true,
                       const GraphStorageIdsMemento *prevIdsMemento = nullptr);
  ~GraphUpdatesRecorder() override;

  // old GraphObserver interface
  // graphAddedNodes
  void addNode(Graph *g, const node n);

  // graphAddedEdges
  void addEdge(Graph *g, const edge e);

  void addEdges(Graph *g, unsigned int nbAddedEdges);

  // graphDeletedNodes
  void delNode(Graph *g, const node n);

  // graphDeletedEdges
  void delEdge(Graph *g, const edge e);

  // revertedEdges
  void reverseEdge(Graph *g, const edge e);

  // oldEdgesEnds
  void beforeSetEnds(Graph *g, const edge e);

  // newEdgesEnds
  void afterSetEnds(Graph *g, const edge e);

  // addedSubGraphs
  void addSubGraph(Graph *g, Graph *sg);

  // deletedSubGraphs
  void delSubGraph(Graph *g, Graph *sg);

  // addedProperties
  void addLocalProperty(Graph *g, const std::string &name);

  // deletedProperties
  void delLocalProperty(Graph *g, const std::string &name);

  // beforeSetAttribute
  void beforeSetAttribute(Graph *g, const std::string &name);

  // removeAttribute
  void removeAttribute(Graph *g, const std::string &name);

  // old PropertyObserver Interface
  // oldValues
  void beforeSetNodeValue(PropertyInterface *p, const node n);

  // oldNodeDefaultValues
  void beforeSetAllNodeValue(PropertyInterface *p);

  // oldValues
  void beforeSetEdgeValue(PropertyInterface *p, const edge e);

  // oldEdgeDefaultValues
  void beforeSetAllEdgeValue(PropertyInterface *p);

  // renamedProperties
  void propertyRenamed(PropertyInterface *p);

protected:
  // override Observable::treatEvent
  void treatEvent(const Event &ev) override;
};
} // namespace tlp

#endif // TLPGRAPHRECORDER_H

///@endcond
