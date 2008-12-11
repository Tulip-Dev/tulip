//-*-c++-*-
/*
 Author: Antoine Lambert

 Email : antoine.lambert@labri.fr

 Last modification : 12/08

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


enum BoxPlotValue {BOTTOM_OUTLIER = 0, FIRST_QUARTILE = 1, MEDIAN = 2, THIRD_QUARTILE = 3, TOP_OUTLIER = 4, NO_VALUE = 5};

// Class which allows to render a quantitative axis
// Associated datatypes can be real or integer
class QuantitativeParallelAxis : public ParallelAxis {

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

  template<typename PROPERTY, typename PROPERTYTYPE>
  Coord getAxisCoordForValue(typename PROPERTYTYPE::RealType value);

  std::string getTopSliderTextValue();
  std::string getBottomSliderTextValue();

  std::set<unsigned int> getDataInSlidersRange();
  void updateSlidersWithDataSubset(const std::set<unsigned int> &dataSubset);

  void setBoxPlotHighlightBounds(BoxPlotValue lowBound, BoxPlotValue highBound) {boxPlotLowBound = lowBound ; boxPlotHighBound = highBound;}
  std::set<unsigned int> getDataBetweenBoxPlotBounds();

  bool hasAscendindOrder() const {return ascendingOrder;}
  void setAscendindOrder(const bool ascendingOrder);

  // Axis BoxPlot methods
  Coord getBottomOutlierCoord() const {return boxPlotValuesCoord[BOTTOM_OUTLIER];}
  Coord getFirstQuartileCoord() const {return boxPlotValuesCoord[FIRST_QUARTILE];}
  Coord getMedianCoord() const {return boxPlotValuesCoord[MEDIAN];}
  Coord getThirdQuartileCoord() const {return boxPlotValuesCoord[THIRD_QUARTILE];}
  Coord getTopOutlierCoord() const {return boxPlotValuesCoord[TOP_OUTLIER];}
  std::string getBottomOutlierStringValue() const {return boxPlotStringValues[BOTTOM_OUTLIER];}
  std::string getFirstQuartileStringValue() const {return boxPlotStringValues[FIRST_QUARTILE];}
  std::string getMedianStringValue() const {return boxPlotStringValues[MEDIAN];}
  std::string getThirdQuartileStringValue() const {return boxPlotStringValues[THIRD_QUARTILE];}
  std::string getTopOutlierStringValue() const {return boxPlotStringValues[TOP_OUTLIER];}

 private:

  void setLabelsAndComputeDataCoords();

  std::set<unsigned int> getDataInRange(float yLowBound, float yHighBound);

  template<typename PROPERTY, typename PROPERTYTYPE>
  void computeDataPointsCoord();

  template<typename PROPERTY, typename PROPERTYTYPE>
  void setLabels();

  template<typename PROPERTY, typename PROPERTYTYPE>
  void computeDataPointsCoordWithLog10scale();

  template<typename PROPERTY, typename PROPERTYTYPE>
  void setLabelsWithLog10Scale();

  template<typename PROPERTY, typename PROPERTYTYPE>
  void computeBoxPlotCoords(std::set<typename PROPERTYTYPE::RealType> propertyValuesSet);

  void showAxisConfigDialog();


  double scale;
  bool ascendingOrder;
  unsigned int nbAxisGrad;
  ParallelCoordinatesGraphProxy *graphProxy;
  std::map<unsigned int, Coord> dataCoords;
  bool log10Scale;

  std::vector<Coord> boxPlotValuesCoord;
  std::vector<std::string> boxPlotStringValues;
  BoxPlotValue boxPlotLowBound, boxPlotHighBound;

};

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif
