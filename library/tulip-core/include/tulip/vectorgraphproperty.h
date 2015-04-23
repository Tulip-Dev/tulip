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

#ifndef VECTORGRAPHPROPERTY_H
#define VECTORGRAPHPROPERTY_H
#include <algorithm>

namespace tlp {
class VectorGraph;
/**
 * @brief Internal class to access to a stl::vector in VectorGraph
 * @warning never use that class
 */
class ValArrayInterface {
  friend class VectorGraph;
protected:
  virtual void addElement(const unsigned int id) = 0;
  virtual void reserve(const size_t size) = 0;
  virtual ~ValArrayInterface() {}
};
//===========================================
/**
 * @brief Internal class to access to a stl::vector in VectorGraph
 * @warning never use that class
 */
template <typename TYPE>
class  ValArray : public  ValArrayInterface {
  friend class VectorGraph;
protected:
  ValArray(const unsigned int size = 0, const unsigned int capacity = 0) {
    _data.reserve(capacity);
    _data.resize(size);
  }
  virtual ~ValArray() {}
  void addElement(const unsigned int id) {
    if (id >= _data.size()) {
      _data.resize(id);
      _data.push_back(TYPE());
    }
  }
  void reserve(const size_t size) {
    _data.reserve(size);
  }

public:
  std::vector<TYPE> _data; /**< TODO */
};
/**
  * @class VectorGraphProperty
  * @brief That class enables to factorize code for NodeProperty and EdgeProperty in VectorGraph, it could not be used directly.
  * @see NodeProperty
  * @see EdgeProperty
  * @see VectorGraph
  */
template <typename TYPE>
class VectorGraphProperty {
  friend class VectorGraph;
public:
  /**
    * @brief read/write accessor
    *
    * return the value of the ith element and enables to modify it.
    */
  typename std::vector<TYPE>::reference operator[](const size_t id) {
    //assert(isValid());
    assert(id < (*_array)._data.size());
    return (*_array)._data[id];
  }
  /**
    * @brief read accessor
    *
    * return the value of the ith element.
    */
  typename std::vector<TYPE>::const_reference operator[](const size_t id) const {
    //assert(isValid());
    assert(id < (*_array)._data.size());
    return (*_array)._data[id];
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
    fill(_array->_data.begin(), _array->_data.end(), obj);
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

protected:
  VectorGraphProperty():_array(0), _graph(0) {
  }
  VectorGraphProperty(const VectorGraphProperty &obj): _array(obj._array), _graph(obj._graph) {
  }
  VectorGraphProperty(ValArray<TYPE> *array, VectorGraph *graph):_array(array), _graph(graph) {
  }
protected:
  ValArray<TYPE> *_array; /**< TODO */
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
 * @warning After the call to free all The copy of the orignal EdgeProperty are no more valid
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
 * edge e;
 * forEach(e, g.getEdges()) {
 *   weight[n] = g.deg(g.target(e)) + g.deg(g.source(e));
 * }
 * EdgeProperty<double> weight2 = weight; //weight2 and weight are pointing on the same memory addres
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
  EdgeProperty():VectorGraphProperty<TYPE>() {}
  EdgeProperty(const EdgeProperty &obj): VectorGraphProperty<TYPE>(obj) {}
#ifndef NDEBUG
  bool isValid() const;
#endif

private:
  EdgeProperty(ValArray<TYPE> *array, VectorGraph *graph):VectorGraphProperty<TYPE>(array, graph) {}

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
  * @warning After the call to free all The copy of the orignal NodeProperty are no more valid
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
  * node n;
  * forEach(n, g.getNodes()) {
  *   weight[n] = g.deg(n);
  * }
  * NodeProperty<double> weight2 = weight; //weight2 and weight are pointing on the same memory addres
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
  NodeProperty():VectorGraphProperty<TYPE>() {}
  NodeProperty(const NodeProperty &obj): VectorGraphProperty<TYPE>(obj) {}
#ifndef NDEBUG
  bool isValid() const;
#endif

private:
  NodeProperty(ValArray<TYPE> *array, VectorGraph *graph):VectorGraphProperty<TYPE>(array, graph) {}
};

}
#endif // VECTORGRAPHPROPERTY_H
///@endcond
