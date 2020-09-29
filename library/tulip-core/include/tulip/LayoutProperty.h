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

#ifndef TULIP_LAYOUT_H
#define TULIP_LAYOUT_H

#include <tulip/PropertyTypes.h>
#include <tulip/Observable.h>
#include <tulip/AbstractProperty.h>
#include <tulip/PropertyAlgorithm.h>
#include <tulip/minmaxproperty.h>

namespace tlp {

template <>
void tlp::MinMaxProperty<tlp::PointType, tlp::LineType>::updateEdgeValue(
    tlp::edge e, tlp::LineType::RealType newValue);

template <>
std::pair<tlp::Coord, tlp::Coord>
tlp::MinMaxProperty<tlp::PointType, tlp::LineType>::computeMinMaxNode(const Graph *sg);

class PropertyContext;
class Graph;

typedef AbstractProperty<tlp::PointType, tlp::LineType> AbstractLayoutProperty;
typedef MinMaxProperty<tlp::PointType, tlp::LineType> LayoutMinMaxProperty;

/**
 * @ingroup Graph
 * @brief A graph property that maps a tlp::Coord value to graph nodes and std::vector<tlp::Coord>
 * for edges.
 */
class TLP_SCOPE LayoutProperty : public LayoutMinMaxProperty {
public:
  LayoutProperty(Graph *graph, const std::string &name = "" /*, bool updateOnEdgeReversal=true*/);

  // override some PropertyInterface methods
  PropertyInterface *clonePrototype(Graph *, const std::string &) const override;
  static const std::string propertyTypename;
  const std::string &getTypename() const override {
    return propertyTypename;
  }
  DEFINE_GET_CPP_CLASS_NAME;

  //=======================================
  // Methods for extra layout information
  //=======================================

  /**
   * Returns the maximum coordinate of the layout, i.e. the top-right corner of the induced bounding
   *box
   *
   * @param subgraph If not null, returns the maximum coordinate for a subgraph layout
   **/
  Coord getMax(const Graph *subgraph = nullptr);

  /**
   * Returns the minimum coordinate of the layout, i.e. the bottom-left corner of the induced
   *bounding box
   *
   * @param subgraph If not null, returns the minimum coordinate for a subgraph layout
   **/
  Coord getMin(const Graph *subgraph = nullptr);

  //============================================
  // Functions for layout modification
  //============================================

  /**
   * Translates the current layout according to a movement vector
   *
   * @param move a movement vector
   * @param subgraph If not null, only translates the layout of that subgraph
   **/
  void translate(const Vec3f &move, const Graph *subgraph = nullptr);

  /**
   * Translates the layout of a set of nodes and edges provided through iterators
   * according to a movement vector
   *
   * @param move a movement vector
   * @param itNodes an iterator on graph nodes
   * @param itEdges an iterator on graph edges
   *
   * @warning The iterators are not deleted
   **/
  void translate(const Vec3f &move, Iterator<node> *itNodes, Iterator<edge> *itEdges);

  /**
   * Scales the current layout according to a vector of scale factors (sx, sy, sz).
   *
   * @param scaleFactors a vector of scale factors
   * @param subgraph If not null, only scales the layout of that subgraph
   **/
  void scale(const Vec3f &scaleFactors, const Graph *subgraph = nullptr);

  /**
   * Scales the layout of a set of nodes and edges provided through iterators
   * according to a vector of scale factors (sx, sy, sz).
   *
   * @param scaleFactors a vector of scale factors
   * @param itNodes an iterator on graph nodes
   * @param itEdges an iterator on graph edges
   *
   * @warning The iterators are not deleted
   **/
  void scale(const Vec3f &scaleFactors, Iterator<node> *itNodes, Iterator<edge> *itEdges);

  /**
   * Rotates the layout around the X-axis according to an angle in degrees.
   *
   * @param alpha an angle in degrees
   * @param subgraph If not null, only rotates the layout of that subgraph
   **/
  void rotateX(const double &alpha, const Graph *subgraph = nullptr);

  /**
   * Rotates the layout around the Y-axis according to an angle in degrees.
   *
   * @param alpha an angle in degrees
   * @param subgraph If not null, only rotates the layout of that subgraph
   **/
  void rotateY(const double &alpha, const Graph *subgraph = nullptr);

  /**
   * Rotates the layout around the Z-axis according to an angle in degrees.
   *
   * @param alpha an angle in degrees
   * @param subgraph If not null, only rotates the layout of that subgraph
   **/
  void rotateZ(const double &alpha, const Graph *subgraph = nullptr);

  /**
   * Rotates the layout around the X-axis of the nodes and edges provided
   * through iterators according to an angle in degrees.
   *
   * @param alpha an angle in degrees
   * @param itNodes an iterator on graph nodes
   * @param itEdges an iterator on graph edges
   *
   * @warning The iterators are not deleted
   **/
  void rotateX(const double &alpha, Iterator<node> *itNodes, Iterator<edge> *itEdges);

  /**
   * Rotates the layout around the Y-axis of the nodes and edges provided
   * through iterators according to an angle in degrees.
   *
   * @param alpha an angle in degrees
   * @param itNodes an iterator on graph nodes
   * @param itEdges an iterator on graph edges
   *
   * @warning The iterators are not deleted
   **/
  void rotateY(const double &alpha, Iterator<node> *itNodes, Iterator<edge> *itEdges);

  /**
   * Rotates the layout around the Z-axis of the nodes and edges provided through
   * iterators according to an angle in degrees.
   *
   * @param alpha an angle in degrees
   * @param itNodes an iterator on graph nodes
   * @param itEdges an iterator on graph edges
   *
   * @warning The iterators are not deleted
   **/
  void rotateZ(const double &alpha, Iterator<node> *itNodes, Iterator<edge> *itEdges);

  /**
   * Centers the layout, meaning translating it in order that
   * the center of its bounding box is (0,0,0)
   *
   * @param subgraph If not null, only centers the layout of that subgraph
   **/
  void center(const Graph *subgraph = nullptr);

  /**
   * Centers the layout to newCenter, meaning translating it in order that
   * the center of its bounding box is equal to newCenter
   *
   * @param newCenter the coordinate of the new layout center
   * @param subgraph If not null, only centers the layout of that subgraph
   **/
  void center(const Vec3f &newCenter, const Graph *subgraph = nullptr);

  /**
   * Normalizes the layout, meaning dividing each nodes and edges coordinate by the maximum
   *magnitude of the whole coordinates set
   *
   * @param subgraph If not null, only normalizes the layout of that subgraph
   **/
  void normalize(const Graph *subgraph = nullptr);

  /**
   * Scales the layout in order to approach an aspect ratio (width / height) of 1.0 .
   * @param subgraph If not null, only scales the layout of that subgraph
   **/
  void perfectAspectRatio(const Graph *subgraph = nullptr);

  //=======================================================================
  // Set of function in order to measure the quality of the LayoutAlgorithm
  //=======================================================================

  /**
   * Returns the length of an edge, the bends are taken into account.
   * Thus, it measure the length of a polyline.
   *
   * @param e the graph edge on which to compute its length
   *
   * @warning this function only measure the length of the polyline between bends,
   * when using curves like Bézier etc... the result will not be the length of the curve.
   **/
  double edgeLength(const edge e) const;

  /**
   * Returns the average edge length of the layout, the bends are taken into account
   *
   * @param subgraph If not null, only compute the average edge length for that subgraph
   **/
  double averageEdgeLength(const Graph *subgraph = nullptr) const;

  /**
   * Returns the average angular resolution of the layout.
   * It is only defined for 2D drawing, meaning the third coordinate
   * is omitted
   *
   * @param subgraph It not null, only computes the average angular resolution for that subgraph
   **/
  double averageAngularResolution(const Graph *subgraph = nullptr) const;

  /**
   * Returns the average angular resolution of a node.
   * It is only defined for 2D drawing, meaning the third coordinate
   * is omitted
   *
   * @param n the graph node on which to compute the angular resolution
   * @param subgraph If not null, only computes the average angular resolution for the node in that
   *subgraph
   **/
  double averageAngularResolution(const node n, const Graph *subgraph = nullptr) const;

  /**
   * Returns a vector of all angular resolution of a node.
   * It is only defined for 2D drawing, meaning the third coordinate
   * is omitted
   *
   * @param n the graph node on which to compute the angular resolution
   * @param subgraph If not null, only computes the average angular resolution for the node in that
   *subgraph
   **/
  std::vector<double> angularResolutions(const node n, const Graph *subgraph = nullptr) const;

  /**
   * Fixes embedding of the graph according to the layout
   * ie. sort edges around nodes according to their neighbors/bends position in the layout/
   * Only works in 2D, the third coordinate is not taken into account.
   *
   * @param subgraph It not null, only fixes embedding in that subgraph
   **/
  void computeEmbedding(Graph *subgraph = nullptr);

  /**
   * Fixes embedding of the node according to the layout
   * ie. sort edges around the node according to its neighbors/bends position in the layout/
   * Only work in 2D, the third coordinate is not taken into account.
   *
   * @param n the graph node on which to fix embedding
   * @param subgraph If not null, only fixes the embedding of the node in that subgraph
   **/
  void computeEmbedding(const node n, Graph *subgraph = nullptr);

  /**
   * Returns the number of crossings in the layout
   **/
  // methods removed until we have a working implementation
  // unsigned int crossingNumber() const;

  // redefinition of some AbstractProperty methods
  void setNodeValue(const node, tlp::StoredType<Coord>::ReturnedConstValue v) override;
  void setEdgeValue(const edge, tlp::StoredType<std::vector<Coord>>::ReturnedConstValue v) override;
  void setAllNodeValue(tlp::StoredType<Coord>::ReturnedConstValue v) override;
  void setValueToGraphNodes(tlp::StoredType<Coord>::ReturnedConstValue v,
                            const Graph *graph) override;
  void setAllEdgeValue(tlp::StoredType<std::vector<Coord>>::ReturnedConstValue v) override;
  void setValueToGraphEdges(tlp::StoredType<std::vector<Coord>>::ReturnedConstValue v,
                            const Graph *graph) override;

protected:
  void clone_handler(AbstractProperty<tlp::PointType, tlp::LineType> &) override;

private:
  void resetBoundingBox();
  void rotate(const double &alpha, int rot, Iterator<node> *, Iterator<edge> *);
  // override Observable::treatEvent
  void treatEvent(const Event &) override;

public:
  // the number of edges with bends
  unsigned int nbBendedEdges;
};

/**
 * @ingroup Graph
 * @brief A graph property that maps a std::vector<tlp::Coord> value to graph elements.
 */
class TLP_SCOPE CoordVectorProperty
    : public AbstractVectorProperty<tlp::CoordVectorType, tlp::PointType> {
public:
  CoordVectorProperty(Graph *g, const std::string &n = "")
      : AbstractVectorProperty<CoordVectorType, tlp::PointType>(g, n) {}
  // redefinition of some PropertyInterface methods
  PropertyInterface *clonePrototype(Graph *, const std::string &) const override;
  static const std::string propertyTypename;
  const std::string &getTypename() const override {
    return propertyTypename;
  }
  DEFINE_GET_CPP_CLASS_NAME;
};

typedef CoordVectorProperty LayoutVectorProperty;
} // namespace tlp
#endif
