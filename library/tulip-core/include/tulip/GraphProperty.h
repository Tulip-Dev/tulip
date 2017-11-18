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

#ifndef TULIP_METAGRAPH_H
#define TULIP_METAGRAPH_H

#include <set>

#include <tulip/PropertyTypes.h>
#include <tulip/AbstractProperty.h>

namespace tlp {

class PropertyContext;
class GraphAbstract;

typedef AbstractProperty<tlp::GraphType, tlp::EdgeSetType> AbstractGraphProperty;

/**
 * @ingroup Graph
 * @brief A graph property that maps a tlp::Graph* value to graph elements.
 *
 * @warning This property is mainly used into the meta node engine. Using GraphProperty outside of
 * this system is strongly discouraged since it could lead to unwanted behavior.
 */
class TLP_SCOPE GraphProperty : public AbstractGraphProperty {
  friend class GraphAbstract;

public:
  GraphProperty(Graph *, const std::string &n = "");
  virtual ~GraphProperty();
  // override Observable::treatEvent
  void treatEvent(const Event &);

  // redefinition of some PropertyInterface methods
  PropertyInterface *clonePrototype(Graph *, const std::string &) const;
  bool setNodeStringValue(const node n, const std::string &v);
  bool setAllNodeStringValue(const std::string &v);
  bool setStringValueToGraphNodes(const std::string &v, const tlp::Graph *graph);
  bool setEdgeStringValue(const edge e, const std::string &v);
  bool setAllEdgeStringValue(const std::string &v);
  bool setStringValueToGraphEdges(const std::string &v, const tlp::Graph *graph);
  static const std::string propertyTypename;
  const std::string &getTypename() const {
    return propertyTypename;
  }

  // redefinition of some AbstractProperty methods
  virtual void setNodeValue(const node n,
                            tlp::StoredType<GraphType::RealType>::ReturnedConstValue g);
  virtual void setAllNodeValue(tlp::StoredType<GraphType::RealType>::ReturnedConstValue g);
  virtual void setValueToGraphNodes(tlp::StoredType<GraphType::RealType>::ReturnedConstValue g,
                                    const Graph *graph);
  virtual bool readNodeDefaultValue(std::istream &iss);
  virtual bool readNodeValue(std::istream &iss, node n);
  // tlp::GraphType encapsulates a tlp::Graph pointer but that is the graph id
  // that gets serialized when using the TLPB format
  virtual unsigned int nodeValueSize() const {
    return sizeof(unsigned int);
  }
  virtual unsigned int edgeValueSize() const {
    return 0;
  }

  // for optimizations purpose
  bool hasNonDefaultValue(const node n) const {
    return nodeProperties.hasNonDefaultValue(n.id);
  }
  bool hasNonDefaultValue(const edge e) const {
    return !edgeProperties.get(e.id).empty();
  }

private:
  MutableContainer<std::set<node> > referencedGraph;
  const std::set<edge> &getReferencedEdges(const edge) const;
};
}
#endif
