//-*-c++-*-
#ifndef TULIP_LAYOUT_H
#define TULIP_LAYOUT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "tulip/AbstractProperty.h"
#include "tulip/TemplateFactory.h"
#include "tulip/LayoutAlgorithm.h"
#include "tulip/Types.h"

namespace tlp {

class PropertyContext;
class Graph;

/** \addtogroup properties */ 
/*@{*/
class TLP_SCOPE LayoutProperty:public AbstractProperty<PointType, LineType, LayoutAlgorithm> {
  friend class LayoutAlgorithm;

public:
  LayoutProperty (Graph *);

  PropertyInterface* clonePrototype(Graph *, std::string );
  void copy( const node, const node, PropertyInterface * );
  void copy( const edge, const edge, PropertyInterface * );

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
  double edgeLength(edge e);
  /**
   * Returns the average angular resolution of the layout
   * it is defined for 2D drawing, the third coordinates
   * is omitted
   */
  double averageAngularResolution(Graph *graph=0);
  /**
   * Returns the average angular resolution of a node
   * it is defined for 2D drawing, the third coordinates
   * is omitted
   */
  double averageAngularResolution(const node n,Graph *graph=0);
  /**
   * Returns a vector of all angular resolution of a node
   * it is defined for 2D drawing, the third coordinates
   * is omitted
   */
  std::vector<double> angularResolutions(const node n, Graph *graph=0);
  /**
   * Returns the number of crossings in the layout
   */
  unsigned int crossingNumber();

protected:
  void clone_handler(AbstractProperty<PointType,LineType> &);
  virtual void setNodeValue_handler(const node n, const PointType::RealType &);
  virtual void setEdgeValue_handler(const edge e, const LineType::RealType &);
  virtual void setAllNodeValue_handler(const PointType::RealType &);
  virtual void setAllEdgeValue_handler(const LineType::RealType &);

private:
  stdext::hash_map<unsigned long,Coord> max,min;
  stdext::hash_map<unsigned long,bool> minMaxOk;
  void computeMinMax(Graph * graph=NULL);
  void rotate(const double& alpha, int rot, Iterator<node> *, Iterator<edge> *);
};
/*@}*/

}
#endif
