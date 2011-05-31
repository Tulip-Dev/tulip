/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
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
#include "tulip/GraphPropertiesTableWidget.h"
#include <tulip/Graph.h>
#include <tulip/PropertyInterface.h>
#include <tulip/ForEach.h>
#include <tulip/TlpQtTools.h>
#include <QtGui/QTableWidgetItem>
#include <QtGui/QHeaderView>
#include <algorithm>
#include <string>
using namespace tlp;
using namespace std;

// same value as in TulipTableWidget.cpp
#define ROW_HEIGHT 18

GraphPropertiesTableWidget::GraphPropertiesTableWidget(QWidget *parent) :
  QTableWidget(parent), graph(NULL), typeFilter(All), nameFilter(".*") {
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  // set sorting order to avoid different default value
  // depending on implementation (needed by gui tests)
  sortItems(0, Qt::AscendingOrder);
}

GraphPropertiesTableWidget::~GraphPropertiesTableWidget() {
}

void GraphPropertiesTableWidget::setGraph(Graph* newGraph) {
  graph = newGraph;
  //Deselect properties in old graph.
  clearSelection();
  updateTable();
}

void GraphPropertiesTableWidget::updateTable() {
  //Keep old selection
  vector<string> oldSelection = getSelectedPropertiesNames();
  clear();
  if (graph != NULL) {
    vector<string> propertiesName;
    string propertyName;
    //Get the properties list to display
    forEach(propertyName,graph->getProperties()) {
      if (checkPropertyFilter(propertyName) && checkPropertyName(propertyName)) {
        propertiesName.push_back(propertyName);
      }
    }
    //Save the sorting state
    bool previousSortingState = isSortingEnabled();
    //We need to disable sort before adding elements as when adding element with sorting enabled the element index is moved.
    setSortingEnabled(false);

    //Update the row number.
    setRowCount(propertiesName.size());
    //Get the header list.
    QStringList headersList = getHeaderList();
    //Update the column number and labels.
    setColumnCount(headersList.size());
    setHorizontalHeaderLabels(getHeaderList());

    //Fill the table.
    for (unsigned int i = 0; i < propertiesName.size(); ++i) {
      for (int j = 0; j < columnCount(); ++j) {
        setItem(i, j, createPropertyItem(graph->getProperty(propertiesName[i]), j));
      }
      setRowHeight(i, ROW_HEIGHT);
    }
    //Restore sorting
    setSortingEnabled(previousSortingState);
    //Restore old selection.
    setSelectedPropertiesNames(oldSelection);
  }
}

QStringList GraphPropertiesTableWidget::getHeaderList() const {
  QStringList headersLabels;
  headersLabels << tr("Name") << tr("Type") << tr("Range");
  return headersLabels;
}
QTableWidgetItem* GraphPropertiesTableWidget::createPropertyItem(PropertyInterface* property, int column) {
  if (column == 0) {
    return new QTableWidgetItem(QString::fromUtf8(property->getName().c_str()));
  }
  else if (column == 1) {
    return new QTableWidgetItem(propertyTypeToPropertyTypeLabel(property->getTypename()));
  }
  else if (column == 2) {
    if (graph->existLocalProperty(property->getName())) {
      return new QTableWidgetItem(tr("Local"));
    }
    else {
      return new QTableWidgetItem(tr("Inherited from graph : ") + QString::number(property->getGraph()->getId()));
    }
  }
  else {
    return NULL;
  }
}

vector<string> GraphPropertiesTableWidget::getSelectedPropertiesNames() const {
  vector<string> properties;
  //Use the range difference to compute the number of row selected.
  QList<QTableWidgetSelectionRange> range = selectedRanges();
  for (QList<QTableWidgetSelectionRange>::iterator it = range.begin(); it != range.end(); ++it) {
    for (int i = 0; i < (*it).rowCount(); ++i) {
      if (((*it).bottomRow() + i) < rowCount()) {
        properties.push_back(getPropertyNameForRow((*it).bottomRow() + i));
      }
    }
  }
  return properties;
}

vector<string> GraphPropertiesTableWidget::getDisplayedPropertiesNames() const {
  vector<string> properties;
  for (int i = 0; i < rowCount(); ++i) {
    properties.push_back(getPropertyNameForRow(i));
  }
  return properties;
}

string GraphPropertiesTableWidget::getPropertyNameForRow(int row) const {
  assert(rowCount()>row);
  return string(item(row, 0)->text().toUtf8().data());
}

void GraphPropertiesTableWidget::setPropertyTypeFilter(PropertyType typeFilter) {
  this->typeFilter = typeFilter;
  updateTable();
}

bool GraphPropertiesTableWidget::checkPropertyFilter(const string& propertyName) {
  if (typeFilter == All) {
    return true;
  }
  bool isViewPrefix = propertyName.substr(0, 4).compare("view") == 0;
  return (typeFilter == View) ? isViewPrefix : !isViewPrefix;
}

bool GraphPropertiesTableWidget::checkPropertyName(const string& propertyName) {
#ifdef NDEBUG
  //Don't display viewMetaGraphProperty in release
  if (propertyName.compare("viewMetaGraph") == 0) {
    return false;
  }
#endif
  if (nameFilter.isValid()) {
    //Find if the reg exp match in the string.
    return nameFilter.indexIn(QString::fromUtf8(propertyName.c_str())) != -1;
  }
  else {
    return true;
  }
}

void GraphPropertiesTableWidget::setPropertyNameFilter(const QRegExp& nameRegExp) {
  nameFilter = nameRegExp;
  updateTable();
}

void GraphPropertiesTableWidget::setSelectedPropertiesNames(const vector<string>& selectedProperties) {
  //Avoid sending signals for selected element properties.
  blockSignals(true);
  //Destroy previous selection.
  clearSelection();
  for (vector<string>::const_iterator it = selectedProperties.begin(); it != selectedProperties.end(); ++it) {
    for (int i = 0; i < rowCount(); ++i) {
      QTableWidgetItem *propertyItem = item(i, 0);
      if (propertyItem->text().compare(QString::fromUtf8(it->c_str())) == 0) {
        selectRow(i);
      }
    }
  }
  blockSignals(false);
}
