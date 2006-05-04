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

#ifndef Tulip_GraphDecorator_H
#define Tulip_GraphDecorator_H

#include <tulip/Graph.h>
#include <tulip/Face.h>

class GraphDecorator : public Graph {  // non-orienté, planaire
 public:
  GraphDecorator(Graph* s){assert(s); sg_component = s;}
  virtual ~GraphDecorator(){}

  inline virtual void clear();
  inline virtual Graph *addSubGraph(BooleanProperty *selection=0);
  inline virtual void delSubGraph(Graph * s);
  inline virtual void delAllSubGraphs(Graph *s);
  inline virtual Graph* getFather()const;
  inline virtual void setFather(Graph* s);
  inline virtual Graph* getRoot() const;
  inline virtual Iterator<Graph *> * getSubGraphs() const;

  //============================================================
  inline virtual node source(const edge e)const;
  inline virtual node target(const edge e)const;
  inline virtual node opposite(const edge e, const node n)const;
  inline virtual edge existEdge(const node n, const node m)const;
  inline virtual void reverse(const edge e);
  inline virtual unsigned int deg(const node n)const;
  inline virtual unsigned int indeg(const node n)const;
  inline virtual unsigned int outdeg(const node n)const;

  //============================================================
  inline virtual node getOneNode() const;
  inline virtual node getInNode(const node n,unsigned int i)const;
  inline virtual node getOutNode(const node n,unsigned int i) const;
  inline virtual edge getOneEdge() const;
  
  inline virtual unsigned int numberOfNodes()const;
  inline virtual unsigned int numberOfEdges()const;

  //============================================================
  inline virtual bool isElement(const node n) const;
  inline virtual bool isElement(const edge e) const;
  inline virtual node addNode();
  inline virtual void addNode(const node n);
  inline virtual edge addEdge(const node n, const node n2);
  
  inline virtual void addEdge(const edge e);
  inline virtual void delNode(const node n);
  inline virtual void delAllNode(const node n);
  inline virtual void delEdge(const edge e);
  inline virtual void delAllEdge(const edge e);
  inline virtual void setEdgeOrder(const node n,const std::vector<edge> & s);
  inline virtual void swapEdgeOrder(const node n ,const edge e1, const edge e2);

  inline virtual Iterator<node>* getNodes() const;
  inline virtual Iterator<node>* getInNodes(const node n) const;
  inline virtual Iterator<node>* getOutNodes(const node n) const;
  inline virtual Iterator<node>* getInOutNodes(const node n) const;
  inline virtual Iterator<edge>* getEdges() const;
  inline virtual Iterator<edge>* getOutEdges(const node n) const;
  inline virtual Iterator<edge>* getInOutEdges(const node n) const;
  inline virtual Iterator<edge>* getInEdges(const node n) const;

  //============================================================
  inline virtual DataSet & getAttributes();
  inline virtual PropertyInterface* getProperty(const std::string &name);
  inline virtual bool existProperty(const std::string&name);
  inline virtual bool existLocalProperty(const std::string&name);
  inline virtual void delLocalProperty(const std::string&name);
  inline virtual Iterator<std::string>* getLocalProperties();
  inline virtual Iterator<std::string>* getInheritedProperties();
  inline virtual Iterator<std::string>* getProperties();

  //============================================================

protected:
  Graph* sg_component;
private:
  virtual PropertyManager *getPropertyManager(){return getPropertyManager();} 
  // to examinate !!!
};


#include <tulip/cxx/GraphDecorator.cxx>

#endif

