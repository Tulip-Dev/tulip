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

#ifndef PARALLELCOORDINATESGRAPHPROXY_H_INCLUDE
#define PARALLELCOORDINATESGRAPHPROXY_H_INCLUDE

#ifndef DOXYGEN_NOTFOR_DEVEL

#include <list>
#include <string>
#include <set>
#include <tulip/GraphDecorator.h>
#include <tulip/Color.h>
#include <tulip/ColorProperty.h>
#include <tulip/Size.h>
#include <tulip/StableIterator.h>

#include "ParallelTools.h"

namespace tlp {

class ParallelCoordinatesGraphProxy : public GraphDecorator {

public :

  ParallelCoordinatesGraphProxy(Graph *graph, const ElementType location=NODE);
  ~ParallelCoordinatesGraphProxy();

  unsigned int getNumberOfSelectedProperties() const;
  std::vector<std::string> getAllProperties();
  const std::vector<std::string>& getSelectedProperties() ;
  void setSelectedProperties(const std::vector<std::string>& properties);
  void removePropertyFromSelection(const std::string &propertyName);

  ElementType getDataLocation() const;
  void setDataLocation(const ElementType location) {dataLocation = location;}

  Iterator<unsigned int> *getDataIterator();
  unsigned int getDataCount() const;
  Color getDataColor(const unsigned int dataId);
  std::string getDataTexture(const unsigned int dataId);
  Size getDataViewSize(const unsigned int dataId);
  std::string getDataLabel(const unsigned int dataId);
  std::string getToolTipTextforData(const unsigned int dataId);

  bool isDataSelected(const unsigned int dataId);
  void setDataSelected(const unsigned int dataId, const bool dataSelected);
  void resetSelection();

  void deleteData(const unsigned int dataId);

  Graph * getGraph() const {return graph_component;}

  void addOrRemoveEltToHighlight(const unsigned int eltId);
  void unsetHighlightedElts();
  bool highlightedEltsSet() const;
  bool isDataHighlighted(const unsigned int dataId);
  void selectHighlightedElements();
  const std::set<unsigned int> &getHighlightedElts() const {return highlightedElts;}
  void resetHighlightedElts(const std::set<unsigned int> &highlightedData);
  void removeHighlightedElement(const unsigned int dataId);
  void colorDataAccordingToHighlightedElts();
  bool unhighlightedEltsColorOk();

  template<typename PROPERTY, typename PROPERTYTYPE>
  typename PROPERTYTYPE::RealType getPropertyValueForData(const std::string &propertyName, const unsigned int dataId) {
	  if (getDataLocation() == NODE) {
		  return ((PROPERTY *)getProperty(propertyName))->getNodeValue(node(dataId));
	  } else {
		  return ((PROPERTY *)getProperty(propertyName))->getEdgeValue(edge(dataId));
	  }
  }

  template<typename PROPERTY, typename PROPERTYTYPE>
  void setPropertyValueForData(const std::string &propertyName, const unsigned int dataId, const typename PROPERTYTYPE::RealType propertyValue) {
	  if (getDataLocation() == NODE) {
		  ((PROPERTY *)getProperty(propertyName))->setNodeValue(node(dataId), propertyValue);
  	  } else {
  		  ((PROPERTY *)getProperty(propertyName))->setEdgeValue(edge(dataId), propertyValue);
  	  }
  }

  template<typename PROPERTY, typename PROPERTYTYPE>
  void setPropertyValueForAllData(const std::string &propertyName, const typename PROPERTYTYPE::RealType propertyValue) {
	  if (getDataLocation() == NODE) {
		  ((PROPERTY *)getProperty(propertyName))->setAllNodeValue(propertyValue);
	  } else {
		  ((PROPERTY *)getProperty(propertyName))->setAllEdgeValue(propertyValue);
	  }
  }

  template<typename PROPERTY, typename PROPERTYTYPE>
  typename PROPERTYTYPE::RealType getPropertyMinValue(const std::string &propertyName) {
	  if (getDataLocation() == NODE) {
		  return ((PROPERTY *)getProperty(propertyName))->getNodeMin();
	  } else {
		  return ((PROPERTY *)getProperty(propertyName))->getEdgeMin();
	  }
  }

  template<typename PROPERTY, typename PROPERTYTYPE>
  typename PROPERTYTYPE::RealType getPropertyMaxValue(const std::string &propertyName) {
	  if (getDataLocation() == NODE) {
		  return ((PROPERTY *)getProperty(propertyName))->getNodeMax();
	  } else {
		  return ((PROPERTY *)getProperty(propertyName))->getEdgeMax();
	  }
  }

private:

  Color getOriginalDataColor(const unsigned int dataId);

  ColorProperty *dataColors;
  ColorProperty *originalDataColors;
  std::set<unsigned int> highlightedElts;
  std::vector<std::string> selectedProperties;
  ElementType dataLocation;
};

template <typename GraphDataSource>
class ParallelCoordinatesDataIterator : public Iterator<unsigned int> {

public :

	ParallelCoordinatesDataIterator(Iterator<GraphDataSource> *graphDataSourceIterator) : graphDataSourceIt(graphDataSourceIterator) {}

	unsigned int next() {
		return graphDataSourceIt.next().id;
	}

	bool hasNext() {
		return graphDataSourceIt.hasNext();
	}

private :

	StableIterator<GraphDataSource> graphDataSourceIt;
};

}

#endif // DOXYGEN_NOTFOR_DEVEL

#endif // PARALLELCOORDINATESGRAPHPROXY_H_INCLUDE
