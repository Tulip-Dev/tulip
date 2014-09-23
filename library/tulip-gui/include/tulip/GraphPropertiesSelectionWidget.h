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
///@cond DOXYGEN_HIDDEN


#ifndef GRAPHPROPERTYSELECTIONWIDGET_H_
#define GRAPHPROPERTYSELECTIONWIDGET_H_

#include "StringsListSelectionWidget.h"

namespace tlp {

class Graph;
/** \brief A widget which allows the user to select a subset of graph properties
 *
 *  This widget allows the user to select a subset of graph properties. The properties
 *  to select can be filtered according to their datatypes.
 */
class TLP_QT_SCOPE GraphPropertiesSelectionWidget : public StringsListSelectionWidget {

public :

  /**
   * Default Constructor (usefull for qt designer)
   */
  GraphPropertiesSelectionWidget(QWidget *parent = NULL, const StringsListSelectionWidget::ListType &listType = StringsListSelectionWidget::SIMPLE_LIST,
                                 const unsigned int maxNbSelectedProperties = 0);

  /**
   *  Constructor which creates a GraphPropertiesSelectionWidget for a given graph
   *
   *  \param graph The graph on which we want to select properties
   *  \param propertiesType To select properties with particular datatypes, fill a vector of string with the wanted datatypes name and passed it as parameter of this constructor. The datatypes name must be the same as those returned by the getTypename method of the PropertyInterface class (e.g. "int", "double", "string"). If the vector is empty, there is no restriction on property datatypes.
   *  \param includeViewProperties If true, the view properties (e.g. "viewLabel", "viewMetric") will be selectable
   */
  GraphPropertiesSelectionWidget(Graph *graph, QWidget *parent = NULL, const StringsListSelectionWidget::ListType &listType = StringsListSelectionWidget::SIMPLE_LIST,
                                 const unsigned int maxNbSelectedProperties = 0, const std::vector<std::string> &propertiesTypes = std::vector<std::string>(),
                                 const bool includeViewProperties = false);

  /**
   * Method to set the widget parameters
   *
   * \param graph The graph on which we want to select properties
   * \param propertiesType To select properties with particular datatypes, fill a vector of string with the wanted datatypes name and passed it as parameter of this constructor. The datatypes name must be the same as those returned by the getTypename method of the PropertyInterface class (e.g. "int", "double", "string"). If the vector is empty, there is no restriction on property datatypes.
   * \param includeViewProperties If true, the view properties (e.g. "viewLabel", "viewMetric") will be selectable
   */
  void setWidgetParameters(Graph *graph, const std::vector<std::string> &propertiesTypes = std::vector<std::string>(), const bool includeViewProperties = false);

  /**
   * Method to set the input graph properties list
   * If there is datatypes restriction, the types of properties are checked before inserting them in the input list
   * \param inputPropertiesList A vector containing input properties names
   */
  void setInputPropertiesList(std::vector<std::string> &inputPropertiesList);

  /**
   * Method to set the output graph properties list
   * If there is datatypes restriction, the types of properties are checked before inserting them in the output list
   * \param outputPropertiesList A vector containing output properties names
   */
  void setOutputPropertiesList(std::vector<std::string> &outputPropertiesList);

  /**
   * Method which returns the names of the graph properties selected by the user
   */
  std::vector<std::string> getSelectedProperties() const;


  /**
   *  Method which clears the content of the input list and the output list
   */
  void clearLists();

private :

  void initWidget();
  bool propertySelectable(const std::string &propertyName);

  Graph *graph;
  std::vector<std::string> propertiesTypes;
  bool includeViewProperties;

};

}

#endif /* GRAPHPROPERTYSELECTIONWIDGET_H_ */
///@endcond
