//-*-c++-*-
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

  virtual void erase(const node) =0;
  virtual void erase(const edge) =0;
  virtual void copy(const node, const node, PropertyInterface *) =0;
  virtual void copy(const edge, const edge, PropertyInterface *) =0;
  virtual PropertyInterface* clonePrototype(Graph *, const std::string&) =0;
  //=================================================================================
  // Returns a string describing the type of the property.
  // i.e. "graph", "double", "layout", "string", "integer", "color", "size", ...
  virtual std::string getTypename() const = 0;
  static  std::string getTypename(const PropertyInterface *);

  // name management
  const std::string& getName() const {
    return name;
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
  virtual void computeMetaValue( node mN, Graph* sg )=0;
  // mE is the meta edge, itE is an iterator on the underlying edges
  // g is the graph owning the underlying edges
  virtual void computeMetaValue( edge mE, Iterator<edge>* itE, Graph* g )=0;
  // the ones below are used by GraphUpdatesRecorder
  virtual Iterator<node>* getNonDefaultValuatedNodes() const = 0;
  virtual Iterator<edge>* getNonDefaultValuatedEdges() const = 0;
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
