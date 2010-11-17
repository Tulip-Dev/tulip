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
#include "tulip/tulipconf.h"
#include "tulip/Observable.h"
#include "tulip/ObservableProperty.h"
#include "tulip/Iterator.h"
#include "tulip/Node.h"
#include "tulip/Edge.h"
#include "tulip/Reflect.h"

namespace tlp {

/**
 * \defgroup properties
 */ 
/*@{*/
//=============================================================
class TLP_SCOPE PropertyInterface: public Observable, public ObservableProperty {
  friend class PropertyManager;
protected:
  // name of the property when registered as a property of a graph
  std::string name;
  // the graph for whom the property is registered
  Graph *graph;

public:
  virtual ~PropertyInterface();
  /**
   * Remove the value stored for the node given in parameter.
   */
  virtual void erase(const node) = 0;
  /**
   * Remove the value stored for the edge given in parameter.
   */
  virtual void erase(const edge) = 0;
  /**
   * Set the value of a node (first argument) in the current property (this)
   * with the value of the node (second argument) defined in prop (third argument)
   * If the fourth argument is set to true, the value will be copied only if
   * it is not the default value.
   */
  virtual void copy(const node src, const node dst, PropertyInterface *prop,
		    bool ifNotDefault = false) =0;
  /**
   * Set the value of an edge (first argument) in the current property (this)
   * with the value of the edge (second argument) defined in prop (third argument)
   * defined in this property (this).
   * If the fourth argument is set to true, the value will be copied only if
   * it is not the default value.
   */
  virtual void copy(const edge src, const edge dst , PropertyInterface *prop,
		    bool ifNotDefault = false) =0;
  /**
    * Create an object of the same real type of the current property, in the
    * the graph (first parameter) with the name (second parameter).
    * Values are not copied.
    */
  virtual PropertyInterface* clonePrototype(Graph *, const std::string&) =0;
  //=================================================================================
  /**
    * Returns a string describing the type of the property.
    * i.e. "graph", "double", "layout", "string", "integer", "color", "size", ...
    */
  virtual std::string getTypename() const = 0;
  /**
    * Returns a string describing the type a property (first parameter).
    * i.e. "graph", "double", "layout", "string", "integer", "color", "size", ...
    */
  static  std::string getTypename(const PropertyInterface *);

  /**
    * Return the name of the property
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

  // Untyped accessors
  virtual std::string getNodeDefaultStringValue() const = 0;
  virtual std::string getEdgeDefaultStringValue() const = 0;
  virtual std::string getNodeStringValue( const node n ) const = 0;
  virtual std::string getEdgeStringValue( const edge e ) const = 0;
  virtual bool setNodeStringValue( const node n, const std::string & v ) = 0;
  virtual bool setEdgeStringValue( const edge e, const std::string & v ) = 0;
  virtual bool setAllNodeStringValue( const std::string & v ) = 0;
  virtual bool setAllEdgeStringValue( const std::string & v ) = 0;
  // two methods to compute the values for meta nodes or edges
  // mN is the meta node, sg is the corresponding subgraph
  // and g is the graph owning mN
  virtual void computeMetaValue(node mN, Graph* sg, Graph* mg)=0;
  // mE is the meta edge, itE is an iterator on the underlying edges
  // mg is the graph owning mE
  virtual void computeMetaValue(edge mE, tlp::Iterator<edge>* itE, Graph* mg)=0;
  // the ones below are used by GraphUpdatesRecorder
  virtual tlp::Iterator<node>* getNonDefaultValuatedNodes(const Graph* = NULL) const = 0;
  virtual tlp::Iterator<edge>* getNonDefaultValuatedEdges(const Graph* = NULL) const = 0;
  virtual DataMem* getNodeDefaultDataMemValue() const = 0;
  virtual DataMem* getEdgeDefaultDataMemValue() const = 0;
  virtual DataMem* getNodeDataMemValue( const node n ) const = 0;
  virtual DataMem* getEdgeDataMemValue( const edge e ) const = 0;
  virtual DataMem* getNonDefaultDataMemValue( const node n ) const = 0;
  virtual DataMem* getNonDefaultDataMemValue( const edge e ) const = 0;
  virtual void setNodeDataMemValue( const node n, const DataMem* v) = 0;
  virtual void setEdgeDataMemValue( const edge e, const DataMem* v) = 0;
  virtual void setAllNodeDataMemValue(const DataMem* v ) = 0;
  virtual void setAllEdgeDataMemValue(const DataMem* v ) = 0;

  // superclass of further MetaValueCalculator
  class MetaValueCalculator {
  public:
    virtual ~MetaValueCalculator() {}
  };

  // method to specify the meta value calculator.
  // be careful that its destruction is not managed by the property
  virtual void setMetaValueCalculator(MetaValueCalculator* mvCalc) {
    metaValueCalculator = mvCalc;
  }

  MetaValueCalculator* getMetaValueCalculator() {
    return metaValueCalculator;
  }

 protected:
  MetaValueCalculator* metaValueCalculator;
  // redefinitions of ObservableProperty methods
  void notifyAfterSetNodeValue(PropertyInterface*,const node n);
  void notifyAfterSetEdgeValue(PropertyInterface*,const edge e);
  void notifyAfterSetAllNodeValue(PropertyInterface*);
  void notifyAfterSetAllEdgeValue(PropertyInterface*);
  void notifyDestroy(PropertyInterface*);
};

}

#endif // PROPERTY_INTERFACE_H
