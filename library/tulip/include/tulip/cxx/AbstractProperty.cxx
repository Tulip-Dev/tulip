/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
typename tlp::StoredType<typename Tnode::RealType>::ReturnedConstValue tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getNodeValue(const tlp::node n ) const {
  assert(n.isValid());
  return nodeProperties.get(n.id);
}
//=============================================================
template <class Tnode, class Tedge, class TPROPERTY>
typename tlp::StoredType<typename Tedge::RealType>::ReturnedConstValue tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getEdgeValue(const tlp::edge e) const {
  assert(e.isValid());
  return edgeProperties.get(e.id);
} 
//=============================================================
template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::setNodeValue(const tlp::node n,const typename Tnode::RealType &v) {
  assert(n.isValid());
  this->notifyBeforeSetNodeValue(this, n);
  nodeProperties.set(n.id,v);
  this->notifyAfterSetNodeValue(this, n);
}
//=============================================================
template <class Tnode, class Tedge, class TPROPERTY>
void tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::setEdgeValue(const tlp::edge e,const typename Tedge::RealType &v) {
  assert(e.isValid());
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
//============================================================
// define template iterator class to iterate over graph elts
// belonging to a given graph instance
// used by the two methods below
#ifndef DOXYGEN_NOTFOR_DEVEL
template <typename ELT_TYPE>
class GraphEltIterator :public tlp::Iterator<ELT_TYPE> {
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

private:
  tlp::Iterator<ELT_TYPE> *it;
  const tlp::Graph* graph;
  ELT_TYPE curElt;
  bool _hasnext;
};
#endif // DOXYGEN_NOTFOR_DEVEL
//============================================================
template <class Tnode, class Tedge, class TPROPERTY>
tlp::Iterator<tlp::node>* tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getNonDefaultValuatedNodes(const Graph* g) const {
  tlp::Iterator<tlp::node> *it =
    new tlp::UINTIterator<tlp::node>(nodeProperties.findAll(nodeDefaultValue, false));
  if (name.empty())
    // we always need to check that nodes belong to graph
    // for non registered properties, because deleted nodes are not erased
    // from them
    return new GraphEltIterator<tlp::node>(g != NULL ? g : graph, it);
  return ((g == NULL) || (g == graph)) ? it : new GraphEltIterator<tlp::node>(g, it);
}
//============================================================
template <class Tnode, class Tedge, class TPROPERTY>
tlp::Iterator<tlp::edge>* tlp::AbstractProperty<Tnode,Tedge,TPROPERTY>::getNonDefaultValuatedEdges(const Graph* g) const {
  tlp::Iterator<tlp::edge>* it =
    new tlp::UINTIterator<tlp::edge>(edgeProperties.findAll(edgeDefaultValue, false));
  if (name.empty())
    // we always need to check that edges belong to graph
    // for non registered properties, because deleted edges are not erased
    // from them
    return new GraphEltIterator<tlp::edge>(g != NULL ? g : graph, it);
  return ((g == NULL) || (g == graph)) ? it : new GraphEltIterator<tlp::edge>(g, it);
}
//============================================================
template <typename vectType, typename eltType>
tlp::AbstractVectorProperty<vectType, eltType>::AbstractVectorProperty(tlp::Graph* g, std::string name) :AbstractProperty<vectType, vectType>(g, name) {}
//============================================================
template <typename vectType, typename eltType>
void tlp::AbstractVectorProperty<vectType, eltType>::setNodeEltValue(const node n, unsigned int i, const eltType &v) {
  assert(n.isValid());
  bool isNotDefault;
  typename vectType::RealType& vect =
    AbstractProperty<vectType, vectType>::nodeProperties.get(n, isNotDefault);
  assert(vect->size() > i);
  this->notifyBeforeSetNodeValue(this, n);
  if (isNotDefault)
    vect[i] = v;
  else {
    typename vectType::RealType tmp(vect);
    tmp[i] = v;
     AbstractProperty<vectType, vectType>::nodeProperties.set(n.id, tmp);
  }
  this->notifyAfterSetNodeValue(this, n);
}
//============================================================
template <typename vectType, typename eltType>
const eltType& tlp::AbstractVectorProperty<vectType, eltType>::getNodeEltValue(const node n, unsigned int i) const {
  assert(n.isValid());
  const typename vectType::RealType& vect =
    AbstractProperty<vectType, vectType>::nodeProperties.get(n);
  assert(vect.size() > i);
  return vect[i];
}
//============================================================
template <typename vectType, typename eltType>
void tlp::AbstractVectorProperty<vectType, eltType>::pushBackNodeEltValue(const node n, const eltType &v) {
  assert(n.isValid());
  bool isNotDefault;
  typename vectType::RealType& vect =
    AbstractProperty<vectType, vectType>::nodeProperties.get(n, isNotDefault);
  this->notifyBeforeSetNodeValue(this, n);
  if (isNotDefault)
    vect.push_back(v);
  else {
    typename vectType::RealType tmp(vect);
    tmp.push_back(v);
    AbstractProperty<vectType, vectType>::nodeProperties.set(n, tmp);
  }
  this->notifyAfterSetNodeValue(this, n);
}
//============================================================
template <typename vectType, typename eltType>
void tlp::AbstractVectorProperty<vectType, eltType>::popBackNodeEltValue(const node n) {
  assert(n.isValid());
  bool isNotDefault;
  typename vectType::RealType& vect =
    AbstractProperty<vectType, vectType>::nodeProperties.get(n, isNotDefault);
  this->notifyBeforeSetNodeValue(this, n);
  assert(isNotDefault);
  vect.pop_back();
  this->notifyAfterSetNodeValue(this, n);
}
//============================================================
template <typename vectType, typename eltType>
void tlp::AbstractVectorProperty<vectType, eltType>::resizeNodeValue(const node n, size_t size, eltType elt) {
  assert(n.isValid());
  bool isNotDefault;
  typename vectType::RealType& vect =
    AbstractProperty<vectType, vectType>::nodeProperties.get(n, isNotDefault);
  assert(isNotDefault);
  this->notifyBeforeSetNodeValue(this, n);
  vect.resize(size, elt);
  this->notifyAfterSetNodeValue(this, n);
}
//============================================================
template <typename vectType, typename eltType>
void tlp::AbstractVectorProperty<vectType, eltType>::setEdgeEltValue(const edge e, unsigned int i, const eltType &v) {
  assert(e.isValid());
  bool isNotDefault;
  typename vectType::RealType& vect =
    AbstractProperty<vectType, vectType>::edgeProperties.get(e, isNotDefault);
  assert(vect.size() > i);
  this->notifyBeforeSetEdgeValue(this, e);
  if (isNotDefault)
    vect[i] = v;
  else {
    typename vectType::RealType tmp(vect);
    tmp[i] = v;
     AbstractProperty<vectType, vectType>::edgeProperties.set(e, tmp);
  }
  this->notifyAfterSetEdgeValue(this, e);
}
//============================================================
template <typename vectType, typename eltType>
const eltType& tlp::AbstractVectorProperty<vectType, eltType>::getEdgeEltValue(const edge e, unsigned int i) const {
  assert(e.isValid());
  const typename vectType::RealType& vect =
    AbstractProperty<vectType, vectType>::edgeProperties.get(e);
  assert(vect.size() > i);
  return vect[i];
}//============================================================
template <typename vectType, typename eltType>
void tlp::AbstractVectorProperty<vectType, eltType>::pushBackEdgeEltValue(const edge e, const eltType &v) {
  assert(e.isValid());
  bool isNotDefault;
  typename vectType::RealType& vect =
    AbstractProperty<vectType, vectType>::edgeProperties.get(e, isNotDefault);
  this->notifyBeforeSetEdgeValue(this, e);
  if (isNotDefault)
    vect.push_back(v);
  else {
    typename vectType::RealType tmp(vect);
    tmp.push_back(v);
    AbstractProperty<vectType, vectType>::edgeProperties.set(e, tmp);
  }    
  this->notifyAfterSetEdgeValue(this, e);
}
//============================================================
template <typename vectType, typename eltType>
void tlp::AbstractVectorProperty<vectType, eltType>::popBackEdgeEltValue(const edge e) {
  assert(e.isValid());
  bool isNotDefault;
  typename vectType::RealType& vect =
    AbstractProperty<vectType, vectType>::edgeProperties.get(e, isNotDefault);
  this->notifyBeforeSetEdgeValue(this, e);
  assert(isNotDefault);
  vect.pop_back();
  this->notifyAfterSetEdgeValue(this, e);
}
//============================================================
template <typename vectType, typename eltType>
void tlp::AbstractVectorProperty<vectType, eltType>::resizeEdgeValue(const edge e, size_t size, eltType elt) {
  assert(e.isValid());
  bool isNotDefault;
  typename vectType::RealType& vect =
    AbstractProperty<vectType, vectType>::edgeProperties.get(e, isNotDefault);
  assert(isNotDefault);
  this->notifyBeforeSetEdgeValue(this, e);
  vect.resize(size, elt);
  this->notifyAfterSetEdgeValue(this, e);
}
