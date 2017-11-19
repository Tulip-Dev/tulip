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
  ~GraphAbstract() override;
  void clear() override;
  // use to enforce subgraph id
  Graph *addSubGraph(unsigned int id, BooleanProperty *selection = nullptr,
                     const std::string &name = "unnamed");
  Graph *addSubGraph(BooleanProperty *selection = nullptr,
                     const std::string &name = "unnamed") override {
    return addSubGraph(0, selection, name);
  }
  void delSubGraph(Graph *) override;
  void delAllSubGraphs(Graph *) override;
  inline Graph *getSuperGraph() const override {
    return supergraph;
  }
  inline Graph *getRoot() const override {
    return root;
  }
  Iterator<Graph *> *getSubGraphs() const override;
  bool isSubGraph(const Graph *sg) const override;
  bool isDescendantGraph(const Graph *sg) const override;
  Graph *getSubGraph(unsigned int id) const override;
  Graph *getSubGraph(const std::string &name) const override;
  Graph *getDescendantGraph(unsigned int id) const override;
  Graph *getDescendantGraph(const std::string &name) const override;
  Graph *getNthSubGraph(unsigned int n) const override;
  inline unsigned int numberOfSubGraphs() const override {
    return subgraphs.size();
  }
  unsigned int numberOfDescendantGraphs() const override;

  //=======================================
  bool isMetaNode(const node) const override;
  Graph *getNodeMetaInfo(const node) const override;
  void delNodes(Iterator<node> *itN, bool deleteInAllGraphs) override;
  bool isMetaEdge(const edge) const override;
  Iterator<edge> *getEdgeMetaInfo(const edge) const override;
  void delEdges(Iterator<edge> *itE, bool deleteInAllGraphs = false) override;
  //=======================================
  node getOneNode() const override;
  node getRandomNode() const override;
  node getInNode(const node, unsigned int) const override;
  node getOutNode(const node, unsigned int) const override;
  edge getOneEdge() const override;
  edge getRandomEdge() const override;
  //========================================
  bool existProperty(const std::string &) const override;
  bool existLocalProperty(const std::string &) const override;
  void delLocalProperty(const std::string &) override;
  void addLocalProperty(const std::string &name, PropertyInterface *prop) override;
  Iterator<std::string> *getLocalProperties() const override;
  Iterator<std::string> *getInheritedProperties() const override;
  Iterator<std::string> *getProperties() const override;
  Iterator<PropertyInterface *> *getLocalObjectProperties() const override;
  Iterator<PropertyInterface *> *getInheritedObjectProperties() const override;
  Iterator<PropertyInterface *> *getObjectProperties() const override;
  PropertyInterface *getProperty(const std::string &) const override;

  // to get viewMetaGraph property
  GraphProperty *getMetaGraphProperty();

  void setName(const std::string &name) override;
  std::string getName() const override;

  Iterator<node> *bfs(const node root = node()) const override;
  Iterator<node> *dfs(const node root = node()) const override;

protected:
  DataSet &getNonConstAttributes() override {
    return attributes;
  }
  void setSuperGraph(Graph *) override;
  PropertyManager *propertyContainer;
  const std::set<edge> &getReferencedEdges(const edge) const;

  bool renameLocalProperty(PropertyInterface *prop, const std::string &newName) override;

  // internally used to deal with sub graph deletion
  void clearSubGraphs() override;
  void removeSubGraph(Graph *) override;
  void restoreSubGraph(Graph *) override;
  void setSubGraphToKeep(Graph *) override;

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
