//-*-c++-*-
#ifndef TULIP_LAYOUT_H
#define TULIP_LAYOUT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "AbstractProperty.h"
#include "TemplateFactory.h"
#include "LayoutAlgorithm.h"
#include "Types.h"

class PropertyContext;
class Graph;

/** \addtogroup properties */ 
/*@{*/
class TLP_SCOPE LayoutProperty:public AbstractProperty<PointType, LineType, LayoutAlgorithm> {
  friend class LayoutAlgorithm;

public:
  LayoutProperty (Graph *);
  ~LayoutProperty();

  PropertyInterface* clonePrototype(Graph *, std::string );
  void copy( const node, const node, PropertyInterface * );
  void copy( const edge, const edge, PropertyInterface * );

  //=======================================
  //Functions for extra layout information
  Coord getMax(Graph *sg=0);
  Coord getMin(Graph *sg=0);

  //============================================
  //Functions for layout modification
  void translate(const tlp::Vector<float,3>& v, Graph *sg=0);
  /*
   * translate a set of nodes and edges. Warning : the iterators are not deleted.
   */
  void translate(const tlp::Vector<float,3>& v, Iterator<node> *, Iterator<edge> *);
  void scale(const tlp::Vector<float,3>& v, Graph *sg=0);
  void scale(const tlp::Vector<float,3>& v, Iterator<node> *, Iterator<edge> *);
  void rotateZ(const double& alpha, Graph *sg=0);
  void rotateZ(const double& alpha, Iterator<node> *, Iterator<edge> *);
  void center(Graph *sg = 0);
  void normalize(Graph *sg = 0);
  void perfectAspectRatio();
  void resetBoundingBox();
  //=======================================================================
  // Set of function in order to measure the quality of the LayoutAlgorithm
  //======================================================================
  /**
   * Return the length of an edge, the bends are taken into account.
   * Thus, it measure the length of a polyline. Warning this function 
   * only measure the length of the polyline between bends, when using
   * curves like bezier etc... the result will not be the length of the curve.
   */
  double edgeLength(edge e);
  /**
   * Return the average angular resolution of the layout
   * it is defined for 2D drawing, the third coordinates
   * is omitted
   */
  double averageAngularResolution(Graph *sg=0);
  /**
   * Return the average angular resolution of a node
   * it is defined for 2D drawing, the third coordinates
   * is omitted
   */
  double averageAngularResolution(const node n,Graph *sg=0);
  /**
   * Return a vector of all angular resolution of a node
   * it is defined for 2D drawing, the third coordinates
   * is omitted
   */
  std::vector<double> angularResolutions(const node n, Graph *sg=0);
  /**
   * Return the number of crossings in the layout
   */
  unsigned int crossingNumber();

protected:
  void recompute_handler();
  void reset_handler();
  void clone_handler(AbstractProperty<PointType,LineType> &);
  virtual void setNodeValue_handler(const node n, const PointType::RealType &);
  virtual void setEdgeValue_handler(const edge e, const LineType::RealType &);
  virtual void setAllNodeValue_handler(const PointType::RealType &);
  virtual void setAllEdgeValue_handler(const LineType::RealType &);

private:
  stdext::hash_map<unsigned long,Coord> max,min;
  stdext::hash_map<unsigned long,bool> minMaxOk;
  void computeMinMax(Graph * sg=NULL);
};
/*@}*/
#endif
