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

#ifndef TULIP_SUPERGRAPHABSTRACT_H
#define TULIP_SUPERGRAPHABSTRACT_H

#ifndef DOXYGEN_NOTFOR_USER

#include <set>
#include <vector>
#include <tulip/Graph.h>
#include <tulip/DataSet.h>

namespace tlp {

#define GRAPH_SEQ std::vector<Graph *>

template<class C>struct Iterator;
class PropertyManager;
class GraphProperty;

///Abstract class for default graph operations.
class TLP_SCOPE GraphAbstract:public Graph {
  friend class PropertyManager;
protected:
  GraphAbstract(Graph *supergraph, unsigned int id);
public:
  virtual ~GraphAbstract();
  virtual void clear();
  virtual Graph *addSubGraph(BooleanProperty *selection=NULL,
                             const std::string& name = "unnamed");
  // used to force the id of the newly created subgraph
  // when importing (tlp/tlpjson)
  Graph *addSubGraph(unsigned int id,
                     BooleanProperty *selection=NULL,
                     const std::string& name = "");
  virtual void delSubGraph(Graph *);
  virtual void delAllSubGraphs(Graph *);
  virtual Graph* getSuperGraph()const;
  virtual Graph* getRoot() const;
  virtual Iterator<Graph *>* getSubGraphs() const;
  virtual bool isSubGraph(const Graph* sg) const;
  virtual bool isDescendantGraph(const Graph* sg) const;
  virtual Graph* getSubGraph(unsigned int id) const;
  virtual Graph* getSubGraph(const std::string& name) const;
  virtual Graph* getDescendantGraph(unsigned int id) const;
  virtual Graph* getDescendantGraph(const std::string &name) const;
  virtual Graph *getNthSubGraph(unsigned int n) const;
  virtual unsigned int numberOfSubGraphs() const;
  virtual unsigned int numberOfDescendantGraphs() const;

  //=======================================
  virtual unsigned int deg(const node) const;
  virtual unsigned int indeg(const node) const;
  virtual unsigned int outdeg(const node) const;
  virtual bool isMetaNode(const node) const;
  virtual Graph* getNodeMetaInfo(const node) const;
  virtual void delNodes(Iterator<node>* itN, bool deleteInAllGraphs);
  virtual node source(const edge) const;
  virtual void setSource(const edge, const node);
  virtual node target(const edge) const;
  virtual void setTarget(const edge, const node);
  virtual const std::pair<node, node>& ends(const edge) const;
  virtual void setEnds(const edge, const node, const node);
  virtual node opposite(const edge, const node)const;
  virtual void reverse(const edge);
  virtual bool isMetaEdge(const edge) const;
  virtual Iterator<edge>* getEdgeMetaInfo(const edge) const;
  virtual void delEdges(Iterator<edge>* itE, bool deleteInAllGraphs = false);
  //=======================================
  virtual node getOneNode() const;
  virtual node getInNode(const node,unsigned int ) const;
  virtual node getOutNode(const node,unsigned int ) const;
  virtual edge getOneEdge() const;
  virtual unsigned int numberOfNodes() const;
  virtual unsigned int numberOfEdges() const;
  //========================================
  bool existProperty(const std::string&) const;
  bool existLocalProperty(const std::string&) const;
  void delLocalProperty(const std::string&);
  void addLocalProperty(const std::string &name, PropertyInterface *prop);
  Iterator<std::string>* getLocalProperties() const;
  Iterator<std::string>* getInheritedProperties() const;
  Iterator<std::string>* getProperties() const;
  Iterator<PropertyInterface*>* getLocalObjectProperties() const;
  Iterator<PropertyInterface*>* getInheritedObjectProperties() const;
  Iterator<PropertyInterface*>* getObjectProperties() const;
  PropertyInterface* getProperty(const std::string &) const;

  // to get viewMetaGraph property
  GraphProperty* getMetaGraphProperty();

  virtual void setName(const std::string &name);
  virtual std::string getName() const;

  virtual Iterator<node>* bfs(const node root = node()) const;
  virtual Iterator<node>* dfs(const node root = node()) const;

protected:
  DataSet& getNonConstAttributes();
  void setSuperGraph(Graph *);
  PropertyManager *propertyContainer;
  const std::set<edge>& getReferencedEdges(const edge) const;

  virtual bool renameLocalProperty(PropertyInterface *prop,
                                   const std::string& newName);

  // internally used to deal with sub graph deletion
  virtual void clearSubGraphs();
  virtual void removeSubGraph(Graph*);
  virtual void restoreSubGraph(Graph*);
  virtual void setSubGraphToKeep(Graph*);

private:
  DataSet attributes;
  Graph *supergraph;
  Graph* const root;
  GRAPH_SEQ subgraphs;
  Graph* subGraphToKeep;
  // pointer to root viewMetaGraph property
  GraphProperty* metaGraphProperty;
  // notification of addition/deletion of inherited properties
  void notifyBeforeAddInheritedProperty(const std::string& prop);
  void notifyAddInheritedProperty(const std::string& prop);
  void notifyBeforeDelInheritedProperty(const std::string& prop);
  void notifyAfterDelInheritedProperty(const std::string& prop);
  // notification of property renaming
  void notifyBeforeRenameLocalProperty(PropertyInterface* prop,
                                       const std::string& newName);
  void notifyAfterRenameLocalProperty(PropertyInterface* prop,
                                      const std::string& oldName);
};

}
#endif // DOXYGEN_NOTFOR_USER

#endif
///@endcond
