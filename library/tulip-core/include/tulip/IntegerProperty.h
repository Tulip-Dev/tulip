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
///@cond DOXYGEN_HIDDEN

#ifndef TULIP_INT_H
#define TULIP_INT_H

#include <tulip/tuliphash.h>
#include <tulip/PropertyTypes.h>
#include <tulip/AbstractProperty.h>
#include <tulip/PropertyAlgorithm.h>
#include <tulip/Observable.h>
#include <tulip/minmaxproperty.h>
#include <tulip/NumericProperty.h>

namespace tlp {

class Graph;
class PropertyContext;

typedef MinMaxProperty<tlp::IntegerType, tlp::IntegerType, tlp::NumericProperty> IntegerMinMaxProperty;

/**
 * @ingroup Graph
 * @brief A graph property that maps an integer value to graph elements.
 */
class TLP_SCOPE IntegerProperty : public IntegerMinMaxProperty {

public :
  IntegerProperty(Graph *, const std::string& n = "");

  PropertyInterface* clonePrototype(Graph *, const std::string& );
  static const std::string propertyTypename;
  const std::string& getTypename() const {
    return propertyTypename;
  }
  virtual void setNodeValue(const node n, const int &v);
  virtual void setEdgeValue(const edge e, const int &v);
  virtual void setAllNodeValue(const int &v);
  virtual void setAllEdgeValue(const int &v);

  int compare(const node n1, const node n2) const;
  int compare(const edge e1, const edge e2) const;

  // NumericProperty interface
  virtual double getNodeDoubleValue(const node n) const {
    return (double) getNodeValue(n);
  }
  virtual double getNodeDoubleDefaultValue() const {
    return (double) getNodeDefaultValue();
  }
  virtual double getNodeDoubleMin(Graph* g = NULL) {
    return (double) getNodeMin(g);
  }
  virtual double getNodeDoubleMax(Graph* g = NULL) {
    return (double) getNodeMax(g);
  }
  virtual double getEdgeDoubleValue(const edge e) const {
    return (double) getEdgeValue(e);
  }
  virtual double getEdgeDoubleDefaultValue() const {
    return (double) getEdgeDefaultValue();
  }
  virtual double getEdgeDoubleMin(Graph* g = NULL) {
    return (double) getEdgeMin(g);
  }
  virtual double getEdgeDoubleMax(Graph* g = NULL) {
    return (double) getEdgeMax(g);
  }

  void nodesUniformQuantification(unsigned int);

  void edgesUniformQuantification(unsigned int);

  NumericProperty* copyProperty(Graph *g) {
    IntegerProperty* newProp = new IntegerProperty(g);
    newProp->copy(this);

    return newProp;
  }

protected:
  virtual void clone_handler(AbstractProperty<tlp::IntegerType, tlp::IntegerType, tlp::NumericProperty> &);

private:
  // override Observable::treatEvent
  void treatEvent(const Event&);
};

/**
 * @ingroup Graph
 * @brief A graph property that maps a std::vector<int> value to graph elements.
 */
class TLP_SCOPE IntegerVectorProperty:public AbstractVectorProperty<tlp::IntegerVectorType, tlp::IntegerType> {
public :
  IntegerVectorProperty(Graph *g, const std::string& n =""):AbstractVectorProperty<IntegerVectorType, tlp::IntegerType>(g, n) {}
  // redefinition of some PropertyInterface methods
  PropertyInterface* clonePrototype(Graph *, const std::string& );
  static const std::string propertyTypename;
  const std::string& getTypename() const {
    return propertyTypename;
  }

};


}
#endif
///@endcond
