//-*-c++-*-
/*
 Author: Sophie BARDET & Alexandre BURNETT
 Email : sophie.bardet@gmail.com, aburnett397@gmail.com
 Last modification : 19/06/2005
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#ifndef DOXYGEN_NOTFOR_DEVEL

#ifndef Tulip_GraphDecorator_H
#define Tulip_GraphDecorator_H

#include <tulip/Graph.h>

namespace tlp {

class TLP_SCOPE GraphDecorator : public Graph {  // non-orienté, planaire
 public:
  GraphDecorator(Graph* s){assert(s); graph_component = s;}
  virtual ~GraphDecorator(){
    notifyDestroy();
  }

  virtual void clear();
  virtual Graph *addSubGraph(BooleanProperty *selection=0,
			     unsigned int id = 0);
  virtual void delSubGraph(Graph * s);
  virtual void delAllSubGraphs(Graph *s);
  virtual Graph* getSuperGraph()const;
  virtual void setSuperGraph(Graph* s);
  virtual Graph* getRoot() const;
  virtual Iterator<Graph *> * getSubGraphs() const;
  virtual bool isSubGraph(Graph* sg) const;
  virtual bool isDescendantGraph(Graph* sg) const;
  virtual Graph* getSubGraph(unsigned int id) const;
  virtual Graph* getDescendantGraph(unsigned int id) const;

  //============================================================
  virtual node source(const edge e)const;
  virtual node target(const edge e)const;
  virtual node opposite(const edge e, const node n)const;
  virtual edge existEdge(const node n, const node m, bool directed = true)const;
  virtual const std::pair<node, node>& ends(const edge e) const;
  virtual void reverse(const edge e);
  virtual unsigned int deg(const node n)const;
  virtual unsigned int indeg(const node n)const;
  virtual unsigned int outdeg(const node n)const;
  virtual Graph* getNodeMetaInfo(const node n)const;

  //============================================================
  virtual node getOneNode() const;
  virtual node getInNode(const node n,unsigned int i)const;
  virtual node getOutNode(const node n,unsigned int i) const;
  virtual edge getOneEdge() const;
  
  virtual unsigned int numberOfNodes()const;
  virtual unsigned int numberOfEdges()const;

  //============================================================
  virtual bool isElement(const node n) const;
  virtual bool isMetaNode(const node n) const;
  virtual bool isElement(const edge e) const;
  virtual bool isMetaEdge(const edge e) const;
  virtual node addNode();
  virtual void addNode(const node n);
  virtual edge addEdge(const node n, const node n2);
  
  virtual void addEdge(const edge e);
  virtual void delNode(const node n);
  virtual void delAllNode(const node n);
  virtual void delEdge(const edge e);
  virtual void delAllEdge(const edge e);
  virtual void setEdgeOrder(const node n,const std::vector<edge> & s);
  virtual void swapEdgeOrder(const node n ,const edge e1, const edge e2);

  virtual Iterator<node>* getNodes() const;
  virtual Iterator<node>* getInNodes(const node n) const;
  virtual Iterator<node>* getOutNodes(const node n) const;
  virtual Iterator<node>* getInOutNodes(const node n) const;
  virtual Iterator<edge>* getEdges() const;
  virtual Iterator<edge>* getOutEdges(const node n) const;
  virtual Iterator<edge>* getInOutEdges(const node n) const;
  virtual Iterator<edge>* getInEdges(const node n) const;
  virtual Iterator<edge>* getEdgeMetaInfo(const edge) const;
  //============================================================
  virtual PropertyInterface* getProperty(const std::string &name);
  virtual bool existProperty(const std::string&name);
  virtual bool existLocalProperty(const std::string&name);
  virtual void delLocalProperty(const std::string&name);
  virtual void addLocalProperty(const std::string &name, PropertyInterface *prop);
  virtual Iterator<std::string>* getLocalProperties();
  virtual Iterator<std::string>* getInheritedProperties();
  virtual Iterator<std::string>* getProperties();
  // updates management
  virtual void push(bool unpopAllowed = true);
  virtual void pop(bool unpopAllowed = true);
  virtual bool nextPopKeepPropertyUpdates(PropertyInterface* prop);
  virtual void unpop();
  virtual bool canPop();
  virtual bool canUnpop();
  //============================================================

protected:
  virtual DataSet & getNonConstAttributes();
  Graph* graph_component;
  // designed to reassign an id to a previously deleted elt
  // called by GraphUpdatesRecorder
  virtual node restoreNode(node);
  virtual edge restoreEdge(edge, node source, node target);
  // designed to only update own structures
  // used by GraphUpdatesRecorder
  virtual void removeNode(const node);
  virtual void removeEdge(const edge, const node = node());
  // to deal with sub graph deletion
  virtual void removeSubGraph(Graph*);
  virtual void clearSubGraphs();
  // only called by GraphUpdatesRecorder
  virtual void restoreSubGraph(Graph*, bool);
  virtual void setSubGraphToKeep(Graph*);
};

}

#endif

#endif //DOXYGEN_NOTFOR_DEVEL
