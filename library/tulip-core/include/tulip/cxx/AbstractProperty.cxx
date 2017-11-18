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
#include <cstdlib>
#include <tulip/GraphIterator.h>

template <class Tnode, class Tedge, class Tprop>
tlp::AbstractProperty<Tnode, Tedge, Tprop>::AbstractProperty(tlp::Graph *sg, const std::string &n) {
  Tprop::graph = sg;
  Tprop::name = n;
  nodeDefaultValue = Tnode::defaultValue();
  edgeDefaultValue = Tedge::defaultValue();
  nodeProperties.setAll(Tnode::defaultValue());
  edgeProperties.setAll(Tedge::defaultValue());
  Tprop::metaValueCalculator = NULL;
}
//=============================================================
template <class Tnode, class Tedge, class Tprop>
inline typename Tnode::RealType
tlp::AbstractProperty<Tnode, Tedge, Tprop>::getNodeDefaultValue() const {
  return nodeDefaultValue;
}
//=============================================================
template <class Tnode, class Tedge, class Tprop>
inline typename Tedge::RealType
tlp::AbstractProperty<Tnode, Tedge, Tprop>::getEdgeDefaultValue() const {
  return edgeDefaultValue;
}
//=============================================================
template <class Tnode, class Tedge, class Tprop>
inline typename tlp::StoredType<typename Tnode::RealType>::ReturnedConstValue
tlp::AbstractProperty<Tnode, Tedge, Tprop>::getNodeValue(const tlp::node n) const {
  assert(n.isValid());
  return nodeProperties.get(n.id);
}
//=============================================================
template <class Tnode, class Tedge, class Tprop>
inline typename tlp::StoredType<typename Tedge::RealType>::ReturnedConstValue
tlp::AbstractProperty<Tnode, Tedge, Tprop>::getEdgeValue(const tlp::edge e) const {
  assert(e.isValid());
  return edgeProperties.get(e.id);
}
//=================================================================================
template <class Tnode, class Tedge, class Tprop>
tlp::Iterator<tlp::node> *tlp::AbstractProperty<Tnode, Tedge, Tprop>::getNodesEqualTo(
    typename tlp::StoredType<typename Tnode::RealType>::ReturnedConstValue val,
    const Graph *sg) const {
  if (sg == NULL)
    sg = this->graph;

  tlp::Iterator<unsigned int> *it = NULL;

  if (sg == this->graph)
    it = nodeProperties.findAll(val);

  if (it == NULL)
    return new tlp::SGraphNodeIterator<typename Tnode::RealType>(sg, nodeProperties, val);

  return (new tlp::UINTIterator<node>(it));
}
//=================================================================================
template <class Tnode, class Tedge, class Tprop>
tlp::Iterator<tlp::edge> *tlp::AbstractProperty<Tnode, Tedge, Tprop>::getEdgesEqualTo(
    typename tlp::StoredType<typename Tedge::RealType>::ReturnedConstValue val,
    const Graph *sg) const {
  if (sg == NULL)
    sg = this->graph;

  tlp::Iterator<unsigned int> *it = NULL;

  if (sg == this->graph)
    it = edgeProperties.findAll(val);

  if (it == NULL)
    return new tlp::SGraphEdgeIterator<typename Tedge::RealType>(sg, edgeProperties, val);

  return (new tlp::UINTIterator<edge>(it));
}
//=============================================================
template <class Tnode, class Tedge, class Tprop>
void tlp::AbstractProperty<Tnode, Tedge, Tprop>::setNodeValue(
    const tlp::node n, typename tlp::StoredType<typename Tnode::RealType>::ReturnedConstValue v) {
  assert(n.isValid());
  Tprop::notifyBeforeSetNodeValue(n);
  nodeProperties.set(n.id, v);
  Tprop::notifyAfterSetNodeValue(n);
}
//=============================================================
template <class Tnode, class Tedge, class Tprop>
void tlp::AbstractProperty<Tnode, Tedge, Tprop>::setEdgeValue(
    const tlp::edge e, typename tlp::StoredType<typename Tedge::RealType>::ReturnedConstValue v) {
  assert(e.isValid());
  Tprop::notifyBeforeSetEdgeValue(e);
  edgeProperties.set(e.id, v);
  Tprop::notifyAfterSetEdgeValue(e);
}
//=============================================================
template <class Tnode, class Tedge, class Tprop>
void tlp::AbstractProperty<Tnode, Tedge, Tprop>::setAllNodeValue(
    typename tlp::StoredType<typename Tnode::RealType>::ReturnedConstValue v) {
  Tprop::notifyBeforeSetAllNodeValue();
  nodeDefaultValue = v;
  nodeProperties.setAll(v);
  Tprop::notifyAfterSetAllNodeValue();
}
//=============================================================
template <class Tnode, class Tedge, class Tprop>
void tlp::AbstractProperty<Tnode, Tedge, Tprop>::setNodeDefaultValue(
    typename tlp::StoredType<typename Tnode::RealType>::ReturnedConstValue v) {
  if (nodeDefaultValue == v) {
    return;
  }

  // backup old default value
  typename Tnode::RealType oldDefaultValue = nodeDefaultValue;
  // we need to get the list of nodes whose value equals the current default one first
  std::vector<tlp::node> nodesOldDefaultToUpdate;
  std::vector<tlp::node> nodesDefaultToUpdate;
  const std::vector<tlp::node> &nodes = this->getGraph()->nodes();

  for (size_t i = 0; i < nodes.size(); ++i) {
    const typename Tnode::RealType &val = this->getNodeValue(nodes[i]);

    if (val == oldDefaultValue) {
      nodesOldDefaultToUpdate.push_back(nodes[i]);
    } else if (val == v) {
      nodesDefaultToUpdate.push_back(nodes[i]);
    }
  }

  // set new default value that will be associated to future added nodes
  nodeDefaultValue = v;
  nodeProperties.setDefault(v);

  // reset the backup nodes to the old default value as there is a new one in the
  // underlying MutableContainer
  for (size_t i = 0; i < nodesOldDefaultToUpdate.size(); ++i) {
    nodeProperties.set(nodesOldDefaultToUpdate[i].id, oldDefaultValue);
  }

  // reset the backup nodes to their current value in order
  // to synchronize the underlying MutableContainer state
  for (size_t i = 0; i < nodesDefaultToUpdate.size(); ++i) {
    nodeProperties.set(nodesDefaultToUpdate[i].id, v);
  }
}
//=============================================================
template <class Tnode, class Tedge, class Tprop>
void tlp::AbstractProperty<Tnode, Tedge, Tprop>::setAllNodeValue(
    typename tlp::StoredType<typename Tnode::RealType>::ReturnedConstValue v, const Graph *graph) {
  setValueToGraphNodes(v, graph);
}
//=============================================================
template <class Tnode, class Tedge, class Tprop>
void tlp::AbstractProperty<Tnode, Tedge, Tprop>::setValueToGraphNodes(
    typename tlp::StoredType<typename Tnode::RealType>::ReturnedConstValue v, const Graph *graph) {
  if (this->getGraph() == graph || this->getGraph()->isDescendantGraph(graph)) {
    const std::vector<node> &nodes = graph->nodes();
    unsigned int nbNodes = nodes.size();

    for (unsigned int i = 0; i < nbNodes; ++i) {
      setNodeValue(nodes[i], v);
    }
  }
}
//=============================================================
template <class Tnode, class Tedge, class Tprop>
void tlp::AbstractProperty<Tnode, Tedge, Tprop>::setEdgeDefaultValue(
    typename tlp::StoredType<typename Tedge::RealType>::ReturnedConstValue v) {
  if (edgeDefaultValue == v) {
    return;
  }

  // backup old default value
  typename Tedge::RealType oldDefaultValue = edgeDefaultValue;
  // backup list of edges whose value equals the current default one
  std::vector<tlp::edge> edgesOldDefaultToUpdate;
  // backup list of edges whose value equals the new default one
  std::vector<tlp::edge> edgesDefaultToUpdate;
  const std::vector<tlp::edge> &edges = this->getGraph()->edges();

  for (size_t i = 0; i < edges.size(); ++i) {
    const typename Tedge::RealType &val = this->getEdgeValue(edges[i]);

    if (val == oldDefaultValue) {
      edgesOldDefaultToUpdate.push_back(edges[i]);
    } else if (val == v) {
      edgesDefaultToUpdate.push_back(edges[i]);
    }
  }

  // set new default value that will be associated to future added edges
  edgeDefaultValue = v;
  edgeProperties.setDefault(v);

  // reset the backup edges to the old default value as there is a new one in the
  // underlying MutableContainer
  for (size_t i = 0; i < edgesOldDefaultToUpdate.size(); ++i) {
    edgeProperties.set(edgesOldDefaultToUpdate[i].id, oldDefaultValue);
  }

  // reset the backup edges to their current value in order
  // to synchronize the underlying MutableContainer state
  for (size_t i = 0; i < edgesDefaultToUpdate.size(); ++i) {
    edgeProperties.set(edgesDefaultToUpdate[i].id, v);
  }
}
//============================================================
template <class Tnode, class Tedge, class Tprop>
void tlp::AbstractProperty<Tnode, Tedge, Tprop>::setAllEdgeValue(
    typename tlp::StoredType<typename Tedge::RealType>::ReturnedConstValue v) {
  Tprop::notifyBeforeSetAllEdgeValue();
  edgeDefaultValue = v;
  edgeProperties.setAll(v);
  Tprop::notifyAfterSetAllEdgeValue();
}
//============================================================
template <class Tnode, class Tedge, class Tprop>
void tlp::AbstractProperty<Tnode, Tedge, Tprop>::setAllEdgeValue(
    typename tlp::StoredType<typename Tedge::RealType>::ReturnedConstValue v, const Graph *graph) {
  setValueToGraphEdges(v, graph);
}
//============================================================
template <class Tnode, class Tedge, class Tprop>
void tlp::AbstractProperty<Tnode, Tedge, Tprop>::setValueToGraphEdges(
    typename tlp::StoredType<typename Tedge::RealType>::ReturnedConstValue v, const Graph *graph) {
  if (this->getGraph() == graph || this->getGraph()->isDescendantGraph(graph)) {
    const std::vector<edge> &edges = graph->edges();
    unsigned int nbEdges = edges.size();

    for (unsigned int i = 0; i < nbEdges; ++i) {
      setEdgeValue(edges[i], v);
    }
  }
}
//============================================================
template <class Tnode, class Tedge, class Tprop>
int tlp::AbstractProperty<Tnode, Tedge, Tprop>::compare(const node n1, const node n2) const {
  const typename Tnode::RealType &n1Value = getNodeValue(n1);
  const typename Tnode::RealType &n2Value = getNodeValue(n2);
  return (n1Value < n2Value) ? -1 : ((n1Value == n2Value) ? 0 : 1);
}
//============================================================
template <class Tnode, class Tedge, class Tprop>
int tlp::AbstractProperty<Tnode, Tedge, Tprop>::compare(const edge e1, const edge e2) const {
  const typename Tedge::RealType &e1Value = getEdgeValue(e1);
  const typename Tedge::RealType &e2Value = getEdgeValue(e2);
  return (e1Value < e2Value) ? -1 : ((e1Value == e2Value) ? 0 : 1);
}
//============================================================
// define template iterator class to iterate over graph elts
// belonging to a given graph instance
// used by the two methods below
///@cond DOXYGEN_HIDDEN
template <typename ELT_TYPE>
class GraphEltIterator : public tlp::Iterator<ELT_TYPE> {
public:
  ELT_TYPE next() {
    ELT_TYPE tmp = curElt;

    if ((_hasnext = it->hasNext())) {
      curElt = it->next();

      while (!(_hasnext = (!graph || graph->isElement(curElt)))) {
        if (!it->hasNext())
          break;

        curElt = it->next();
      }
    }

    return tmp;
  }
  GraphEltIterator(const tlp::Graph *g, tlp::Iterator<ELT_TYPE> *itN)
      : it(itN), graph(g), curElt(ELT_TYPE()), _hasnext(false) {
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
  const tlp::Graph *graph;
  ELT_TYPE curElt;
  bool _hasnext;
};
///@endcond

//============================================================
template <class Tnode, class Tedge, class Tprop>
tlp::Iterator<tlp::node> *
tlp::AbstractProperty<Tnode, Tedge, Tprop>::getNonDefaultValuatedNodes(const Graph *g) const {
  tlp::Iterator<tlp::node> *it =
      new tlp::UINTIterator<tlp::node>(nodeProperties.findAll(nodeDefaultValue, false));

  if (Tprop::name.empty())
    // we always need to check that nodes belong to graph
    // for non registered properties, because deleted nodes are not erased
    // from them
    return new GraphEltIterator<tlp::node>(g != NULL ? g : Tprop::graph, it);

  return ((g == NULL) || (g == Tprop::graph)) ? it : new GraphEltIterator<tlp::node>(g, it);
}
//============================================================
template <class Tnode, class Tedge, class Tprop>
unsigned int
tlp::AbstractProperty<Tnode, Tedge, Tprop>::numberOfNonDefaultValuatedNodes(const Graph *g) const {
  if (g == NULL) {
    return nodeProperties.numberOfNonDefaultValues();
  } else {
    unsigned int ret = 0;
    node n;
    forEach(n, getNonDefaultValuatedNodes(g)) {
      ++ret;
    }
    return ret;
  }
}
//============================================================
template <class Tnode, class Tedge, class Tprop>
unsigned int tlp::AbstractProperty<Tnode, Tedge, Tprop>::nodeValueSize() const {
  return Tnode::valueSize();
}
//============================================================
template <class Tnode, class Tedge, class Tprop>
void tlp::AbstractProperty<Tnode, Tedge, Tprop>::writeNodeDefaultValue(std::ostream &oss) const {
  Tnode::writeb(oss, nodeDefaultValue);
}
//============================================================
template <class Tnode, class Tedge, class Tprop>
void tlp::AbstractProperty<Tnode, Tedge, Tprop>::writeNodeValue(std::ostream &oss, node n) const {
  assert(n.isValid());
  Tnode::writeb(oss, nodeProperties.get(n.id));
}
//============================================================
template <class Tnode, class Tedge, class Tprop>
bool tlp::AbstractProperty<Tnode, Tedge, Tprop>::readNodeDefaultValue(std::istream &iss) {
  if (Tnode::readb(iss, nodeDefaultValue)) {
    nodeProperties.setAll(nodeDefaultValue);
    return true;
  }

  return false;
}
//============================================================
template <class Tnode, class Tedge, class Tprop>
bool tlp::AbstractProperty<Tnode, Tedge, Tprop>::readNodeValue(std::istream &iss, node n) {
  typename Tnode::RealType val;

  if (Tnode::readb(iss, val)) {
    nodeProperties.set(n.id, val);
    return true;
  }

  return false;
}
//============================================================
template <class Tnode, class Tedge, class Tprop>
tlp::Iterator<tlp::edge> *
tlp::AbstractProperty<Tnode, Tedge, Tprop>::getNonDefaultValuatedEdges(const Graph *g) const {
  tlp::Iterator<tlp::edge> *it =
      new tlp::UINTIterator<tlp::edge>(edgeProperties.findAll(edgeDefaultValue, false));

  if (Tprop::name.empty())
    // we always need to check that edges belong to graph
    // for non registered properties, because deleted edges are not erased
    // from them
    return new GraphEltIterator<tlp::edge>(g != NULL ? g : Tprop::graph, it);

  return ((g == NULL) || (g == Tprop::graph)) ? it : new GraphEltIterator<tlp::edge>(g, it);
}
//============================================================
template <class Tnode, class Tedge, class Tprop>
unsigned int
tlp::AbstractProperty<Tnode, Tedge, Tprop>::numberOfNonDefaultValuatedEdges(const Graph *g) const {
  if (g == NULL) {
    return edgeProperties.numberOfNonDefaultValues();
  } else {
    unsigned int ret = 0;
    edge e;
    forEach(e, getNonDefaultValuatedEdges(g)) {
      ++ret;
    }
    return ret;
  }
}
//============================================================
template <class Tnode, class Tedge, class Tprop>
unsigned int tlp::AbstractProperty<Tnode, Tedge, Tprop>::edgeValueSize() const {
  return Tedge::valueSize();
}
//============================================================
template <class Tnode, class Tedge, class Tprop>
void tlp::AbstractProperty<Tnode, Tedge, Tprop>::writeEdgeDefaultValue(std::ostream &oss) const {
  Tedge::writeb(oss, edgeDefaultValue);
}
//============================================================
template <class Tnode, class Tedge, class Tprop>
void tlp::AbstractProperty<Tnode, Tedge, Tprop>::writeEdgeValue(std::ostream &oss, edge e) const {
  assert(e.isValid());
  Tedge::writeb(oss, edgeProperties.get(e.id));
}
//============================================================
template <class Tnode, class Tedge, class Tprop>
bool tlp::AbstractProperty<Tnode, Tedge, Tprop>::readEdgeDefaultValue(std::istream &iss) {
  if (Tedge::readb(iss, edgeDefaultValue)) {
    edgeProperties.setAll(edgeDefaultValue);
    return true;
  }

  return false;
}
//============================================================
template <class Tnode, class Tedge, class Tprop>
bool tlp::AbstractProperty<Tnode, Tedge, Tprop>::readEdgeValue(std::istream &iss, edge e) {
  typename Tedge::RealType val;

  if (Tedge::readb(iss, val)) {
    edgeProperties.set(e.id, val);
    return true;
  }

  return false;
}
//============================================================
template <typename vectType, typename eltType, typename propType>
tlp::AbstractVectorProperty<vectType, eltType, propType>::AbstractVectorProperty(
    tlp::Graph *g, const std::string &name)
    : AbstractProperty<vectType, vectType, propType>(g, name) {}
//============================================================
template <typename vectType, typename eltType, typename propType>
bool tlp::AbstractVectorProperty<vectType, eltType, propType>::setNodeStringValueAsVector(
    const node n, const std::string &s, char openChar, char sepChar, char closeChar) {
  typename vectType::RealType v;
  std::istringstream iss(s);

  if (!vectType::read(iss, v, openChar, sepChar, closeChar))
    return false;

  this->setNodeValue(n, v);
  return true;
}
//============================================================
template <typename vectType, typename eltType, typename propType>
bool tlp::AbstractVectorProperty<vectType, eltType, propType>::setEdgeStringValueAsVector(
    const edge e, const std::string &s, char openChar, char sepChar, char closeChar) {
  typename vectType::RealType v;
  std::istringstream iss(s);

  if (!vectType::read(iss, v, openChar, sepChar, closeChar))
    return false;

  this->setEdgeValue(e, v);
  return true;
}
//============================================================
template <typename vectType, typename eltType, typename propType>
void tlp::AbstractVectorProperty<vectType, eltType, propType>::setNodeEltValue(
    const node n, unsigned int i,
    typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue v) {
  assert(n.isValid());
  bool isNotDefault;
  typename vectType::RealType &vect =
      AbstractProperty<vectType, vectType, propType>::nodeProperties.get(n, isNotDefault);
  assert(vect.size() > i);
  this->propType::notifyBeforeSetNodeValue(n);

  if (isNotDefault)
    vect[i] = v;
  else {
    typename vectType::RealType tmp(vect);
    tmp[i] = v;
    AbstractProperty<vectType, vectType, propType>::nodeProperties.set(n.id, tmp);
  }

  this->propType::notifyAfterSetNodeValue(n);
}
//============================================================
template <typename vectType, typename eltType, typename propType>
typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue
tlp::AbstractVectorProperty<vectType, eltType, propType>::getNodeEltValue(const node n,
                                                                          unsigned int i) const {
  assert(n.isValid());
  const typename vectType::RealType &vect =
      AbstractProperty<vectType, vectType, propType>::nodeProperties.get(n);
  assert(vect.size() > i);
  return vect[i];
}
//============================================================
template <typename vectType, typename eltType, typename propType>
void tlp::AbstractVectorProperty<vectType, eltType, propType>::pushBackNodeEltValue(
    const node n, typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue v) {
  assert(n.isValid());
  bool isNotDefault;
  typename vectType::RealType &vect =
      AbstractProperty<vectType, vectType, propType>::nodeProperties.get(n, isNotDefault);
  this->propType::notifyBeforeSetNodeValue(n);

  if (isNotDefault)
    vect.push_back(v);
  else {
    typename vectType::RealType tmp(vect);
    tmp.push_back(v);
    AbstractProperty<vectType, vectType, propType>::nodeProperties.set(n, tmp);
  }

  this->propType::notifyAfterSetNodeValue(n);
}
//============================================================
template <typename vectType, typename eltType, typename propType>
void tlp::AbstractVectorProperty<vectType, eltType, propType>::popBackNodeEltValue(const node n) {
  assert(n.isValid());
  bool isNotDefault;
  typename vectType::RealType &vect =
      AbstractProperty<vectType, vectType, propType>::nodeProperties.get(n, isNotDefault);
  this->propType::notifyBeforeSetNodeValue(n);
  assert(isNotDefault);
  vect.pop_back();
  this->propType::notifyAfterSetNodeValue(n);
}
//============================================================
template <typename vectType, typename eltType, typename propType>
void tlp::AbstractVectorProperty<vectType, eltType, propType>::resizeNodeValue(
    const node n, size_t size, typename eltType::RealType elt) {
  assert(n.isValid());
  bool isNotDefault;
  typename vectType::RealType &vect =
      AbstractProperty<vectType, vectType, propType>::nodeProperties.get(n, isNotDefault);
  assert(isNotDefault);
  this->propType::notifyBeforeSetNodeValue(n);
  vect.resize(size, elt);
  this->propType::notifyAfterSetNodeValue(n);
}
//============================================================
template <typename vectType, typename eltType, typename propType>
void tlp::AbstractVectorProperty<vectType, eltType, propType>::setEdgeEltValue(
    const edge e, unsigned int i,
    typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue v) {
  assert(e.isValid());
  bool isNotDefault;
  typename vectType::RealType &vect =
      AbstractProperty<vectType, vectType, propType>::edgeProperties.get(e, isNotDefault);
  assert(vect.size() > i);
  this->propType::notifyBeforeSetEdgeValue(e);

  if (isNotDefault)
    vect[i] = v;
  else {
    typename vectType::RealType tmp(vect);
    tmp[i] = v;
    AbstractProperty<vectType, vectType, propType>::edgeProperties.set(e, tmp);
  }

  this->propType::notifyAfterSetEdgeValue(e);
}
//============================================================
template <typename vectType, typename eltType, typename propType>
typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue
tlp::AbstractVectorProperty<vectType, eltType, propType>::getEdgeEltValue(const edge e,
                                                                          unsigned int i) const {
  assert(e.isValid());
  const typename vectType::RealType &vect =
      AbstractProperty<vectType, vectType, propType>::edgeProperties.get(e);
  assert(vect.size() > i);
  return vect[i];
} //============================================================
template <typename vectType, typename eltType, typename propType>
void tlp::AbstractVectorProperty<vectType, eltType, propType>::pushBackEdgeEltValue(
    const edge e, typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue v) {
  assert(e.isValid());
  bool isNotDefault;
  typename vectType::RealType &vect =
      AbstractProperty<vectType, vectType, propType>::edgeProperties.get(e, isNotDefault);
  this->propType::notifyBeforeSetEdgeValue(e);

  if (isNotDefault)
    vect.push_back(v);
  else {
    typename vectType::RealType tmp(vect);
    tmp.push_back(v);
    AbstractProperty<vectType, vectType, propType>::edgeProperties.set(e, tmp);
  }

  this->propType::notifyAfterSetEdgeValue(e);
}
//============================================================
template <typename vectType, typename eltType, typename propType>
void tlp::AbstractVectorProperty<vectType, eltType, propType>::popBackEdgeEltValue(const edge e) {
  assert(e.isValid());
  bool isNotDefault;
  typename vectType::RealType &vect =
      AbstractProperty<vectType, vectType, propType>::edgeProperties.get(e, isNotDefault);
  this->propType::notifyBeforeSetEdgeValue(e);
  assert(isNotDefault);
  vect.pop_back();
  this->propType::notifyAfterSetEdgeValue(e);
}
//============================================================
template <typename vectType, typename eltType, typename propType>
void tlp::AbstractVectorProperty<vectType, eltType, propType>::resizeEdgeValue(
    const edge e, size_t size, typename eltType::RealType elt) {
  assert(e.isValid());
  bool isNotDefault;
  typename vectType::RealType &vect =
      AbstractProperty<vectType, vectType, propType>::edgeProperties.get(e, isNotDefault);
  assert(isNotDefault);
  this->propType::notifyBeforeSetEdgeValue(e);
  vect.resize(size, elt);
  this->propType::notifyAfterSetEdgeValue(e);
}
