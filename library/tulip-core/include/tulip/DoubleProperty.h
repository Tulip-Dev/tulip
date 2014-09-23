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

#ifndef TULIP_METRIC_H
#define TULIP_METRIC_H

#include <tulip/tuliphash.h>
#include <tulip/PropertyTypes.h>
#include <tulip/AbstractProperty.h>
#include <tulip/PropertyAlgorithm.h>
#include <tulip/minmaxproperty.h>
#include <tulip/NumericProperty.h>

namespace tlp {

class PropertyContext;

typedef MinMaxProperty<tlp::DoubleType, tlp::DoubleType, tlp::NumericProperty> DoubleMinMaxProperty;

/**
 * @ingroup Graph
 * @brief A graph property that maps a double value to graph elements.
 */
class TLP_SCOPE DoubleProperty : public DoubleMinMaxProperty {
public :
  DoubleProperty (Graph *, const std::string& n="");

  virtual void clone_handler(AbstractProperty<tlp::DoubleType, tlp::DoubleType, tlp::NumericProperty> &);

  PropertyInterface* clonePrototype(Graph *, const std::string& );
  static const std::string propertyTypename;
  const std::string& getTypename() const {
    return propertyTypename;
  }

  virtual void setNodeValue(const node n, const double &v);
  virtual void setEdgeValue(const edge e, const double &v);
  virtual void setAllNodeValue(const double &v);
  virtual void setAllEdgeValue(const double &v);

  enum PredefinedMetaValueCalculator {NO_CALC = 0, AVG_CALC = 1, SUM_CALC = 2,
                                      MAX_CALC = 3, MIN_CALC = 4
                                     };

  // setMetaValueCalculator overloading
  virtual void setMetaValueCalculator(PropertyInterface::MetaValueCalculator* calc);
  void setMetaValueCalculator(PredefinedMetaValueCalculator nodeCalc = AVG_CALC,
                              PredefinedMetaValueCalculator edgeCalc = AVG_CALC);

  // NumericProperty interface
  virtual double getNodeDoubleValue(const node n) const {
    return getNodeValue(n);
  }
  virtual double getNodeDoubleDefaultValue() const {
    return getNodeDefaultValue();
  }
  virtual double getNodeDoubleMin(Graph* g = NULL) {
    return getNodeMin(g);
  }
  virtual double getNodeDoubleMax(Graph* g = NULL) {
    return getNodeMax(g);
  }
  virtual double getEdgeDoubleValue(const edge e) const {
    return getEdgeValue(e);
  }
  virtual double getEdgeDoubleDefaultValue() const {
    return getEdgeDefaultValue();
  }
  virtual double getEdgeDoubleMin(Graph* g = NULL) {
    return getEdgeMin(g);
  }
  virtual double getEdgeDoubleMax(Graph* g = NULL) {
    return getEdgeMax(g);
  }

  void nodesUniformQuantification(unsigned int);

  void edgesUniformQuantification(unsigned int);

  NumericProperty* copyProperty(Graph *g) {
    DoubleProperty* newProp = new DoubleProperty(g);
    newProp->copy(this);

    return newProp;
  }

private:
  // override Observable::treatEvent
  void treatEvent(const Event&);

};

/**
 * @ingroup Graph
 * @brief A graph property that maps a std::vector<double> value to graph elements.
 */

class TLP_SCOPE DoubleVectorProperty:public AbstractVectorProperty<tlp::DoubleVectorType, tlp::DoubleType> {
public :
  DoubleVectorProperty(Graph *g, const std::string& n=""):AbstractVectorProperty<DoubleVectorType, tlp::DoubleType>(g, n) {}
  // redefinition of some PropertyInterface methods
  PropertyInterface* clonePrototype(Graph *, const std::string& );
  static const std::string propertyTypename;
  const std::string& getTypename() const {
    return propertyTypename;
  }

};


}
#endif
