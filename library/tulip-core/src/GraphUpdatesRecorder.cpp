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
#include <tulip/GraphUpdatesRecorder.h>
#include <tulip/Graph.h>
#include <tulip/ForEach.h>
#include <tulip/GraphImpl.h>

using namespace std;
using namespace tlp;

GraphUpdatesRecorder::GraphUpdatesRecorder(bool allowRestart):
#if !defined(NDEBUG)
  recordingStopped(true),
#endif
  updatesReverted(false),
  restartAllowed(allowRestart),
  newValuesRecorded(false), oldIdsState(NULL), newIdsState(NULL) {
}

GraphUpdatesRecorder::~GraphUpdatesRecorder() {
  deleteDeletedObjects();
  deleteValues(oldValues);
  deleteValues(newValues);
  deleteDefaultValues(oldNodeDefaultValues);
  deleteDefaultValues(newNodeDefaultValues);
  deleteDefaultValues(oldEdgeDefaultValues);
  deleteDefaultValues(newEdgeDefaultValues);

  if (oldIdsState)
    delete oldIdsState;

  if (newIdsState)
    delete newIdsState;

  // delete 'sets' of added nodes
  deleteContainerValues<GraphEltsRecord*>(graphAddedNodes);

  // delete 'sets' of deleted nodes
  deleteContainerValues<GraphEltsRecord*>(graphDeletedNodes);

  // delete 'sets' of added edges
  deleteContainerValues<GraphEltsRecord*>(graphAddedEdges);

  // delete 'sets' of deleted edges
  deleteContainerValues<GraphEltsRecord*>(graphDeletedEdges);

  // delete ends of added edges
  deleteContainerValues<std::pair<node, node>*>(addedEdgesEnds);

  // delete ends of deleted edges
  deleteContainerValues<std::pair<node, node>*>(deletedEdgesEnds);

  // delete old edges containers
  deleteContainerValues<std::vector<edge>*>(oldContainers);

  // delete new edges containers
  deleteContainerValues<std::vector<edge>*>(newContainers);
}

void GraphUpdatesRecorder::treatEvent(const Event& ev) {
  if (typeid(ev) == typeid(GraphEvent)) {
    const GraphEvent* gEvt = static_cast<const GraphEvent*>(&ev);
    Graph* graph = gEvt->getGraph();

    switch(gEvt->getType()) {
    case GraphEvent::TLP_ADD_NODE:
      addNode(graph, gEvt->getNode());
      break;

    case GraphEvent::TLP_DEL_NODE:
      delNode(graph, gEvt->getNode());
      break;

    case GraphEvent::TLP_ADD_EDGE:
      addEdge(graph, gEvt->getEdge());
      break;

    case GraphEvent::TLP_DEL_EDGE:
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
      const std::vector<node>& nodes = gEvt->getNodes();

      for (unsigned int i = 0; i < nodes.size(); ++i)
        addNode(graph, nodes[i]);

      break;
    }

    case GraphEvent::TLP_ADD_EDGES: {
      const std::vector<edge>& edges = gEvt->getEdges();

      for (unsigned int i = 0; i < edges.size(); ++i)
        addEdge(graph, edges[i]);

      break;
    }

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

    default:
      break;
    }
  }
  else {
    const PropertyEvent* propEvt = dynamic_cast<const PropertyEvent*>(&ev);

    if (propEvt) {
      PropertyInterface* prop = propEvt->getProperty();

      switch(propEvt->getType()) {
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

  TLP_HASH_MAP<Graph*, set<PropertyInterface*> >& propertiesToDelete =
    updatesReverted ? addedProperties : deletedProperties;

  std::list<std::pair<Graph*, Graph*> >& subGraphsToDelete =
    updatesReverted ? addedSubGraphs : deletedSubGraphs;

  // loop on properties
  TLP_HASH_MAP<Graph*, set<PropertyInterface*> >::const_iterator itdp =
    propertiesToDelete.begin();

  while(itdp != propertiesToDelete.end()) {
    set<PropertyInterface*>::const_iterator itp =  itdp->second.begin();
    set<PropertyInterface*>::const_iterator ite = itdp->second.end();

    while(itp != ite) {
      delete (*itp);
      ++itp;
    }

    ++itdp;
  }

  // loop on sub graphs
  std::list<std::pair<Graph*, Graph*> > ::const_iterator itds =
    subGraphsToDelete.begin();

  while(itds != subGraphsToDelete.end()) {
    itds->second->clearSubGraphs();
    delete itds->second;
    ++itds;
  }
}

// clean up all the MutableContainers
void GraphUpdatesRecorder::deleteValues(TLP_HASH_MAP<PropertyInterface*,
                                        RecordedValues>& values) {
  TLP_HASH_MAP<PropertyInterface*, RecordedValues>::const_iterator itv =
    values.begin();

  while(itv != values.end()) {
    delete itv->second.values;

    if (itv->second.recordedNodes)
      delete itv->second.recordedNodes;

    if (itv->second.recordedEdges)
      delete itv->second.recordedEdges;

    ++itv;
  }

  values.clear();
}

// delete all the DataMem referenced by a TLP_HASH_MAP
void GraphUpdatesRecorder::deleteDefaultValues(TLP_HASH_MAP<PropertyInterface*,
    DataMem*>& values) {
  TLP_HASH_MAP<PropertyInterface*, DataMem*>::const_iterator itv =
    values.begin();

  while(itv != values.end()) {
    delete itv->second;
    ++itv;
  }

  values.clear();
}

void GraphUpdatesRecorder::recordEdgeContainer(MutableContainer<vector<edge>*>& containers, GraphImpl* g, node n) {
  if (!containers.get(n)) {
    vector<edge>* ctnr = new vector<edge>();
    // record edges of n
    g->storage.getInOutEdges(n, *ctnr);
    containers.set(n, ctnr);
  }
}

void GraphUpdatesRecorder::removeFromEdgeContainer(MutableContainer<vector<edge>*>& containers, edge e, node n) {
  vector<edge>* ctnr = containers.get(n);

  if (ctnr) {
    vector<edge>::iterator it = ctnr->begin();

    while(it != ctnr->end()) {
      if ((*it) == e) {
        ctnr->erase(it);
        break;
      }

      ++it;
    }
  }
}

void GraphUpdatesRecorder::recordNewValues(GraphImpl* g) {
  assert(restartAllowed);

  if (!newValuesRecorded) {
    // from now on it will be done
    newValuesRecorded = true;

    // get ids memento
    GraphImpl* root = (GraphImpl*) g;
    assert(newIdsState == NULL);
    newIdsState = root->storage.getIdsMemento();
    // record new edges containers
    IteratorValue *itae = addedEdgesEnds.findAllValues(NULL, false);

    while(itae->hasNext()) {
      TypedValueContainer<std::pair<node, node>*> ends;
      itae->nextValue(ends);
      recordEdgeContainer(newContainers, root, ends.value->first);
      recordEdgeContainer(newContainers, root, ends.value->second);
    }

    delete itae;

    // record new properties default values & new values
    // loop on oldNodeDefaultValues
    TLP_HASH_MAP<PropertyInterface*, DataMem*>::const_iterator itdv =
      oldNodeDefaultValues.begin();

    while(itdv != oldNodeDefaultValues.end()) {
      PropertyInterface* p = itdv->first;
      newNodeDefaultValues[p] = p->getNodeDefaultDataMemValue();
      recordNewNodeValues(p);
      ++itdv;
    }

    // loop on node oldValues
    TLP_HASH_MAP<PropertyInterface*, RecordedValues>::const_iterator itov =
      oldValues.begin();

    while(itov != oldValues.end()) {
      PropertyInterface* p = itov->first;

      if (itov->second.recordedNodes &&
          (oldNodeDefaultValues.find(p) == oldNodeDefaultValues.end()))
        recordNewNodeValues(p);

      ++itov;
    }

    // loop on updatedPropsAddedNodes
    TLP_HASH_MAP<PropertyInterface*, std::set<node> >::const_iterator itan =
      updatedPropsAddedNodes.begin();

    while(itan != updatedPropsAddedNodes.end()) {
      PropertyInterface* p = itan->first;
      TLP_HASH_MAP<PropertyInterface*, RecordedValues>::iterator itnv =
        newValues.find(p);
      PropertyInterface*  nv;
      MutableContainer<bool>* rn;
      bool created = itnv == newValues.end();
      bool hasNewValues = false;

      if (created) {
        nv = p->clonePrototype(p->getGraph(), "");
        rn = new MutableContainer<bool>();
      }
      else {
        nv = itnv->second.values;
        rn = itnv->second.recordedNodes;

        if (!rn)
          rn = itnv->second.recordedNodes = new MutableContainer<bool>();
      }

      set<node>::const_iterator itn = itan->second.begin();
      set<node>::const_iterator itne = itan->second.end();

      while(itn != itne) {
        node n(*itn);

        // record value only if it is not the default one
        if (nv->copy(n, n, p, true)) {
          rn->set(n, true);
          hasNewValues = true;
        }

        ++itn;
      }

      if (created) {
        if (hasNewValues)
          newValues[p] = RecordedValues(nv, rn);
        else {
          delete nv;
          delete rn;
        }
      }

      ++itan;
    }

    // loop on oldEdgeDefaultValues
    itdv = oldEdgeDefaultValues.begin();

    while(itdv != oldEdgeDefaultValues.end()) {
      PropertyInterface* p = itdv->first;
      newEdgeDefaultValues[p] = p->getEdgeDefaultDataMemValue();
      recordNewEdgeValues(p);
      ++itdv;
    }

    // loop on edge oldValues
    itov = oldValues.begin();

    while(itov != oldValues.end()) {
      PropertyInterface* p = itov->first;

      if (itov->second.recordedEdges &&
          (oldEdgeDefaultValues.find(p) == oldEdgeDefaultValues.end()))
        recordNewEdgeValues(p);

      ++itov;
    }

    // loop on updatedPropsAddedEdges
    TLP_HASH_MAP<PropertyInterface*, std::set<edge> >::const_iterator iten =
      updatedPropsAddedEdges.begin();

    while(iten != updatedPropsAddedEdges.end()) {
      PropertyInterface* p = iten->first;
      TLP_HASH_MAP<PropertyInterface*, RecordedValues>::iterator itnv =
        newValues.find(p);
      PropertyInterface*  nv;
      MutableContainer<bool>* re;
      bool created = itnv == newValues.end();
      bool hasNewValues = false;

      if (created) {
        nv = p->clonePrototype(p->getGraph(), "");
        re = new MutableContainer<bool>();
      }
      else {
        nv = itnv->second.values;
        re = itnv->second.recordedEdges;

        if (!re)
          re = itnv->second.recordedEdges = new MutableContainer<bool>();
      }

      set<edge>::const_iterator ite = iten->second.begin();
      set<edge>::const_iterator itee = iten->second.end();

      while(ite != itee) {
        edge e(*ite);

        // record value only if it is not the default one
        if (nv->copy(e, e, p, true)) {
          re->set(e, true);
          hasNewValues = true;
        }

        ++ite;
      }

      if (created) {
        if (hasNewValues)
          newValues[p] = RecordedValues(nv, NULL, re);
        else {
          delete nv;
          delete re;
        }
      }

      ++iten;
    }

    // record graph attribute new values
    TLP_HASH_MAP<Graph*, DataSet>::const_iterator itav =
      oldAttributeValues.begin();

    while (itav != oldAttributeValues.end()) {
      Graph* g = itav->first;
      Iterator<pair<string, DataType*> > *itv = itav->second.getValues();
      const DataSet& gAttValues = g->getAttributes();
      DataSet& nAttValues = newAttributeValues[g];

      while(itv->hasNext()) {
        pair<string, DataType*> pval = itv->next();
        nAttValues.setData(pval.first, gAttValues.getData(pval.first));
      }

      delete itv;
      ++itav;
    }
  }
}

void GraphUpdatesRecorder::recordNewNodeValues(PropertyInterface* p) {
  TLP_HASH_MAP<PropertyInterface*, RecordedValues>::iterator itnv =
    newValues.find(p);
  assert(itnv == newValues.end() ||
         (itnv->second.recordedNodes == NULL));

  PropertyInterface* nv;
  MutableContainer<bool>* rn = new MutableContainer<bool>();

  if (itnv == newValues.end())
    nv = p->clonePrototype(p->getGraph(), "");
  else
    nv = itnv->second.values;

  bool hasNewValues = false;

  // record updated nodes new values
  if (oldNodeDefaultValues.find(p) != oldNodeDefaultValues.end()) {
    // loop on non default valuated nodes
    Iterator<node>* itn = p->getNonDefaultValuatedNodes();

    while(itn->hasNext()) {
      node n(itn->next());
      nv->copy(n, n, p);
      rn->set(n, true);
      hasNewValues = true;
    }

    delete itn;
  }
  else {
    TLP_HASH_MAP<PropertyInterface*, RecordedValues>::const_iterator itp =
      oldValues.find(p);

    if (itp != oldValues.end() && itp->second.recordedNodes) {
      Iterator<unsigned int>* itov = itp->second.recordedNodes->findAll(true);

      while(itov->hasNext()) {
        node n(itov->next());

        // record value only if it is not the default one
        if (nv->copy(n, n, p, true)) {
          rn->set(n, true);
          hasNewValues = true;
        }
      }

      delete itov;
    }
  }

  if (hasNewValues) {
    if (itnv == newValues.end())
      newValues[p] = RecordedValues(nv, rn);
    else
      itnv->second.recordedNodes = rn;
  }
  else {
    delete rn;

    if (itnv == newValues.end())
      delete nv;
  }
}

void GraphUpdatesRecorder::recordNewEdgeValues(PropertyInterface* p) {
  TLP_HASH_MAP<PropertyInterface*, RecordedValues>::iterator itnv =
    newValues.find(p);
  assert(itnv == newValues.end() ||
         (itnv->second.recordedEdges == NULL));

  PropertyInterface* nv;
  MutableContainer<bool>* re = new MutableContainer<bool>();

  if (itnv == newValues.end())
    nv = p->clonePrototype(p->getGraph(), "");
  else
    nv = itnv->second.values;

  bool hasNewValues = false;

  // record updated edges new values
  if  (oldEdgeDefaultValues.find(p) != oldEdgeDefaultValues.end()) {
    // loop on non default valuated edges
    Iterator<edge>* ite = p->getNonDefaultValuatedEdges();

    while(ite->hasNext()) {
      edge e(ite->next());
      nv->copy(e, e, p);
      re->set(e, true);
      hasNewValues = true;
    }

    delete ite;
  }
  else {
    TLP_HASH_MAP<PropertyInterface*, RecordedValues>::const_iterator itp =
      oldValues.find(p);

    if (itp != oldValues.end() && itp->second.recordedEdges) {
      Iterator<unsigned int>* itov = itp->second.recordedEdges->findAll(true);

      while(itov->hasNext()) {
        edge e(itov->next());

        // record value only if it is not the default one
        if (nv->copy(e, e, p, true)) {
          re->set(e, true);
          hasNewValues = true;
        }
      }

      delete itov;
    }
  }

  if (hasNewValues) {
    if (itnv == newValues.end())
      newValues[p] = RecordedValues(nv, NULL, re);
    else
      itnv->second.recordedEdges = re;
  }
  else {
    delete re;

    if (itnv == newValues.end())
      delete nv;
  }
}

void GraphUpdatesRecorder::startRecording(GraphImpl* g) {
  if (g->getSuperGraph() == g) {
    assert(oldIdsState == NULL);
    oldIdsState = ((GraphImpl*) g)->storage.getIdsMemento();
  }

  restartRecording(g);
}

void GraphUpdatesRecorder::restartRecording(Graph* g) {
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
    assert(newIdsState != NULL);
    delete newIdsState;
    newIdsState = NULL;
    newValuesRecorded = false;
  }

  g->addListener(this);

  // add self as a PropertyObserver for all previously
  // existing properties
  TLP_HASH_MAP<Graph*, set<PropertyInterface*> >::const_iterator itp =
    addedProperties.find(g);
  const set<PropertyInterface*>* newProps =
    (itp == addedProperties.end()) ? NULL : &(itp->second);

  PropertyInterface* prop;
  forEach(prop, g->getLocalObjectProperties()) {
    if (newProps && (newProps->find(prop) != newProps->end()))
      continue;

    prop->addListener(this);
  }

  // add self as a GraphObserver for all previously
  // existing subgraphs
  const set<Graph*>* newSubGraphs = NULL;
  set<Graph *> sgSet;
  std::list<std::pair<Graph*, Graph*> >::iterator it = addedSubGraphs.begin();

  for (; it != addedSubGraphs.end() ; ++it) {
    if (it->first == g) {
      sgSet.insert(it->second);
    }
  }

  if (!sgSet.empty()) {
    newSubGraphs = &sgSet;
  }

  Graph* sg;
  forEach(sg, g->getSubGraphs()) {
    if (!newSubGraphs || newSubGraphs->find(sg) == newSubGraphs->end())
      restartRecording(sg);
  }
}

void GraphUpdatesRecorder::stopRecording(Graph* g) {
#if !defined(NDEBUG)

  if (g->getSuperGraph() == g) {
    assert(!recordingStopped);
    recordingStopped = true;
  }

#endif
  g->removeListener(this);
  PropertyInterface* prop;
  forEach(prop, g->getLocalObjectProperties())
  prop->removeListener(this);
  Graph* sg;
  forEach(sg, g->getSubGraphs())
  stopRecording(sg);
}

void GraphUpdatesRecorder::doUpdates(GraphImpl* g, bool undo) {
  assert(updatesReverted != undo);
  updatesReverted = undo;

  Observable::holdObservers();
  // loop on propsToDel
  TLP_HASH_MAP<Graph*,  set<PropertyInterface*> >& propsToDel =
    undo ? addedProperties : deletedProperties;
  TLP_HASH_MAP<Graph*,  set<PropertyInterface*> >::const_iterator itpg =
    propsToDel.begin();

  while(itpg != propsToDel.end()) {
    Graph* g = (Graph*) itpg->first;
    set<PropertyInterface*>::const_iterator itp = itpg->second.begin();
    set<PropertyInterface*>::const_iterator itpe = itpg->second.end();

    while(itp != itpe) {
      g->delLocalProperty((*itp)->getName());
      ++itp;
    }

    ++itpg;
  }

  // loop on subGraphsToDel
  std::list<std::pair<Graph*, Graph*> >& subGraphsToDel = undo ? addedSubGraphs : deletedSubGraphs;
  std::list<std::pair<Graph*, Graph*> >::const_iterator its = subGraphsToDel.begin();

  while(its != subGraphsToDel.end()) {
    Graph* g = (Graph*) its->first;
    Graph* sg = its->second;

    // remove from list of subgraphs + notify observers
    g->notifyBeforeDelSubGraph(sg);
    g->removeSubGraph(sg);

    if (!undo) {
      // restore its subgraphs as subgraph of its supergraph
      // only if we are redoing its deletion
      Iterator<Graph *> *itss = sg->getSubGraphs();

      while (itss->hasNext()) {
        Graph* ssg = itss->next();
        g->restoreSubGraph(ssg);
      }

      delete itss;
    }

    g->notifyAfterDelSubGraph(sg);
    sg->notifyDestroy();

    ++its;
  }

  // loop on edgesToDel
  MutableContainer<GraphEltsRecord*>& edgesToDel =
    undo ? graphAddedEdges : graphDeletedEdges;
  // edges must be removed in the decreasing order of the graphs ids
  // because for a coherent observation of deleted edges
  // they must be first deleted from a graph before being deleted
  // from the supergraph
  std::set<GraphEltsRecord*> geSet;

  IteratorValue* itge = edgesToDel.findAllValues(NULL,false);

  while(itge->hasNext()) {
    TypedValueContainer<GraphEltsRecord*> ctnr;
    itge->nextValue(ctnr);
    geSet.insert(ctnr.value);
  }

  delete itge;

  std::set<GraphEltsRecord*>::const_reverse_iterator itrse = geSet.rbegin();

  while(itrse != geSet.rend()) {
    GraphEltsRecord* ger = (*itrse);
    // loop on graph's recorded edges
    Iterator<unsigned int>* ite = ger->elts.findAll(true, true);

    while(ite->hasNext()) {
      edge e(ite->next());

      if (ger->graph->isElement(e))
        ger->graph->removeEdge(e);
    }

    delete ite;
    ++itrse;
  }

  // loop on nodesToDel
  MutableContainer<GraphEltsRecord*>& nodesToDel =
    undo ? graphAddedNodes : graphDeletedNodes;
  IteratorValue* itgn = nodesToDel.findAllValues(NULL, false);

  while(itgn->hasNext()) {
    TypedValueContainer<GraphEltsRecord*> gnr;
    itgn->nextValue(gnr);

    // loop on graph's recorded nodes
    Iterator<unsigned int>* itn = gnr.value->elts.findAll(true, true);

    while(itn->hasNext()) {
      gnr.value->graph->removeNode(node(itn->next()));
    }

    delete itn;
  }

  delete itgn;

  // loop on subGraphsToAdd
  std::list<std::pair<Graph*, Graph*> >& subGraphsToAdd = undo ? deletedSubGraphs : addedSubGraphs;
  its = subGraphsToAdd.begin();

  while(its != subGraphsToAdd.end()) {
    Graph* g = its->first;
    Graph* sg = its->second;

    // notify its addition
    g->notifyBeforeAddSubGraph(sg);
    // restore sg as subgraph of g
    g->restoreSubGraph(sg);
    Iterator<Graph *> *itss = sg->getSubGraphs();

    // and sg subgraphs are no longer subgraphs of g
    while (itss->hasNext()) {
      Graph* ssg = itss->next();
      g->removeSubGraph(ssg);
      ssg->setSuperGraph(sg);
    }

    delete itss;
    // notify its addition
    g->notifyAfterAddSubGraph(sg);

    ++its;
  }

  // loop on nodesToAdd
  MutableContainer<GraphEltsRecord*>&  nodesToAdd =
    undo ? graphDeletedNodes : graphAddedNodes;
  itgn = nodesToAdd.findAllValues(NULL, false);

  while(itgn->hasNext()) {
    TypedValueContainer<GraphEltsRecord*> gnr;
    itgn->nextValue(gnr);

    // loop on graph's recorded nodes
    Iterator<unsigned int>* itn = gnr.value->elts.findAll(true, true);

    while(itn->hasNext()) {
      gnr.value->graph->restoreNode(node(itn->next()));
    }

    delete itn;
  }

  delete itgn;

  // now restore ids manager state
  // this is done before the loop on the edges to add
  // because of some assertion in debug mode
  // while calling the restoreEdge method
  g->storage.restoreIdsMemento(undo? oldIdsState : newIdsState);

  // loop on revertedEdges
  set<edge>::const_iterator itre = revertedEdges.begin();

  while(itre != revertedEdges.end()) {
    g->reverse((*itre));
    ++itre;
  }

  // loop on edgesEnds
  TLP_HASH_MAP<edge, pair<node, node> >& updatedEdgesEnds =
    undo ? oldEdgesEnds : newEdgesEnds;
  TLP_HASH_MAP<edge, pair<node, node> >::const_iterator itee =
    updatedEdgesEnds.begin();

  while(itee != updatedEdgesEnds.end()) {
    g->setEnds(itee->first, itee->second.first, itee->second.second);
    ++itee;
  }

  // loop on containers
  MutableContainer<std::vector<edge>*>& containers =
    undo ? oldContainers : newContainers;
  IteratorValue* itc = containers.findAllValues(NULL, false);

  while(itc->hasNext()) {
    TypedValueContainer<std::vector<edge>*> tvc;
    node n(itc->nextValue(tvc));
    g->storage.restoreAdj(n, *(tvc.value));
  }

  delete itc;

  // loop on edgesToAdd
  MutableContainer<GraphEltsRecord*>& edgesToAdd =
    undo ? graphDeletedEdges : graphAddedEdges;
  MutableContainer<std::pair<node, node>*>& edgesEnds =
    undo ? deletedEdgesEnds : addedEdgesEnds;
  // edges must be restored in the increasing order of the graphs ids
  // because for a coherent observation of added edges
  // they must be first added to the supergraph before being added
  // to a graph
  geSet.clear();
  itge = edgesToAdd.findAllValues(NULL,false);

  while(itge->hasNext()) {
    TypedValueContainer<GraphEltsRecord*> ger;
    itge->nextValue(ger);
    geSet.insert(ger.value);
  }

  delete itge;

  std::set<GraphEltsRecord*>::const_iterator itse = geSet.begin();

  while(itse != geSet.end()) {
    GraphEltsRecord* ger = (*itse);
    // loop on graph's recorded edges
    Iterator<unsigned int>* ite = ger->elts.findAll(true, true);

    while(ite->hasNext()) {
      edge e(ite->next());
      std::pair<node, node>* eEnds = edgesEnds.get(e);

      if (eEnds) {
        ger->graph->restoreEdge(e, eEnds->first, eEnds->second);
      }
      else {
        // restoration of an edge in a sub-graph that was already an element of the root graph (i.e., not a newly added edge)
        ger->graph->restoreEdge(e, ger->graph->getRoot()->source(e), ger->graph->getRoot()->target(e));
      }
    }

    delete ite;
    ++itse;
  }

  // loop on propsToAdd
  TLP_HASH_MAP<Graph*,  set<PropertyInterface*> >& propsToAdd =
    undo ? deletedProperties : addedProperties;
  itpg = propsToAdd.begin();

  while(itpg != propsToAdd.end()) {
    Graph* g = (Graph*) itpg->first;
    set<PropertyInterface*>::const_iterator itp = itpg->second.begin();
    set<PropertyInterface*>::const_iterator itpe = itpg->second.end();

    while(itp != itpe) {
      PropertyInterface* prop = (*itp);
      g->addLocalProperty(prop->getName(), prop);
      ++itp;
    }

    ++itpg;
  }

  // loop on renamedProperties
  if (!renamedProperties.empty()) {
    TLP_HASH_MAP<PropertyInterface*, std::string>::iterator itrp =
      renamedProperties.begin();

    std::vector<std::pair<PropertyInterface*, std::string> > renamings(renamedProperties.size());

    for(unsigned int i = 0; itrp != renamedProperties.end(); ++itrp, ++i) {
      PropertyInterface* prop = itrp->first;
      std::string newName = prop->getName();
      // switch names
      prop->rename(itrp->second);
      renamings[i] = std::make_pair(prop, newName);
    }

    // rebuild
    renamedProperties.clear();

    for(unsigned int i = 0; i < renamings.size(); ++i) {
      const std::pair<PropertyInterface*, std::string>& renaming = renamings[i];
      renamedProperties[renaming.first] = renaming.second;
    }
  }

  // loop on nodeDefaultValues
  TLP_HASH_MAP<PropertyInterface*, DataMem*>& nodeDefaultValues =
    undo ? oldNodeDefaultValues : newNodeDefaultValues;
  TLP_HASH_MAP<PropertyInterface*, DataMem*>::const_iterator itdv =
    nodeDefaultValues.begin();

  while(itdv != nodeDefaultValues.end()) {
    PropertyInterface* prop = itdv->first;
    prop->setAllNodeDataMemValue(itdv->second);
    ++itdv;
  }

  // loop on edgeDefaultValues
  TLP_HASH_MAP<PropertyInterface*, DataMem*>& edgeDefaultValues =
    undo ? oldEdgeDefaultValues : newEdgeDefaultValues;
  itdv = edgeDefaultValues.begin();

  while(itdv != edgeDefaultValues.end()) {
    PropertyInterface* prop = itdv->first;
    prop->setAllEdgeDataMemValue(itdv->second);
    ++itdv;
  }

  // loop on recorded values
  TLP_HASH_MAP<PropertyInterface*, RecordedValues>& rvalues =
    undo ? oldValues : newValues;
  TLP_HASH_MAP<PropertyInterface*, RecordedValues>::const_iterator itrv =
    rvalues.begin();

  while (itrv != rvalues.end()) {
    PropertyInterface* prop = itrv->first;
    PropertyInterface* nv = itrv->second.values;

    if (itrv->second.recordedNodes) {
      IteratorValue* itv =
        itrv->second.recordedNodes->findAllValues(false, false);

      while(itv->hasNext()) {
        node n(itv->next());
        prop->copy(n, n, nv);
      }

      delete itv;
    }

    if (itrv->second.recordedEdges) {
      IteratorValue* itv =
        itrv->second.recordedEdges->findAllValues(false, false);

      while(itv->hasNext()) {
        edge e(itv->next());
        prop->copy(e, e, nv);
      }

      delete itv;
    }

    ++itrv;
  }

  // loop on attribute values to restore
  TLP_HASH_MAP<Graph*, DataSet>& attValues =
    undo ? oldAttributeValues : newAttributeValues;
  TLP_HASH_MAP<Graph*, DataSet>::const_iterator itav = attValues.begin();

  while (itav != attValues.end()) {
    Graph* g = itav->first;
    Iterator<pair<string, DataType*> > *itv = itav->second.getValues();

    while(itv->hasNext()) {
      pair<string, DataType*> pval = itv->next();

      if (pval.second)
        g->getNonConstAttributes().setData(pval.first, pval.second);
      else
        g->getNonConstAttributes().remove(pval.first);
    }

    delete itv;
    ++itav;
  }

  Observable::unholdObservers();
}

bool GraphUpdatesRecorder::dontObserveProperty(PropertyInterface* prop) {
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
      Graph* g = prop->getGraph();
      TLP_HASH_MAP<Graph*, set<PropertyInterface*> >::iterator it =
        addedProperties.find(g);

      if (it != addedProperties.end() &&
          (it->second.find(prop) != it->second.end()))
        // the property is no longer recorded
        it->second.erase(prop);

      return true;
    }
  }

  return false;
}

bool GraphUpdatesRecorder::isAddedOrDeletedProperty(Graph* g,
    PropertyInterface *prop) {
  TLP_HASH_MAP<Graph*, set<PropertyInterface*> >::const_iterator it =
    addedProperties.find(g);

  if (it != addedProperties.end() &&
      (it->second.find(prop) != it->second.end()))
    return true;

  it = deletedProperties.find(g);
  return it != deletedProperties.end() &&
         (it->second.find(prop) != it->second.end());
}


void GraphUpdatesRecorder::addNode(Graph* g, node n) {
  GraphEltsRecord *gnr = graphAddedNodes.get(g->getId());

  if (gnr == NULL) {
    gnr = new GraphEltsRecord(g);
    graphAddedNodes.set(g->getId(), gnr);
  }

  gnr->elts.set(n, true);

  if (g->getRoot() == g) {
    addedNodes.set(n, true);
  }
}

void GraphUpdatesRecorder::addEdge(Graph* g, edge e) {
  GraphEltsRecord *ger = graphAddedEdges.get(g->getId());

  if (ger == NULL) {
    ger = new GraphEltsRecord(g);
    graphAddedEdges.set(g->getId(), ger);
  }

  ger->elts.set(e, true);

  if (g->getRoot() == g) {
    addedEdgesEnds.set(e, new std::pair<node, node>(g->ends(e)));
  }
}

void GraphUpdatesRecorder::delNode(Graph* g, node n) {
  GraphEltsRecord *gnr = graphAddedNodes.get(g->getId());

  if (gnr != NULL && gnr->elts.get(n)) {
    // remove n from graph's recorded nodes if it is a newly added node
    gnr->elts.set(n, false);
    // but don't remove it from addedNodes
    // to ensure further erasal from property will not
    // record a value as if it was a preexisting node
    return;
  }

  // insert n into graphDeletedNodes
  gnr = graphDeletedNodes.get(g->getId());

  if (gnr == NULL) {
    gnr = new GraphEltsRecord(g);
    graphDeletedNodes.set(g->getId(), gnr);
  }

  gnr->elts.set(n, true);

  // no need of the loop below because properties are observed too
  // loop on properties to save the node's associated values

  /*PropertyInterface* prop;
  forEach(prop, g->getLocalObjectProperties()) {
  beforeSetNodeValue(prop, n);
  }*/
  if (g == g->getSuperGraph())
    recordEdgeContainer(oldContainers, (GraphImpl*) g, n);
}

void GraphUpdatesRecorder::delEdge(Graph* g, edge e) {
  GraphEltsRecord* ger = graphAddedEdges.get(g->getId());

  // remove e from addedEdges if it is a newly added edge
  if (ger != NULL && ger->elts.get(e)) {
    ger->elts.set(e, false);
    // do not remove from addedEdgesEnds
    // to ensure further erasal from property will not
    // record a value as if it was a preexisting edge
    /* if (graphs.empty())
    addedEdges.erase(it); */
    // remove from revertedEdges if needed
    set<edge>::iterator itR = revertedEdges.find(e);

    if (itR != revertedEdges.end())
      revertedEdges.erase(itR);

    // remove edge from nodes newContainers if needed
    std::pair<node, node>* eEnds = addedEdgesEnds.get(e);

    if (eEnds) {
      removeFromEdgeContainer(newContainers, e, eEnds->first);
      removeFromEdgeContainer(newContainers, e, eEnds->second);
    }

    return;
  }

  // insert e into graph's deleted edges
  ger = graphDeletedEdges.get(g->getId());

  if (ger == NULL) {
    ger = new GraphEltsRecord(g);
    graphDeletedEdges.set(g->getId(), ger);
  }

  if (deletedEdgesEnds.get(e) == NULL) {
    const pair<node, node> &eEnds = g->ends(e);

    if (g == g->getSuperGraph()) {
      // remove from revertedEdges if needed
      set<edge>::iterator it = revertedEdges.find(e);

      if (it != revertedEdges.end()) {
        revertedEdges.erase(it);
        deletedEdgesEnds.set(e, new std::pair<node, node>(eEnds.second, eEnds.first));
      }
      else {
        // iterator below cannot be const
        // because stdext::hash_map::erase (used on MACOSX)
        // does not support a const iterator as argument
        TLP_HASH_MAP<edge, pair<node, node> >::iterator ite =
          oldEdgesEnds.find(e);

        if (ite == oldEdgesEnds.end())
          deletedEdgesEnds.set(e, new std::pair<node, node>(eEnds));
        else {
          deletedEdgesEnds.set(e, new std::pair<node, node>(ite->second));
          // remove from oldEdgesEnds
          oldEdgesEnds.erase(ite);
          // remove from newEdgesEnds
          newEdgesEnds.erase(e);
        }
      }
    }
    else
      deletedEdgesEnds.set(e, new std::pair<node, node>(eEnds));
  }

  ger->elts.set(e, true);

  // no need of the loop below because properties are observed too
  // loop on properties

  /*PropertyInterface* prop;
  // loop on properties to save the edge's associated values
  forEach(prop, g->getLocalObjectProperties()) {
  beforeSetEdgeValue(prop, e);
  }*/
  if (g == g->getSuperGraph()) {
    // record source & target old containers
    const pair<node, node> &eEnds = g->ends(e);
    recordEdgeContainer(oldContainers, (GraphImpl*) g, eEnds.first);
    recordEdgeContainer(oldContainers, (GraphImpl*) g, eEnds.second);
  }
}

void GraphUpdatesRecorder::reverseEdge(Graph* g, edge e) {
  if (g == g->getSuperGraph()) {
    pair<node, node>* eEnds = addedEdgesEnds.get(e);

    // if it is a newly added edge revert its source and target
    if (eEnds != NULL) {
      node src = eEnds->first;
      eEnds->first = eEnds->second;
      eEnds->second = src;
      return;
    }

    TLP_HASH_MAP<edge, pair<node, node> >::iterator itne = newEdgesEnds.find(e);

    if (itne != newEdgesEnds.end()) {
      // revert ends of itne
      node src = itne->second.first;
      itne->second.first = itne->second.second;
      itne->second.second = src;
    }
    else {   // update reverted edges
      set<edge>::iterator it = revertedEdges.find(e);

      if (it != revertedEdges.end())
        revertedEdges.erase(it);
      else {
        revertedEdges.insert(e);
        // record source & target old containers
        const pair<node, node>& eEnds = g->ends(e);
        recordEdgeContainer(oldContainers, (GraphImpl*) g, eEnds.first);
        recordEdgeContainer(oldContainers, (GraphImpl*) g, eEnds.second);
      }
    }
  }
}

void GraphUpdatesRecorder::beforeSetEnds(Graph* g, edge e) {
  if (g == g->getSuperGraph() && oldEdgesEnds.find(e) == oldEdgesEnds.end()
      && addedEdgesEnds.get(e) == NULL) {
    pair<node, node> ends = g->ends(e);
    set<edge>::iterator it = revertedEdges.find(e);

    // if it is a reverted edge
    // remove it from the set
    if (it != revertedEdges.end()) {
      revertedEdges.erase(it);
      // revert ends of it
      node tgt = ends.first;
      ends.first = ends.second;
      ends.second = tgt;
    }
    else {
      // record source & target old containers
      recordEdgeContainer(oldContainers, (GraphImpl*) g, ends.first);
      recordEdgeContainer(oldContainers, (GraphImpl*) g, ends.second);
    }

    // add e old ends in oldEdgesEnds
    oldEdgesEnds[e] = ends;
  }
}

void GraphUpdatesRecorder::afterSetEnds(Graph* g, edge e) {
  if (g == g->getSuperGraph()) {
    const pair<node, node>& ends = g->ends(e);
    std::pair<node, node>* eEnds = addedEdgesEnds.get(e);

    // if it is a newly added edge update its source and target
    if (eEnds != NULL) {
      eEnds->first = ends.first;
      eEnds->second = ends.second;
      return;
    }

    // update new ends in newEdgesEnds
    newEdgesEnds[e] = ends;
  }
}

void GraphUpdatesRecorder::addSubGraph(Graph* g, Graph* sg) {
  // last added sub-graph will be deleted first during undo/redo
  addedSubGraphs.push_front(std::make_pair(g, sg));

  if (restartAllowed)
    sg->addListener(this);
}

void GraphUpdatesRecorder::delSubGraph(Graph* g, Graph* sg) {

  std::pair<Graph*, Graph*> p = std::make_pair(g, sg);

  std::list<std::pair<Graph*, Graph*> >::iterator it = std::find(addedSubGraphs.begin(), addedSubGraphs.end(), p);

  // remove sg from addedSubGraphs if it is a newly added subgraph
  if (it != addedSubGraphs.end()) {

    addedSubGraphs.erase(it);

    // remove any update data concerning the removed sub-graph
    // as it will be deleted
    removeGraphData(sg);

    // but set its subgraphs as added in its supergraph
    Iterator<Graph *> *itss = sg->getSubGraphs();

    while (itss->hasNext()) {
      Graph* ssg = itss->next();
      addSubGraph(g, ssg);
    }

    delete itss;

    return;
  }

  // last deleted sub-graph will be the last one created during undo/redo
  deletedSubGraphs.push_back(p);

  // sg is no longer observed
  sg->removeListener(this);

  // but it must not be really deleted
  g->setSubGraphToKeep(sg);
}

void GraphUpdatesRecorder::removeGraphData(Graph *g) {
  Graph *sg = NULL;
  forEach(sg, g->getSubGraphs()) {
    std::pair<Graph*, Graph*> p = std::make_pair(g, sg);
    std::list<std::pair<Graph*, Graph*> >::iterator it = std::find(addedSubGraphs.begin(), addedSubGraphs.end(), p);

    if (it != addedSubGraphs.end()) {
      addedSubGraphs.erase(it);
    }
  }
  graphAddedNodes.set(g->getId(), NULL);
  graphDeletedNodes.set(g->getId(), NULL);
  graphAddedEdges.set(g->getId(), NULL);
  graphDeletedEdges.set(g->getId(), NULL);
  addedProperties.erase(g);
  deletedProperties.erase(g);
  oldAttributeValues.erase(g);
  newAttributeValues.erase(g);
}

void GraphUpdatesRecorder::addLocalProperty(Graph* g, const string& name) {
  TLP_HASH_MAP<Graph*, set<PropertyInterface*> >::const_iterator it =
    addedProperties.find(g);

  PropertyInterface* prop = g->getProperty(name);

  if (it == addedProperties.end()) {
    set<PropertyInterface*>  props;
    props.insert(prop);
    addedProperties[g] = props;
  }
  else
    addedProperties[g].insert(prop);
}

void GraphUpdatesRecorder::delLocalProperty(Graph* g, const string& name) {
  PropertyInterface* prop = g->getProperty(name);
  TLP_HASH_MAP<Graph*, set<PropertyInterface*> >::iterator it =
    addedProperties.find(g);

  // remove p from addedProperties if it is a newly added one
  if (it != addedProperties.end() && (it->second.find(prop) != it->second.end())) {
    // the property is no longer recorded
    it->second.erase(prop);
    // remove from renamed properties
    // if needed
    TLP_HASH_MAP<PropertyInterface*, std::string>::iterator itr =
      renamedProperties.find(prop);

    if (itr != renamedProperties.end())
      renamedProperties.erase(itr);

    return;
  }

  // insert p into deletedProperties
  it = deletedProperties.find(g);

  if (it == deletedProperties.end()) {
    set<PropertyInterface*>  props;
    props.insert(prop);
    deletedProperties[g] = props;
  }
  else
    deletedProperties[g].insert(prop);

  // the property is no longer observed
  prop->removeListener(this);
}

void GraphUpdatesRecorder::propertyRenamed(PropertyInterface* prop) {
  TLP_HASH_MAP<Graph*, set<PropertyInterface*> >::iterator it =
    addedProperties.find(prop->getGraph());

  // remove p from addedProperties if it is a newly added one
  if (it != addedProperties.end() &&
      (it->second.find(prop) != it->second.end())) {
    return;
  }
  else {
    if (renamedProperties.find(prop) == renamedProperties.end())
      renamedProperties[prop] = prop->getName();
  }
}

void GraphUpdatesRecorder::beforeSetNodeValue(PropertyInterface* p, node n) {
  // dont record the old value if the default one has been changed
  if (oldNodeDefaultValues.find(p) != oldNodeDefaultValues.end())
    return;

  // don't record old values for newly added nodes
  if (addedNodes.get(n)) {
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
  }
  else {
    TLP_HASH_MAP<PropertyInterface*, RecordedValues>::iterator it =
      oldValues.find(p);

    if (it == oldValues.end()) {
      PropertyInterface* pv = p->clonePrototype(p->getGraph(), "");
      MutableContainer<bool>* rn = new MutableContainer<bool>();

      pv->copy(n, n, p);
      rn->set(n, true);
      oldValues[p] = RecordedValues(pv, rn);
    }
    // check for a previously recorded old value
    else {
      if (it->second.recordedNodes) {
        if (it->second.recordedNodes->get(n))
          return;
      }
      else
        it->second.recordedNodes = new MutableContainer<bool>();

      it->second.values->copy(n, n, p);
      it->second.recordedNodes->set(n, true);
    }
  }
}

void GraphUpdatesRecorder::beforeSetAllNodeValue(PropertyInterface* p) {
  if  (oldNodeDefaultValues.find(p) == oldNodeDefaultValues.end()) {
    // first save the already existing value for all non default valuated nodes
    node n;
    forEach(n, p->getNonDefaultValuatedNodes())
    beforeSetNodeValue(p, n);
    // then record the old default value
    // because beforeSetNodeValue does nothing if it has already been changed
    oldNodeDefaultValues[p] = p->getNodeDefaultDataMemValue();
  }
}

void GraphUpdatesRecorder::beforeSetEdgeValue(PropertyInterface* p, edge e) {
  // dont record the old value if the default one has been changed
  if (oldEdgeDefaultValues.find(p) !=  oldEdgeDefaultValues.end())
    return;

  // dont record old value for newly added edge
  if (addedEdgesEnds.get(e)) {
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
  }
  else {
    TLP_HASH_MAP<PropertyInterface*, RecordedValues>::iterator it =
      oldValues.find(p);

    if (it == oldValues.end()) {
      PropertyInterface* pv = p->clonePrototype(p->getGraph(), "");
      MutableContainer<bool>* re = new MutableContainer<bool>();

      pv->copy(e, e, p);
      re->set(e, true);
      oldValues[p] = RecordedValues(pv, NULL, re);
    }
    // check for a previously recorded old value
    else {
      if (it->second.recordedEdges) {
        if (it->second.recordedEdges->get(e))
          return;
      }
      else
        it->second.recordedEdges = new MutableContainer<bool>();

      it->second.values->copy(e, e, p);
      it->second.recordedEdges->set(e, true);
    }
  }
}

void GraphUpdatesRecorder::beforeSetAllEdgeValue(PropertyInterface* p) {
  if (oldEdgeDefaultValues.find(p) == oldEdgeDefaultValues.end()) {
    // first save the already existing value for all non default valuated edges
    edge e;
    forEach(e, p->getNonDefaultValuatedEdges())
    beforeSetEdgeValue(p, e);
    // then record the old default value
    // because beforeSetEdgeValue does nothing if it has already been changed
    oldEdgeDefaultValues[p] = p->getEdgeDefaultDataMemValue();
  }
}

void GraphUpdatesRecorder::beforeSetAttribute(Graph* g,
    const std::string& name) {
  TLP_HASH_MAP<Graph*, DataSet>::iterator it =
    oldAttributeValues.find(g);

  if (it != oldAttributeValues.end() && it->second.exist(name))
    return;

  // save the previously existing value
  DataType* valType = g->getAttributes().getData(name);
  oldAttributeValues[g].setData(name, valType);
}

void GraphUpdatesRecorder::removeAttribute(Graph* g,
    const std::string& name) {
  beforeSetAttribute(g, name);
}
