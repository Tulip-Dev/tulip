/*
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#ifndef TULIP_SUPERGRAPHABSTRACT_H
#define TULIP_SUPERGRAPHABSTRACT_H

#include <set>
#include <vector>
#include <tulip/Graph.h>
#include <tulip/DataSet.h>

namespace tlp {

#define GRAPH_SEQ std::vector<Graph *>

template <class C>
struct Iterator;
class PropertyManager;
class GraphProperty;

/// Abstract class for default graph operations.
class TLP_SCOPE GraphAbstract : public Graph {
  friend class PropertyManager;
  DataSet attributes;
  Graph *supergraph;
  Graph *const root;
  GRAPH_SEQ subgraphs;
  Graph *subGraphToKeep;
  // pointer to root viewMetaGraph property
  GraphProperty *metaGraphProperty;

protected:
  GraphAbstract(Graph *supergraph, unsigned int id = 0);

public:
  virtual ~GraphAbstract();
  virtual void clear();
  // use to enforce subgraph id
  Graph *addSubGraph(unsigned int id, BooleanProperty *selection = nullptr,
                     const std::string &name = "unnamed");
  virtual Graph *addSubGraph(BooleanProperty *selection = nullptr,
                             const std::string &name = "unnamed") {
    return addSubGraph(0, selection, name);
  }
  virtual void delSubGraph(Graph *);
  virtual void delAllSubGraphs(Graph *);
  inline Graph *getSuperGraph() const {
    return supergraph;
  }
  inline Graph *getRoot() const {
    return root;
  }
  virtual Iterator<Graph *> *getSubGraphs() const;
  virtual bool isSubGraph(const Graph *sg) const;
  virtual bool isDescendantGraph(const Graph *sg) const;
  virtual Graph *getSubGraph(unsigned int id) const;
  virtual Graph *getSubGraph(const std::string &name) const;
  virtual Graph *getDescendantGraph(unsigned int id) const;
  virtual Graph *getDescendantGraph(const std::string &name) const;
  virtual Graph *getNthSubGraph(unsigned int n) const;
  inline unsigned int numberOfSubGraphs() const {
    return subgraphs.size();
  }
  virtual unsigned int numberOfDescendantGraphs() const;

  //=======================================
  virtual bool isMetaNode(const node) const;
  virtual Graph *getNodeMetaInfo(const node) const;
  virtual void delNodes(Iterator<node> *itN, bool deleteInAllGraphs);
  virtual bool isMetaEdge(const edge) const;
  virtual Iterator<edge> *getEdgeMetaInfo(const edge) const;
  virtual void delEdges(Iterator<edge> *itE, bool deleteInAllGraphs = false);
  //=======================================
  virtual node getOneNode() const;
  virtual node getRandomNode() const;
  virtual node getInNode(const node, unsigned int) const;
  virtual node getOutNode(const node, unsigned int) const;
  virtual edge getOneEdge() const;
  virtual edge getRandomEdge() const;
  //========================================
  bool existProperty(const std::string &) const;
  bool existLocalProperty(const std::string &) const;
  void delLocalProperty(const std::string &);
  void addLocalProperty(const std::string &name, PropertyInterface *prop);
  Iterator<std::string> *getLocalProperties() const;
  Iterator<std::string> *getInheritedProperties() const;
  Iterator<std::string> *getProperties() const;
  Iterator<PropertyInterface *> *getLocalObjectProperties() const;
  Iterator<PropertyInterface *> *getInheritedObjectProperties() const;
  Iterator<PropertyInterface *> *getObjectProperties() const;
  PropertyInterface *getProperty(const std::string &) const;

  // to get viewMetaGraph property
  GraphProperty *getMetaGraphProperty();

  virtual void setName(const std::string &name);
  virtual std::string getName() const;

  virtual Iterator<node> *bfs(const node root = node()) const;
  virtual Iterator<node> *dfs(const node root = node()) const;

protected:
  DataSet &getNonConstAttributes() {
    return attributes;
  }
  void setSuperGraph(Graph *);
  PropertyManager *propertyContainer;
  const std::set<edge> &getReferencedEdges(const edge) const;

  virtual bool renameLocalProperty(PropertyInterface *prop, const std::string &newName);

  // internally used to deal with sub graph deletion
  virtual void clearSubGraphs();
  virtual void removeSubGraph(Graph *);
  virtual void restoreSubGraph(Graph *);
  virtual void setSubGraphToKeep(Graph *);

private:
  // notification of addition/deletion of inherited properties
  void notifyBeforeAddInheritedProperty(const std::string &prop);
  void notifyAddInheritedProperty(const std::string &prop);
  void notifyBeforeDelInheritedProperty(const std::string &prop);
  void notifyAfterDelInheritedProperty(const std::string &prop);
  // notification of property renaming
  void notifyBeforeRenameLocalProperty(PropertyInterface *prop, const std::string &newName);
  void notifyAfterRenameLocalProperty(PropertyInterface *prop, const std::string &oldName);
};
}
#endif

///@endcond
