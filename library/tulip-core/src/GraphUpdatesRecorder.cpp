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
#include <tulip/GraphUpdatesRecorder.h>
#include <tulip/Graph.h>
#include <tulip/GraphImpl.h>

using namespace std;
using namespace tlp;

GraphUpdatesRecorder::GraphUpdatesRecorder(bool allowRestart,
                                           const GraphStorageIdsMemento *prevIdsMemento)
    :
#if !defined(NDEBUG)
      recordingStopped(true),
#endif
      updatesReverted(false), restartAllowed(allowRestart), newValuesRecorded(false),
      oldIdsStateRecorded(prevIdsMemento == nullptr), oldIdsState(prevIdsMemento),
      newIdsState(nullptr) {
}

GraphUpdatesRecorder::~GraphUpdatesRecorder() {
  deleteDeletedObjects();
  deleteValues(oldValues);
  deleteValues(newValues);
  deleteDefaultValues(oldNodeDefaultValues);
  deleteDefaultValues(newNodeDefaultValues);
  deleteDefaultValues(oldEdgeDefaultValues);
  deleteDefaultValues(newEdgeDefaultValues);

  if (oldIdsStateRecorded && oldIdsState)
    delete oldIdsState;

  if (newIdsState)
    delete newIdsState;
}

void GraphUpdatesRecorder::treatEvent(const Event &ev) {
  if (typeid(ev) == typeid(GraphEvent)) {
    const GraphEvent *gEvt = static_cast<const GraphEvent *>(&ev);
    Graph *graph = gEvt->getGraph();

    switch (gEvt->getType()) {
    case GraphEvent::TLP_ADD_NODE:
      addNode(graph, gEvt->getNode());
      break;

    case GraphEvent::TLP_BEFORE_DEL_NODE:
      delNode(graph, gEvt->getNode());
      break;

    case GraphEvent::TLP_ADD_EDGE:
      addEdge(graph, gEvt->getEdge());
      break;

    case GraphEvent::TLP_BEFORE_DEL_EDGE:
      delEdge(graph, gEvt->getEdge());
      break;

    case GraphEvent::TLP_REVERSE_EDGE:
      reverseEdge(graph, gEvt->getEdge());
      break;

    case GraphEvent::TLP_BEFORE_SET_ENDS:
      beforeSetEnds(graph, gEvt->getEdge());
      break;

    case GraphEvent::TLP_AFTER_SET_ENDS:
      afterSetEnds(graph, gEvt->getEdge());
      break;

    case GraphEvent::TLP_ADD_NODES: {
      const std::vector<node> &nodes = graph->nodes();

      for (unsigned int i = nodes.size() - gEvt->getNumberOfNodes(); i < nodes.size(); ++i)
        addNode(graph, nodes[i]);

      break;
    }

    case GraphEvent::TLP_ADD_EDGES:
      addEdges(graph, gEvt->getNumberOfEdges());
      break;

    case GraphEvent::TLP_AFTER_ADD_SUBGRAPH:
      addSubGraph(graph, const_cast<Graph *>(gEvt->getSubGraph()));
      break;

    case GraphEvent::TLP_AFTER_DEL_SUBGRAPH:
      delSubGraph(graph, const_cast<Graph *>(gEvt->getSubGraph()));
      break;

    case GraphEvent::TLP_ADD_LOCAL_PROPERTY:
      addLocalProperty(graph, gEvt->getPropertyName());
      break;

    case GraphEvent::TLP_BEFORE_DEL_LOCAL_PROPERTY:
      delLocalProperty(graph, gEvt->getPropertyName());
      break;

    case GraphEvent::TLP_BEFORE_RENAME_LOCAL_PROPERTY:
      propertyRenamed(gEvt->getProperty());
      break;

    case GraphEvent::TLP_BEFORE_SET_ATTRIBUTE:
      beforeSetAttribute(graph, gEvt->getAttributeName());
      break;

    case GraphEvent::TLP_REMOVE_ATTRIBUTE:
      removeAttribute(graph, gEvt->getAttributeName());
      [[fallthrough]];
    default:
      break;
    }
  } else {
    const PropertyEvent *propEvt = dynamic_cast<const PropertyEvent *>(&ev);

    if (propEvt) {
      PropertyInterface *prop = propEvt->getProperty();

      switch (propEvt->getType()) {
      case PropertyEvent::TLP_BEFORE_SET_NODE_VALUE:
        beforeSetNodeValue(prop, propEvt->getNode());
        break;

      case PropertyEvent::TLP_BEFORE_SET_ALL_NODE_VALUE:
        beforeSetAllNodeValue(prop);
        break;

      case PropertyEvent::TLP_BEFORE_SET_ALL_EDGE_VALUE:
        beforeSetAllEdgeValue(prop);
        break;

      case PropertyEvent::TLP_BEFORE_SET_EDGE_VALUE:
        beforeSetEdgeValue(prop, propEvt->getEdge());
        break;

      default:
        break;
      }
    }
  }
}

// delete the objects collected as to be deleted
void GraphUpdatesRecorder::deleteDeletedObjects() {

  tlp_hash_map<Graph *, set<PropertyInterface *>> &propertiesToDelete =
      updatesReverted ? addedProperties : deletedProperties;

  std::list<std::pair<Graph *, Graph *>> &subGraphsToDelete =
      updatesReverted ? addedSubGraphs : deletedSubGraphs;

  // loop on properties
  for (auto &itdp : propertiesToDelete) {
    set<PropertyInterface *>::const_iterator itp = itdp.second.begin();
    set<PropertyInterface *>::const_iterator ite = itdp.second.end();

    while (itp != ite) {
      delete (*itp);
      ++itp;
    }
  }

  // loop on sub graphs
  for (auto &itds : subGraphsToDelete) {
    itds.second->clearSubGraphs();
    delete itds.second;
  }
}

// clean up all the MutableContainers
void GraphUpdatesRecorder::deleteValues(tlp_hash_map<PropertyInterface *, RecordedValues> &values) {
  for (auto &itv : values) {
    delete itv.second.values;

    if (itv.second.recordedNodes)
      delete itv.second.recordedNodes;

    if (itv.second.recordedEdges)
      delete itv.second.recordedEdges;
  }

  values.clear();
}

// delete all the DataMem referenced by a tlp_hash_map
void GraphUpdatesRecorder::deleteDefaultValues(
    tlp_hash_map<PropertyInterface *, DataMem *> &values) {
  for (auto &itv : values) {
    delete itv.second;
  }

  values.clear();
}

void GraphUpdatesRecorder::recordEdgeContainer(tlp_hash_map<node, std::vector<edge>> &containers,
                                               GraphImpl *g, node n, edge e, bool loop) {
  if (containers.find(n) == containers.end()) {
    auto itAdj = containers.emplace(n, g->storage.adj(n)).first;
    // if we got a valid edge, this means that we must record
    // the node adjacencies before that edge was added (see addEdge)
    if (e.isValid()) {
      // as the edge is the last added
      // it must be at the last adj position (two last if loop)
      auto &adj = itAdj->second;
      auto size = adj.size() - (loop ? 2 : 1);
      assert(e == adj[size]);
      adj.resize(size);
    }
  }
}

void GraphUpdatesRecorder::recordEdgeContainer(tlp_hash_map<node, std::vector<edge>> &containers,
                                               GraphImpl *g, node n, const vector<edge> &gEdges,
                                               unsigned int nbAdded) {
  if (containers.find(n) == containers.end()) {
    auto &adj = containers.emplace(n, g->storage.adj(n)).first->second;
    // we must ensure that the last edges added in gEdges
    // are previously removed from the current node adjacencies,
    // so we look (in reverse order because they must be at the end)
    // for the elts of adj that are in the last edges added and remove them
    unsigned int adjAdded = 0;
    unsigned int lastAdded = gEdges.size();
    for (unsigned int i = adj.size() - 1; i > 0; --i) {
      edge e = adj[i];
      while (nbAdded) {
        --nbAdded;
        if (e == gEdges[--lastAdded]) {
          ++adjAdded;
          // check for loop
          if (i && (e == adj[i - 1]))
            ++adjAdded, --i;
          break;
        }
      }
      if (nbAdded == 0)
        break;
    }
    assert(adjAdded);
    adj.resize(adj.size() - adjAdded);
  }
}

void GraphUpdatesRecorder::removeFromEdgeContainer(
    tlp_hash_map<node, std::vector<edge>> &containers, edge e, node n) {
  const auto itAdj = containers.find(n);

  if (itAdj != containers.end()) {
    auto &adj = itAdj->second;
    auto it = adj.begin();

    while (it != adj.end()) {
      if ((*it) == e) {
        adj.erase(it);
        break;
      }

      ++it;
    }
  }
}

void GraphUpdatesRecorder::recordNewValues(GraphImpl *g) {
  assert(restartAllowed);

  if (!newValuesRecorded) {
    // from now on it will be done
    newValuesRecorded = true;

    // get ids memento
    GraphImpl *root = static_cast<GraphImpl *>(g);
    assert(newIdsState == nullptr);

    // record ids memento only if needed
    if ((graphAddedNodes.find(g) != graphAddedNodes.end()) ||
        (graphAddedEdges.find(g) != graphAddedEdges.end()))
      newIdsState = root->storage.getIdsMemento();

    // record new edges containers
    for (const auto &itae : addedEdgesEnds) {
      edge e = itae.first;

      // e may have been deleted (see delEdge)
      if (root->isElement(e)) {
        auto ends = itae.second;
        recordEdgeContainer(newContainers, root, ends.first);
        recordEdgeContainer(newContainers, root, ends.second);
      }
    }

    // record new properties default values & new values
    // loop on oldNodeDefaultValues
    for (const auto &itdv : oldNodeDefaultValues) {
      PropertyInterface *p = itdv.first;
      newNodeDefaultValues.emplace(p, p->getNodeDefaultDataMemValue());
      recordNewNodeValues(p);
    }

    // loop on node oldValues
    for (const auto &itov : oldValues) {
      PropertyInterface *p = itov.first;

      if (itov.second.recordedNodes && (oldNodeDefaultValues.find(p) == oldNodeDefaultValues.end()))
        recordNewNodeValues(p);
    }

    // loop on updatedPropsAddedNodes
    for (const auto &itan : updatedPropsAddedNodes) {
      PropertyInterface *p = itan.first;
      auto itnv = newValues.find(p);
      PropertyInterface *nv;
      MutableContainer<bool> *rn;
      bool created = itnv == newValues.end();
      bool hasNewValues = false;

      if (created) {
        nv = p->clonePrototype(p->getGraph(), "");
        rn = new MutableContainer<bool>();
      } else {
        nv = itnv->second.values;
        rn = itnv->second.recordedNodes;

        if (!rn)
          rn = itnv->second.recordedNodes = new MutableContainer<bool>();
      }

      for (auto n : itan.second) {
        if (nv->copy(n, n, p)) {
          rn->set(n, true);
          hasNewValues = true;
        }
      }

      if (created) {
        if (hasNewValues)
          newValues.emplace(p, RecordedValues(nv, rn));
        else {
          delete nv;
          delete rn;
        }
      }
    }

    // loop on oldEdgeDefaultValues
    for (const auto &itdv : oldEdgeDefaultValues) {
      PropertyInterface *p = itdv.first;
      newEdgeDefaultValues.emplace(p, p->getEdgeDefaultDataMemValue());
      recordNewEdgeValues(p);
    }

    // loop on edge oldValues
    for (const auto &itov : oldValues) {
      PropertyInterface *p = itov.first;

      if (itov.second.recordedEdges && (oldEdgeDefaultValues.find(p) == oldEdgeDefaultValues.end()))
        recordNewEdgeValues(p);
    }

    // loop on updatedPropsAddedEdges
    for (const auto &iten : updatedPropsAddedEdges) {
      PropertyInterface *p = iten.first;
      auto itnv = newValues.find(p);
      PropertyInterface *nv;
      MutableContainer<bool> *re;
      bool created = itnv == newValues.end();
      bool hasNewValues = false;

      if (created) {
        nv = p->clonePrototype(p->getGraph(), "");
        re = new MutableContainer<bool>();
      } else {
        nv = itnv->second.values;
        re = itnv->second.recordedEdges;

        if (!re)
          re = itnv->second.recordedEdges = new MutableContainer<bool>();
      }

      for (auto e : iten.second) {
        if (nv->copy(e, e, p)) {
          re->set(e, true);
          hasNewValues = true;
        }
      }

      if (created) {
        if (hasNewValues)
          newValues.emplace(p, RecordedValues(nv, nullptr, re));
        else {
          delete nv;
          delete re;
        }
      }
    }

    // record graph attribute new values
    for (const auto &itav : oldAttributeValues) {
      Graph *g = itav.first;
      const DataSet &gAttValues = g->getAttributes();
      DataSet &nAttValues = newAttributeValues[g];

      for (const auto &pval : itav.second.getValues()) {
        DataType *data = gAttValues.getData(pval.first);
        nAttValues.setData(pval.first, data);
        delete data;
      }
    }
  }
}

void GraphUpdatesRecorder::recordNewNodeValues(PropertyInterface *p) {
  auto itnv = newValues.find(p);
  assert(itnv == newValues.end() || (itnv->second.recordedNodes == nullptr));

  PropertyInterface *nv;
  MutableContainer<bool> *rn = new MutableContainer<bool>();

  if (itnv == newValues.end())
    nv = p->clonePrototype(p->getGraph(), "");
  else
    nv = itnv->second.values;

  bool hasNewValues = false;

  // record updated nodes new values
  if (oldNodeDefaultValues.find(p) != oldNodeDefaultValues.end()) {
    // loop on non default valuated nodes
    for (auto n : p->getNonDefaultValuatedNodes()) {
      nv->copy(n, n, p);
      rn->set(n, true);
      hasNewValues = true;
    }
  } else {
    const auto itp = oldValues.find(p);

    if (itp != oldValues.end() && itp->second.recordedNodes) {

      for (unsigned int id : itp->second.recordedNodes->findAll(true)) {
        node n(id);

        if (nv->copy(n, n, p)) {
          rn->set(n, true);
          hasNewValues = true;
        }
      }
    }
  }

  if (hasNewValues) {
    if (itnv == newValues.end())
      newValues.emplace(p, RecordedValues(nv, rn));
    else
      itnv->second.recordedNodes = rn;
  } else {
    delete rn;

    if (itnv == newValues.end())
      delete nv;
  }
}

void GraphUpdatesRecorder::recordNewEdgeValues(PropertyInterface *p) {
  auto itnv = newValues.find(p);
  assert(itnv == newValues.end() || (itnv->second.recordedEdges == nullptr));

  PropertyInterface *nv;
  MutableContainer<bool> *re = new MutableContainer<bool>();

  if (itnv == newValues.end())
    nv = p->clonePrototype(p->getGraph(), "");
  else
    nv = itnv->second.values;

  bool hasNewValues = false;

  // record updated edges new values
  if (oldEdgeDefaultValues.find(p) != oldEdgeDefaultValues.end()) {
    // loop on non default valuated edges
    for (auto e : p->getNonDefaultValuatedEdges()) {
      nv->copy(e, e, p);
      re->set(e, true);
      hasNewValues = true;
    }
  } else {
    const auto itp = oldValues.find(p);

    if (itp != oldValues.end() && itp->second.recordedEdges) {
      for (unsigned int id : itp->second.recordedEdges->findAll(true)) {
        edge e(id);
        if (nv->copy(e, e, p)) {
          re->set(e, true);
          hasNewValues = true;
        }
      }
    }
  }

  if (hasNewValues) {
    if (itnv == newValues.end())
      newValues.emplace(p, RecordedValues(nv, nullptr, re));
    else
      itnv->second.recordedEdges = re;
  } else {
    delete re;

    if (itnv == newValues.end())
      delete nv;
  }
}

void GraphUpdatesRecorder::startRecording(GraphImpl *g) {
  if (g->getSuperGraph() == g) {
    if (oldIdsState == nullptr)
      oldIdsState = static_cast<GraphImpl *>(g)->storage.getIdsMemento();
  }

  restartRecording(g);
}

void GraphUpdatesRecorder::restartRecording(Graph *g) {
#if !defined(NDEBUG)

  if (g->getSuperGraph() == g) {
    assert(recordingStopped);
    recordingStopped = false;
  }

#endif

  if (newValuesRecorded) {
    deleteValues(newValues);
    deleteValues(newValues);
    deleteDefaultValues(newNodeDefaultValues);
    deleteDefaultValues(newEdgeDefaultValues);

    if (newIdsState)
      delete newIdsState;

    newIdsState = nullptr;
    newValuesRecorded = false;
  }

  g->addListener(this);

  // add self as a PropertyObserver for all previously
  // existing properties
  const auto itp = addedProperties.find(g);
  const set<PropertyInterface *> *newProps =
      (itp == addedProperties.end()) ? nullptr : &(itp->second);

  for (PropertyInterface *prop : g->getLocalObjectProperties()) {
    if (newProps && (newProps->find(prop) != newProps->end()))
      continue;

    prop->addListener(this);
  }

  // add self as a GraphObserver for all previously
  // existing subgraphs
  const set<Graph *> *newSubGraphs = nullptr;
  set<Graph *> sgSet;

  for (const auto &it : addedSubGraphs) {
    if (it.first == g) {
      sgSet.insert(it.second);
    }
  }

  if (!sgSet.empty()) {
    newSubGraphs = &sgSet;
  }

  for (Graph *sg : g->subGraphs()) {
    if (!newSubGraphs || newSubGraphs->find(sg) == newSubGraphs->end())
      restartRecording(sg);
  }
}

void GraphUpdatesRecorder::stopRecording(Graph *g) {
#if !defined(NDEBUG)

  if (g->getSuperGraph() == g) {
    assert(!recordingStopped);
    recordingStopped = true;
  }

#endif
  g->removeListener(this);
  for (PropertyInterface *prop : g->getLocalObjectProperties())
    prop->removeListener(this);
  for (Graph *sg : g->subGraphs())
    stopRecording(sg);
}

void GraphUpdatesRecorder::doUpdates(GraphImpl *g, bool undo) {
  assert(updatesReverted != undo);
  updatesReverted = undo;

  Observable::holdObservers();
  // loop on propsToDel
  tlp_hash_map<Graph *, set<PropertyInterface *>> &propsToDel =
      undo ? addedProperties : deletedProperties;

  for (const auto &itpg : propsToDel) {
    Graph *g = itpg.first;

    for (auto prop : itpg.second) {
      g->delLocalProperty(prop->getName());
    }
  }

  // loop on subGraphsToDel
  std::list<std::pair<Graph *, Graph *>> &subGraphsToDel = undo ? addedSubGraphs : deletedSubGraphs;

  for (const auto &its : subGraphsToDel) {
    Graph *g = its.first;
    Graph *sg = its.second;

    // remove from list of subgraphs + notify observers
    g->notifyBeforeDelSubGraph(sg);
    g->setSubGraphToKeep(sg);
    g->removeSubGraph(sg);

    if (!undo) {
      // restore its subgraphs as subgraph of its supergraph
      // only if we are redoing its deletion
      for (Graph *ssg : sg->subGraphs()) {
        g->restoreSubGraph(ssg);
      }
    }

    g->notifyAfterDelSubGraph(sg);
    sg->notifyDestroy();
  }

  // loop on edgesToDel
  std::map<Graph *, std::unordered_set<edge>> &edgesToDel =
      undo ? graphAddedEdges : graphDeletedEdges;

  // edges must be removed in the decreasing order of the graphs ids
  // because for a coherent observation of deleted edges
  // they must be first deleted from a graph before being deleted
  // from its supergraph
  for (auto itge = edgesToDel.rbegin(); itge != edgesToDel.rend(); ++itge) {
    Graph *g = itge->first;
    // loop on graph's recorded edges
    for (edge e : itge->second)
      if (g->isElement(e))
        g->removeEdge(e);
  }

  // loop on nodesToDel
  tlp_hash_map<Graph *, std::unordered_set<node>> &nodesToDel =
      undo ? graphAddedNodes : graphDeletedNodes;

  for (const auto &itgn : nodesToDel) {
    Graph *g = itgn.first;
    // loop on graph's recorded nodes
    for (node n : itgn.second)
      g->removeNode(n);
  }

  // loop on subGraphsToAdd
  std::list<std::pair<Graph *, Graph *>> &subGraphsToAdd = undo ? deletedSubGraphs : addedSubGraphs;

  for (const auto &its : subGraphsToAdd) {
    Graph *g = its.first;
    Graph *sg = its.second;

    // notify its addition
    g->notifyBeforeAddSubGraph(sg);
    // restore sg as subgraph of g
    g->setSubGraphToKeep(sg);
    g->restoreSubGraph(sg);

    // and sg subgraphs are no longer subgraphs of g
    for (Graph *ssg : sg->subGraphs()) {
      g->removeSubGraph(ssg);
      ssg->setSuperGraph(sg);
    }

    // notify its addition
    g->notifyAfterAddSubGraph(sg);
  }

  // loop on nodesToAdd
  tlp_hash_map<Graph *, unordered_set<node>> &nodesToAdd =
      undo ? graphDeletedNodes : graphAddedNodes;

  for (const auto &itgn : nodesToAdd) {
    Graph *g = itgn.first;
    // loop on graph's recorded nodes
    for (node n : itgn.second)
      g->restoreNode(n);
  }

  // now restore ids manager state
  // this is done before the loop on the edges to add
  // because of some assertion in debug mode
  // while calling the restoreEdge method
  const GraphStorageIdsMemento *idsState = undo ? oldIdsState : newIdsState;

  if (idsState)
    g->storage.restoreIdsMemento(idsState);

  // loop on revertedEdges
  for (auto e : revertedEdges) {
    g->reverse(e);
  }

  // loop on edgesEnds
  tlp_hash_map<edge, pair<node, node>> &updatedEdgesEnds = undo ? oldEdgesEnds : newEdgesEnds;
  tlp_hash_map<edge, pair<node, node>>::const_iterator itee = updatedEdgesEnds.begin();

  while (itee != updatedEdgesEnds.end()) {
    g->setEnds(itee->first, itee->second.first, itee->second.second);
    ++itee;
  }

  // loop on containers
  tlp_hash_map<node, std::vector<edge>> &containers = undo ? oldContainers : newContainers;
  for (auto itc = containers.begin(); itc != containers.end(); ++itc) {
    node n(itc->first);
    // n may have been deleted as a previously added node
    if (g->isElement(n))
      g->storage.restoreAdj(n, itc->second);
  }

  // loop on edgesToAdd
  std::map<Graph *, unordered_set<edge>> &edgesToAdd = undo ? graphDeletedEdges : graphAddedEdges;
  tlp_hash_map<edge, std::pair<node, node>> &edgesEnds = undo ? deletedEdgesEnds : addedEdgesEnds;

  for (auto itge = edgesToAdd.begin(); itge != edgesToAdd.end(); ++itge) {
    Graph *g = itge->first;
    // loop on graph's recorded edges
    for (edge e : itge->second) {
      auto itEnds = edgesEnds.find(e);

      if (itEnds != edgesEnds.end()) {
        auto eEnds = itEnds->second;
        g->restoreEdge(e, eEnds.first, eEnds.second);
      } else {
        // restoration of an edge in a subgraph that was already an element of the root graph
        // (i.e., not a newly added edge)
        g->restoreEdge(e, g->getRoot()->source(e), g->getRoot()->target(e));
      }
    }
  }

  // loop on propsToAdd
  tlp_hash_map<Graph *, set<PropertyInterface *>> &propsToAdd =
      undo ? deletedProperties : addedProperties;

  for (const auto &itpg : propsToAdd) {
    Graph *g = itpg.first;

    for (auto prop : itpg.second) {
      g->addLocalProperty(prop->getName(), prop);
    }
  }

  // loop on renamedProperties
  if (!renamedProperties.empty()) {
    for (auto &itrp : renamedProperties) {
      PropertyInterface *prop = itrp.first;
      std::string newName = prop->getName();
      // switch names
      prop->rename(itrp.second);
      itrp.second = newName;
    }
  }

  // loop on nodeDefaultValues
  tlp_hash_map<PropertyInterface *, DataMem *> &nodeDefaultValues =
      undo ? oldNodeDefaultValues : newNodeDefaultValues;

  for (const auto &itdv : nodeDefaultValues) {
    PropertyInterface *prop = itdv.first;
    prop->setAllNodeDataMemValue(itdv.second);
  }

  // loop on edgeDefaultValues
  tlp_hash_map<PropertyInterface *, DataMem *> &edgeDefaultValues =
      undo ? oldEdgeDefaultValues : newEdgeDefaultValues;

  for (const auto &itdv : edgeDefaultValues) {
    PropertyInterface *prop = itdv.first;
    prop->setAllEdgeDataMemValue(itdv.second);
  }

  // loop on recorded values
  tlp_hash_map<PropertyInterface *, RecordedValues> &rvalues = undo ? oldValues : newValues;

  for (const auto &itrv : rvalues) {
    PropertyInterface *prop = itrv.first;
    PropertyInterface *nv = itrv.second.values;

    if (itrv.second.recordedNodes) {

      for (unsigned int id : itrv.second.recordedNodes->findAllValues(false, false)) {
        node n(id);
        prop->copy(n, n, nv);
      }
    }

    if (itrv.second.recordedEdges) {

      for (unsigned int id : itrv.second.recordedEdges->findAllValues(false, false)) {
        edge e(id);
        prop->copy(e, e, nv);
      }
    }
  }

  // loop on attribute values to restore
  tlp_hash_map<Graph *, DataSet> &attValues = undo ? oldAttributeValues : newAttributeValues;

  for (const auto &itav : attValues) {
    Graph *g = itav.first;

    for (const pair<string, DataType *> &pval : itav.second.getValues()) {
      if (pval.second)
        g->getNonConstAttributes().setData(pval.first, pval.second);
      else
        g->getNonConstAttributes().remove(pval.first);
    }
  }

  Observable::unholdObservers();
}

bool GraphUpdatesRecorder::hasUpdates() {
  assert(updatesReverted == false);

  // check addedProperties
  if (!addedProperties.empty())
    return true;

  // check addedSubGraphs
  if (!addedSubGraphs.empty())
    return true;

  // check graphAddedEdges
  if (!graphAddedEdges.empty())
    return true;

  // check graphAddedNodes
  for (const auto &itgn : graphAddedNodes) {
    if (!itgn.second.empty())
      return true;
  }

  // check deletedSubGraphs
  if (!deletedSubGraphs.empty())
    return true;

  // check graphDeletedNodes
  if (!graphDeletedNodes.empty())
    return true;

  // check revertedEdges
  if (!revertedEdges.empty())
    return true;

  // check oldEdgesEnds
  if (!oldEdgesEnds.empty())
    return true;

  // check oldcontainers
  if (!oldContainers.empty())
    return true;

  // check graphDeletedEdges
  if (!graphDeletedEdges.empty())
    return true;

  // check deletedProperties
  if (!deletedProperties.empty())
    return true;

  // check renamedProperties
  if (!renamedProperties.empty())
    return true;

  // check oldNodeDefaultValues
  if (!oldNodeDefaultValues.empty())
    return true;

  // check oldEdgeDefaultValues
  if (!oldEdgeDefaultValues.empty())
    return true;

  // check oldValues
  if (!oldValues.empty())
    return true;

  // check oldAttributeValues
  if (!oldAttributeValues.empty())
    return true;

  return false;
}

bool GraphUpdatesRecorder::dontObserveProperty(PropertyInterface *prop) {
  if (!restartAllowed) {
    // check if nothing is yet recorded for prop
    if ((oldNodeDefaultValues.find(prop) == oldNodeDefaultValues.end()) &&
        (oldEdgeDefaultValues.find(prop) == oldEdgeDefaultValues.end()) &&
        (oldValues.find(prop) == oldValues.end()) &&
        (updatedPropsAddedNodes.find(prop) == updatedPropsAddedNodes.end()) &&
        (updatedPropsAddedEdges.find(prop) == updatedPropsAddedEdges.end())) {
      // prop is no longer observed
      prop->removeListener(this);
      // may be a newly added property
      Graph *g = prop->getGraph();
      const auto it = addedProperties.find(g);

      if (it != addedProperties.end() && (it->second.find(prop) != it->second.end()))
        // the property is no longer recorded
        it->second.erase(prop);

      return true;
    }
  }

  return false;
}

bool GraphUpdatesRecorder::isAddedOrDeletedProperty(Graph *g, PropertyInterface *prop) {
  auto it = addedProperties.find(g);

  if (it != addedProperties.end() && (it->second.find(prop) != it->second.end()))
    return true;

  it = deletedProperties.find(g);
  return it != deletedProperties.end() && (it->second.find(prop) != it->second.end());
}

void GraphUpdatesRecorder::addNode(Graph *g, node n) {
  auto itgn = graphAddedNodes.find(g);

  if (itgn == graphAddedNodes.end())
    graphAddedNodes.emplace(g, unordered_set<node>{n});
  else
    itgn->second.insert(n);

  if (g->getRoot() == g) {
    addedNodes.insert(n);
  }

  // we need to backup properties values of the newly added node
  // in order to restore them when reading the node through the tlp::Graph::unpop() method
  // as the default properties values might change
  for (PropertyInterface *prop : g->getLocalObjectProperties()) {
    beforeSetNodeValue(prop, n);
  }
}

void GraphUpdatesRecorder::addEdge(Graph *g, edge e) {
  auto itge = graphAddedEdges.find(g);

  if (itge == graphAddedEdges.end())
    graphAddedEdges.emplace(g, unordered_set<edge>{e});
  else
    itge->second.insert(e);

  if (g == g->getRoot()) {
    auto eEnds = g->ends(e);
    addedEdgesEnds.emplace(e, eEnds);
    // record source & target old adjacencies
    bool loop = eEnds.first == eEnds.second;
    recordEdgeContainer(oldContainers, static_cast<GraphImpl *>(g), eEnds.first, e, loop);
    if (!loop)
      recordEdgeContainer(oldContainers, static_cast<GraphImpl *>(g), eEnds.second, e);
  }

  // we need to backup properties values of the newly added edge
  // in order to restore them when reading the node through the tlp::Graph::unpop() method
  // as the default properties values can change
  for (PropertyInterface *prop : g->getLocalObjectProperties()) {
    beforeSetEdgeValue(prop, e);
  }
}

void GraphUpdatesRecorder::addEdges(Graph *g, unsigned int nbAdded) {
  auto itge = graphAddedEdges.find(g);

  if (itge == graphAddedEdges.end()) {
    itge = graphAddedEdges.emplace(g, unordered_set<edge>{}).first;
  }

  unordered_set<edge> &ge = itge->second;
  const vector<edge> &gEdges = g->edges();

  for (unsigned int i = gEdges.size() - nbAdded; i < gEdges.size(); ++i) {
    edge e = gEdges[i];
    ge.insert(e);

    if (g == g->getRoot()) {
      auto eEnds = g->ends(e);
      addedEdgesEnds.emplace(e, eEnds);
      // record source & target old adjacencies
      recordEdgeContainer(oldContainers, static_cast<GraphImpl *>(g), eEnds.first, gEdges, nbAdded);
      recordEdgeContainer(oldContainers, static_cast<GraphImpl *>(g), eEnds.second, gEdges,
                          nbAdded);
    }

    // we need to backup properties values of the newly added edge
    // in order to restore them when reading the node through the tlp::Graph::unpop() method
    // as the default properties values can change
    for (PropertyInterface *prop : g->getLocalObjectProperties()) {
      beforeSetEdgeValue(prop, e);
    }
  }
}

void GraphUpdatesRecorder::delNode(Graph *g, node n) {
  auto itgn = graphAddedNodes.find(g);

  if (itgn != graphAddedNodes.end()) {
    auto itn = itgn->second.find(n);
    if (itn != itgn->second.end()) {
      // remove n from graph's recorded nodes if it is a newly added node
      itgn->second.erase(itn);
      // but don't remove it from addedNodes
      // to ensure further erasal from property will not
      // record a value as if it was a preexisting node
      return;
    }
  }

  // insert n into graphDeletedNodes
  itgn = graphDeletedNodes.find(g);

  if (itgn == graphDeletedNodes.end())
    graphDeletedNodes.emplace(g, unordered_set<node>{n});
  else
    itgn->second.insert(n);

  // get the set of added properties if any
  const auto itp = addedProperties.find(g);
  const set<PropertyInterface *> *newProps =
      (itp == addedProperties.end()) ? nullptr : &(itp->second);

  for (PropertyInterface *prop : g->getLocalObjectProperties()) {
    // nothing to record for newly added properties
    if (newProps && (newProps->find(prop) != newProps->end()))
      continue;

    beforeSetNodeValue(prop, n);
  }

  if (g == g->getSuperGraph())
    recordEdgeContainer(oldContainers, static_cast<GraphImpl *>(g), n);
}

void GraphUpdatesRecorder::delEdge(Graph *g, edge e) {
  auto itge = graphAddedEdges.find(g);

  if (itge != graphAddedEdges.end()) {
    auto ite = itge->second.find(e);
    if (ite != itge->second.end()) {
      // remove e as it is a newly added edge
      itge->second.erase(ite);
      // do not remove from addedEdgesEnds
      // to ensure further erasal from property will not
      // record a value as if it was a preexisting edge
      /* if (graphs.empty())
         addedEdges.erase(it); */
      // remove from revertedEdges if needed
      auto itR = revertedEdges.find(e);

      if (itR != revertedEdges.end())
        revertedEdges.erase(itR);

      // remove edge from nodes newContainers if needed
      auto itEnds = addedEdgesEnds.find(e);

      if (itEnds != addedEdgesEnds.end()) {
        auto eEnds = itEnds->second;
        removeFromEdgeContainer(newContainers, e, eEnds.first);
        removeFromEdgeContainer(newContainers, e, eEnds.second);
      }

      return;
    }
  }

  // insert e into graph's deleted edges
  itge = graphDeletedEdges.find(g);

  if (itge == graphDeletedEdges.end())
    graphDeletedEdges.emplace(g, unordered_set<edge>{e});
  else
    itge->second.insert(e);

  auto eEnds = g->ends(e);
  if (deletedEdgesEnds.find(e) == deletedEdgesEnds.end()) {
    if (g == g->getRoot()) {
      // remove from revertedEdges if needed
      auto it = revertedEdges.find(e);

      if (it != revertedEdges.end()) {
        revertedEdges.erase(it);
        deletedEdgesEnds.emplace(e, std::make_pair(eEnds.second, eEnds.first));
      } else {

        const auto ite = oldEdgesEnds.find(e);

        if (ite == oldEdgesEnds.end())
          deletedEdgesEnds.emplace(e, eEnds);
        else {
          deletedEdgesEnds.emplace(e, std::make_pair(ite->second.first, ite->second.second));
          // remove from oldEdgesEnds
          oldEdgesEnds.erase(ite);
          // remove from newEdgesEnds
          newEdgesEnds.erase(e);
        }
      }
    } else
      deletedEdgesEnds.emplace(e, eEnds);
  }

  // get the set of added properties if any
  const auto itp = addedProperties.find(g);
  const set<PropertyInterface *> *newProps =
      (itp == addedProperties.end()) ? nullptr : &(itp->second);

  // loop on properties to save the edge's associated values
  for (PropertyInterface *prop : g->getLocalObjectProperties()) {
    // nothing to record for newly added properties
    if (newProps && (newProps->find(prop) != newProps->end()))
      continue;

    beforeSetEdgeValue(prop, e);
  }

  if (g == g->getRoot()) {
    // record source & target old containers
    recordEdgeContainer(oldContainers, static_cast<GraphImpl *>(g), eEnds.first);
    recordEdgeContainer(oldContainers, static_cast<GraphImpl *>(g), eEnds.second);
  }
}

void GraphUpdatesRecorder::reverseEdge(Graph *g, edge e) {
  if (g == g->getSuperGraph()) {
    auto itEnds = addedEdgesEnds.find(e);

    // if it is a newly added edge revert its source and target
    if (itEnds != addedEdgesEnds.end()) {
      node src = itEnds->second.first;
      itEnds->second.first = itEnds->second.second;
      itEnds->second.second = src;
      return;
    }

    auto itne = newEdgesEnds.find(e);

    if (itne != newEdgesEnds.end()) {
      // revert ends of itne
      node src = itne->second.first;
      itne->second.first = itne->second.second;
      itne->second.second = src;
    } else { // update reverted edges
      auto it = revertedEdges.find(e);

      if (it != revertedEdges.end())
        revertedEdges.erase(it);
      else {
        revertedEdges.insert(e);
        // record source & target old containers
        const pair<node, node> &eEnds = g->ends(e);
        recordEdgeContainer(oldContainers, static_cast<GraphImpl *>(g), eEnds.first);
        recordEdgeContainer(oldContainers, static_cast<GraphImpl *>(g), eEnds.second);
      }
    }
  }
}

void GraphUpdatesRecorder::beforeSetEnds(Graph *g, edge e) {
  if ((g == g->getRoot()) && (oldEdgesEnds.find(e) == oldEdgesEnds.end()) &&
      (addedEdgesEnds.find(e) == addedEdgesEnds.end())) {
    auto ends = g->ends(e);
    auto it = revertedEdges.find(e);

    // if it is a reverted edge
    // remove it from the set
    if (it != revertedEdges.end()) {
      revertedEdges.erase(it);
      // revert ends of it
      node tgt = ends.first;
      ends.first = ends.second;
      ends.second = tgt;
    } else {
      // record source & target old containers
      recordEdgeContainer(oldContainers, static_cast<GraphImpl *>(g), ends.first);
      recordEdgeContainer(oldContainers, static_cast<GraphImpl *>(g), ends.second);
    }

    // add e old ends in oldEdgesEnds
    oldEdgesEnds[e] = ends;
  }
}

void GraphUpdatesRecorder::afterSetEnds(Graph *g, edge e) {
  if (g == g->getSuperGraph()) {
    auto ends = g->ends(e);
    auto itEnds = addedEdgesEnds.find(e);

    // if it is a newly added edge update its source and target
    if (itEnds != addedEdgesEnds.end()) {
      itEnds->second = ends;
      return;
    }

    // update new ends in newEdgesEnds
    newEdgesEnds[e] = ends;
  }
}

void GraphUpdatesRecorder::addSubGraph(Graph *g, Graph *sg) {
  // last added subgraph will be deleted first during undo/redo
  addedSubGraphs.push_front(std::make_pair(g, sg));

  // sg may already have nodes and edges
  // cf addCloneSubGraph
  if (sg->numberOfNodes()) {

    for (auto n : sg->nodes()) {
      addNode(sg, n);
    }

    for (auto e : sg->edges()) {
      addEdge(sg, e);
    }
  }

  sg->addListener(this);
}

void GraphUpdatesRecorder::delSubGraph(Graph *g, Graph *sg) {

  std::pair<Graph *, Graph *> p(g, sg);

  auto it = std::find(addedSubGraphs.begin(), addedSubGraphs.end(), p);

  // remove sg from addedSubGraphs if it is a newly added subgraph
  if (it != addedSubGraphs.end()) {

    addedSubGraphs.erase(it);

    // remove any update data concerning the removed subgraph
    // as it will be deleted
    removeGraphData(sg);

    // but set its subgraphs as added in its supergraph
    for (Graph *ssg : sg->subGraphs()) {
      addSubGraph(g, ssg);
    }

    return;
  }

  // last deleted subgraph will be the last one created during undo/redo
  deletedSubGraphs.push_back(p);

  // sg is no longer observed
  sg->removeListener(this);

  // but it must not be really deleted
  g->setSubGraphToKeep(sg);
}

void GraphUpdatesRecorder::removeGraphData(Graph *g) {
  for (Graph *sg : g->subGraphs()) {
    std::pair<Graph *, Graph *> p(g, sg);
    auto it = std::find(addedSubGraphs.begin(), addedSubGraphs.end(), p);

    if (it != addedSubGraphs.end()) {
      addedSubGraphs.erase(it);
    }
  }
  graphAddedNodes.erase(g);
  graphDeletedNodes.erase(g);
  graphAddedEdges.erase(g);
  graphDeletedEdges.erase(g);
  addedProperties.erase(g);
  deletedProperties.erase(g);
  oldAttributeValues.erase(g);
  newAttributeValues.erase(g);
}

void GraphUpdatesRecorder::addLocalProperty(Graph *g, const string &name) {
  const auto it = addedProperties.find(g);

  PropertyInterface *prop = g->getProperty(name);

  if (it == addedProperties.end())
    addedProperties.emplace(g, set<PropertyInterface *>{prop});
  else
    it->second.insert(prop);
}

void GraphUpdatesRecorder::delLocalProperty(Graph *g, const string &name) {
  PropertyInterface *prop = g->getProperty(name);

  auto it = addedProperties.find(g);

  // remove p from addedProperties if it is a newly added one
  if (it != addedProperties.end() && (it->second.find(prop) != it->second.end())) {
    // the property is no longer recorded
    it->second.erase(prop);
    // remove from renamed properties
    // if needed
    auto itr = renamedProperties.find(prop);

    if (itr != renamedProperties.end())
      renamedProperties.erase(itr);

    updatedPropsAddedNodes.erase(prop);
    updatedPropsAddedEdges.erase(prop);

    return;
  }

  // insert p into deletedProperties
  it = deletedProperties.find(g);

  if (it == deletedProperties.end())
    deletedProperties.emplace(g, set<PropertyInterface *>{prop});
  else
    it->second.insert(prop);

  // the property is no longer observed
  prop->removeListener(this);
}

void GraphUpdatesRecorder::propertyRenamed(PropertyInterface *prop) {
  const auto it = addedProperties.find(prop->getGraph());

  // remove p from addedProperties if it is a newly added one
  if (it != addedProperties.end() && (it->second.find(prop) != it->second.end())) {
    return;
  } else {
    if (renamedProperties.find(prop) == renamedProperties.end())
      renamedProperties.emplace(prop, prop->getName());
  }
}

void GraphUpdatesRecorder::beforeSetNodeValue(PropertyInterface *p, node n) {
  // do not record the old value if the default one has been changed
  if (oldNodeDefaultValues.find(p) != oldNodeDefaultValues.end())
    return;

  // do not record old values for newly added nodes
  if (addedNodes.find(n) != addedNodes.end()) {
    if (!restartAllowed)
      return;
    else {
      if (p->getGraph()->isElement(n))
        updatedPropsAddedNodes[p].insert(n);
      else
        // n has been deleted in the whole graph hierarchy, so we don't
        // need to backup its property value in the next push as the node
        // does not belong to a graph anymore
        updatedPropsAddedNodes[p].erase(n);
    }
  } else {
    auto it = oldValues.find(p);

    if (it == oldValues.end()) {
      PropertyInterface *pv = p->clonePrototype(p->getGraph(), "");
      MutableContainer<bool> *rn = new MutableContainer<bool>();

      pv->copy(n, n, p);
      rn->set(n, true);
      oldValues.emplace(p, RecordedValues(pv, rn));
    }
    // check for a previously recorded old value
    else {
      if (it->second.recordedNodes) {
        if (it->second.recordedNodes->get(n))
          return;
      } else
        it->second.recordedNodes = new MutableContainer<bool>();

      it->second.values->copy(n, n, p);
      it->second.recordedNodes->set(n, true);
    }
  }
}

void GraphUpdatesRecorder::beforeSetAllNodeValue(PropertyInterface *p) {
  if (oldNodeDefaultValues.find(p) == oldNodeDefaultValues.end()) {
    // first save the already existing value for all non default valuated nodes
    for (auto n : p->getNonDefaultValuatedNodes())
      beforeSetNodeValue(p, n);
    // then record the old default value
    // because beforeSetNodeValue does nothing if it has already been changed
    oldNodeDefaultValues.emplace(p, p->getNodeDefaultDataMemValue());
  }
}

void GraphUpdatesRecorder::beforeSetEdgeValue(PropertyInterface *p, edge e) {
  // do not record the old value if the default one has been changed
  if (oldEdgeDefaultValues.find(p) != oldEdgeDefaultValues.end())
    return;

  // do not record the old value for a newly added edge
  if (addedEdgesEnds.find(e) != addedEdgesEnds.end()) {
    if (!restartAllowed)
      return;

    if (p->getGraph()->isElement(e))
      updatedPropsAddedEdges[p].insert(e);
    else {
      // e has been deleted in the whole graph hierarchy, so we don't
      // need to backup its property value in the next push as the edge
      // does not belong to a graph anymore
      updatedPropsAddedEdges[p].erase(e);
    }
  } else {
    auto it = oldValues.find(p);

    if (it == oldValues.end()) {
      PropertyInterface *pv = p->clonePrototype(p->getGraph(), "");
      MutableContainer<bool> *re = new MutableContainer<bool>();

      pv->copy(e, e, p);
      re->set(e, true);
      oldValues.emplace(p, RecordedValues(pv, nullptr, re));
    }
    // check for a previously recorded old value
    else {
      if (it->second.recordedEdges) {
        if (it->second.recordedEdges->get(e))
          return;
      } else
        it->second.recordedEdges = new MutableContainer<bool>();

      it->second.values->copy(e, e, p);
      it->second.recordedEdges->set(e, true);
    }
  }
}

void GraphUpdatesRecorder::beforeSetAllEdgeValue(PropertyInterface *p) {
  if (oldEdgeDefaultValues.find(p) == oldEdgeDefaultValues.end()) {
    // first save the already existing value for all non default valuated edges
    for (auto e : p->getNonDefaultValuatedEdges())
      beforeSetEdgeValue(p, e);
    // then record the old default value
    // because beforeSetEdgeValue does nothing if it has already been changed
    oldEdgeDefaultValues.emplace(p, p->getEdgeDefaultDataMemValue());
  }
}

void GraphUpdatesRecorder::beforeSetAttribute(Graph *g, const std::string &name) {
  const auto it = oldAttributeValues.find(g);

  if (it != oldAttributeValues.end() && it->second.exists(name))
    return;

  // save the previously existing value
  DataType *valType = g->getAttributes().getData(name);
  oldAttributeValues[g].setData(name, valType);
  delete valType;
}

void GraphUpdatesRecorder::removeAttribute(Graph *g, const std::string &name) {
  beforeSetAttribute(g, name);
}
