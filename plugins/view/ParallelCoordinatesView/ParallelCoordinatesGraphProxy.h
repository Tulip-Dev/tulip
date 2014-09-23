/**
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
#include <tulip/PropertyTypes.h>


namespace tlp {

class ParallelCoordinatesGraphProxy : public GraphDecorator {

public :

  ParallelCoordinatesGraphProxy(Graph *graph, const ElementType location=NODE);
  ~ParallelCoordinatesGraphProxy();

  unsigned int getNumberOfSelectedProperties() const;
  std::vector<std::string> getAllProperties();
  std::vector<std::string> getSelectedProperties();
  void setSelectedProperties(const std::vector<std::string>& properties);
  void removePropertyFromSelection(const std::string &propertyName);

  ElementType getDataLocation() const;
  void setDataLocation(const ElementType location) {
    dataLocation = location;
  }

  unsigned int getDataCount() const;
  Iterator<unsigned int> *getDataIterator();
  Iterator<unsigned int> *getSelectedDataIterator();
  Iterator<unsigned int> *getUnselectedDataIterator();
  Color getDataColor(const unsigned int dataId);
  std::string getDataTexture(const unsigned int dataId);
  Size getDataViewSize(const unsigned int dataId);
  std::string getDataLabel(const unsigned int dataId);
  std::string getToolTipTextforData(const unsigned int dataId);

  bool isDataSelected(const unsigned int dataId);
  void setDataSelected(const unsigned int dataId, const bool dataSelected);
  void resetSelection();

  void deleteData(const unsigned int dataId);

  Graph * getGraph() const {
    return graph_component;
  }

  void setUnhighlightedEltsColorAlphaValue(const unsigned int alpha) {
    unhighlightedEltsColorAlphaValue = alpha;
  }
  unsigned int getUnhighlightedEltsColorAlphaValue() const {
    return unhighlightedEltsColorAlphaValue;
  }
  void addOrRemoveEltToHighlight(const unsigned int eltId);
  void unsetHighlightedElts();
  bool highlightedEltsSet() const;
  bool isDataHighlighted(const unsigned int dataId);
  void selectHighlightedElements();
  const std::set<unsigned int> &getHighlightedElts() const {
    return highlightedElts;
  }
  void resetHighlightedElts(const std::set<unsigned int> &highlightedData);
  void removeHighlightedElement(const unsigned int dataId);
  bool graphColorsModified() const {
    return graphColorsChanged;
  }
  void colorDataAccordingToHighlightedElts();

  void update(std::set<Observable *>::iterator begin ,std::set<Observable *>::iterator end);
  void observableDestroyed(Observable *) {}

  template<typename Proxytype>
  Proxytype* getLocalProperty(const std::string &name) {
    return graph_component->getLocalProperty<Proxytype>(name);
  }

  template<typename Proxytype>
  Proxytype* getProperty(const std::string &name) {
    return graph_component->getProperty<Proxytype>(name);
  }

  PropertyInterface* getProperty(const std::string &name) const {
    return graph_component->getProperty(name);
  }

  template<typename PROPERTY, typename PROPERTYTYPE>
  typename StoredType<typename PROPERTYTYPE::RealType>::ReturnedConstValue getPropertyValueForData(const std::string &propertyName, const unsigned int dataId) {
    if (getDataLocation() == NODE) {
      return getProperty<PROPERTY>(propertyName)->getNodeValue(node(dataId));
    }
    else {
      return getProperty<PROPERTY>(propertyName)->getEdgeValue(edge(dataId));
    }
  }

  template<typename PROPERTY, typename PROPERTYTYPE>
  void setPropertyValueForData(const std::string &propertyName, const unsigned int dataId, const typename PROPERTYTYPE::RealType &propertyValue) {
    if (getDataLocation() == NODE) {
      getProperty<PROPERTY>(propertyName)->setNodeValue(node(dataId), propertyValue);
    }
    else {
      getProperty<PROPERTY>(propertyName)->setEdgeValue(edge(dataId), propertyValue);
    }
  }

  template<typename PROPERTY, typename PROPERTYTYPE>
  void setPropertyValueForAllData(const std::string &propertyName, const typename PROPERTYTYPE::RealType &propertyValue) {
    if (getDataLocation() == NODE) {
      getProperty<PROPERTY>(propertyName)->setAllNodeValue(propertyValue);
    }
    else {
      getProperty<PROPERTY>(propertyName)->setAllEdgeValue(propertyValue);
    }
  }

  template<typename PROPERTY, typename PROPERTYTYPE>
  typename PROPERTYTYPE::RealType getPropertyMinValue(const std::string &propertyName) {
    if (getDataLocation() == NODE) {
      return getProperty<PROPERTY>(propertyName)->getNodeMin(graph_component);
    }
    else {
      return getProperty<PROPERTY>(propertyName)->getEdgeMin(graph_component);
    }
  }

  template<typename PROPERTY, typename PROPERTYTYPE>
  typename PROPERTYTYPE::RealType getPropertyMaxValue(const std::string &propertyName) {
    if (getDataLocation() == NODE) {
      return getProperty<PROPERTY>(propertyName)->getNodeMax(graph_component);
    }
    else {
      return getProperty<PROPERTY>(propertyName)->getEdgeMax(graph_component);
    }
  }

private:

  Color getOriginalDataColor(const unsigned int dataId);

  bool graphColorsChanged;
  ColorProperty *dataColors;
  ColorProperty *originalDataColors;
  std::set<unsigned int> highlightedElts;
  std::vector<std::string> selectedProperties;
  ElementType dataLocation;
  unsigned int unhighlightedEltsColorAlphaValue;
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
