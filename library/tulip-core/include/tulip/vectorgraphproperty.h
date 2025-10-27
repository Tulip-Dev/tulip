/*
 *
 * This file is part of Tulip (https://tulip.labri.fr)
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

#ifndef VECTORGRAPHPROPERTY_H
#define VECTORGRAPHPROPERTY_H
#include <algorithm>
#include <cassert>
#include <vector>

namespace tlp {
class VectorGraph;

/**
 * @brief Internal class to access to a stl::vector in VectorGraph
 * @warning never use that class
 */
class VectorGraphValues {
  friend class VectorGraph;

protected:
  virtual void addElement(const unsigned int id) = 0;
  virtual void reserve(const size_t size) = 0;
  virtual ~VectorGraphValues() {}
};

/**
 * @class VectorGraphProperty
 * @brief That class enables to factorize code for NodeProperty and EdgeProperty in VectorGraph, it
 * could not be used directly.
 * @see NodeProperty
 * @see EdgeProperty
 * @see VectorGraph
 */
template <typename TYPE>
class VectorGraphProperty {
  friend class VectorGraph;

protected:
  //===========================================
  /**
   * @brief Internal class to access to a stl::vector in VectorGraph
   * @warning never use that class
   */
  struct ValuesImpl : public VectorGraphValues, public std::vector<TYPE> {

    ValuesImpl(const unsigned int size = 0, const unsigned int capacity = 0) {
      std::vector<TYPE>::reserve(capacity);
      std::vector<TYPE>::resize(size);
    }
    ~ValuesImpl() override {}
    void addElement(const unsigned int id) override {
      if (id >= std::vector<TYPE>::size()) {
        std::vector<TYPE>::resize(id + 1);
      }
    }
    void reserve(const size_t size) override {
      std::vector<TYPE>::reserve(size);
    }
  };

public:
  virtual ~VectorGraphProperty() {}
  /**
   * @brief read/write accessor
   *
   * return the value of the ith element and enables to modify it.
   */
  typename std::vector<TYPE>::reference operator[](const size_t id) {
    // assert(isValid());
    assert(id < _values->size());
    return (*_values)[id];
  }
  /**
   * @brief read accessor
   *
   * return the value of the ith element.
   */
  typename std::vector<TYPE>::const_reference operator[](const size_t id) const {
    // assert(isValid());
    assert(id < _values->size());
    return (*_values)[id];
  }
  /**
   * @bried Set all the value of the property to the value given in parameter
   *
   * That function affect the same value to all elements of the vector, there
   * is no effect on the future value added in the vector
   * @warning There is differences between the setAll of the MutableContainer and
   * the setAll of VectorProperty (NodeProperty or EdgeProperty). The MutableContainer,
   * ensures that new inserted element will have the value given by the last setAll
   *
   * @see MutableContainer
   */
  void setAll(const TYPE &obj) {
    fill(_values->begin(), _values->end(), obj);
  }
  /**
   * @brief write accessor
   *
   * change the value of the ith element.
   */
  void set(const size_t id, const TYPE &obj) {
    (*this)[id] = obj;
  }
  /**
   * @brief read accessor
   *
   * return the value of the ith element.
   */
  typename std::vector<TYPE>::const_reference get(const size_t id) const {
    return (*this)[id];
  }
#ifndef NDEBUG
  virtual bool isValid() const = 0;
#endif

  void swap(VectorGraphProperty<TYPE> &v) {
    assert(_values && (_graph == v._graph));
    _values->swap(*(v._values));
  }

protected:
  VectorGraphProperty() : _values(nullptr), _graph(nullptr) {}
  VectorGraphProperty(const VectorGraphProperty &obj) : _values(obj._values), _graph(obj._graph) {}
  VectorGraphProperty(ValuesImpl *values, VectorGraph *graph) : _values(values), _graph(graph) {}
  VectorGraphProperty &operator=(const VectorGraphProperty &) = default;

  ValuesImpl *_values; /**< TODO */
  VectorGraph *_graph; /**< TODO */
};

/**
 * @class EdgeProperty
 * @brief That class enables to define a property/attribute on edges on a VectorGraph.
 *
 * Using EdgeProperty you can assign any kind of attribute to edges. To use that class
 * you must first create an instance of EdgeProperty and then connect it to your graph.
 * NodeProperty can be copied in another EdgeProperty, however the to NodeProperty will
 * share the same content. You can consider that EdgeProperty is just a pointer on a stl:vector.
 * to free memory used by a EdgeProperty connected to a graph you must use the free function.
 * @warning After the call to free all The copy of the original EdgeProperty are no more valid
 *
 * Using EdgeProperty you can store and access to values with the same efficiency as if
 * you created manually a vector. EdgeProperty manage for you the resize, etc... when the
 * graph is modified.
 *
 * Furthemrore, in DEBUG mode, operator[] check if one try to access outside of the Array Bound.
 * in DEBUG mode, the validy of the Property is also checked (if it has been free/alloc etc...)
 *
 * @code
 * VectorGraph g;
 * EdgeProperty<double> weight;
 * g.alloc(weight); //connect weight to g, g allocate memory for that attribute
 * for (const edge &e : g.edges()) {
 *   weight[n] = g.deg(g.target(e)) + g.deg(g.source(e));
 * }
 * EdgeProperty<double> weight2 = weight; //weight2 and weight are pointing on the same memory
 * address
 * weight2[g[0]] = 3;
 * tlp::debug() << weight[g[0]]; //output 3
 * g.free(weight2); //free the memory, weight and weight2 are no more valid.
 * @endcode
 * @see VectorGraph alloc(EdgeProperty)
 * @see VectorGraph free(EdgeProperty)
 * @see VectorGraph
 */
template <typename TYPE>
class EdgeProperty : public VectorGraphProperty<TYPE> {
  friend class VectorGraph;

public:
  EdgeProperty() : VectorGraphProperty<TYPE>() {}
  EdgeProperty(const EdgeProperty &obj) : VectorGraphProperty<TYPE>(obj) {}
  EdgeProperty &operator=(const EdgeProperty &) = default;
#ifndef NDEBUG
  bool isValid() const;
#endif

private:
  EdgeProperty(typename VectorGraphProperty<TYPE>::ValuesImpl *values, VectorGraph *graph)
      : VectorGraphProperty<TYPE>(values, graph) {}
};
/**
 * @class NodeProperty
 * @brief That class enables to define a property/attribute on nodes on a VectorGraph.
 *
 * Using NodeProperty you can assign any kind of attribute to nodes. To use that class
 * you must first create an instance of NodeProperty and then connect it to your graph.
 * NodeProperty can be copied in another NodeProperty, however the to NodeProperty will
 * share the same content. You can consider that NodeProperty is just a pointer on a stl:vector.
 * to free memory used by a NodeProperty connected to a graph you must use the free function.
 * @warning After the call to free all The copy of the original NodeProperty are no more valid
 *
 * Using NodeProperty you can store and access to values with the same efficiency as if
 * you created manually a vector. NodeProperty manage for you the resize, etc... when the
 * graph is modified.
 *
 * Furthemrore, in DEBUG mode, operator[] check if one try to access outside of the Array Bound.
 * in DEBUG mode, the validy of the Property is also checked (if it has been free/alloc etc...)
 *
 * @code
 * VectorGraph g;
 * NodeProperty<double> weight;
 * g.alloc(weight); //connect weight to g, g allocate memory for that attribute
 * for(const node &n : g.nodes()) {
 *   weight[n] = g.deg(n);
 * }
 * NodeProperty<double> weight2 = weight; //weight2 and weight are pointing on the same memory
 * address
 * weight2[g[0]] = 3;
 * tlp::debug() << weight[g[0]]; //output 3
 * g.free(weight2); //free the memory, weight and weight2 are no more valid.
 * @endcode
 * @see VectorGraph alloc(NodeProperty)
 * @see VectorGraph free(NodeProperty)
 */
template <typename TYPE>
class NodeProperty : public VectorGraphProperty<TYPE> {
  friend class VectorGraph;

public:
  NodeProperty() : VectorGraphProperty<TYPE>() {}
  NodeProperty(const NodeProperty &obj) : VectorGraphProperty<TYPE>(obj) {}
  NodeProperty &operator=(const NodeProperty &) = default;
#ifndef NDEBUG
  bool isValid() const;
#endif

private:
  NodeProperty(typename VectorGraphProperty<TYPE>::ValuesImpl *values, VectorGraph *graph)
      : VectorGraphProperty<TYPE>(values, graph) {}
};
} // namespace tlp
#endif // VECTORGRAPHPROPERTY_H
///@endcond
