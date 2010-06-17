/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
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
#include "tulip/GraphUpdatesRecorder.h"
#include "tulip/ForEach.h"
#include "tulip/GraphImpl.h"
#include "tulip/GraphProperty.h"

using namespace std;
using namespace stdext;
using namespace tlp;

GraphUpdatesRecorder::GraphUpdatesRecorder(bool allowRestart) : GraphObserver(false), PropertyObserver(false),
#if !defined(NDEBUG)
  recordingStopped(true),
#endif
  updatesReverted(false),
  restartAllowed(allowRestart),
  newValuesRecorded(false)
{}

GraphUpdatesRecorder::~GraphUpdatesRecorder() {
  deleteDeletedObjects();
  deleteValues(oldNodeValues);
  deleteValues(newNodeValues);
  deleteValues(oldEdgeValues);
  deleteValues(newEdgeValues);
  deleteDefaultValues(oldNodeDefaultValues);
  deleteDefaultValues(newNodeDefaultValues);
  deleteDefaultValues(oldEdgeDefaultValues);
  deleteDefaultValues(newEdgeDefaultValues);
}

// delete the objects collected as to be deleted
void GraphUpdatesRecorder::deleteDeletedObjects() {
  TLP_HASH_MAP<unsigned long, set<Graph *> >& subGraphsToDelete =
    updatesReverted ? addedSubGraphs : deletedSubGraphs;
  TLP_HASH_MAP<unsigned long, set<PropertyRecord> >& propertiesToDelete =
    updatesReverted ? addedProperties : deletedProperties;

  // loop on sub graphs
  TLP_HASH_MAP<unsigned long, set<Graph *> >::iterator itds =
     subGraphsToDelete.begin();
  while(itds != subGraphsToDelete.end()) {
    set<Graph*>::iterator its = (*itds).second.begin();
    set<Graph*>::iterator ite = (*itds).second.end();
    while(its != ite) {
      delete (*its);
      ++its;
    }
    ++itds;
  }

  // loop on properties
  TLP_HASH_MAP<unsigned long, set<PropertyRecord> >::iterator itdp =
    propertiesToDelete.begin();
  while(itdp != propertiesToDelete.end()) {
    set<PropertyRecord>::iterator itp =  (*itdp).second.begin();
    set<PropertyRecord>::iterator ite = (*itdp).second.end();
    while(itp != ite) {
      delete ((*itp).prop);
      ++itp;
    }
    ++itdp;
  }
}

// clean up all the MutableContainers
void GraphUpdatesRecorder::deleteValues(TLP_HASH_MAP<unsigned long,
					MutableContainer<DataMem*>* >& values) {
  TLP_HASH_MAP<unsigned long, MutableContainer<DataMem*>* >::iterator itnv =
    values.begin();
  while(itnv != values.end()) {
    deleteValues((*itnv).second);
    ++itnv;
  }
  values.clear();
}

// delete all the DataMem referenced by a MutableContainer
void GraphUpdatesRecorder::deleteValues(MutableContainer<DataMem*>* values) {
  IteratorValue* itv = values->findAll(NULL, false);
  while(itv->hasNext()) {
    TypedValueContainer<DataMem*> tVal;
    itv->nextValue(tVal);
    delete tVal.value;
  }
  delete itv;
  delete values;
}

// delete all the DataMem referenced by a TLP_HASH_MAP
void GraphUpdatesRecorder::deleteDefaultValues(TLP_HASH_MAP<unsigned long,
					       DataMem*>& values) {
  TLP_HASH_MAP<unsigned long, DataMem*>::iterator itv = values.begin();
  TLP_HASH_MAP<unsigned long, DataMem*>::iterator itve = values.end();
  while(itv != itve) {
    delete itv->second;
    ++itv;
  }
  values.clear();
}

void GraphUpdatesRecorder::recordEdgeContainer(TLP_HASH_MAP<node, vector<edge> >& containers, GraphImpl* g, node n) {
  TLP_HASH_MAP<node, vector<edge> >::iterator itc =
    containers.find(n);
  if (itc == containers.end()) {
    // record edges container of n
    vector<edge> vEdges;
    GraphImpl::EdgeContainer& edges = g->nodes[n.id];
    for (unsigned int i = 0; i < edges.size(); ++i)
      vEdges.push_back(edges[i]);
    containers[n] = vEdges;
  }
}

void GraphUpdatesRecorder::removeFromEdgeContainer(TLP_HASH_MAP<node, vector<edge> >& containers, edge e, node n) {
  TLP_HASH_MAP<node, vector<edge> >::iterator itc =
    containers.find(n);
  if (itc != containers.end()) {
    vector<edge>::iterator it = (*itc).second.begin();
    vector<edge>::iterator ite = (*itc).second.end();
    while(it != ite) {
      if ((*it) == e) {
	(*itc).second.erase(it);
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

    // record nodeIds & edgeIds
    GraphImpl* root = (GraphImpl*) g;
    newNodeIdManager = root->nodeIds;
    newEdgeIdManager = root->edgeIds;
    // record new edges containers
    TLP_HASH_MAP<edge, EdgeRecord>::iterator itae = addedEdges.begin();
    while(itae != addedEdges.end()) {
      recordEdgeContainer(newContainers, root,(*itae).second.source);
      recordEdgeContainer(newContainers, root,(*itae).second.target);
      ++itae;
    }
    // record new properties default values & new values
    // loop on oldNodeDefaultValues
    TLP_HASH_MAP<unsigned long, DataMem*>::iterator itdv =
      oldNodeDefaultValues.begin();
    while(itdv != oldNodeDefaultValues.end()) {
      PropertyInterface* p = (PropertyInterface *) (*itdv).first;
      newNodeDefaultValues[(unsigned long) p] =
	p->getNodeDefaultDataMemValue();
      recordNewNodeValues(p);
      ++itdv;
    }
    // loop on oldNodeValues
    TLP_HASH_MAP<unsigned long, MutableContainer<DataMem*>* >::iterator itov =
      oldNodeValues.begin();
    while(itov != oldNodeValues.end()) {
      PropertyInterface* p = (PropertyInterface *) (*itov).first;
      if (oldNodeDefaultValues.find((unsigned long) p) ==
	  oldNodeDefaultValues.end())
	recordNewNodeValues(p);
      itov++;
    }
    // loop on updatedPropsAddedNodes
    TLP_HASH_MAP<unsigned long, std::set<node> >::iterator itan =
      updatedPropsAddedNodes.begin();
    while(itan != updatedPropsAddedNodes.end()) {
      PropertyInterface* p = (PropertyInterface *) (*itan).first;
      TLP_HASH_MAP<unsigned long, MutableContainer<DataMem*>* >::iterator itnv =
	newNodeValues.find((unsigned long) p);
      MutableContainer<DataMem*>*  nv;
      bool created = itnv == newNodeValues.end();
      bool hasNewValues = false;
      if (created) {
	nv = new MutableContainer<DataMem*>;
	nv->setAll(NULL);
      } else
	nv = (*itnv).second;
      set<node>::iterator itn = (*itan).second.begin();
      set<node>::iterator itne = (*itan).second.end();
      while(itn != itne) {
	DataMem* value = p->getNonDefaultDataMemValue(*itn);
	// record value only if it is not the default one
	if (value) {
	  nv->set((*itn).id, value);
	  hasNewValues = true;
	}
	itn++;
      }
      if (created) {
	if (hasNewValues)
	  newNodeValues[(unsigned long) p] = nv;
	else
	  delete nv;
      }
      itan++;
    }
    // loop on oldEdgeDefaultValues
    itdv = oldEdgeDefaultValues.begin();
    while(itdv != oldNodeDefaultValues.end()) {
      PropertyInterface* p = (PropertyInterface *) (*itdv).first;
      newEdgeDefaultValues[(unsigned long) p] =
	p->getEdgeDefaultDataMemValue();
      recordNewEdgeValues(p);
      itdv++;
    }
    // loop on oldEdgeValues
    itov = oldEdgeValues.begin();
    while(itov != oldEdgeValues.end()) {
      PropertyInterface* p = (PropertyInterface *) (*itov).first;
      if (oldEdgeDefaultValues.find((unsigned long) p) ==
	  oldEdgeDefaultValues.end())
	recordNewEdgeValues(p);
      itov++;
    }
    // loop on updatedPropsAddedEdges
    TLP_HASH_MAP<unsigned long, std::set<edge> >::iterator iten =
      updatedPropsAddedEdges.begin();
    while(iten != updatedPropsAddedEdges.end()) {
      PropertyInterface* p = (PropertyInterface *) (*iten).first;
      TLP_HASH_MAP<unsigned long, MutableContainer<DataMem*>* >::iterator itnv =
	newEdgeValues.find((unsigned long) p);
      MutableContainer<DataMem*>*  nv;
      bool created = itnv == newEdgeValues.end();
      bool hasNewValues = false;
      if (created) {
	nv = new MutableContainer<DataMem*>;
	nv->setAll(NULL);
      } else
	nv = (*itnv).second;
      set<edge>::iterator ite = (*iten).second.begin();
      set<edge>::iterator itee = (*iten).second.end();
      while(ite != itee) {
	DataMem* value = p->getNonDefaultDataMemValue(*ite);
	// record value only if it is not the default one
	if (value) {
	  nv->set((*ite).id, value);
	  hasNewValues = true;
	}
	ite++;
      }
      if (created) {
	if (hasNewValues)
	  newEdgeValues[(unsigned long) p] = nv;
	else
	  delete nv;
      }
      iten++;
    }
    // record graph attribute new values
    TLP_HASH_MAP<unsigned long, DataSet>::iterator itav =
      oldAttributeValues.begin();
    while (itav != oldAttributeValues.end()) {
      Graph* g = (Graph *) (*itav).first;
      Iterator<pair<string, DataType*> > *itv = (*itav).second.getValues();
      const DataSet& gAttValues = g->getAttributes();
      DataSet& nAttValues = newAttributeValues[(unsigned long) g];
      while(itv->hasNext()) {
      pair<string, DataType*> pval = itv->next();
      nAttValues.setData(pval.first, gAttValues.getData(pval.first));
    } delete itv;
    ++itav;
    }
  }
}

void GraphUpdatesRecorder::recordNewNodeValues(PropertyInterface* p) {
  assert(newNodeValues.find((unsigned long) p) == newNodeValues.end());
  MutableContainer<DataMem*>*  nv = new MutableContainer<DataMem*>;
  nv->setAll(NULL);
  bool hasNewValues = false;
  // record updated nodes new values
  if  (oldNodeDefaultValues.find((unsigned long) p) !=
       oldNodeDefaultValues.end()) {
    // loop on non default valuated nodes
    Iterator<node>* itn = p->getNonDefaultValuatedNodes();
    while(itn->hasNext()) {
      node n(itn->next());
      DataMem* value = p->getNonDefaultDataMemValue(n);
      nv->set(n.id, value);
      hasNewValues = true;
    } delete itn;
  } else {
    TLP_HASH_MAP<unsigned long, MutableContainer<DataMem*>* >::iterator itp = 
      oldNodeValues.find((unsigned long) p);
    if (itp != oldNodeValues.end()) {
      MutableContainer<DataMem*>* opv = (*itp).second;
      IteratorValue* itov = opv->findAll(NULL, false);
      while(itov->hasNext()) {
	node n(itov->next());
	DataMem* value = p->getNonDefaultDataMemValue(n);
	// record value only if it is not the default one
	if (value) {
	  nv->set(n.id, value);
	  hasNewValues = true;
	}
      }
      delete itov;
    }
  }
  if (hasNewValues)
    newNodeValues[(unsigned long) p] = nv;
  else
    delete nv;
}

void GraphUpdatesRecorder::recordNewEdgeValues(PropertyInterface* p) {
  assert(newEdgeValues.find((unsigned long) p) == newEdgeValues.end());
  MutableContainer<DataMem*>*  nv = new MutableContainer<DataMem*>;
  nv->setAll(NULL);
  bool hasNewValues = false;
  // record updated edges new values
  if  (oldEdgeDefaultValues.find((unsigned long) p) !=
       oldEdgeDefaultValues.end()) {
    // loop on non default valuated edges
    Iterator<edge>* ite = p->getNonDefaultValuatedEdges();
    while(ite->hasNext()) {
      edge e(ite->next());
      DataMem* value = p->getNonDefaultDataMemValue(e);
      nv->set(e.id, value);
      hasNewValues = true;
    } delete ite;
  } else {
    TLP_HASH_MAP<unsigned long, MutableContainer<DataMem*>* >::iterator itp = 
      oldEdgeValues.find((unsigned long) p);
    if (itp != oldEdgeValues.end()) {
      MutableContainer<DataMem*>* opv = (*itp).second;
      IteratorValue* itov = opv->findAll(NULL, false);
      while(itov->hasNext()) {
	edge e(itov->next());
	DataMem* value = p->getNonDefaultDataMemValue(e);
	// record value only if it is not the default one
	if (value) {
	  nv->set(e.id, value);
	  hasNewValues = true;
	}
      }
      delete itov;
    }
  }
  if (hasNewValues)
    newEdgeValues[(unsigned long) p] = nv;
  else
    delete nv;
}

void GraphUpdatesRecorder::startRecording(GraphImpl* g) {
  if (g->getSuperGraph() == g) {
    GraphImpl* root = (GraphImpl*) g;
    oldNodeIdManager = root->nodeIds;
    oldEdgeIdManager = root->edgeIds;
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
    deleteValues(newNodeValues);
    deleteValues(newEdgeValues);
    deleteDefaultValues(newNodeDefaultValues);
    deleteDefaultValues(newEdgeDefaultValues);
    newValuesRecorded = false;
  }

  g->addGraphObserver(this);
  
  // add self as a PropertyObserver for all previously
  // existing properties
  TLP_HASH_MAP<unsigned long, set<PropertyRecord> >::iterator itp =
    addedProperties.find((unsigned long) g);
  set<PropertyRecord>*  newProps = NULL;
  if (itp != addedProperties.end())
    newProps = &((*itp).second);
  PropertyInterface* prop;
  forEach(prop, g->getLocalObjectProperties()) {
    if (newProps) {
      PropertyRecord p(prop,  prop->getName());
      if (newProps->find(p) != newProps->end())
	continue;
    }
    prop->addPropertyObserver(this);
  }

  // add self as a GraphObserver for all previously
  // existing subgraphs
  TLP_HASH_MAP<unsigned long, set<Graph*> >::iterator itg =
    addedSubGraphs.find((unsigned long) g);
  set<Graph*>* newSubGraphs = NULL;
  if (itg != addedSubGraphs.end())
    newSubGraphs = &((*itg).second);
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
  g->removeGraphObserver(this);
  PropertyInterface* prop;
  forEach(prop, g->getLocalObjectProperties())
    prop->removePropertyObserver(this);
  Graph* sg;
  forEach(sg, g->getSubGraphs())
    stopRecording(sg);  
}

void GraphUpdatesRecorder::doUpdates(GraphImpl* g, bool undo) {
  assert(updatesReverted != undo);
  updatesReverted = undo;

  Observable::holdObservers();
  // loop on propsToDel
  TLP_HASH_MAP<unsigned long,  set<PropertyRecord> >& propsToDel =
    undo ? addedProperties : deletedProperties; 
  TLP_HASH_MAP<unsigned long,  set<PropertyRecord> >::iterator itpg =
    propsToDel.begin();
  while(itpg != propsToDel.end()) {
    Graph* g = (Graph*) (*itpg).first;
    set<PropertyRecord>::iterator itp = (*itpg).second.begin();
    set<PropertyRecord>::iterator itpe = (*itpg).second.end();
    while(itp != itpe) {
      g->delLocalProperty((*itp).name);
      ++itp;
    }
    ++itpg;
  }
  // loop on subGraphsToDel
  TLP_HASH_MAP<unsigned long, std::set<Graph*> >& subGraphsToDel =
    undo ? addedSubGraphs : deletedSubGraphs; 
  TLP_HASH_MAP<unsigned long, std::set<Graph*> >::iterator its =
    subGraphsToDel.begin();
  while(its != subGraphsToDel.end()) {
    Graph* g = (Graph*) (*its).first;
    set<Graph*>::iterator itg = (*its).second.begin();
    set<Graph*>::iterator itge = (*its).second.end();
    while(itg != itge) {
      // remove from list of subgraphs + notify observers
      g->removeSubGraph((*itg), true);
      ++itg;
    }
    ++its;
  }
  // loop on edgesToDel
  TLP_HASH_MAP<edge, EdgeRecord>& edgesToDel =
    undo ? addedEdges : deletedEdges;
  TLP_HASH_MAP<edge, EdgeRecord>::iterator ite = edgesToDel.begin();
  while(ite != edgesToDel.end()) {
    edge e = (*ite).first;
    // iterate in reverse order
    // to ensure it will be removed at last from the graph root
    set<Graph*>::reverse_iterator itg = (*ite).second.graphs.rbegin();
    set<Graph*>::reverse_iterator itge = (*ite).second.graphs.rend();
    while(itg != itge) {
      if ((*itg)->isElement(e))
	(*itg)->removeEdge(e);
      ++itg;
    }
    ++ite;
  }
  // loop on nodesToDel
  TLP_HASH_MAP<node, set<Graph*> >& nodesToDel =
    undo ? addedNodes : deletedNodes;
  TLP_HASH_MAP<node, set<Graph*> >::iterator itn = nodesToDel.begin();
  while(itn != nodesToDel.end()) {
    node n = (*itn).first;
    set<Graph*>::iterator itg = (*itn).second.begin();
    set<Graph*>::iterator itge = (*itn).second.end();
    while(itg != itge) {
      (*itg)->removeNode(n);
      ++itg;
    }
    ++itn;
  }
  // loop on subGraphsToAdd
  TLP_HASH_MAP<unsigned long, std::set<Graph*> >& subGraphsToAdd =
    undo ? deletedSubGraphs : addedSubGraphs; 
  its = subGraphsToAdd.begin();
  while(its != subGraphsToAdd.end()) {
    Graph* g = (Graph*) (*its).first;
    set<Graph*>::iterator itg = (*its).second.begin();
    set<Graph*>::iterator itge = (*its).second.end();
    while(itg != itge) {
      g->restoreSubGraph((*itg), true);
      ++itg;
    }
    ++its;
  }
  // loop on nodesToAdd
  TLP_HASH_MAP<node, set<Graph*> >& nodesToAdd =
    undo ? deletedNodes : addedNodes;
  itn = nodesToAdd.begin();
  while(itn != nodesToAdd.end()) {
    node n = (*itn).first;
    set<Graph*>::iterator itg = (*itn).second.begin();
    set<Graph*>::iterator itge = (*itn).second.end();
    while(itg != itge) {
      (*itg)->restoreNode(n);
      ++itg;
    }
    ++itn;
  }
  
  // now restore ids manager
  // this is done before the loop on the edges to add
  // because of some assertion in debug mode
  // while calling the restoreEdge method
  g->nodeIds = undo ? oldNodeIdManager : newNodeIdManager;
  g->edgeIds = undo ? oldEdgeIdManager : newEdgeIdManager;

  // loop on edgesToAdd
  TLP_HASH_MAP<edge, EdgeRecord>& edgesToAdd =
    undo ? deletedEdges : addedEdges;
  ite = edgesToAdd.begin();
  while(ite != edgesToAdd.end()) {
    edge e = (*ite).first;
    node s((*ite).second.source);
    node t((*ite).second.target);
    set<Graph*>::iterator itg = (*ite).second.graphs.begin();
    set<Graph*>::iterator itge = (*ite).second.graphs.end();
    while(itg != itge) {
      (*itg)->restoreEdge(e, s, t);
      ++itg;
    }
    ++ite;
  }

  // loop on propsToAdd
  TLP_HASH_MAP<unsigned long,  set<PropertyRecord> >& propsToAdd =
    undo ? deletedProperties : addedProperties; 
  itpg = propsToAdd.begin();
  while(itpg != propsToAdd.end()) {
    Graph* g = (Graph*) (*itpg).first;
    set<PropertyRecord>::iterator itp = (*itpg).second.begin();
    set<PropertyRecord>::iterator itpe = (*itpg).second.end();
    while(itp != itpe) {
      g->addLocalProperty((*itp).name, (*itp).prop);
      ++itp;
    }
    ++itpg;
  }
  // loop on nodeDefaultValues
  TLP_HASH_MAP<unsigned long, DataMem*>& nodeDefaultValues =
    undo ? oldNodeDefaultValues : newNodeDefaultValues;
  TLP_HASH_MAP<unsigned long, DataMem*>::iterator itdv =
    nodeDefaultValues.begin();
  while(itdv != nodeDefaultValues.end()) {
    PropertyInterface* prop = (PropertyInterface *) (*itdv).first;
    prop->setAllNodeDataMemValue((*itdv).second);
    ++itdv;
  }
  // loop on edgeDefaultValues
  TLP_HASH_MAP<unsigned long, DataMem*>& edgeDefaultValues =
    undo ? oldEdgeDefaultValues : newEdgeDefaultValues;
  itdv = edgeDefaultValues.begin();
  while(itdv != edgeDefaultValues.end()) {
    PropertyInterface* prop = (PropertyInterface *) (*itdv).first;
    prop->setAllEdgeDataMemValue((*itdv).second);
    ++itdv;
  }

  // loop on revertedEdges
  set<edge>::iterator itre = revertedEdges.begin();
  while(itre != revertedEdges.end()) {
    g->reverse((*itre));
    ++itre;
  }
  // loop on containers
  TLP_HASH_MAP<node, vector<edge> >& containers =
    undo ? oldContainers : newContainers;
  TLP_HASH_MAP<node, vector<edge> >::iterator itc = containers.begin();
  while(itc != containers.end()) {
    g->restoreContainer((*itc).first, (*itc).second);
    ++itc;
  }
  // loop on nodeValues
  TLP_HASH_MAP<unsigned long, MutableContainer<DataMem*>* >& nodeValues =
    undo ? oldNodeValues : newNodeValues;
  TLP_HASH_MAP<unsigned long, MutableContainer<DataMem*>* >::iterator itnv =
    nodeValues.begin();
  while(itnv != nodeValues.end()) {
    PropertyInterface* prop = (PropertyInterface *) (*itnv).first;
    IteratorValue* itv = (*itnv).second->findAll(NULL, false);
    while(itv->hasNext()) {
      TypedValueContainer<DataMem*> tVal;
      node n(itv->nextValue(tVal));
      prop->setNodeDataMemValue(n, tVal.value);
    }
    delete itv;
    ++itnv;
  }
  // loop on edgeValues
  TLP_HASH_MAP<unsigned long, MutableContainer<DataMem*>* >& edgeValues =
    undo ? oldEdgeValues : newEdgeValues;
  TLP_HASH_MAP<unsigned long, MutableContainer<DataMem*>* >::iterator itev =
    edgeValues.begin();
  while(itev != edgeValues.end()) {
    PropertyInterface* prop = (PropertyInterface *) (*itev).first;
    IteratorValue* itv = (*itev).second->findAll(NULL, false);
    while(itv->hasNext()) {
      TypedValueContainer<DataMem*> tVal;
      edge e(itv->nextValue(tVal));
      prop->setEdgeDataMemValue(e, tVal.value);
    }
    delete itv;
    ++itev;
  }
  // loop on attribute values to restore
  TLP_HASH_MAP<unsigned long, DataSet>& attValues =
    undo ? oldAttributeValues : newAttributeValues;
  TLP_HASH_MAP<unsigned long, DataSet>::iterator itav = attValues.begin();
  while (itav != attValues.end()) {
    Graph* g = (Graph *) (*itav).first;
    Iterator<pair<string, DataType*> > *itv = (*itav).second.getValues();
    while(itv->hasNext()) {
      pair<string, DataType*> pval = itv->next();
      if (pval.second)
	g->getNonConstAttributes().setData(pval.first, pval.second);
      else
	g->getNonConstAttributes().remove(pval.first);
    } delete itv;
    ++itav;
  }
  Observable::unholdObservers();
}

bool GraphUpdatesRecorder::dontObserveProperty(PropertyInterface* prop) {
  if (!restartAllowed) {
    // check if nothing is yet recorded for prop
    unsigned long p = (unsigned long) prop;
    if ((oldNodeDefaultValues.find(p) == oldNodeDefaultValues.end()) &&
	(oldEdgeDefaultValues.find(p) == oldEdgeDefaultValues.end()) &&
	(oldNodeValues.find(p) == oldNodeValues.end()) &&
	(oldEdgeValues.find(p) == oldEdgeValues.end()) &&
	(updatedPropsAddedNodes.find(p) == updatedPropsAddedNodes.end()) &&
	(updatedPropsAddedEdges.find(p) == updatedPropsAddedEdges.end())) {
      // prop is no longer observed
      prop->removePropertyObserver(this);
      return true;
    }
  }
  return false;
}

bool GraphUpdatesRecorder::isAddedOrDeletedProperty(Graph* g,
						    PropertyInterface *prop) {
  PropertyRecord p(prop,  prop->getName());
  TLP_HASH_MAP<unsigned long, set<PropertyRecord> >::iterator it =
    addedProperties.find((unsigned long) g);
  if (it != addedProperties.end() &&
      ((*it).second.find(p) != (*it).second.end()))
    return true;
  it = deletedProperties.find((unsigned long) g);
  return it != deletedProperties.end() &&
    ((*it).second.find(p) != (*it).second.end());
}


void GraphUpdatesRecorder::addNode(Graph* g, node n) {
  TLP_HASH_MAP<node, set<Graph*> >::iterator it =
    addedNodes.find(n);
  if (it == addedNodes.end()) {
    set<Graph*> graphs;
    graphs.insert(g);
    addedNodes[n] = graphs;
  } else
    (*it).second.insert(g);
}

void GraphUpdatesRecorder::addEdge(Graph* g, edge e) {
  TLP_HASH_MAP<edge, EdgeRecord>::iterator it =
    addedEdges.find(e);
  if (it == addedEdges.end()) {
    const pair<node, node> &eEnds = g->ends(e);
    addedEdges[e] = EdgeRecord(g, eEnds.first, eEnds.second);
  } else
    (*it).second.graphs.insert(g);
}

void GraphUpdatesRecorder::delNode(Graph* g, node n) {
  TLP_HASH_MAP<node, set<Graph*> >::iterator it = addedNodes.find(n);
  // remove n from addedNodes if it is a newly added node
  if (it != addedNodes.end()) {
    set<Graph*>& graphs = (*it).second;
    graphs.erase(g);
    // do not remove from addedNodes
    // to ensure further erasal from property will not
    // record a value as if it was a preexisting node
    /*if (graphs.empty())
      addedNodes.erase(it);*/
    return;
  }
  // insert n into deletedNodes
  it = deletedNodes.find(n);
  if (it == deletedNodes.end()) {
    set<Graph*> graphs;
    graphs.insert(g);
    deletedNodes[n] = graphs;
  } else
    (*it).second.insert(g);    set<node> nodes;
  // loop on properties to save the node's associated values
  PropertyInterface* prop;
  forEach(prop, g->getLocalObjectProperties()) {
    beforeSetNodeValue(prop, n);
  }
  if (g == g->getSuperGraph())
    recordEdgeContainer(oldContainers, (GraphImpl*) g, n);
}

void GraphUpdatesRecorder::delEdge(Graph* g, edge e) {
  TLP_HASH_MAP<edge, EdgeRecord>::iterator it = addedEdges.find(e);
  // remove e from addedEdges if it is a newly added edge
  if (it != addedEdges.end()) {
    node src = (*it).second.source;
    node tgt = (*it).second.target;
    set<Graph*>& graphs = (*it).second.graphs;
    graphs.erase(g);
    // do not remove from addedEdges
    // to ensure further erasal from property will not
    // record a value as if it was a preexisting edge
    /* if (graphs.empty())
       addedEdges.erase(it); */
    // remove from revertedEdges if needed
    set<edge>::iterator itR = revertedEdges.find(e);
    if (itR != revertedEdges.end())
      revertedEdges.erase(itR);
    // remove edge from nodes newContainers if needed
    removeFromEdgeContainer(newContainers, e, src);
    removeFromEdgeContainer(newContainers, e, tgt);
    return;
  }
  // insert e into deletedEdges
  it = deletedEdges.find(e);
  if (it == deletedEdges.end()) {
    // remove from revertedEdges if needed
    const pair<node, node> &eEnds = g->ends(e);
    set<edge>::iterator it = revertedEdges.find(e);
    if (it != revertedEdges.end()) {
      revertedEdges.erase(it);
      deletedEdges[e] = EdgeRecord(g, eEnds.second, eEnds.first);
    } else
      deletedEdges[e] = EdgeRecord(g, eEnds.first, eEnds.second);
  }
  else
    (*it).second.graphs.insert(g);
  // loop on properties
  PropertyInterface* prop;
  // loop on properties to save the edge's associated values
  forEach(prop, g->getLocalObjectProperties()) {
    beforeSetEdgeValue(prop, e);
  }
  if (g == g->getSuperGraph()) {
    // record source & target old containers
    const pair<node, node> &eEnds = g->ends(e);
    recordEdgeContainer(oldContainers, (GraphImpl*) g, eEnds.first);
    recordEdgeContainer(oldContainers, (GraphImpl*) g, eEnds.second);
  }
}

void GraphUpdatesRecorder::reverseEdge(Graph* g,  edge e) {
  TLP_HASH_MAP<edge, EdgeRecord>::iterator itA = addedEdges.find(e);
  // remove e from addedEdges if it is a newly added edge
  if (itA != addedEdges.end()) {
    node src = (*itA).second.source;
    node tgt = (*itA).second.target;
    (*itA).second.source = tgt;
    (*itA).second.target = src;
    return;
  }
  set<edge>::iterator it = revertedEdges.find(e);
  if (it != revertedEdges.end())
    revertedEdges.erase(it);
  else
    revertedEdges.insert(e);
}

void GraphUpdatesRecorder::addSubGraph(Graph* g, Graph* sg) {
  TLP_HASH_MAP<unsigned long, set<Graph*> >::iterator it =
    addedSubGraphs.find((unsigned long) g);
  if (it == addedSubGraphs.end()) {
    set<Graph*> subgraphs;
    subgraphs.insert(sg);
    addedSubGraphs[(unsigned long) g] = subgraphs;
  } else
    addedSubGraphs[(unsigned long) g].insert(sg);
  // no need to observe the newly added subgraph
}

void GraphUpdatesRecorder::delSubGraph(Graph* g, Graph* sg) {
  TLP_HASH_MAP<unsigned long, set<Graph*> >::iterator it =
    addedSubGraphs.find((unsigned long) g);
  // remove sg from addedSubGraphs if it is a newly added subgraph
  if (it != addedSubGraphs.end() &&
      ((*it).second.find(sg) != (*it).second.end())) {
    (*it).second.erase(sg);
    return;
  }
  // insert sg into deletedSubGraphs
  it = deletedSubGraphs.find((unsigned long) g);
  if (it == deletedSubGraphs.end()) {
    set<Graph*> subgraphs;
    subgraphs.insert(sg);
    deletedSubGraphs[(unsigned long) g] = subgraphs;
  } else
    deletedSubGraphs[(unsigned long) g].insert(sg);
  // sg is no longer observed
  sg->removeGraphObserver(this);
  // but it must not be really deleted
  g->setSubGraphToKeep(sg);
}

void GraphUpdatesRecorder::addLocalProperty(Graph* g, const string& name) {
  PropertyRecord p(g->getProperty(name),  name);
  TLP_HASH_MAP<unsigned long, set<PropertyRecord> >::iterator it =
    addedProperties.find((unsigned long) g);
  if (it == addedProperties.end()) {
    set<PropertyRecord>  props;
    props.insert(p);
    addedProperties[(unsigned long) g] = props;
  } else
    addedProperties[(unsigned long) g].insert(p);
  // no need to observe the newly added property
}

void GraphUpdatesRecorder::delLocalProperty(Graph* g, const string& name) {
  PropertyRecord p(g->getProperty(name),  name);
  TLP_HASH_MAP<unsigned long, set<PropertyRecord> >::iterator it =
    addedProperties.find((unsigned long) g);
  // remove p from addedProperties if it is a newly added node
  if (it != addedProperties.end() && ((*it).second.find(p) != (*it).second.end())) {
    // the property is no longer recorded
    (*it).second.erase(p);
    return;
  }
  // insert p into deletedProperties
  it = deletedProperties.find((unsigned long) g);
  if (it == deletedProperties.end()) {
    set<PropertyRecord>  props;
    props.insert(p);
    deletedProperties[(unsigned long) g] = props;
  } else
    deletedProperties[(unsigned long) g].insert(p);
  // the property is no longer observed
  p.prop->removePropertyObserver(this);
}

void GraphUpdatesRecorder::beforeSetNodeValue(PropertyInterface* p, node n) {
  // dont record the old value if the default one has been changed
  if (oldNodeDefaultValues.find((unsigned long) p) !=
      oldNodeDefaultValues.end())
    return;
  TLP_HASH_MAP<node, set<Graph*> >::iterator ita = addedNodes.find(n);
  // don't record old values for newly added nodes
  if (ita != addedNodes.end()) {
    if (!restartAllowed)
      return;
    updatedPropsAddedNodes[(unsigned long) p].insert(n);
  } else {
    TLP_HASH_MAP<unsigned long, MutableContainer<DataMem*>* >::iterator it = 
      oldNodeValues.find((unsigned long) p);
    if (it == oldNodeValues.end()) {
      MutableContainer<DataMem*>* pv = new MutableContainer<DataMem*>;
      pv->setAll(NULL);
      pv->set(n.id, p->getNodeDataMemValue(n));
      oldNodeValues[(unsigned long) p] = pv;
    } else {
      if ((*it).second->get(n.id) == NULL)
	(*it).second->set(n.id, p->getNodeDataMemValue(n));
    }
  }
}
            
void GraphUpdatesRecorder::beforeSetAllNodeValue(PropertyInterface* p) {
  if  (oldNodeDefaultValues.find((unsigned long) p) ==
       oldNodeDefaultValues.end()) {
    // first save the already existing value for all non default valuated nodes
    node n;
    forEach(n, p->getNonDefaultValuatedNodes())
      beforeSetNodeValue(p, n);
    // then record the old default value
    // because beforeSetNodeValue does nothing if it has already been changed
    oldNodeDefaultValues[(unsigned long) p] = p->getNodeDefaultDataMemValue();
  }
}
            
void GraphUpdatesRecorder::beforeSetEdgeValue(PropertyInterface* p, edge e) {
  // dont record the old value if the default one has been changed
  if (oldEdgeDefaultValues.find((unsigned long) p) !=
       oldEdgeDefaultValues.end())
    return;
  TLP_HASH_MAP<edge, EdgeRecord>::iterator ita = addedEdges.find(e);
  // dont record old value for newly added edge
  if (ita != addedEdges.end()) {
    if (!restartAllowed)
      return;
    updatedPropsAddedEdges[(unsigned long) p].insert(e);
  } else {
    TLP_HASH_MAP<unsigned long, MutableContainer<DataMem*>* >::iterator it = 
      oldEdgeValues.find((unsigned long) p);
    if (it == oldEdgeValues.end()) {
      MutableContainer<DataMem*>* pv = new MutableContainer<DataMem*>;
      pv->setAll(NULL);
      pv->set(e.id, p->getEdgeDataMemValue(e));
      oldEdgeValues[(unsigned long) p] = pv;
    } else {
      if ((*it).second->get(e.id) == NULL)
	(*it).second->set(e.id, p->getEdgeDataMemValue(e));
    }
  }
}
            
void GraphUpdatesRecorder::beforeSetAllEdgeValue(PropertyInterface* p) {
  if (oldEdgeDefaultValues.find((unsigned long) p) ==
       oldEdgeDefaultValues.end()) {
    // first save the already existing value for all non default valuated edges
    edge e;
    forEach(e, p->getNonDefaultValuatedEdges())
      beforeSetEdgeValue(p, e);
    // then record the old default value
    // because beforeSetEdgeValue does nothing if it has already been changed
    oldEdgeDefaultValues[(unsigned long) p] = p->getEdgeDefaultDataMemValue();
  }
}

void GraphUpdatesRecorder::beforeSetAttribute(Graph* g,
					      const std::string& name) {
  TLP_HASH_MAP<unsigned long, DataSet>::iterator it =
    oldAttributeValues.find((unsigned long) g);
  if (it != oldAttributeValues.end() && it->second.exist(name))
    return;
  // save the previously existing value
  DataType* valType = g->getAttributes().getData(name);
  oldAttributeValues[(unsigned long) g].setData(name, valType);
}

void GraphUpdatesRecorder::removeAttribute(Graph* g,
					   const std::string& name) {
  beforeSetAttribute(g, name);
}
