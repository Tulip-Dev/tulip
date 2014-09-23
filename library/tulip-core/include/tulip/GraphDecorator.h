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

#ifndef Tulip_GraphDecorator_H
#define Tulip_GraphDecorator_H

#include <tulip/Graph.h>

namespace tlp {

class TLP_SCOPE GraphDecorator : public Graph {
public:
  GraphDecorator(Graph* s);
  virtual ~GraphDecorator();

  virtual void clear();
  virtual Graph *addSubGraph(BooleanProperty *selection=0,
                             const std::string& name = "unnamed");
  virtual void delSubGraph(Graph * s);
  virtual void delAllSubGraphs(Graph *s);
  virtual Graph* getSuperGraph()const;
  virtual void setSuperGraph(Graph* s);
  virtual Graph* getRoot() const;
  virtual Iterator<Graph *> * getSubGraphs() const;
  virtual bool isSubGraph(const Graph* sg) const;
  virtual bool isDescendantGraph(const Graph* sg) const;
  virtual Graph* getSubGraph(unsigned int id) const;
  virtual Graph* getSubGraph(const std::string &name) const;
  virtual Graph* getDescendantGraph(unsigned int id) const;
  virtual Graph* getDescendantGraph(const std::string &name) const;
  virtual unsigned int numberOfSubGraphs() const;
  virtual unsigned int numberOfDescendantGraphs() const;

  //============================================================
  virtual node source(const edge e)const;
  virtual void setSource(const edge, const node);
  virtual node target(const edge e)const;
  virtual void setTarget(const edge, const node);
  virtual node opposite(const edge e, const node n)const;
  virtual edge existEdge(const node n, const node m, bool directed = true)const;
  virtual bool hasEdge(const node source, const node target,
                       bool directed = true) const;
  virtual std::vector<edge> getEdges(const node source, const node target,
                                     bool directed = true) const;
  virtual const std::pair<node, node>& ends(const edge e) const;
  virtual void setEnds(const edge, const node, const node);
  virtual void reverse(const edge e);
  virtual unsigned int deg(const node n)const;
  virtual unsigned int indeg(const node n)const;
  virtual unsigned int outdeg(const node n)const;
  virtual Graph* getNodeMetaInfo(const node n)const;

  //============================================================
  virtual node createMetaNode(const std::set<node> &nodeSet, bool multiEdges = true, bool delAllEdge = true);
  virtual void createMetaNodes(Iterator<Graph *> *itS, Graph *quotientGraph, std::vector<node>& metaNodes);
  virtual node createMetaNode(Graph* subGraph, bool multiEdges = true, bool delAllEdge = true);

  //============================================================
  virtual node getOneNode() const;
  virtual node getInNode(const node n,unsigned int i)const;
  virtual node getOutNode(const node n,unsigned int i) const;
  virtual edge getOneEdge() const;

  virtual unsigned int numberOfNodes()const;
  virtual unsigned int numberOfEdges()const;

  //=========================================================================

  virtual void reserveNodes(unsigned int nbNodes);
  virtual void reserveEdges(unsigned int nbEdges);

  //============================================================
  virtual bool isElement(const node n) const;
  virtual bool isMetaNode(const node n) const;
  virtual bool isElement(const edge e) const;
  virtual bool isMetaEdge(const edge e) const;
  virtual node addNode();
  virtual void addNodes(unsigned int nb, std::vector<node>& addedNodes);
  virtual void addNode(const node);
  virtual void addNodes(Iterator<node>* nodes);
  virtual edge addEdge(const node n1, const node n2);
  virtual void addEdges(const std::vector<std::pair<node, node> >& edges,
                        std::vector<edge>& addedEdges);
  virtual void addEdge(const edge);
  virtual void addEdges(Iterator<edge>* edges);
  virtual void delNode(const tlp::node n, bool deleteInAllGraphs = false);
  virtual void delNodes(Iterator<node>* itN, bool deleteInAllGraphs);
  virtual void delEdge(const tlp::edge e, bool deleteInAllGraphs = false);
  virtual void delEdges(Iterator<edge>* itE, bool deleteInAllGraphs = false);
  virtual void setEdgeOrder(const node n,const std::vector<edge> & s);
  virtual void swapEdgeOrder(const node n ,const edge e1, const edge e2);

  virtual Iterator<node>* getNodes() const;
  virtual Iterator<node>* getInNodes(const node n) const;
  virtual Iterator<node>* getOutNodes(const node n) const;
  virtual Iterator<node>* getInOutNodes(const node n) const;
  virtual Iterator<node>* bfs(const node root = node()) const;
  virtual Iterator<node>* dfs(const node root = node()) const;
  virtual Iterator<edge>* getEdges() const;
  virtual Iterator<edge>* getOutEdges(const node n) const;
  virtual Iterator<edge>* getInOutEdges(const node n) const;
  virtual Iterator<edge>* getInEdges(const node n) const;
  virtual Iterator<edge>* getEdgeMetaInfo(const edge) const;
  //============================================================
  virtual PropertyInterface* getProperty(const std::string &name) const;
  virtual bool existProperty(const std::string&name) const;
  virtual bool existLocalProperty(const std::string&name) const;
  virtual void delLocalProperty(const std::string&name);
  virtual void addLocalProperty(const std::string &name, PropertyInterface *prop);
  virtual Iterator<std::string>* getLocalProperties() const;
  virtual Iterator<std::string>* getInheritedProperties() const;
  virtual Iterator<std::string>* getProperties() const;
  virtual Iterator<PropertyInterface*>* getLocalObjectProperties() const;
  virtual Iterator<PropertyInterface*>* getInheritedObjectProperties() const;
  virtual Iterator<PropertyInterface*>* getObjectProperties() const;
  // updates management
  virtual void push(bool unpopAllowed = true,
                    std::vector<PropertyInterface*>* propertiesToPreserveOnPop= NULL);
  virtual void pop(bool unpopAllowed = true);
  virtual void unpop();
  virtual bool canPop();
  virtual bool canUnpop();
  virtual bool canPopThenUnpop();
  //============================================================

  virtual void setName(const std::string &name);
  virtual std::string getName() const;

protected:
  virtual DataSet & getNonConstAttributes();
  Graph* graph_component;
  // designed to reassign an id to a previously deleted elt
  // called by GraphUpdatesRecorder
  virtual node restoreNode(node);
  virtual edge restoreEdge(edge, node source, node target);
  virtual void restoreNodes(const std::vector<node>&);
  virtual void restoreEdges(const std::vector<edge>& edges,
                            const std::vector<std::pair<node, node> >& ends);
  // designed to only update own structures
  // used by GraphUpdatesRecorder
  virtual void removeNode(const node);
  virtual void removeEdge(const edge);
  // internally used to deal with sub graph deletion
  virtual void removeSubGraph(Graph*);
  virtual void clearSubGraphs();
  virtual void restoreSubGraph(Graph*);
  virtual void setSubGraphToKeep(Graph*);
  // internally used for property renaming
  virtual bool renameLocalProperty(PropertyInterface* prop,
                                   const std::string& newName);
};

}

#endif

#endif //DOXYGEN_NOTFOR_DEVEL
///@endcond
