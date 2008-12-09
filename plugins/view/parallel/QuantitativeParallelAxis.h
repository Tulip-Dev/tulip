//-*-c++-*-
/*
 Author: Didier Bathily, Nicolas Bellino, Jonathan Dubois, Christelle Jolly, Antoine Lambert, Nicolas Sorraing

 Email : didier.bathily@etu.u-bordeaux1.fr, nicolas.bellino@etu.u-bordeaux1.fr, jonathan.dubois@etu.u-bordeaux1.fr, christelle.jolly@etu.u-bordeaux1.fr, antoine.lambert@etu.u-bordeaux1.fr, nicolas.sorraing@etu.u-bordeaux1.fr

 Last modification : 03/08

 This program is free software; you can redistribute it and/or modify  *
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
*/

#ifndef QUANTITATIVE_PARALLEL_AXIS
#define QUANTITATIVE_PARALLEL_AXIS

#ifndef DOXYGEN_NOTFOR_DEVEL

#include "ParallelAxis.h"
#include "ParallelTools.h"
#include "ParallelCoordinatesGraphProxy.h"
#include <tulip/Color.h>

#include <vector>
#include <algorithm>
#include <string>

#include <tulip/Iterator.h>

const unsigned int DEFAULT_NB_AXIS_GRAD = 20;

namespace tlp {

// Class which allows to render a quantitative axis
// Associated datatypes can be real or integer
class TLP_GL_SCOPE QuantitativeParallelAxis : public ParallelAxis {

public :

  QuantitativeParallelAxis(const Coord &baseCoord, const float height, const float axisAreaWidth, ParallelCoordinatesGraphProxy *graphProxy, const std::string &graphPropertyName, const bool ascendingOrder = true, const Color &axisColor = Color(0,0,0));
  void setNbAxisGrad(const unsigned int nbAxisGrad) {this->nbAxisGrad = nbAxisGrad;}
  unsigned int getNbAxisGrad() const {return nbAxisGrad;}
  void translate(const Coord &c);
  Coord getPointCoordOnAxisForData(const unsigned int dataIdx);
  void redraw();
  void showConfigDialog();
  std::string getAxisDataTypeName() const;
  void setLog10Scale(const bool log10Scale) {this->log10Scale = log10Scale;}
  bool hasLog10Scale() const {return log10Scale;}

  template<typename PROPERTY, typename PROPERTYTYPE>
  typename PROPERTYTYPE::RealType getAxisMinValue();

  template<typename PROPERTY, typename PROPERTYTYPE>
  typename PROPERTYTYPE::RealType getAxisMaxValue();

  template<typename PROPERTY, typename PROPERTYTYPE>
  typename PROPERTYTYPE::RealType getValueForAxisCoord(const Coord &axisCoord);

  std::string getTopSliderTextValue();
  std::string getBottomSliderTextValue();

  std::set<unsigned int> getDataInSlidersRange();
  void updateSlidersWithDataSubset(const std::set<unsigned int> &dataSubset);

  bool hasAscendindOrder() const {return ascendingOrder;}
  void setAscendindOrder(const bool ascendingOrder);

 private:

  void setLabelsAndComputeDataCoords();

  template<typename PROPERTY, typename PROPERTYTYPE>
  void computeDataPointsCoord();

  template<typename PROPERTY, typename PROPERTYTYPE>
  void setLabels();

  template<typename PROPERTY, typename PROPERTYTYPE>
  void computeDataPointsCoordWithLog10scale();

  template<typename PROPERTY, typename PROPERTYTYPE>
  void setLabelsWithLog10Scale();

  void showAxisConfigDialog();


  double scale;
  bool ascendingOrder;
  unsigned int nbAxisGrad;
  ParallelCoordinatesGraphProxy *graphProxy;
  std::map<unsigned int, Coord> dataCoords;
  bool log10Scale;

};

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif
