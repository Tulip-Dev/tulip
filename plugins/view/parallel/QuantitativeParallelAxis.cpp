using namespace std;

#include "QuantitativeParallelAxis.h"
#include "AxisConfigDialogs.h"

#include <tulip/IntegerProperty.h>
#include <tulip/DoubleProperty.h>

#include <cmath>

namespace tlp {

QuantitativeParallelAxis::QuantitativeParallelAxis(const Coord &baseCoord, const float height, const float axisAreaWidth, ParallelCoordinatesGraphProxy *graphProxy,
																		   const string &graphPropertyName, const bool ascendingOrder, const Color &axisColor) :
  ParallelAxis(baseCoord, height, axisAreaWidth, graphPropertyName, axisColor), ascendingOrder(ascendingOrder),
  nbAxisGrad(DEFAULT_NB_AXIS_GRAD), graphProxy(graphProxy), log10Scale(false) {

	setLabelsAndComputeDataCoords();
}

void QuantitativeParallelAxis::setLabelsAndComputeDataCoords() {
	spaceBetweenAxisGrads = axisHeight / (nbAxisGrad - 1);
	if (getAxisDataTypeName() == "int") {
		if (!log10Scale) {
			setLabels<IntegerProperty, IntegerType> ();
			computeDataPointsCoord<IntegerProperty, IntegerType> ();
		} else {
			setLabelsWithLog10Scale<IntegerProperty, IntegerType> ();
			computeDataPointsCoordWithLog10scale<IntegerProperty, IntegerType> ();
		}
	} else if (getAxisDataTypeName() == "double") {
		if (!log10Scale) {
			setLabels<DoubleProperty, DoubleType> ();
			computeDataPointsCoord<DoubleProperty, DoubleType> ();
		} else {
			setLabelsWithLog10Scale<DoubleProperty, DoubleType> ();
			computeDataPointsCoordWithLog10scale<DoubleProperty, DoubleType> ();
		}
	}
}

template<typename PROPERTY, typename PROPERTYTYPE>
void QuantitativeParallelAxis::setLabels(){


      typename PROPERTYTYPE::RealType min = getAxisMinValue<PROPERTY, PROPERTYTYPE>();
      typename PROPERTYTYPE::RealType max = getAxisMaxValue<PROPERTY, PROPERTYTYPE>();

      if (min == max) {
          max += (nbAxisGrad / 2);
      }

      typename PROPERTYTYPE::RealType increment = (max - min) / nbAxisGrad;

      // if data are integer, we need to prevent a null increment
      if ((typeid(min) == typeid(int)) && increment == 0) {
    	  increment = 1;
      }

      scale = axisHeight / (double) (max - min);

	  typename PROPERTYTYPE::RealType label;
	  float labelCoord = baseCoord.getY();

	  if (ascendingOrder) {
	      label = min + increment;
	      addLabelDrawing(getStringFromNumber(min), (float) labelCoord);
	      addLabelDrawing(getStringFromNumber(max), (float) labelCoord + axisHeight);
	  } else {
	      label = max - increment;
	      addLabelDrawing(getStringFromNumber(max), (float) labelCoord);
	      addLabelDrawing(getStringFromNumber(min), (float) labelCoord + axisHeight);
	  }

	  for (float i = labelCoord + (increment * scale) ; i < (labelCoord + axisHeight) ; i += (increment * scale)) {
	      addLabelDrawing(getStringFromNumber(label), i);
	      if (ascendingOrder) {
	        label += increment;
	      } else {
	        label -= increment;
	      }
	  }
}

template<typename PROPERTY, typename PROPERTYTYPE>
void QuantitativeParallelAxis::setLabelsWithLog10Scale(){


      typename PROPERTYTYPE::RealType min = getAxisMinValue<PROPERTY, PROPERTYTYPE>();
      typename PROPERTYTYPE::RealType max = getAxisMaxValue<PROPERTY, PROPERTYTYPE>();

      if (min == max) {
    	  max += (nbAxisGrad / 2);
      }

      double logMin, logMax;
      if (min >= 1) {
    	  logMin = log10((double)min);
    	  logMax = log10((double)max);
      } else {
    	  logMin = 0;
    	  logMax = log10((double)((max - min) + 1));
      }

      double increment = (logMax - logMin) / nbAxisGrad;

      scale = axisHeight / (logMax - logMin);

	  double label;
	  float labelCoord = baseCoord.getY();

	  if (ascendingOrder) {
	      label = logMin + increment;
	      addLabelDrawing(getStringFromNumber(min), (float) labelCoord);
	      addLabelDrawing(getStringFromNumber(max), (float) labelCoord + axisHeight);
	  } else {
	      label = logMax - increment;
	      addLabelDrawing(getStringFromNumber(max), (float) labelCoord);
	      addLabelDrawing(getStringFromNumber(min), (float) labelCoord + axisHeight);
	  }

	  for (float i = labelCoord + (increment * scale) ; i < (labelCoord + axisHeight) ; i += (increment * scale)) {
		  double labelValue = pow(10, label);
		  if (min < 1) {
			  labelValue -= (1 - min);
		  }
	      addLabelDrawing(getStringFromNumber(labelValue), i);
	      if (ascendingOrder) {
	        label += increment;
	      } else {
	        label -= increment;
	      }
	  }
}

template<typename PROPERTY, typename PROPERTYTYPE>
void QuantitativeParallelAxis::computeDataPointsCoord() {

	typename PROPERTYTYPE::RealType min = getAxisMinValue<PROPERTY, PROPERTYTYPE>();
	typename PROPERTYTYPE::RealType max = getAxisMaxValue<PROPERTY, PROPERTYTYPE>();

	Iterator<unsigned int> *dataIt = graphProxy->getDataIterator();

	while (dataIt->hasNext()) {
		unsigned int dataId = dataIt->next();

		Coord c = Coord(baseCoord.getX(), 0, 0);
		typename PROPERTYTYPE::RealType propertyValue =
				graphProxy->getPropertyValueForData<PROPERTY, PROPERTYTYPE> (axisName, dataId);

		if (ascendingOrder)
			c.setY(baseCoord.getY() + (propertyValue - min) * scale);
		else
			c.setY(baseCoord.getY() + (max - propertyValue) * scale);

		dataCoords[dataId] = c;

	}

	delete dataIt;
}

template<typename PROPERTY, typename PROPERTYTYPE>
void QuantitativeParallelAxis::computeDataPointsCoordWithLog10scale() {

	typename PROPERTYTYPE::RealType min = getAxisMinValue<PROPERTY, PROPERTYTYPE>();
	typename PROPERTYTYPE::RealType max = getAxisMaxValue<PROPERTY, PROPERTYTYPE>();

	double logMin, logMax;
	if (min >= 1) {
		logMin = log10((double)min);
		logMax = log10((double)max);
	} else {
		logMin = 0;
		logMax = log10((double)((max - min) + 1));
	}

	Iterator<unsigned int> *dataIt = graphProxy->getDataIterator();

	while (dataIt->hasNext()) {
		unsigned int dataId = dataIt->next();

		Coord c = Coord(baseCoord.getX(), 0, 0);
		double propertyValue = (double) graphProxy->getPropertyValueForData<PROPERTY, PROPERTYTYPE> (axisName, dataId);
		if (min < 1) {
			propertyValue += (1 - min);
		}
		double logPropertyValue = log10(propertyValue);

		if (ascendingOrder)
			c.setY(baseCoord.getY() + (logPropertyValue - logMin) * scale);
		else
			c.setY(baseCoord.getY() + (logMax - logPropertyValue) * scale);

		dataCoords[dataId] = c;
	}

	delete dataIt;
}

template<typename PROPERTY, typename PROPERTYTYPE>
typename PROPERTYTYPE::RealType QuantitativeParallelAxis::getAxisMinValue() {
	if (graphProxy->getRootGraph() == graphProxy->getCurrentGraphInHierarchy()) {
		return graphProxy->getPropertyMinValue<PROPERTY, PROPERTYTYPE>(axisName);
	} else {
		Iterator<unsigned int> *dataIt = graphProxy->getDataIterator();
		unsigned int dataId = dataIt->next();
		typename PROPERTYTYPE::RealType min = graphProxy->getPropertyValueForData<PROPERTY, PROPERTYTYPE>(getAxisName(), dataId);
		while (dataIt->hasNext()) {
			dataId = dataIt->next();
			typename PROPERTYTYPE::RealType propertyValue = graphProxy->getPropertyValueForData<PROPERTY, PROPERTYTYPE>(getAxisName(), dataId);
			if (propertyValue < min) {
				min = propertyValue;
			}
		}
		delete dataIt;
		return min;
	}
}

template<typename PROPERTY, typename PROPERTYTYPE>
typename PROPERTYTYPE::RealType QuantitativeParallelAxis::getAxisMaxValue() {
	if (graphProxy->getRootGraph() == graphProxy->getCurrentGraphInHierarchy()) {
		return graphProxy->getPropertyMaxValue<PROPERTY, PROPERTYTYPE>(axisName);
	} else {
		Iterator<unsigned int> *dataIt = graphProxy->getDataIterator();
		unsigned int dataId = dataIt->next();
		typename PROPERTYTYPE::RealType max = graphProxy->getPropertyValueForData<PROPERTY, PROPERTYTYPE>(getAxisName(), dataId);
		while (dataIt->hasNext()) {
			dataId = dataIt->next();
			typename PROPERTYTYPE::RealType propertyValue = graphProxy->getPropertyValueForData<PROPERTY, PROPERTYTYPE>(getAxisName(), dataId);
			if (propertyValue > max) {
				max = propertyValue;
			}
		}
		delete dataIt;
		return max;
	}
}

void QuantitativeParallelAxis::translate(const Coord &c) {
	ParallelAxis::translate(c);
	map<unsigned int, Coord>::iterator it;
	for (it = dataCoords.begin() ; it != dataCoords.end() ; ++ it) {
		(it->second) += c;
	}
}

Coord QuantitativeParallelAxis::getPointCoordOnAxisForData(const unsigned int dataIdx) {
  return dataCoords[dataIdx];
}

void QuantitativeParallelAxis::redraw() {
	//reset(false);
	drawAxisLine();
	setLabelsAndComputeDataCoords();
	addCaption(axisName);
}

void QuantitativeParallelAxis::showConfigDialog() {
	QuantitativeAxisConfigDialog dialog(this);
	dialog.exec();
	updateSlidersWithDataSubset(graphProxy->getHighlightedElts());
}

string QuantitativeParallelAxis::getAxisDataTypeName() const {
	return graphProxy->getProperty(axisName)->getTypename();
}

template<typename PROPERTY, typename PROPERTYTYPE>
typename PROPERTYTYPE::RealType QuantitativeParallelAxis::getValueForAxisCoord(const Coord &axisCoord) {
	typename PROPERTYTYPE::RealType value;
	typename PROPERTYTYPE::RealType min = getAxisMinValue<PROPERTY, PROPERTYTYPE>();
	typename PROPERTYTYPE::RealType max = getAxisMaxValue<PROPERTY, PROPERTYTYPE>();
	if (ascendingOrder) {
		value = min + (axisCoord.getY() - baseCoord.getY()) / scale;
	} else {
		value = max - (axisCoord.getY() - baseCoord.getY()) / scale;
	}

	if (log10Scale) {
		double logMin, logMax;
		if (min >= 1) {
			logMin = log10((double)min);
			logMax = log10((double)max);
		} else {
			logMin = 0;
			logMax = log10((double)((max - min) + 1));
		}

		double logValue;
		if (ascendingOrder) {
			logValue = logMin + (axisCoord.getY() - baseCoord.getY()) / scale;
		} else {
			logValue = logMax - (axisCoord.getY() - baseCoord.getY()) / scale;
		}
		value = pow(10, logValue);
		if (min < 1) {
			value -= (1 - min);
		}
	}

	return value;
}

std::string QuantitativeParallelAxis::getTopSliderTextValue() {
	if (getAxisDataTypeName() == "int") {
		return getStringFromNumber(getValueForAxisCoord<IntegerProperty, IntegerType>(topSliderCoord));
	} else if (getAxisDataTypeName() == "double") {
		return getStringFromNumber(getValueForAxisCoord<DoubleProperty, DoubleType>(topSliderCoord));
	} else {
		return "";
	}
}

std::string QuantitativeParallelAxis::getBottomSliderTextValue() {
	if (getAxisDataTypeName() == "int") {
		return getStringFromNumber(getValueForAxisCoord<IntegerProperty, IntegerType>(bottomSliderCoord));
	} else if (getAxisDataTypeName() == "double") {
		return getStringFromNumber(getValueForAxisCoord<DoubleProperty, DoubleType>(bottomSliderCoord));
	} else {
		return "";
	}
}

set<unsigned int> QuantitativeParallelAxis::getDataInSlidersRange() {
	set<unsigned int> dataSubset;
	map<unsigned int, Coord>::iterator it;
	for (it = dataCoords.begin() ; it != dataCoords.end() ; ++it) {
		if ((it->second).getY() <= topSliderCoord.getY() && (it->second).getY() >= bottomSliderCoord.getY()) {
			dataSubset.insert(it->first);
		}
	}
	return dataSubset;
}

void QuantitativeParallelAxis::updateSlidersWithDataSubset(const set<unsigned int> &dataSubset) {
	set<unsigned int>::iterator it;
	Coord max = baseCoord;
	Coord min = baseCoord + Coord(0, axisHeight);
	for (it = dataSubset.begin() ; it != dataSubset.end() ; ++it) {
		if (dataCoords[*it].getY() < min.getY()) {
			min = dataCoords[*it];
		}

		if (dataCoords[*it].getY() > max.getY()) {
			max = dataCoords[*it];
		}
	}
	bottomSliderCoord = min;
	topSliderCoord = max;
}

void QuantitativeParallelAxis::setAscendindOrder(const bool ascendingOrder) {

	if (ascendingOrder != this->ascendingOrder) {

		float spaceBetweenSlider = topSliderCoord.getY() - bottomSliderCoord.getY();

		float axisCenterTopSliderDist = topSliderCoord.getY() - (baseCoord.getY() + axisHeight / 2);
		topSliderCoord.setY((baseCoord.getY() + axisHeight / 2) - axisCenterTopSliderDist + spaceBetweenSlider);

		float axisCenterBottomSliderDist = bottomSliderCoord.getY() - (baseCoord.getY() + axisHeight / 2);
		bottomSliderCoord.setY((baseCoord.getY() + axisHeight / 2) - axisCenterBottomSliderDist - spaceBetweenSlider);

	}

	this->ascendingOrder = ascendingOrder;
}

}
