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
#include <iostream>
#include <stdlib.h>
#include "tulip/Iterator.h"

// This is not the right place to do the initialization below
// because this file is include in many cpp files
// The initialization has to take place in only one cpp file
// TlpTools.cpp seems to be a good choice to do this
/*template <class Tnode, class Tedge, class TPROPERTY>
  TemplateFactory<PropertyFactory<TPROPERTY >, TPROPERTY, PropertyContext > AbstractProperty<Tnode,Tedge,TPROPERTY>::factory;*/

template <class Tnode, class Tedge, class TPROPERTY>
tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::AbstractProperty(tlp::Graph *sg, std::string n) {
  graph = sg;
  name = n;
  nodeDefaultValue = Tnode::defaultValue();
  edgeDefaultValue = Tedge::defaultValue();
  nodeProperties.setAll(Tnode::defaultValue());
  edgeProperties.setAll(Tedge::defaultValue());
  metaValueCalculator = NULL;
}
//=============================================================
template <class Tnode, class Tedge, class TPROPERTY>
typename Tnode::RealType tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getNodeDefaultValue() const { 
  return nodeDefaultValue; 
}
//=============================================================
template <class Tnode, class Tedge, class TPROPERTY>
typename Tedge::RealType tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getEdgeDefaultValue() const { 
  return edgeDefaultValue; 
}
//=============================================================
template <class Tnode, class Tedge, class TPROPERTY>
typename tlp::ReturnType<typename Tnode::RealType>::ConstValue tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getNodeValue(const tlp::node n ) const {
  return nodeProperties.get(n.id);
}
//=============================================================
template <class Tnode, class Tedge, class TPROPERTY>
typename tlp::ReturnType<typename Tedge::RealType>::ConstValue tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getEdgeValue(const tlp::edge e) const {
  return edgeProperties.get(e.id);
} 
//=============================================================
template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::setNodeValue(const tlp::node n,const typename Tnode::RealType &v) {
  notifyBeforeSetNodeValue(this, n);
  nodeProperties.set(n.id,v);
  notifyAfterSetNodeValue(this, n);
}
//=============================================================
template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::setEdgeValue(const tlp::edge e,const typename Tedge::RealType &v) {
  notifyBeforeSetEdgeValue(this, e);
  edgeProperties.set(e.id,v);
  notifyAfterSetEdgeValue(this, e);
}
//=============================================================
template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::setAllNodeValue(const typename Tnode::RealType &v) {
  notifyBeforeSetAllNodeValue(this);
  nodeDefaultValue = v;
  nodeProperties.setAll(v);
  notifyAfterSetAllNodeValue(this);
}
//============================================================
template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::setAllEdgeValue(const typename Tedge::RealType &v) {
  notifyBeforeSetAllEdgeValue(this);
  edgeDefaultValue = v;
  edgeProperties.setAll(v);
  notifyAfterSetAllEdgeValue(this);
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::erase(const tlp::node n) {
  setNodeValue(n, nodeDefaultValue);
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::erase(const tlp::edge e) {
  setEdgeValue(e, edgeDefaultValue);
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
std::string tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getTypename() const {
  return tlp::PropertyInterface::getTypename( this );
}
//==============================================================
// Untyped (string) accessors/modifiers
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
std::string tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getNodeDefaultStringValue() const {
  typename Tnode::RealType v = getNodeDefaultValue();
  return Tnode::toString( v );
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
std::string tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getEdgeDefaultStringValue() const {
  typename Tedge::RealType v = getEdgeDefaultValue();
  return Tedge::toString( v );
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
std::string tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getNodeStringValue( const tlp::node inN ) const {
  typename Tnode::RealType v = getNodeValue( inN );
  return Tnode::toString( v );
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY> std::string
tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getEdgeStringValue( const tlp::edge inE ) const {
  typename Tedge::RealType v = getEdgeValue( inE );
  return Tedge::toString( v );
}
template <class Tnode, class Tedge, class TPROPERTY>
bool tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::setNodeStringValue( const tlp::node inN, const std::string & inV ) {
  typename Tnode::RealType v;
  if( !Tnode::fromString(v,inV) )
    return false;
  setNodeValue( inN, v );
  return true;
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
bool tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::setEdgeStringValue( const tlp::edge inE, const std::string & inV ) {
  typename Tedge::RealType v;
  if( !Tedge::fromString(v,inV) )
    return false;
  setEdgeValue( inE, v );
  return true;
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
bool tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::setAllNodeStringValue( const std::string & inV ) {
  typename Tnode::RealType v;
  if( !Tnode::fromString(v,inV) )
    return false;
  setAllNodeValue( v );
  return true;
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY> bool
tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::setAllEdgeStringValue( const std::string & inV ) {
  typename Tedge::RealType v;
  if( !Tedge::fromString(v,inV) )
    return false;
  setAllEdgeValue( v );
  return true;
}
//==============================================================
// used to embed a property value
//==============================================================
template<typename T>
struct PropertyValueContainer :public tlp::DataMem {
  T value;
  PropertyValueContainer(const T& val) : value(val) {
    value = val;
  }
  ~PropertyValueContainer() {
  }
};
//==============================================================
// the corresponding accessors/modifiers
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
tlp::DataMem* tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getNodeDefaultDataMemValue() const {
  return new PropertyValueContainer<typename Tnode::RealType>(getNodeDefaultValue());
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
tlp::DataMem* tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getEdgeDefaultDataMemValue() const {
  return new PropertyValueContainer<typename Tedge::RealType>(getEdgeDefaultValue());
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
tlp::DataMem* tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getNodeDataMemValue( const tlp::node inN ) const {
  return new PropertyValueContainer<typename Tnode::RealType>(getNodeValue(inN));
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY> tlp::DataMem*
tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getEdgeDataMemValue( const tlp::edge inE ) const {
  return new PropertyValueContainer<typename Tedge::RealType>(getEdgeValue(inE));
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
tlp::DataMem* tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getNonDefaultDataMemValue(const tlp::node n) const {
  bool notDefault;
  typename ReturnType<typename Tnode::RealType>::Value value = nodeProperties.get(n.id, notDefault);
  if (notDefault)
    return new PropertyValueContainer<typename Tnode::RealType>(value);
  return NULL;
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
tlp::DataMem* tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getNonDefaultDataMemValue( const tlp::edge e ) const {
  bool notDefault;
  typename ReturnType<typename Tedge::RealType>::Value value = edgeProperties.get(e.id, notDefault);
  if (notDefault)
    return new PropertyValueContainer<typename Tedge::RealType>(value);
  return NULL;
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::setNodeDataMemValue( const tlp::node inN, const tlp::DataMem* inV ) {
  setNodeValue(inN, ((PropertyValueContainer<typename Tnode::RealType> *) inV)->value);
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::setEdgeDataMemValue( const tlp::edge inE, const tlp::DataMem* inV ) {
  setEdgeValue(inE, ((PropertyValueContainer<typename Tedge::RealType> *) inV)->value);
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::setAllNodeDataMemValue( const DataMem* inV ) {
  setAllNodeValue(((PropertyValueContainer<typename Tnode::RealType> *) inV)->value);
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY> void
tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::setAllEdgeDataMemValue( const DataMem* inV ) {
  setAllEdgeValue(((PropertyValueContainer<typename Tedge::RealType> *) inV)->value);
}
//==============================================================
// define template iterator class to iterate over graph elts
// belonging to a given graph instance
template <typename ELT_TYPE>
class GraphEltIterator :public tlp::Iterator<ELT_TYPE> {
private:
  tlp::Iterator<ELT_TYPE> *it;
  const tlp::Graph* graph;
  ELT_TYPE curElt;
  bool _hasnext;

public:
  ELT_TYPE next() {
    ELT_TYPE tmp = curElt;
    if ((_hasnext = it->hasNext())) {
      curElt = it->next();
      while (!(_hasnext = (!graph || graph->isElement(curElt)))) {
	if (!it->hasNext()) break;
	curElt=it->next();
      }
    }
    return tmp;
  }
  GraphEltIterator(const tlp::Graph* g, tlp::Iterator<ELT_TYPE>* itN)
    :it(itN), graph(g), curElt(ELT_TYPE()), _hasnext(false) {
    next();
  }

  bool hasNext() {
    return (_hasnext);
  }
  ~GraphEltIterator() {
    delete it;
  }
};
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
tlp::Iterator<tlp::node>* tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getNonDefaultValuatedNodes(const tlp::Graph* g) const {
  tlp::Iterator<tlp::node> *it =
    new tlp::UINTIterator<tlp::node>(nodeProperties.findAll(nodeDefaultValue, false));
  if (name.empty())
    // we always need to check that nodes belong to graph
    // for non registered properties, because deleted nodes are not erased
    // from them
    return new GraphEltIterator<tlp::node>(g != NULL ? g : graph, it);
  return ((g == NULL) || (g == graph)) ? it : new GraphEltIterator<tlp::node>(g, it);
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
tlp::Iterator<tlp::edge>* tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getNonDefaultValuatedEdges(const tlp::Graph* g) const {
  Iterator<tlp::edge>* it =
    new tlp::UINTIterator<tlp::edge>(edgeProperties.findAll(edgeDefaultValue, false));
  if (name.empty())
    // we always need to check that edges belong to graph
    // for non registered properties, because deleted edges are not erased
    // from them
    return new GraphEltIterator<tlp::edge>(g != NULL ? g : graph, it);
  return ((g == NULL) || (g == graph)) ? it : new GraphEltIterator<tlp::edge>(g, it);
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::copy(const tlp::node dst,
							const tlp::node src,
							PropertyInterface* prop,
							bool ifNotDefault) {
  if (prop == NULL)
    return;
  tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>* tp =
    dynamic_cast<tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>*>(prop);
  assert(tp);
  bool notDefault;
  typename ReturnType<typename Tnode::RealType>::Value value =
    tp->nodeProperties.get(src.id, notDefault);
  if (ifNotDefault && !notDefault)
    return;
  setNodeValue(dst, value);
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::copy(const tlp::edge dst,
							const tlp::edge src,
							PropertyInterface* prop,
							bool ifNotDefault) {
  if (prop == NULL)
    return;
  tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>* tp =
    dynamic_cast<tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>*>(prop);
  assert(tp);
  bool notDefault;
  typename ReturnType<typename Tedge::RealType>::Value value =
    tp->edgeProperties.get(src.id, notDefault);
  if (ifNotDefault && !notDefault)
    return;
  setEdgeValue(dst, value);
}
//==============================================================
// meta values management
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::setMetaValueCalculator(PropertyInterface::MetaValueCalculator *mvCalc) {
  if (mvCalc && !dynamic_cast<typename tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::MetaValueCalculator *>(mvCalc)) {
    std::cerr << "Warning : "  << __PRETTY_FUNCTION__ << " ... invalid conversion of " << typeid(mvCalc).name() << "into " << typeid(typename tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::MetaValueCalculator *).name() << std::endl;
    abort();
  }
  metaValueCalculator = mvCalc;
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::computeMetaValue(node n,
								    Graph* sg,
								    Graph* mg) {
  if (metaValueCalculator)
    ((typename tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::MetaValueCalculator *) 
     metaValueCalculator)->computeMetaValue(this, n, sg, mg);
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::computeMetaValue(edge e, Iterator<edge>* itE, Graph* mg) {
  if (metaValueCalculator)
    ((typename tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::MetaValueCalculator *)metaValueCalculator)->computeMetaValue(this, e, itE, mg);
}
