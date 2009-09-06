//-*-c++-*-
#ifndef TULIP_SUPERGRAPHABSTRACT_H
#define TULIP_SUPERGRAPHABSTRACT_H

#ifndef DOXYGEN_NOTFOR_USER

#include <set>
#include "tulip/Graph.h"
#include "tulip/Reflect.h"
#include "tulip/PropertyManager.h"

namespace tlp {

#define GRAPH_SEQ std::vector<Graph *>

template<class C>class Iterator;

///Abstract class for default graph operations.
class GraphAbstract:public Graph {
public:
  GraphAbstract(Graph *supergraph=0);
  virtual ~GraphAbstract();
  virtual void clear();
  virtual Graph *addSubGraph(BooleanProperty *selection=0,
			     unsigned int id = 0);
  virtual void delSubGraph(Graph *);
  virtual void delAllSubGraphs(Graph *);
  virtual Graph* getSuperGraph()const;
  virtual Graph* getRoot() const;
  virtual Iterator<Graph *>* getSubGraphs() const;
  virtual bool isSubGraph(Graph* sg) const;
  virtual bool isDescendantGraph(Graph* sg) const;
  virtual Graph* getSubGraph(unsigned int id) const;
  virtual Graph* getDescendantGraph(unsigned int id) const;
  //=======================================
  virtual unsigned int deg(const node) const;
  virtual unsigned int indeg(const node) const;
  virtual unsigned int outdeg(const node) const;
  virtual bool isMetaNode(const node) const;
  virtual Graph* getNodeMetaInfo(const node) const;
  virtual node source(const edge) const;
  virtual node target(const edge) const;
  virtual const std::pair<node, node>& ends(const edge) const;
  virtual node opposite(const edge, const node)const;
  virtual edge existEdge(const node , const node, bool)const;
  virtual void reverse(const edge);
  virtual bool isMetaEdge(const edge) const;
  virtual Iterator<edge>* getEdgeMetaInfo(const edge) const;
  //=======================================
  virtual node getOneNode() const;
  virtual node getInNode(const node,unsigned int ) const;
  virtual node getOutNode(const node,unsigned int ) const;
  virtual edge getOneEdge() const;
  virtual unsigned int numberOfNodes() const;
  virtual unsigned int numberOfEdges() const;
  //========================================
  bool existProperty(const std::string&);
  bool existLocalProperty(const std::string&);
  void delLocalProperty(const std::string&);
  void addLocalProperty(const std::string &name, PropertyInterface *prop);
  Iterator<std::string>* getLocalProperties();
  Iterator<std::string>* getInheritedProperties();
  Iterator<std::string>* getProperties();
  PropertyInterface* getProperty(const std::string &);

protected:
  DataSet& getNonConstAttributes();
  void setSuperGraph(Graph *);
  PropertyManager *propertyContainer;
  PropertyManager *getPropertyManager() {
    return propertyContainer;
  }
  const std::set<edge>& getReferencedEdges(const edge) const;

  // to deal with sub graph deletion
  virtual void clearSubGraphs();
  virtual void removeSubGraph(Graph*);
  virtual void delAllSubGraphsInternal(Graph*, bool deleteSubGraph);
  // reinstall a previously deleted subgraph
  // only called by GraphUpdatesRecorder
  virtual void restoreSubGraph(Graph*, bool restoreSubGraphs = false);
  virtual void setSubGraphToKeep(Graph*);

private:
  DataSet attributes;
  Graph *supergraph;
  Graph* const root;
  GRAPH_SEQ subgraphs;
  Graph* subGraphToKeep;
};

}
#endif // DOXYGEN_NOTFOR_USER

#endif
