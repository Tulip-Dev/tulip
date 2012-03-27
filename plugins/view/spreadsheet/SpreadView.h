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

#ifndef Tulip_SPREADVIEW_H
#define Tulip_SPREADVIEW_H

#include <QtGui/QWidget>

#include <tulip/tulipconf.h>

#include <tulip/AbstractView.h>
#include <tulip/ObservableProperty.h>

#include <QtCore/QModelIndexList>


namespace Ui {
class SpreadViewWidget;
}


class SpreadViewTableWidget;
namespace tlp {
class Graph;
}


/*@{*/
/** \file
 *  \brief  Table view

 * In this view you can visualize and edit the values of each graph elements (nodes or edges) using a table form.
 * Each column represents a graph property and each row a node or an edge.
 * Refer to the user manual for a more information on the table view.
 *
 * <b>LICENCE</b>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 */
class SpreadView: public tlp::AbstractView , public tlp::PropertyObserver, public tlp::Observable {
  Q_OBJECT
public:

  SpreadView();
  virtual ~SpreadView();

  QWidget *construct(QWidget *parent);
  void setData(tlp::Graph *graph, tlp::DataSet dataSet);
  void getData(tlp::Graph **graph, tlp::DataSet *dataSet);
  tlp::Graph *getGraph();

  void installInteractor(QAction*) {
  }

  QImage createPicture(int width, int height, bool center, int zoom = 1, int xOffset = 0, int yOffset = 0);

  bool eventFilter(QObject *object, QEvent *event);

public slots :

  void draw();
  void refresh();
  void init();
  void setGraph(tlp::Graph *graph);

private:

  /**
    * @brief Get the current displayed table.
    **/
  SpreadViewTableWidget* currentTable()const;
  Ui::SpreadViewWidget *ui;
  tlp::Graph* _graph;

};


#endif
