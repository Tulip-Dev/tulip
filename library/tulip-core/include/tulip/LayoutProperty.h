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

#ifndef TULIP_LAYOUT_H
#define TULIP_LAYOUT_H

#include <tulip/tuliphash.h>
#include <tulip/PropertyTypes.h>
#include <tulip/Observable.h>
#include <tulip/AbstractProperty.h>
#include <tulip/PropertyAlgorithm.h>
#include <tulip/minmaxproperty.h>

namespace tlp {

template <>
void tlp::MinMaxProperty<tlp::PointType, tlp::LineType>::updateEdgeValue(tlp::edge e, tlp::LineType::RealType newValue);

template <>
MINMAX_PAIR(tlp::PointType) tlp::MinMaxProperty<tlp::PointType, tlp::LineType>::computeMinMaxNode(Graph *sg);

class PropertyContext;
class Graph;

typedef AbstractProperty<tlp::PointType, tlp::LineType> AbstractLayoutProperty;
typedef MinMaxProperty<tlp::PointType, tlp::LineType> LayoutMinMaxProperty;

/**
 * @ingroup Graph
 * @brief A graph property that maps a tlp::Coord value to graph nodes and std::vector<tlp::Coord> for edges.
 */
class TLP_SCOPE LayoutProperty : public LayoutMinMaxProperty {
public:
  LayoutProperty(Graph *graph, const std::string& name=""/*, bool updateOnEdgeReversal=true*/);

  // override some PropertyInterface methods
  PropertyInterface* clonePrototype(Graph *, const std::string& );
  static const std::string propertyTypename;
  const std::string& getTypename() const {
    return propertyTypename;
  }

  //=======================================
  //Methods for extra layout information
  //=======================================

  /**
  * Returns the maximum coordinate of the layout, i.e. the top-right corner of the induced bounding box
  *
  * @param subgraph If not null, returns the maximum coordinate for a sub-graph layout
  **/
  Coord getMax(Graph *subgraph=NULL);

  /**
  * Returns the minimum coordinate of the layout, i.e. the bottom-left corner of the induced bounding box
  *
  * @param subgraph If not null, returns the minimum coordinate for a sub-graph layout
  **/
  Coord getMin(Graph *subgraph=NULL);

  //============================================
  //Functions for layout modification
  //============================================

  /**
  * Translates the current layout according to a movement vector
  *
  * @param move a movement vector
  * @param subgraph If not null, only translates the layout of that sub-graph
  **/
  void translate(const Vec3f &move, Graph *subgraph=NULL);

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
  * @param subgraph If not null, only scales the layout of that sub-graph
  **/
  void scale(const Vec3f &scaleFactors, Graph *subgraph=NULL);

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
  * @param subgraph If not null, only rotates the layout of that sub-graph
  **/
  void rotateX(const double &alpha, Graph *subgraph=NULL);

  /**
  * Rotates the layout around the Y-axis according to an angle in degrees.
  *
  * @param alpha an angle in degrees
  * @param subgraph If not null, only rotates the layout of that sub-graph
  **/
  void rotateY(const double &alpha, Graph *subgraph=NULL);

  /**
  * Rotates the layout around the Z-axis according to an angle in degrees.
  *
  * @param alpha an angle in degrees
  * @param subgraph If not null, only rotates the layout of that sub-graph
  **/
  void rotateZ(const double &alpha, Graph *subgraph=NULL);

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
  * @param subgraph If not null, only centers the layout of that sub-graph
  **/
  void center(Graph *subgraph=NULL);

  /**
  * Centers the layout to newCenter, meaning translating it in order that
  * the center of its bounding box is equal to newCenter
  *
  * @param newCenter the coordinate of the new layout center
  * @param subgraph If not null, only centers the layout of that sub-graph
  **/
  void center(const Vec3f &newCenter, Graph *subgraph=NULL);

  /**
  * Normalizes the layout, meaning dividing each nodes and edges coordinate by the maximum magnitude of the whole coordinates set
  *
  * @param subgraph If not null, only normalizes the layout of that sub-graph
  **/
  void normalize(Graph *subgraph=NULL);

  /**
  * Scales the layout in order to approach an aspect ratio (width / height) of 1.0 .
  **/
  void perfectAspectRatio();

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
  * @param subgraph If not null, only compute the average edge length for that sub-graph
  **/
  double averageEdgeLength(const Graph *subgraph=NULL) const;

  /**
  * Returns the average angular resolution of the layout.
  * It is only defined for 2D drawing, meaning the third coordinate
  * is omitted
  *
  * @param subgraph It not null, only computes the average angular resolution for that sub-graph
  **/
  double averageAngularResolution(const Graph *subgraph=NULL) const;

  /**
  * Returns the average angular resolution of a node.
  * It is only defined for 2D drawing, meaning the third coordinate
  * is omitted
  *
  * @param n the graph node on which to compute the angular resolution
  * @param subgraph If not null, only computes the average angular resolution for the node in that sub-graph
  **/
  double averageAngularResolution(const node n, const Graph *subgraph=NULL) const;

  /**
  * Returns a vector of all angular resolution of a node.
  * It is only defined for 2D drawing, meaning the third coordinate
  * is omitted
  *
  * @param n the graph node on which to compute the angular resolution
  * @param subgraph If not null, only computes the average angular resolution for the node in that sub-graph
  **/
  std::vector<double> angularResolutions(const node n, const Graph *subgraph=NULL) const;

  /**
  * Fixes embedding of the graph according to the layout
  * ie. sort edges around nodes according to their neighbors/bends position in the layout/
  * Only works in 2D, the third coordinate is not taken into account.
  *
  * @param subgraph It not null, only fixes embedding in that sub-graph
  **/
  void computeEmbedding(Graph *subgraph=NULL);

  /**
  * Fixes embedding of the node according to the layout
  * ie. sort edges around the node according to its neighbors/bends position in the layout/
  * Only work in 2D, the third coordinate is not taken into account.
  *
  * @param n the graph node on which to fix embedding
  * @param subgraph If not null, only fixes the embedding of the node in that sub-graph
  **/
  void computeEmbedding(const node n, Graph *subgraph=NULL);

  /**
  * Returns the number of crossings in the layout
  **/
  //methods removed until we have a working implementation
  //unsigned int crossingNumber() const;

  // redefinition of some AbstractProperty methods
  virtual void setNodeValue(const node, const Coord &v);
  virtual void setEdgeValue(const edge, const std::vector<Coord> &v);
  virtual void setAllNodeValue(const Coord &v);
  virtual void setAllEdgeValue(const std::vector<Coord> &v);

protected:
  virtual void clone_handler(AbstractProperty<tlp::PointType, tlp::LineType> &);

private:
  void resetBoundingBox();
  void rotate(const double& alpha, int rot, Iterator<node> *, Iterator<edge> *);
  // override Observable::treatEvent
  void treatEvent(const Event&);

public:
  // the number of edges with bends
  unsigned int nbBendedEdges;
};

/**
 * @ingroup Graph
 * @brief A graph property that maps a std::vector<tlp::Coord> value to graph elements.
 */
class TLP_SCOPE CoordVectorProperty:public AbstractVectorProperty<tlp::CoordVectorType, tlp::PointType> {
public :
  CoordVectorProperty(Graph *g, const std::string& n=""):AbstractVectorProperty<CoordVectorType, tlp::PointType>(g, n) {}
  // redefinition of some PropertyInterface methods
  PropertyInterface* clonePrototype(Graph *, const std::string& );
  static const std::string propertyTypename;
  const std::string& getTypename() const {
    return propertyTypename;
  }

};

typedef CoordVectorProperty LayoutVectorProperty;



}
#endif
