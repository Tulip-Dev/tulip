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
#ifndef PROPERTY_INTERFACE_H
#define PROPERTY_INTERFACE_H

#include <string>
#include <tulip/tulipconf.h>
#include <tulip/Observable.h>
#include <tulip/ObservableProperty.h>
#include <tulip/Iterator.h>
#include <tulip/Node.h>
#include <tulip/Edge.h>
#include <tulip/Reflect.h>

namespace tlp {

/**
 * \defgroup properties
 */ 
/*@{*/
//=============================================================
class TLP_SCOPE PropertyInterface: public Observable {
  friend class PropertyManager;
protected:
  // name of the property when registered as a property of a graph
  std::string name;
  // the graph for whom the property is registered
  Graph *graph;

public:

  virtual ~PropertyInterface();
  /**
   * Removes the value stored for the node given in parameter.
   * The new value for the node is the default one.
   */
  virtual void erase(const node) = 0;
  /**
   * Removes the value stored for the edge given in parameter.
   * The new value for the edge is the default one.
   */
  virtual void erase(const edge) = 0;
  /**
   * Sets the value of a node (first argument) in the current property (this)
   * with the value of the node (second argument) defined in prop (third argument)
   * If the fourth argument is set to true, the value will be copied only if
   * it is not the default value.
   */
  virtual void copy(const node src, const node dst, PropertyInterface *prop,
                    bool ifNotDefault = false) =0;
  /**
   * Sets the value of an edge (first argument) in the current property (this)
   * with the value of the edge (second argument) defined in prop (third argument)
   * defined in this property (this).
   * If the fourth argument is set to true, the value will be copied only if
   * it is not the default value.
   */
  virtual void copy(const edge src, const edge dst , PropertyInterface *prop,
                    bool ifNotDefault = false) =0;
  /**
    * Creates an object of the same real type of the current property, in the
    * the graph (first parameter) with the name (second parameter).
    * Values are not copied.
    */
  virtual PropertyInterface* clonePrototype(Graph *, const std::string&) =0;
  /**
    * Returns a string describing the type of the property.
    * i.e. "graph", "double", "layout", "string", "integer", "color", "size", ...
    */
  virtual std::string getTypename() const = 0;

  /**
    * Returns the name of the property.
    */
  const std::string& getName() const {
    return name;
  }

  /**
  * Returns the graph on which the property has been defined.
  * WARNING : If the property is inherited the graph could be different
  * that the one used to get that property.
  */
  tlp::Graph * getGraph() {
      return graph;
  }

  /**
  * Returns a string representation of the node default value.
  */
  virtual std::string getNodeDefaultStringValue() const = 0;
  /**
   * Clears all nodes registered values, and sets a new node default value
   * in converting the given string representation of a value.
   * Returns true if the given string representation has been successfully converted to a value, false otherwise.
   */
  virtual bool setAllNodeStringValue( const std::string & v ) = 0;
  /**
  * Returns a string representation of the edge default value.
  */
  virtual std::string getEdgeDefaultStringValue() const = 0;
  /**
   * Clears all edges registered values, and sets a new edge default value
   * in converting the given string representation of a value.
   * Returns true if the given string representation has been successfully converted to a value, false otherwise.
   */
  virtual bool setAllEdgeStringValue( const std::string & v ) = 0;
  /**
   * Returns a string conversion of the value registered for the given node.
   */
  virtual std::string getNodeStringValue( const node n ) const = 0;
  /**
   * Registers a new value for the given node in converting
   * the given string representation.
   * Returns true if the given string representation has been successfully converted to a value, false otherwise.
   */
  virtual bool setNodeStringValue( const node n, const std::string & v ) = 0;
  /**
   * Returns a string conversion of the value registered for the given edge.
   */
  virtual std::string getEdgeStringValue( const edge e ) const = 0;
  /**
   * Registers a new value for the given edge in converting
   * the given string representation.
   * Returns true if the given string representation has been successfully converted to a value, false otherwise.
   */
  virtual bool setEdgeStringValue( const edge e, const std::string & v ) = 0;
  /**
   * Returns a pointer to a DataMem structure embedding the node default value
   * WARNING: it is of the caller responsibility to delete the returned structure.
  */
  virtual DataMem* getNodeDefaultDataMemValue() const = 0;
  /**
   * Clears all nodes registered values, and set a new node default value
   * using the value embedded in the pointed DataMem structure.
   */
  virtual void setAllNodeDataMemValue(const DataMem* v ) = 0;
  /**
   * Returns a pointer to a DataMem structure embedding the edge default value
   * WARNING: the caller have the responsibility to delete the returned structure.
  */
  virtual DataMem* getEdgeDefaultDataMemValue() const = 0;
  /**
   * Clears all edges registered values, and set a new edge default value
   * using the value embedded in the pointed DataMem structure.
   */
  virtual void setAllEdgeDataMemValue(const DataMem* v ) = 0;
  /**
  * Returns a pointer to a DataMem structure embedding the node registered value
  * WARNING: it is of the caller responsibility to delete the returned structure.
  */
  virtual DataMem* getNodeDataMemValue( const node n ) const = 0;
  /**
   * Returns a NULL pointer if the given node registered value is the default
   * else returns a pointer to a DataMem structure embedding the value
   * WARNING: it is of the caller responsibility to delete the returned structure.
  */
  virtual DataMem* getNonDefaultDataMemValue( const node n ) const = 0;
  /**
   * Registers a new value for the given node
   * using the value embedded in the pointed DataMem structure.
   */
  virtual void setNodeDataMemValue( const node n, const DataMem* v) = 0;
  /**
  * Returns a pointer to a DataMem structure embedding the edge registered value
  * WARNING: it is of the caller responsibility to delete the returned structure.
  */
  virtual DataMem* getEdgeDataMemValue( const edge e ) const = 0;
  /**
   * Returns a NULL pointer if the given edge registered value is the default
   * else returns a pointer to a DataMem structure embedding the value.
   * WARNING: it is of the caller responsibility to delete the returned structure.
  */
  virtual DataMem* getNonDefaultDataMemValue( const edge e ) const = 0;
  /**
   * Registers a new value for the given node
   * using the value embedded in the pointed DataMem structure.
   */
  virtual void setEdgeDataMemValue( const edge e, const DataMem* v) = 0;
  /**
   * Returns an iterator on all nodes whose value is different
   * from the default value. When the pointer to the graph is not NULL
   * only the nodes owned by this graph are returned by the iterator.
   * WARNING: it is of the caller responsibility to delete the returned iterator.
   */
  virtual tlp::Iterator<node>* getNonDefaultValuatedNodes(const Graph* = NULL) const = 0;
  /**
   * Returns an iterator on all edges whose value is different
   * from the default value. When the pointer to the graph is not NULL
   * only the edges owned by this graph are returned by the iterator.
   * WARNING: it is of the caller responsibility to delete the returned iterator.
   */
  virtual tlp::Iterator<edge>* getNonDefaultValuatedEdges(const Graph* = NULL) const = 0;
  /**
   * Sets a computed value for the meta node mN pointing to the sub-graph sg
   * mg is the graph owning the meta node
   */
  virtual void computeMetaValue(node mN, Graph* sg, Graph* mg)=0;
  /**
   * Sets a computed value for the meta edge mE representing the edges
   * from the iterator itE.
   * mg is the graph owning the meta edge.
   */
  virtual void computeMetaValue(edge mE, tlp::Iterator<edge>* itE, Graph* mg)=0;

  // superclass of further MetaValueCalculator
  class MetaValueCalculator {
  public:
    virtual ~MetaValueCalculator() {}
  };


  /**
   * Returns the meta value calculator associated to this property.
   */
  MetaValueCalculator* getMetaValueCalculator() {
    return metaValueCalculator;
  }

  /**
   * Sets the meta value calculator.
   * Be careful that its destruction is not managed by the property.
   */
  virtual void setMetaValueCalculator(MetaValueCalculator* mvCalc) {
    metaValueCalculator = mvCalc;
  }

  /**
   * Registers a new property observer.
   */
  void addPropertyObserver(PropertyObserver *pObs);
  /**
   * Removes a registered property observer.
   */
  void removePropertyObserver(PropertyObserver *pObs);

  /**
   * Returns the number of registered observers.
   */
  unsigned int countPropertyObservers() const {
    return countListeners();
  }

  /**
    * @brief Compare value of the node n1 to the value of the node n2.
    *
    * @return Return 0 if values are equal otherwise otherwise a number different from 0 is returned, with its sign indicating whether the value ot the node n1 is considered greater than the comparing value of the node n2(positive sign), or smaller (negative sign).
    **/
   virtual int compare(const node n1, const node n2) = 0;
   /**
     * @brief Compare value of the edge e1 to the value of the edge e2.
     *
     * @return Return 0 if values are equal otherwise otherwise a number different from 0 is returned, with its sign indicating whether the value of edge e1 is considered greater than the comparing value of the edge e2 (positive sign), or smaller (negative sign).
     **/
   virtual int compare(const edge e1, const edge e2) = 0;

 protected:
  MetaValueCalculator* metaValueCalculator;

  // for notification of PropertyObserver
  void notifyBeforeSetNodeValue(const node n);
  void notifyAfterSetNodeValue(const node n);
  void notifyBeforeSetEdgeValue(const edge e);
  void notifyAfterSetEdgeValue(const edge e);
  void notifyBeforeSetAllNodeValue();
  void notifyAfterSetAllNodeValue();
  void notifyBeforeSetAllEdgeValue();
  void notifyAfterSetAllEdgeValue();
  void notifyDestroy();
};

/// Event class for specific events on PropertyInterface
class TLP_SCOPE PropertyEvent :public Event {
  public:

    // be careful about the ordering of the constants
    // in the enum below because it is used in some assertions
    enum PropertyEventType {TLP_BEFORE_SET_NODE_VALUE = 0,
                            TLP_AFTER_SET_NODE_VALUE,
                            TLP_BEFORE_SET_ALL_NODE_VALUE,
                            TLP_AFTER_SET_ALL_NODE_VALUE,
                            TLP_BEFORE_SET_ALL_EDGE_VALUE,
                            TLP_AFTER_SET_ALL_EDGE_VALUE,
                            TLP_BEFORE_SET_EDGE_VALUE,
                            TLP_AFTER_SET_EDGE_VALUE};
    PropertyEvent(const PropertyInterface& prop, PropertyEventType propEvtType,
                  Event::EventType evtType = Event::TLP_MODIFICATION,
                  unsigned int id = UINT_MAX)
      : Event(prop, evtType), evtType(propEvtType), eltId(id) {}

    PropertyInterface* getProperty() const {
      return reinterpret_cast<PropertyInterface *>(sender());
    }

    node getNode() const {
      assert(evtType < TLP_BEFORE_SET_ALL_NODE_VALUE);
      return node(eltId);
    }

    edge getEdge() const {
      assert(evtType > TLP_AFTER_SET_ALL_EDGE_VALUE);
      return edge(eltId);
    }

    PropertyEventType getType() const {
      return evtType;
    }

  protected:
    PropertyEventType evtType;
    unsigned int eltId;
};
}

#endif // PROPERTY_INTERFACE_H
