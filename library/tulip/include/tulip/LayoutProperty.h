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
#ifndef TULIP_LAYOUT_H
#define TULIP_LAYOUT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/tuliphash.h"
#include "tulip/PropertyTypes.h"
#include "tulip/ObservableGraph.h"
#include "tulip/AbstractProperty.h"
#include "tulip/LayoutAlgorithm.h"

namespace tlp {

class PropertyContext;
class Graph;

typedef AbstractProperty<tlp::PointType, tlp::LineType, tlp::LayoutAlgorithm> AbstractLayoutProperty;

/** \addtogroup properties */ 
/*@{*/
  class TLP_SCOPE LayoutProperty:public AbstractLayoutProperty, public GraphObserver {
  friend class LayoutAlgorithm;

public:
  LayoutProperty(Graph *, std::string n="", bool updateOnEdgeReversal = true);
  ~LayoutProperty();

  // redefinition of some PropertyInterface methods
  PropertyInterface* clonePrototype(Graph *, const std::string& );
  std::string getTypename() const {
    return "layout";
  }

  //=======================================
  //Functions for extra layout information
  Coord getMax(Graph *graph=0);
  Coord getMin(Graph *graph=0);

  //============================================
  //Functions for layout modification
  void translate(const tlp::Vector<float,3>& v, Graph *graph=0);
  /*
   * translate a set of nodes and edges. Warning : the iterators are not deleted.
   */
  void translate(const tlp::Vector<float,3>& v, Iterator<node> *, Iterator<edge> *);
  void scale(const tlp::Vector<float,3>& v, Graph *graph=0);
  void scale(const tlp::Vector<float,3>& v, Iterator<node> *, Iterator<edge> *);
  /**
   * Rotates the layout proxy of the passed graph by alpha degrees
   */
  void rotateZ(const double& alpha, Graph *graph=0);
  /**
   * Rotates the passed nodes and edges in the given layout proxy by
   * alpha degrees
   */
  void rotateX(const double& alpha, Iterator<node> *, Iterator<edge> *);
  void rotateY(const double& alpha, Iterator<node> *, Iterator<edge> *);
  void rotateZ(const double& alpha, Iterator<node> *, Iterator<edge> *);
  void center(Graph *graph = 0);
  void normalize(Graph *graph = 0);
  void perfectAspectRatio();
  void resetBoundingBox();
  //=======================================================================
  // Set of function in order to measure the quality of the LayoutAlgorithm
  //======================================================================
  /**
   * Returns the length of an edge, the bends are taken into account.
   * Thus, it measure the length of a polyline. Warning this function 
   * only measure the length of the polyline between bends, when using
   * curves like bezier etc... the result will not be the length of the curve.
   */
  double edgeLength(const edge e) const;
  /**
   * Returns the average angular resolution of the layout
   * it is defined for 2D drawing, the third coordinates
   * is omitted
   */
  double averageAngularResolution(const Graph *graph = 0) const;
  /**
   * Returns the average angular resolution of a node
   * it is defined for 2D drawing, the third coordinates
   * is omitted
   */
  double averageAngularResolution(const node n,const Graph *graph = 0) const;
  /**
   * Returns a vector of all angular resolution of a node
   * it is defined for 2D drawing, the third coordinates
   * is omitted
   */
  std::vector<double> angularResolutions(const node n, const Graph *graph = 0) const;


  /**
   * Fix embedding of the graph according to the layout
   * ie. sort edges around nodes according to their neighbors/bends position in the layout/
   * Only work in 2D, the third coordinate is not taken into account.
   */
  void computeEmbedding(Graph *sg = 0);
  /**
   * Fix embedding of the node according to the layout
   * ie. sort edges around the node according to its neighbors/bends position in the layout/
   * Only work in 2D, the third coordinate is not taken into account.
   */
  void computeEmbedding(const node n, Graph *sg = 0);
  /**
   * Returns the number of crossings in the layout
   */
  unsigned int crossingNumber() const;

  // redefinition of some AbstractProperty methods
  virtual void setNodeValue(const node, const Coord &v);
  virtual void setEdgeValue(const edge, const std::vector<Coord> &v);
  virtual void setAllNodeValue(const Coord &v);
  virtual void setAllEdgeValue(const std::vector<Coord> &v);

  // redefinition of a GraphObserver method
  virtual void reverseEdge(Graph *, const edge);

protected:
  virtual void clone_handler(AbstractProperty<PointType,LineType, LayoutAlgorithm> &);

private:
  TLP_HASH_MAP<unsigned int, Coord> max,min;
  TLP_HASH_MAP<unsigned int, bool> minMaxOk;
  void computeMinMax(Graph * graph=NULL);
  void rotate(const double& alpha, int rot, Iterator<node> *, Iterator<edge> *);
};

class TLP_SCOPE CoordVectorProperty:public AbstractVectorProperty<tlp::CoordVectorType, tlp::PointType> { 
public :
  CoordVectorProperty(Graph *g, std::string n=""):AbstractVectorProperty<CoordVectorType, tlp::PointType>(g, n) {}
  // redefinition of some PropertyInterface methods
  PropertyInterface* clonePrototype(Graph *, const std::string& );
  std::string getTypename() const {
    return "vector<coord>";
  }
};

/*@}*/

}
#endif
