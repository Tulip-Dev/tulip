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
#include "tulip/tulipgpu.h"
#include <tulip/ForEach.h>

using namespace tlp;
using namespace std;

// gpu graph creation from a Tulip graph
GpuGraph* tlp::genGpuGraph(const Graph& graph) {
  // allocate and fill nodes
  CpuNode* nodes = new CpuNode[graph.numberOfNodes()];
  node n;
  unsigned int i = 0;
  forEach(n, graph.getNodes()) {
    nodes[i].id = n.id;
    nodes[i].indeg = graph.indeg(n);
    nodes[i].outdeg = graph.outdeg(n);
    ++i;
  }

  // allocate and fill edges
  CpuEdge* edges = new CpuEdge[graph.numberOfEdges()];
  i = 0;
  edge e;
  forEach(e, graph.getEdges()) {
    edges[i].source = graph.source(e).id;
    edges[i].target = graph.target(e).id;
    ++i;
  }

  // build GpuGraph
  GpuGraph* gpuGraph = ::genGpuGraph(nodes, graph.numberOfNodes(),
				     edges, graph.numberOfEdges());
  // deallocate nodes & edges
  delete [] nodes;
  delete [] edges;

  return gpuGraph;
}

// GpuProperty creation from a BooleanProperty
GpuProperty* tlp::genGpuProperty(BooleanProperty& prop, const Graph& graph,
				 GpuValueType valType, bool outputOnly) {
  // create the object
  unsigned int nbNodes = (valType == EDGE_VALUES) ? 0 : graph.numberOfNodes();
  unsigned int nbEdges = (valType == NODE_VALUES) ? 0 : graph.numberOfEdges();
  GpuProperty *gpuProp = ::genGpuProperty(nbNodes, nbEdges,
					  sizeof(float), GL_RGBA32F_ARB,
					  GL_RED, GL_FLOAT);
  if (!gpuProp)
    return NULL;
  
  if (!outputOnly) {
    float* values = (float*) getGpuPropertyValues(gpuProp);
    if (nbNodes) {
      node n;
      forEach(n, graph.getNodes()) {
	*values = (prop.getNodeValue(n)) ? 1. : 0.;
	++values;
      }
    }
    if (nbEdges) {
      edge e;
      forEach(e, graph.getEdges()) {
	*values = (prop.getEdgeValue(e)) ? 1. : 0.;
	++values;
      }
    }
  }
  return gpuProp;
}

// GpuProperty creation from a ColorProperty
GpuProperty* tlp::genGpuProperty(ColorProperty& prop, const Graph& graph,
				 GpuValueType valType, bool outputOnly) {
  // create the object
  unsigned int nbNodes = (valType == EDGE_VALUES) ? 0 : graph.numberOfNodes();
  unsigned int nbEdges = (valType == NODE_VALUES) ? 0 : graph.numberOfEdges();
  GpuProperty *gpuProp = ::genGpuProperty(nbNodes, nbEdges,
					  4 * sizeof(float), GL_RGBA32F_ARB,
					  GL_RGBA, GL_FLOAT);
  if (!gpuProp)
    return NULL;

  if (!outputOnly) {
    float* values = (float *) getGpuPropertyValues(gpuProp);
    if (nbNodes) {
      node n;
      forEach(n, graph.getNodes()) {
	Color color = prop.getNodeValue(n);
	*values = color.getR();
	++values;
	*values = color.getG();
	++values;
	*values = color.getB();
	++values;
	*values = color.getA();
	++values;
      }
    }
    if (nbEdges) {
      edge e;
      forEach(e, graph.getEdges()) {
	Color color = prop.getEdgeValue(e);
	*values = color.getR();
	++values;
	*values = color.getG();
	++values;
	*values = color.getB();
	++values;
	*values = color.getA();
	++values;
      }
    }
  }
  return gpuProp;
}

// GpuProperty creation from a DoubleProperty
GpuProperty* tlp::genGpuProperty(DoubleProperty& prop, const Graph& graph,
				 GpuValueType valType, bool outputOnly) {
  // create the object
  unsigned int nbNodes = (valType == EDGE_VALUES) ? 0 : graph.numberOfNodes();
  unsigned int nbEdges = (valType == NODE_VALUES) ? 0 : graph.numberOfEdges();
  GpuProperty *gpuProp = ::genGpuProperty(nbNodes, nbEdges,
					  sizeof(float), GL_RGBA32F_ARB,
					  GL_RED, GL_FLOAT);
  if (!gpuProp)
    return NULL;

  if (!outputOnly) {
    float* values = (float *) getGpuPropertyValues(gpuProp);
    if (nbNodes) {
      node n;
      forEach(n, graph.getNodes()) {
	*values = (float) prop.getNodeValue(n);
	++values;
      }
    }
    if (nbEdges) {
      edge e;
      forEach(e, graph.getEdges()) {
	*values = (float) prop.getEdgeValue(e);
	++values;
      }
    }
  }
  return gpuProp;
}

// GpuProperty creation from an IntegerProperty
GpuProperty* tlp::genGpuProperty(IntegerProperty& prop, const Graph& graph,
				 GpuValueType valType, bool outputOnly) {
  // create the object
  unsigned int nbNodes = (valType == EDGE_VALUES) ? 0 : graph.numberOfNodes();
  unsigned int nbEdges = (valType == NODE_VALUES) ? 0 : graph.numberOfEdges();
  GpuProperty *gpuProp = ::genGpuProperty(nbNodes, nbEdges,
					  sizeof(float), GL_RGBA32F_ARB,
					  GL_RED, GL_FLOAT);
  if (!gpuProp)
    return NULL;

  if (!outputOnly) {
    float* values = (float *) getGpuPropertyValues(gpuProp);
    if (nbNodes) {
      node n;
      forEach(n, graph.getNodes()) {
	*values = (float) prop.getNodeValue(n);
	++values;
      }
    }
    if (nbEdges) {
      edge e;
      forEach(e, graph.getEdges()) {
	*values = (float) prop.getEdgeValue(e);
	++values;
      }
    }
  }
  return gpuProp;
}

// GpuProperty creation from a LayoutProperty
GpuProperty* tlp::genGpuProperty(LayoutProperty& prop, const Graph& graph,
				 bool outputOnly) {
  // create the object with nothing for edges
  GpuProperty *gpuProp = ::genGpuProperty(graph.numberOfNodes(), 0,
					  3 * sizeof(float), GL_RGBA32F_ARB,
					  GL_RGB, GL_FLOAT);
  if (!gpuProp)
    return NULL;

  if (!outputOnly) {
    float* values = (float *) getGpuPropertyValues(gpuProp);
    node n;
    forEach(n, graph.getNodes()) {
      Coord coord = prop.getNodeValue(n);
      *values = coord.getX();
      ++values;
      *values = coord.getY();
      ++values;
      *values = coord.getZ();
      ++values;
    }
  }
  return gpuProp;
}

// GpuProperty creation from a SizeProperty
GpuProperty* tlp::genGpuProperty(SizeProperty& prop, const Graph& graph,
				 GpuValueType valType, bool outputOnly) {
  unsigned int nbNodes = (valType == EDGE_VALUES) ? 0 : graph.numberOfNodes();
  unsigned int nbEdges = (valType == NODE_VALUES) ? 0 : graph.numberOfEdges();
  GpuProperty *gpuProp = ::genGpuProperty(nbNodes, nbEdges,
					  3 * sizeof(float), GL_RGBA32F_ARB,
					  GL_RGB, GL_FLOAT);
  if (!gpuProp)
    return NULL;

  if (!outputOnly) {
    float* values = (float *) getGpuPropertyValues(gpuProp);
    if (nbNodes) {
      // get nodes values
      node n;
      forEach(n, graph.getNodes()) {
	Size size = prop.getNodeValue(n);
	*values = size.getW();
	++values;
	*values = size.getH();
	++values;
	*values = size.getD();
	++values;
      }
    }
    if (nbEdges) {
      // get edges values
      edge e;
      forEach(e, graph.getEdges()) {
	Size size = prop.getEdgeValue(e);
	*values = size.getW();
	++values;
	*values = size.getH();
	++values;
	*values = size.getD();
	++values;
      }
    }
  }
  return gpuProp;
}

// set parameter functions
// function for a Color class
bool setGpuParameter(const string& vName, const Color& val) {
  vec4 v;
  v.color.r = (float) val.getR();
  v.color.g = (float) val.getG();
  v.color.b = (float) val.getB();
  v.color.a = (float) val.getA();
  return setGpuParameter(vName, v);
}

// function for a Coord class
bool setGpuParameter(const string& vName, const Coord& val) {
  vec3 v;
  v.pos.x = val.getX();
  v.pos.y = val.getY();
  v.pos.z = val.getZ();
  return setGpuParameter(vName, v);
}

// function for a Size class
bool setGpuParameter(const string& vName, const Size& val) {
  vec3 v;
  v.pos.x = val.getW();
  v.pos.y = val.getH();
  v.pos.z = val.getD();
  return setGpuParameter(vName, v);
}

// internal function to hold some checks
static void* getGpuOutPropertyValues(unsigned int valSize, unsigned int nbNodes,
				     unsigned int nbEdges, GpuValueType& type) {
  unsigned int nbValues;
  unsigned int vSize;

  void* values = ::getGpuOutPropertyValues(nbValues, vSize, type);
#if !defined(NDEBUG)
  assert(valSize == vSize);
  assert((type == NODE_VALUES) ? (nbValues == nbNodes) : (nbValues == nbEdges));
#endif
  return values;
}

// function to get the result in a BooleanProperty
bool tlp::getGpuOutPropertyValues(BooleanProperty& prop, const Graph& graph) {
  GpuValueType type;
  // set property values with result ones
  float* values =
    (float*) getGpuOutPropertyValues(sizeof(float), graph.numberOfNodes(),
				     graph.numberOfEdges(), type);
  if (!values)
    return false;
  if (type == NODE_VALUES) {
    node n;
    forEach(n, graph.getNodes()) {
      prop.setNodeValue(n, *values ? true : false);
      ++values;
    }
  } else {
    edge e;
    forEach(e, graph.getEdges()) {
      prop.setEdgeValue(e, *values ? true : false);
      ++values;
    }
  }
  return true;
}

// function to get the result in a ColorProperty
bool tlp::getGpuOutPropertyValues(ColorProperty& prop, const Graph& graph) {
  GpuValueType type;
  // set property values with result ones
  float* values =
    (float *) getGpuOutPropertyValues(4 * sizeof(float), graph.numberOfNodes(),
				      graph.numberOfEdges(), type);
  if (!values)
    return false;
  if (type == NODE_VALUES) {
    node n;
    forEach(n, graph.getNodes()) {
      Color color;
      color.setR((unsigned char) *values);
      ++values;
      color.setG((unsigned char) *values);
      ++values;
      color.setB((unsigned char) *values);
      ++values;
      color.setA((unsigned char) *values);
      ++values;
      prop.setNodeValue(n, color);
    }
  } else {
    edge e;
    forEach(e, graph.getEdges()) {
      Color color;
      color.setR((unsigned char) *values);
      ++values;
      color.setG((unsigned char) *values);
      ++values;
      color.setB((unsigned char) *values);
      ++values;
      color.setA((unsigned char) *values);
      ++values;
      prop.setEdgeValue(e, color);
    }
  }
  return true;
}

// function to get the result in a DoubleProperty
bool tlp::getGpuOutPropertyValues(DoubleProperty& prop, const Graph& graph) {
  GpuValueType type;
  // set property values with result ones
  float* values =
    (float *) getGpuOutPropertyValues(sizeof(float), graph.numberOfNodes(),
				      graph.numberOfEdges(), type);
  if (!values)
    return false;
  if (type == NODE_VALUES) {
    node n;
    forEach(n, graph.getNodes()) {
      prop.setNodeValue(n, (double) *values);
      ++values;
    }
  } else {
    edge e;
    forEach(e, graph.getEdges()) {
      prop.setEdgeValue(e, (double) *values);
      ++values;
    }
  }
  return true;
}

// function to get the result in an IntegerProperty
bool tlp::getGpuOutPropertyValues(IntegerProperty& prop, const Graph& graph) {
  GpuValueType type;
  // set property values with result ones
  float* values =
    (float *) ::getGpuOutPropertyValues(sizeof(float), graph.numberOfNodes(),
					graph.numberOfEdges(), type);
  if (!values)
    return false;
  if (type == NODE_VALUES) {
    node n;
    forEach(n, graph.getNodes()) {
      prop.setNodeValue(n, (int) *values);
      ++values;
    }
  } else {
    edge e;
    forEach(e, graph.getEdges()) {
      prop.setEdgeValue(e, (int) *values);
      ++values;
    }
  }
  return true;
}

// function to get the result in a LayoutProperty
bool tlp::getGpuOutPropertyValues(LayoutProperty& prop, const Graph& graph) {
  GpuValueType type;
  // set property values with result ones
  float* values =
    (float *) ::getGpuOutPropertyValues(3 * sizeof(float),
					graph.numberOfNodes(),
					graph.numberOfEdges(), type);
  if (!values)
    return false;
  if (type == NODE_VALUES) {
    node n;
    forEach(n, graph.getNodes()) {
      Coord coord;
      coord.setX(*values);
      ++values;
      coord.setY(*values);
      ++values;
      coord.setZ(*values);
      ++values;
      prop.setNodeValue(n, coord);
    }
  }
  // nothing to to for edges
  return true;
}

// function to get the result in a SizeProperty
bool tlp::getGpuOutPropertyValues(SizeProperty& prop, const Graph& graph) {
  GpuValueType type;
  // set property values with result ones
  float* values =
    (float *) ::getGpuOutPropertyValues(3 * sizeof(float),
					graph.numberOfNodes(),
					graph.numberOfEdges(), type);
  if (!values)
    return false;
  if (type == NODE_VALUES) {
    node n;
    forEach(n, graph.getNodes()) {
      Size size;
      size.setW(*values);
      ++values;
      size.setH(*values);
      ++values;
      size.setD(*values);
      ++values;
      prop.setNodeValue(n, size);
    }
  } else {
    edge e;
    forEach(e, graph.getEdges()) {
      Size size;
      size.setW(*values);
      ++values;
      size.setH(*values);
      ++values;
      size.setD(*values);
      ++values;
      prop.setEdgeValue(e, size);
    }
  }
  return true;
}
