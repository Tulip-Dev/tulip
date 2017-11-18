/**
 *
 * This file is part of Tulip (http://tulip.labri.fr)
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

#ifndef MATRIXVIEWCONFIGURATIONWIDGET_H
#define MATRIXVIEWCONFIGURATIONWIDGET_H

#include <QWidget>

#include <tulip/Color.h>

namespace Ui {
class MatrixViewConfigurationWidget;
}

namespace tlp {
class Graph;

enum GridDisplayMode { SHOW_ALWAYS = 0, SHOW_NEVER = 1, SHOW_ON_ZOOM = 2 };

class MatrixViewConfigurationWidget : public QWidget {
  Q_OBJECT

  Ui::MatrixViewConfigurationWidget *_ui;

public:
  MatrixViewConfigurationWidget(QWidget *parent = 0);
  void setGraph(tlp::Graph *);
  ~MatrixViewConfigurationWidget();

  GridDisplayMode gridDisplayMode() const;
  void setgridmode(int index);
  void setBackgroundColor(const QColor &);
  Color getBackgroundColor() const;
  void setDisplayEdges(const bool state);
  bool displayGraphEdges() const;
  void setEdgeColorInterpolation(const bool state);
  bool isEdgeColorInterpolation() const;
  int orderingProperty() const;
  void setOrderingProperty(int index);
  void setAscendingOrder(const bool state);
  bool ascendingOrder() const;
  void setOriented(const bool state);

protected slots:
  void orderingMetricComboIndexChanged(int i);
  void orderingDirectionChanged();

signals:
  void metricSelected(std::string);
  void changeBackgroundColor(QColor);
  void setGridDisplayMode();
  void showEdges(bool);
  void enableEdgeColorInterpolation(bool);
  void updateOriented(bool);

private:
  bool _modifyingMetricList;
};
}
#endif // MATRIXVIEWCONFIGURATIONWIDGET_H
