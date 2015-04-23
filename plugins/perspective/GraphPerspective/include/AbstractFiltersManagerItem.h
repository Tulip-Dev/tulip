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

#ifndef ABSTRACTFILTERSMANAGERITEM_H
#define ABSTRACTFILTERSMANAGERITEM_H

#include <QWidget>
#include <QTableView>

#include <tulip/Graph.h>
#include <tulip/WithParameter.h>
#include <tulip/ParameterListModel.h>
#include <tulip/PluginLister.h>

namespace tlp {
class BooleanProperty;
}

class QStandardItem;
class QComboBox;

// Combo box helper functions
QStandardItem* itemAt(QComboBox* combo, int i);
QStandardItem* lastItem(QComboBox* combo);
void fillTitle(QComboBox* combo, const QString& s);

class AbstractFiltersManagerItem: public QWidget {
  Q_OBJECT
protected:
  tlp::Graph* _graph;
public:
  explicit AbstractFiltersManagerItem(QWidget* parent = NULL);
  virtual void applyFilter(tlp::BooleanProperty*)=0;
  virtual QString title() const=0;
  void setGraph(tlp::Graph*);
protected:
  virtual void graphChanged();

  void updateGraphModel(QTableView* table,const QString& algName, tlp::Graph* g) {
    int h=0;

    if (!algName.isNull()) {
      tlp::ParameterDescriptionList params = tlp::PluginLister::getPluginParameters(algName.toStdString());
      table->setModel(new tlp::ParameterListModel(params,g,table));

      for (int i=0; i<table->model()->rowCount(); ++i)
        h += table->rowHeight(i);
    }
    else {
      if (table->model() != NULL)
        table->model()->deleteLater();

      table->setModel(NULL);
    }

    table->setMaximumHeight(h);
    table->setMinimumHeight(h);
  }

signals:
  void titleChanged();
};

#endif // ABSTRACTFILTERSMANAGERITEM_H
