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

#ifndef TABLEVIEWCONFIGURATION_H
#define TABLEVIEWCONFIGURATION_H

#include <QtGui/QWidget>

namespace Ui {
class TableViewConfiguration;
}

namespace tlp {
class Graph;
class BooleanProperty;
}

class TableViewConfiguration : public QWidget {
  Q_OBJECT

  Ui::TableViewConfiguration* _ui;
public:
  explicit TableViewConfiguration(QWidget *parent = NULL);
  virtual ~TableViewConfiguration();

  void setGraph(tlp::Graph* g);

  bool isShowNodes() const;
  bool isShowEdges() const;
  tlp::BooleanProperty* filteringProperty() const;

public slots:
  void setShowNodes(bool);
  void setShowEdges(bool);
  void setFilteringProperty(QString);

signals:
  void settingsChanged();
};

#endif // TABLEVIEWCONFIGURATION_H
