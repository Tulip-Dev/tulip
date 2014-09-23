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

#ifndef ABSTRACT_PROPERTY_H
#define ABSTRACT_PROPERTY_H

#include <typeinfo>
#include <string>
#include <cstdlib>
#include <tulip/tulipconf.h>
#include <tulip/StoredType.h>
#include <tulip/MutableContainer.h>
#include <tulip/PropertyInterface.h>
#include <tulip/Iterator.h>
#include <tulip/PropertyAlgorithm.h>
#include <tulip/DataSet.h>
#include <tulip/Graph.h>

namespace tlp {

class GraphView;

//==============================================================

/**
 * @ingroup Graph
 * @brief This class extends upon PropertyInterface, and adds type-safe methods to
 * get and set the node and edge values, through the magic of template programming.
 *
 * Nodes and Edges can have different types (e.g. tlp::LayoutProperty has tlp::PointType as node type and tlp::LineType as edge type),
 * but most of the time they have the same type (e.g. tlp::DoubleProperty, tlp::IntegerProperty).
 *
 * Some of the pure virtual functions of PropertyInterface are implemented in this class (e.g. erase()).
 *
 * The actual data is stored in this class, and it manages the default values.
 */
template <class Tnode, class Tedge, class Tprop=PropertyInterface>
class TLP_SCOPE AbstractProperty : public Tprop {
  friend class Graph;
  friend class GraphView;

public:
  AbstractProperty(Graph *, const std::string& n = "");

  /**
   * @brief Gets the default node value of the property.
   * @return The default value of nodes.
   */
  virtual typename Tnode::RealType getNodeDefaultValue() const;

  /**
   * @brief Gets the default edge value of the property.
   * @return The default value of edges.
   **/
  virtual typename Tedge::RealType getEdgeDefaultValue() const;

  /**
   * @brief Returns the value associated with the node n in this property.
   * If there is no value, it returns the default node value.
   *
   * @param n The node for which we want to get the value of the property.
   * @return :StoredType< Tnode::RealType >::ReturnedConstValue The value of the property for this node.
   **/
  virtual typename tlp::StoredType<typename Tnode::RealType>::ReturnedConstValue getNodeValue(const node n ) const;

  /**
   * @brief Returns the value associated to the edge e in this property.
   * If there is no value, it returns the default edge value.
   *
   * @param e The edge for which we want to get the value of the property.
   * @return :StoredType< Tedge::RealType >::ReturnedConstValue The value of the property for this edge.
   **/
  virtual typename tlp::StoredType<typename Tedge::RealType>::ReturnedConstValue getEdgeValue(const edge e) const;

  /**
   * @brief Sets the value of a node and notify the observers of a modification.
   *
   * @param n The node to set the value of.
   * @param v The value to affect for this node.
   **/
  virtual void setNodeValue(const node n, const typename Tnode::RealType &v);

  /**
   * @brief Set the value of an edge and notify the observers of a modification.
   *
   * @param e The edge to set the value of.
   * @param v The value to affect for this edge.
   **/
  virtual void setEdgeValue(const edge e, const typename Tedge::RealType &v);

  /**
   * @brief Sets the value of all nodes and notify the observers.
   *
   * @param v The value to set to all nodes.
   *
   **/
  virtual void setAllNodeValue(const typename Tnode::RealType &v);

  /**
   * @brief Sets the value of all edges and notify the observers.
   *
   * @param v The value to set to all edges.
   *
   **/
  virtual void setAllEdgeValue(const typename Tedge::RealType &v);
  //=================================================================================

  /**
   * @brief Resets the value of a node to the default value.
   *
   * @param n The node to reset the value of.
   *
   **/
  virtual void erase(const node n) {
    setNodeValue(n, nodeDefaultValue);
  }
  //=================================================================================

  /**
   * @brief Resets the value of an edge to the default value.
   *
   * @param e The edge to reset the value of.
   *
   **/
  virtual void erase(const edge e) {
    setEdgeValue(e, edgeDefaultValue);
  }
  //=================================================================================
  /**
   * @brief This operator overload allows to copy a property using the following syntax :
   *
   * @code
   * IntegerProperty* shape = graph->getProperty<IntegerProperty>("viewShape");
   * IntegerProperty* backup = graph->getProperty<IntegerProperty>("shapeBackup");
   * *backup = *shape; // all the values from 'shape' will be copied into 'backup'.
   * @endcode
   * @param prop The property nto copy the values from.
   * @return This property with the values copied.
   */
  virtual AbstractProperty<Tnode,Tedge,Tprop>& operator =(AbstractProperty<Tnode,Tedge,Tprop> &prop) {
    if (this!= &prop) {
      if (Tprop::graph == NULL) Tprop::graph = prop.Tprop::graph;

      if (Tprop::graph == prop.Tprop::graph) {
        setAllNodeValue(prop.getNodeDefaultValue());
        setAllEdgeValue(prop.getEdgeDefaultValue());
        Iterator<node> *itN = prop.getNonDefaultValuatedNodes();

        while (itN->hasNext()) {
          node itn = itN->next();
          setNodeValue(itn, prop.getNodeValue(itn));
        }

        delete itN;
        Iterator<edge> *itE = prop.getNonDefaultValuatedEdges();

        while (itE->hasNext()) {
          edge ite = itE->next();
          setEdgeValue(ite, prop.getEdgeValue(ite));
        }

        delete itE;
      }
      else {
        //==============================================================*
        Iterator<node>* itN = Tprop::graph->getNodes();

        while (itN->hasNext()) {
          node itn=itN->next();

          if (prop.Tprop::graph->isElement(itn))
            setNodeValue(itn, prop.getNodeValue(itn));
        }

        delete itN;
        Iterator<edge>*itE = Tprop::graph->getEdges();

        while (itE->hasNext()) {
          edge ite=itE->next();

          if (prop.Tprop::graph->isElement(ite))
            setEdgeValue(ite, prop.getEdgeValue(ite));
        }

        delete itE;
      }

      clone_handler(prop);
    }

    return *this;
  }
  //=================================================================================
  // Untyped accessors inherited from PropertyInterface, documentation is inherited
  virtual std::string getNodeDefaultStringValue() const {
    typename Tnode::RealType v = getNodeDefaultValue();
    return Tnode::toString( v );
  }
  virtual std::string getEdgeDefaultStringValue() const {
    typename Tedge::RealType v = getEdgeDefaultValue();
    return Tedge::toString( v );
  }
  virtual std::string getNodeStringValue( const node n ) const {
    typename Tnode::RealType v = getNodeValue( n );
    return Tnode::toString( v );
  }
  virtual std::string getEdgeStringValue( const edge e ) const {
    typename Tedge::RealType v = getEdgeValue( e );
    return Tedge::toString( v );
  }
  virtual bool setNodeStringValue( const node inN, const std::string & inV ) {
    typename Tnode::RealType v;

    if( !Tnode::fromString(v,inV) )
      return false;

    setNodeValue( inN, v );
    return true;
  }
  virtual bool setEdgeStringValue( const edge inE, const std::string & inV ) {
    typename Tedge::RealType v;

    if( !Tedge::fromString(v,inV) )
      return false;

    setEdgeValue( inE, v );
    return true;
  }
  virtual bool setAllNodeStringValue( const std::string & inV ) {
    typename Tnode::RealType v;

    if( !Tnode::fromString(v,inV) )
      return false;

    setAllNodeValue( v );
    return true;
  }
  virtual bool setAllEdgeStringValue( const std::string & inV ) {
    typename Tedge::RealType v;

    if( !Tedge::fromString(v,inV) )
      return false;

    setAllEdgeValue( v );
    return true;
  }
  virtual tlp::Iterator<node>* getNonDefaultValuatedNodes(const Graph* g = NULL) const;
  virtual unsigned int numberOfNonDefaultValuatedNodes() const;
  virtual unsigned int nodeValueSize() const;
  virtual void writeNodeDefaultValue(std::ostream&) const;
  virtual void writeNodeValue(std::ostream&, node) const;
  virtual bool readNodeDefaultValue(std::istream&);
  virtual bool readNodeValue(std::istream&, node);
  virtual tlp::Iterator<edge>* getNonDefaultValuatedEdges(const Graph* g = NULL) const;
  virtual unsigned int numberOfNonDefaultValuatedEdges() const;
  virtual unsigned int edgeValueSize() const;
  virtual void writeEdgeDefaultValue(std::ostream&) const;
  virtual void writeEdgeValue(std::ostream&, edge) const;
  virtual bool readEdgeDefaultValue(std::istream&);
  virtual bool readEdgeValue(std::istream&, edge);
  virtual bool copy(const node destination, const node source, PropertyInterface *property,
                    bool ifNotDefault = false) {
    if (property == NULL)
      return false;

    tlp::AbstractProperty<Tnode,Tedge,Tprop>* tp =
      dynamic_cast<tlp::AbstractProperty<Tnode,Tedge,Tprop>*>(property);
    assert(tp);
    bool notDefault;
    typename StoredType<typename Tnode::RealType>::ReturnedValue value =
      tp->nodeProperties.get(source.id, notDefault);

    if (ifNotDefault && !notDefault)
      return false;

    setNodeValue(destination, value);
    return true;
  }
  virtual bool copy(const edge destination, const edge source, PropertyInterface *property,
                    bool ifNotDefault = false) {
    if (property == NULL)
      return false;

    tlp::AbstractProperty<Tnode,Tedge,Tprop>* tp =
      dynamic_cast<tlp::AbstractProperty<Tnode,Tedge,Tprop>*>(property);
    assert(tp);
    bool notDefault;
    typename StoredType<typename Tedge::RealType>::ReturnedValue value =
      tp->edgeProperties.get(source.id, notDefault);

    if (ifNotDefault && !notDefault)
      return false;

    setEdgeValue(destination, value);
    return true;
  }
  virtual void copy(PropertyInterface* property) {
    tlp::AbstractProperty<Tnode,Tedge,Tprop>* prop =
      dynamic_cast<typename tlp::AbstractProperty<Tnode,Tedge,Tprop>*>(property);
    assert(prop != NULL);
    *this = *prop;
  }
  // for performance reason and use in GraphUpdatesRecorder
  virtual DataMem* getNodeDefaultDataMemValue() const {
    return new TypedValueContainer<typename Tnode::RealType>(getNodeDefaultValue());
  }
  virtual DataMem* getEdgeDefaultDataMemValue() const {
    return new TypedValueContainer<typename Tedge::RealType>(getEdgeDefaultValue());
  }
  virtual DataMem* getNodeDataMemValue(const node n) const {
    return new TypedValueContainer<typename Tnode::RealType>(getNodeValue(n));
  }
  virtual DataMem* getEdgeDataMemValue(const edge e) const {
    return new TypedValueContainer<typename Tedge::RealType>(getEdgeValue(e));
  }
  virtual DataMem* getNonDefaultDataMemValue( const node n ) const {
    bool notDefault;
    typename StoredType<typename Tnode::RealType>::ReturnedValue value = nodeProperties.get(n.id, notDefault);

    if (notDefault)
      return new TypedValueContainer<typename Tnode::RealType>(value);

    return NULL;
  }
  virtual DataMem* getNonDefaultDataMemValue( const edge e ) const {
    bool notDefault;
    typename StoredType<typename Tedge::RealType>::ReturnedValue value = edgeProperties.get(e.id, notDefault);

    if (notDefault)
      return new TypedValueContainer<typename Tedge::RealType>(value);

    return NULL;
  }
  virtual void setNodeDataMemValue( const node n, const DataMem* v) {
    setNodeValue(n, ((TypedValueContainer<typename Tnode::RealType> *) v)->value);
  }
  virtual void setEdgeDataMemValue( const edge e, const DataMem* v) {
    setEdgeValue(e, ((TypedValueContainer<typename Tedge::RealType> *) v)->value);
  }
  virtual void setAllNodeDataMemValue(const DataMem* v) {
    setAllNodeValue(((TypedValueContainer<typename Tnode::RealType> *) v)->value);
  }
  virtual void setAllEdgeDataMemValue(const DataMem* v) {
    setAllEdgeValue(((TypedValueContainer<typename Tedge::RealType> *) v)->value);
  }

  // PropertyInterface methods
  // mN is the meta node, sg is the corresponding subgraph
  // and mg is the graph owning mN
  virtual void computeMetaValue(node n, Graph* sg, Graph* mg) {
    if (Tprop::metaValueCalculator)
      ((typename tlp::AbstractProperty<Tnode,Tedge,Tprop>::MetaValueCalculator *)
       Tprop::metaValueCalculator)->computeMetaValue(this, n, sg, mg);
  }
  // mE is the meta edge, itE is an iterator on the underlying edges
  // mg is the graph owning mE
  virtual void computeMetaValue(edge e, tlp::Iterator<edge>* itE, Graph* mg) {
    if (Tprop::metaValueCalculator)
      ((typename tlp::AbstractProperty<Tnode,Tedge,Tprop>::MetaValueCalculator *) Tprop::metaValueCalculator)->computeMetaValue(this, e, itE, mg);
  }
  virtual void setMetaValueCalculator(PropertyInterface::MetaValueCalculator *mvCalc) {
    if (mvCalc && !dynamic_cast<typename tlp::AbstractProperty<Tnode,Tedge,Tprop>::MetaValueCalculator *>(mvCalc)) {
      tlp::warning() << "Warning : "  << __PRETTY_FUNCTION__ << " ... invalid conversion of " << typeid(mvCalc).name() << "into " << typeid(typename tlp::AbstractProperty<Tnode,Tedge,Tprop>::MetaValueCalculator *).name() << std::endl;
      abort();
    }

    Tprop::metaValueCalculator = mvCalc;
  }

  int compare(const node n1, const node n2) const;
  int compare(const edge e1, const edge e2) const;


  /**
   * @brief This class is used to delegate the computation of the values associated to meta nodes or edges.
   **/
  class MetaValueCalculator :public PropertyInterface::MetaValueCalculator {
  public:
    // computes the value of the meta node mN of the graph mg
    // for the property prop, according to the values associated
    // to the underlying nodes i.e the nodes of the subgraph sg.
    virtual void computeMetaValue(AbstractProperty<Tnode,Tedge,Tprop>*,
                                  node,
                                  Graph*, Graph*) {}
    // computes the value of the meta node mE of the graph mg
    // for the property prop, according to the values associated
    // to the underlying edges given by the iterator itE.
    // The method do not have to delete the iterator
    virtual void computeMetaValue(AbstractProperty<Tnode,Tedge,Tprop>*,
                                  edge, tlp::Iterator<edge>*,
                                  Graph*) {}
  };

protected:
  //=================================================================================
  ///Enable to clone part of sub_class
  virtual void clone_handler(AbstractProperty<Tnode,Tedge,Tprop> &) {}

  MutableContainer<typename Tnode::RealType> nodeProperties;
  MutableContainer<typename Tedge::RealType> edgeProperties;
  typename Tnode::RealType nodeDefaultValue;
  typename Tedge::RealType edgeDefaultValue;
};

template <typename vectType,typename eltType,typename propType=VectorPropertyInterface>
class TLP_SCOPE AbstractVectorProperty : public AbstractProperty<vectType, vectType, propType> {
public:
  AbstractVectorProperty(Graph *, const std::string& name = "");

  // 2 methods inherited from VectorPropertyInterface
  bool setNodeStringValueAsVector(const node, const std::string&,
                                  char, char, char);

  bool setEdgeStringValueAsVector(const edge, const std::string&,
                                  char, char, char);

  /**
   * @brief Sets the value for node n, at index i, to v, and notify the observers of a modification.
   *
   * @param n The node to set a value of.
   * @param i The index at which the value should be set.
   * @param v The value to set.
   *
   **/
  void setNodeEltValue(const node n, unsigned int i, typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue v);
  /**
   * @brief Gets the value associated to node n, at index i.
   *
   * @param n The node to set a value of.
   * @param i The index at which to set the value.
   * @return const eltType& The value at index i in the vector for node n.
   **/
  typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue getNodeEltValue(const node n, unsigned int i) const;
  /**
   * @brief Appends a new value at the end of the vector associated to node n, and notify the observers of a modification.
   *
   * @param n The node to add a value to.
   * @param v The value to append at the end of the vector.
   *
   **/
  void pushBackNodeEltValue(const node n, typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue v);
  /**
   * @brief Removes the value at the end of the vector associated to node n, and notify the observers of a modification.
   *
   * @param n The node to remove a value from.
   *
   **/
  void popBackNodeEltValue(const node n);
  /**
   * @brief Resizes the vector associated to node n, and notify the observers of a modification.
   *
   * @param n The node associated to the vector to resize.
   * @param size The new size of the vector.
   * @param elt The default value to set at indices where there was no value before. Defaults to eltType().
   *
   **/
  void resizeNodeValue(const node n, size_t size, typename eltType::RealType elt = eltType::defaultValue());
  /**
   * @brief Sets the value for edge e, at index i, to v, and notify the observers of a modification.
   *
   * @param e The edge to set the value of.
   * @param i The index at which the value should be set.
   * @param v The value to set.
   *
   **/
  void setEdgeEltValue(const edge e, unsigned int i, typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue v);
  /**
   * @brief Gets the value associated to edge e, at index i.
   *
   * @param e The edge to set a value of.
   * @param i The index at which to set the value.
   * @return const eltType& The value at index i in the vector for node n.
   **/
  typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue getEdgeEltValue(const edge n, unsigned int i) const;
  /**
   * @brief Appends a new value at the end of the vector associated to edge e, and notify the observers of a modification.
   *
   * @param e The node to add a value to.
   * @param v The value to append at the end of the vector.
   *
   **/
  void pushBackEdgeEltValue(const edge e, typename tlp::StoredType<typename eltType::RealType>::ReturnedConstValue v);
  /**
   * @brief Removes the value at the end of the vector associated to edge e, and notify the observers of a modification.
   *
   * @param e The edge to remove a value from.
   *
   **/
  void popBackEdgeEltValue(const edge e);
  /**
   * @brief Resizes the vector associated to edge e, and notify the observers of a modification.
   *
   * @param e The edge associated to the vector to resize.
   * @param size The new size of the vector.
   * @param elt The default value to set at indices where there was no value before. Defaults to eltType().
   *
   **/
  void resizeEdgeValue(const edge e, size_t size, typename eltType::RealType elt = eltType::defaultValue());
};


}
#if !defined(_MSC_VER) || defined(DLL_TULIP) //When using VC++, we only want to include this when we are in the TULIP dll. With any other compiler, include it all the time
# include "cxx/AbstractProperty.cxx"
#endif
#endif
