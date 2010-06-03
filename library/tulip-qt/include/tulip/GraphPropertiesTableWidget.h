/*
 * PropertiesTable.h
 *
 *  Created on: 19 févr. 2010
 *      Author: Jonathan Dubois
 *       Email: jonathan.dubois@labri.fr
 *
 *	This program is free software; you can redistribute it and/or modify  *
 *  it under the terms of the GNU Lesser General Public License as published by  
 *  the Free Software Foundation; either version 2 of the License, or     
 *  (at your option) any later version.
 */

#ifndef GRAPHPROPERTIESTABLEWIDGET_H_
#define GRAPHPROPERTIESTABLEWIDGET_H_
#include <QtGui/QTableWidget>
#include <tulip/ObservableGraph.h>
#include <QtCore/QStringList>
#include <QtCore/QRegExp>
class QTableWidgetItem;

namespace tlp {
class Graph;
class PropertyInterface;

/**
 * @brief Widget to display properties for a graph.
 * Widget to display properties for a graph.
 * User can filter displayed properties in function of their name with a QRegularExpression. Property name don't need to fully match regular expression to be displayed, if only one part of the name match the property is displayed.
 */
class GraphPropertiesTableWidget: public QTableWidget {
Q_OBJECT
public:
  enum PropertyType {
    All, User, View
  };

  GraphPropertiesTableWidget(QWidget *parent = NULL);
  virtual ~GraphPropertiesTableWidget();

  virtual void setGraph(tlp::Graph* newGraph);

  /**
   * @brief Clear and redraw the widget.
   */
  virtual void updateTable();
  Graph* getGraph() const {
    return graph;
  }

  std::vector<std::string> getSelectedPropertiesNames() const;
  virtual std::string getPropertyNameForRow(int row) const;

  /**
   * @brief Set the kind of view to display.
   */
  void setPropertyTypeFilter(PropertyType typeFilter);
  PropertyType getPropertyType() const {
    return typeFilter;
  }

  /**
   * @brief Set a regular expression to filter the properties on their name.
   */
  void setPropertyNameFilter(const QRegExp& nameRegExp);

protected:
  /**
   * Return the column headers list.
   * Each string in the list correspond to a column in the table.
   * This function is called when filling the table.
   * @return The list of the headers for the columns.
   */
  virtual QStringList getHeaderList() const;
  /**
   * Return the QTableWidgetItem for the given property and the given column.
   * @param property The property interface of the item.
   * @param column The column number of the item to create.
   */
  virtual QTableWidgetItem* createPropertyItem(PropertyInterface* property, int column);

private:

  /**
   * @brief Test if the property name correspond to the current filter.
   * @return True if the property correspond to the filer.
   */
  bool checkPropertyFilter(const std::string& propertyName);
  /**
   * @brief Test if a part property name match regular expression filter.
   * @return True if a part property name match regular expression filter.
   */
  bool checkPropertyName(const std::string& propertyName);
  Graph* graph;
  PropertyType typeFilter;
  QRegExp nameFilter;
};
}
#endif /* PROPERTIESTABLE_H_ */
