#include "QuantitativeParallelAxis.h"
#include "AxisConfigDialogs.h"

#include <tulip/IntegerProperty.h>
#include <tulip/DoubleProperty.h>

#include <cmath>

using namespace std;

namespace tlp {

QuantitativeParallelAxis::QuantitativeParallelAxis(const Coord &baseCoord, const float height, const float axisAreaWidth, ParallelCoordinatesGraphProxy *graphProxy,
																		   const string &graphPropertyName, const bool ascendingOrder, const Color &axisColor) :
  ParallelAxis(baseCoord, height, axisAreaWidth, graphPropertyName, axisColor), ascendingOrder(ascendingOrder),
  nbAxisGrad(DEFAULT_NB_AXIS_GRAD), graphProxy(graphProxy), log10Scale(false) {
  boxPlotValuesCoord.resize(5);
  boxPlotStringValues.resize(5);
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

	  for (float i = labelCoord + (increment * scale) ; i < (labelCoord + axisHeight); i += (increment * scale)) {

		  if ((labelCoord + axisHeight - i) < ((increment * scale) / (2.5)))
			  break;

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

	  for (float i = labelCoord + (increment * scale) ; i < (labelCoord + axisHeight); i += (increment * scale)) {
		  if (typeid(min) == typeid(int) && (labelCoord + axisHeight - i) < (increment * scale))
			  break;
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

	set<typename PROPERTYTYPE::RealType> propertyValuesSet;

	Iterator<unsigned int> *dataIt = graphProxy->getDataIterator();

	while (dataIt->hasNext()) {
		unsigned int dataId = dataIt->next();

		Coord c = Coord(baseCoord.getX(), 0, 0);
		typename PROPERTYTYPE::RealType propertyValue =
				graphProxy->getPropertyValueForData<PROPERTY, PROPERTYTYPE> (axisName, dataId);

		propertyValuesSet.insert(propertyValue);

		if (ascendingOrder)
			c.setY(baseCoord.getY() + (propertyValue - min) * scale);
		else
			c.setY(baseCoord.getY() + (max - propertyValue) * scale);

		dataCoords[dataId] = c;

	}

	delete dataIt;

	computeBoxPlotCoords<PROPERTY, PROPERTYTYPE>(propertyValuesSet);


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

	set<typename PROPERTYTYPE::RealType> propertyValuesSet;

	Iterator<unsigned int> *dataIt = graphProxy->getDataIterator();

	while (dataIt->hasNext()) {
		unsigned int dataId = dataIt->next();

		Coord c = Coord(baseCoord.getX(), 0, 0);
		typename PROPERTYTYPE::RealType value = graphProxy->getPropertyValueForData<PROPERTY, PROPERTYTYPE> (axisName, dataId);
		propertyValuesSet.insert(value);

		double propertyValue = (double) value;
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

	computeBoxPlotCoords<PROPERTY, PROPERTYTYPE>(propertyValuesSet);
}

template<typename PROPERTY, typename PROPERTYTYPE>
void QuantitativeParallelAxis::computeBoxPlotCoords(set<typename PROPERTYTYPE::RealType> propertyValuesSet) {

	vector<typename PROPERTYTYPE::RealType> propertyValuesVector(propertyValuesSet.begin(), propertyValuesSet.end());
	unsigned int vectorSize = propertyValuesVector.size();

	if (vectorSize < 4) {

		boxPlotValuesCoord[BOTTOM_OUTLIER] = Coord(-1,-1,-1);
		boxPlotValuesCoord[FIRST_QUARTILE] = Coord(-1,-1,-1);
		boxPlotValuesCoord[MEDIAN] = Coord(-1,-1,-1);
		boxPlotValuesCoord[THIRD_QUARTILE] = Coord(-1,-1,-1);
		boxPlotValuesCoord[TOP_OUTLIER] = Coord(-1,-1,-1);

		boxPlotStringValues[BOTTOM_OUTLIER] = "KO";
		boxPlotStringValues[FIRST_QUARTILE] = "KO";
		boxPlotStringValues[MEDIAN] = "KO";
		boxPlotStringValues[THIRD_QUARTILE] = "KO";
		boxPlotStringValues[TOP_OUTLIER] = "KO";

	} else {

		typename PROPERTYTYPE::RealType median;
		if (vectorSize % 2 == 1) {
			median = propertyValuesVector[vectorSize / 2];
		} else {
			median = (propertyValuesVector[(vectorSize / 2) - 1] + propertyValuesVector[vectorSize / 2]) / 2;
		}

		typename PROPERTYTYPE::RealType firstQuartile;
		if (vectorSize % 2 == 1) {
			firstQuartile = propertyValuesVector[vectorSize / 4];
		} else {
			firstQuartile = (propertyValuesVector[(vectorSize / 4) - 1] + propertyValuesVector[vectorSize / 4]) / 2;
		}

		typename PROPERTYTYPE::RealType thirdQuartile;
		if (vectorSize % 2 == 1) {
			thirdQuartile = propertyValuesVector[3 *(vectorSize / 4)];
		} else {
			thirdQuartile = (propertyValuesVector[3 *(vectorSize / 4) - 1] + propertyValuesVector[3 *(vectorSize / 4)]) / 2;
		}

		typename PROPERTYTYPE::RealType lowBorder = (typename PROPERTYTYPE::RealType) (firstQuartile - (1.5 * (thirdQuartile - firstQuartile)));
		typename PROPERTYTYPE::RealType bottomOutlier;
		typename vector<typename PROPERTYTYPE::RealType>::iterator it;
		for (it = propertyValuesVector.begin() ; it != propertyValuesVector.end() ; ++it) {
			if (*it > lowBorder) {
				bottomOutlier = *it;
				break;
			}
		}

		typename PROPERTYTYPE::RealType highBorder = (typename PROPERTYTYPE::RealType) (thirdQuartile + (1.5 * (thirdQuartile - firstQuartile)));
		typename PROPERTYTYPE::RealType topOutlier;
		typename vector<typename PROPERTYTYPE::RealType>::reverse_iterator itr;
		for (itr = propertyValuesVector.rbegin() ; itr != propertyValuesVector.rend() ; ++itr) {
			if (*itr < highBorder) {
				topOutlier = *itr;
				break;
			}
		}

		boxPlotValuesCoord[BOTTOM_OUTLIER] = getAxisCoordForValue<PROPERTY, PROPERTYTYPE>(bottomOutlier);
		boxPlotValuesCoord[FIRST_QUARTILE] = getAxisCoordForValue<PROPERTY, PROPERTYTYPE>(firstQuartile);
		boxPlotValuesCoord[MEDIAN] = getAxisCoordForValue<PROPERTY, PROPERTYTYPE>(median);
		boxPlotValuesCoord[THIRD_QUARTILE] = getAxisCoordForValue<PROPERTY, PROPERTYTYPE>(thirdQuartile);
		boxPlotValuesCoord[TOP_OUTLIER] = getAxisCoordForValue<PROPERTY, PROPERTYTYPE>(topOutlier);

		boxPlotStringValues[BOTTOM_OUTLIER] = getStringFromNumber(bottomOutlier);
		boxPlotStringValues[FIRST_QUARTILE] = getStringFromNumber(firstQuartile);
		boxPlotStringValues[MEDIAN] = getStringFromNumber(median);
		boxPlotStringValues[THIRD_QUARTILE] = getStringFromNumber(thirdQuartile);
		boxPlotStringValues[TOP_OUTLIER] = getStringFromNumber(topOutlier);

	}
}


template<typename PROPERTY, typename PROPERTYTYPE>
typename PROPERTYTYPE::RealType QuantitativeParallelAxis::getAxisMinValue() {
	if (graphProxy->getGraph()->getRoot() == graphProxy->getGraph()) {
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
	if (graphProxy->getGraph()->getRoot() == graphProxy->getGraph()) {
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
	boxPlotValuesCoord[BOTTOM_OUTLIER] += c;
	boxPlotValuesCoord[FIRST_QUARTILE] += c;
	boxPlotValuesCoord[MEDIAN] += c;
	boxPlotValuesCoord[THIRD_QUARTILE] += c;
	boxPlotValuesCoord[TOP_OUTLIER] += c;

}

Coord QuantitativeParallelAxis::getPointCoordOnAxisForData(const unsigned int dataIdx) {
  return dataCoords[dataIdx];
}

template<typename PROPERTY, typename PROPERTYTYPE>
Coord QuantitativeParallelAxis::getAxisCoordForValue(typename PROPERTYTYPE::RealType value) {
	typename PROPERTYTYPE::RealType min = getAxisMinValue<PROPERTY, PROPERTYTYPE>();
	typename PROPERTYTYPE::RealType max = getAxisMaxValue<PROPERTY, PROPERTYTYPE>();
	if (!log10Scale) {
		if (ascendingOrder) {
			return Coord(baseCoord.getX(), baseCoord.getY() + (value - min) * scale, 0);
		} else {
			return Coord(baseCoord.getX(), baseCoord.getY() + (max - value) * scale, 0);
		}
	} else {
		double logMin, logMax;
		double logPropertyValue;
		if (min >= 1) {
			logMin = log10((double)min);
			logMax = log10((double)max);
		} else {
			logMin = 0;
			logMax = log10((double)((max - min) + 1));
		}
		if (min < 1) {
			logPropertyValue = log10(value + (1 - min));
		} else {
			logPropertyValue = log10(value);
		}

		if (ascendingOrder) {
			return Coord(baseCoord.getX(), baseCoord.getY() + (logPropertyValue - logMin) * scale, 0);
		} else {
			return Coord(baseCoord.getX(), baseCoord.getY() + (logMax - logPropertyValue) * scale, 0);
		}
	}
}

void QuantitativeParallelAxis::redraw() {
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

set<unsigned int> QuantitativeParallelAxis::getDataInRange(float yLowBound, float yHighBound) {
	set<unsigned int> dataSubset;
	map<unsigned int, Coord>::iterator it;
	for (it = dataCoords.begin() ; it != dataCoords.end() ; ++it) {
		if ((it->second).getY() <= yHighBound && (it->second).getY() >= yLowBound) {
			dataSubset.insert(it->first);
		}
	}
	return dataSubset;
}

set<unsigned int> QuantitativeParallelAxis::getDataInSlidersRange() {
	return getDataInRange(bottomSliderCoord.getY(), topSliderCoord.getY());
}

set<unsigned int> QuantitativeParallelAxis::getDataBetweenBoxPlotBounds() {
	if (boxPlotLowBound != NO_VALUE && boxPlotHighBound != NO_VALUE) {
		return getDataInRange(boxPlotValuesCoord[boxPlotLowBound].getY(), boxPlotValuesCoord[boxPlotHighBound].getY());
	} else {
		return set<unsigned int>();
	}
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
