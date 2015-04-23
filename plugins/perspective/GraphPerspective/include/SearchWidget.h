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

#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QWidget>

namespace Ui {
class SearchWidget;
}

class QComboBox;
class SearchOperator;

namespace tlp {
class GraphHierarchiesModel;
class Graph;
class PropertyInterface;
}

class SearchWidget : public QWidget {
  Q_OBJECT

  static QVector<SearchOperator*> NUMERIC_OPERATORS;
  static QVector<SearchOperator*> STRING_OPERATORS;
  static QVector<SearchOperator*> NOCASE_STRING_OPERATORS;

  Ui::SearchWidget* _ui;
  tlp::PropertyInterface* term(QComboBox* combo);

  SearchOperator* searchOperator();
  bool isNumericComparison();

public:
  explicit SearchWidget(QWidget *parent = 0);
  virtual ~SearchWidget();

signals:

public slots:
  void setModel(tlp::GraphHierarchiesModel* model);
  void currentGraphChanged(tlp::Graph* g);
  void setGraph(tlp::Graph* g);
  void search();

protected slots:

  void graphIndexChanged();
  void termAChanged();
  void termBChanged();
  void updateOperators(tlp::PropertyInterface* a, tlp::PropertyInterface* b);
  void updateOperators(tlp::PropertyInterface* a, const QString& b);
  void setNumericOperatorsEnabled(bool e);
  void updateEditorWidget();
  void selectionModeChanged(int index);

protected:
  void dragEnterEvent(QDragEnterEvent *);
  void dropEvent(QDropEvent *);
};

#endif // SEARCHWIDGET_H
