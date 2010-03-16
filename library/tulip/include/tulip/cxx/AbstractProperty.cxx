//-*-c++-*-
/**
 Author: David Auber
 Email : auber@labri.fr
  This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by  
 the Free Software Foundation; either version 2 of the License, or     
 (at your option) any later version.
*/
#include <iostream>
#include <stdlib.h>

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
typename tlp::ReturnType<typename Tnode::RealType>::ConstValue tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getNodeValue(const node n ) const {
  return nodeProperties.get(n.id);
}
//=============================================================
template <class Tnode, class Tedge, class TPROPERTY>
typename tlp::ReturnType<typename Tedge::RealType>::ConstValue tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getEdgeValue(const edge e) const {
  return edgeProperties.get(e.id);
} 
//=============================================================
template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::setNodeValue(const node n,const typename Tnode::RealType &v) {
  notifyBeforeSetNodeValue(this, n);
  nodeProperties.set(n.id,v);
  notifyAfterSetNodeValue(this, n);
}
//=============================================================
template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::setEdgeValue(const edge e,const typename Tedge::RealType &v) {
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
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::erase(const node n) {
  setNodeValue(n, nodeDefaultValue);
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::erase(const edge e) {
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
std::string tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getNodeStringValue( const node inN ) const {
  typename Tnode::RealType v = getNodeValue( inN );
  return Tnode::toString( v );
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY> std::string
tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getEdgeStringValue( const edge inE ) const {
  typename Tedge::RealType v = getEdgeValue( inE );
  return Tedge::toString( v );
}
template <class Tnode, class Tedge, class TPROPERTY>
bool tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::setNodeStringValue( const node inN, const std::string & inV ) {
  typename Tnode::RealType v;
  if( !Tnode::fromString(v,inV) )
    return false;
  setNodeValue( inN, v );
  return true;
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
bool tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::setEdgeStringValue( const edge inE, const std::string & inV ) {
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
tlp::DataMem* tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getNodeDataMemValue( const node inN ) const {
  return new PropertyValueContainer<typename Tnode::RealType>(getNodeValue(inN));
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY> tlp::DataMem*
tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getEdgeDataMemValue( const edge inE ) const {
  return new PropertyValueContainer<typename Tedge::RealType>(getEdgeValue(inE));
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
tlp::DataMem* tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getNonDefaultDataMemValue(const node n) const {
  bool notDefault;
  typename ReturnType<typename Tnode::RealType>::Value value = nodeProperties.get(n.id, notDefault);
  if (notDefault)
    return new PropertyValueContainer<typename Tnode::RealType>(value);
  return NULL;
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
tlp::DataMem* tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getNonDefaultDataMemValue( const edge e ) const {
  bool notDefault;
  typename ReturnType<typename Tedge::RealType>::Value value = edgeProperties.get(e.id, notDefault);
  if (notDefault)
    return new PropertyValueContainer<typename Tedge::RealType>(value);
  return NULL;
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::setNodeDataMemValue( const node inN, const tlp::DataMem* inV ) {
  setNodeValue(inN, ((PropertyValueContainer<typename Tnode::RealType> *) inV)->value);
}

template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::setEdgeDataMemValue( const edge inE, const tlp::DataMem* inV ) {
  setEdgeValue(inE, ((PropertyValueContainer<typename Tedge::RealType> *) inV)->value);
}

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
template <class Tnode, class Tedge, class TPROPERTY>
tlp::Iterator<tlp::node>* tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getNonDefaultValuatedNodes() const {
  return new tlp::UINTIterator<tlp::node>(nodeProperties.findAll(nodeDefaultValue, false));
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
tlp::Iterator<tlp::edge>* tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getNonDefaultValuatedEdges() const {
  return new tlp::UINTIterator<tlp::edge>(edgeProperties.findAll(edgeDefaultValue, false));
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
								    Graph* sg) {
  if (metaValueCalculator)
    ((typename tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::MetaValueCalculator *) 
    metaValueCalculator)->computeMetaValue(this, n, sg);
}
//==============================================================
template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::computeMetaValue(edge e, Iterator<edge>* itE, Graph* g) {
  if (metaValueCalculator)
    ((typename tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::MetaValueCalculator *)metaValueCalculator)->computeMetaValue(this, e, itE, g);
}
