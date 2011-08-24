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
#include "SpreadView.h"

#include <iostream>
#include <sstream>
#include "ui_SpreadViewWidget.h"
#include <tulip/Graph.h>
#include <tulip/TlpQtTools.h>
#include <QtGui/QPixmap>
#include <QtGui/QToolBar>
#include <QtGui/QKeyEvent>
#include "GraphTableModel.h"
#include "TulipTableWidgetColumnSelectionWidget.h"
#include "GraphTableWidget.h"

using namespace std;
using namespace tlp;


/**
  * This class is needed to define a custom SizeHint for the spread view.
  **/
class SpreadViewWidget : public QWidget {
public:
  SpreadViewWidget(QSize sizeHints,QWidget*parent = NULL):QWidget(parent),_sizeHint(sizeHints) {

  }

  QSize sizeHint()const {
    return _sizeHint;
  }

  QSize minimumSizeHint()const {
    return _sizeHint;
  }

  QSize _sizeHint;
};

SpreadView::SpreadView() :
  AbstractView(),ui(new Ui::SpreadViewWidget),_graph(NULL) {
}
SpreadView::~SpreadView() {
  delete ui;
}

QWidget *SpreadView::construct(QWidget *parent) {
  QWidget *widget = AbstractView::construct(parent);
  QWidget* mainWidget = new SpreadViewWidget(QSize(640,480),widget);
  ui->setupUi(mainWidget);

  setCentralWidget(mainWidget);
  installEventFilter(mainWidget);

  //Show hide columns.
  ui->editColumnPushButton->setMirrored(true);
  ui->editColumnPushButton->setOrientation(Qt::Vertical);
  ui->editColumnPushButton->setFlat(true);
  ui->editColumnPushButton->setCheckable(true);
  ui->editColumnPushButton->setAutoFillBackground(true);
  ui->editColumnPushButton->resize(ui->editColumnPushButton->sizeHint());
  ui->toolBarWidget->resize(ui->editColumnPushButton->width(),ui->toolBarWidget->height());
  ui->toolBarWidget->setMinimumWidth(ui->editColumnPushButton->sizeHint().width());
  ui->toolBarWidget->setMaximumWidth(ui->editColumnPushButton->sizeHint().width());
  ui->editColumnPushButton->updateGeometry();
  ui->toolBarWidget->updateGeometry();


  connect(ui->editColumnPushButton, SIGNAL(toggled(bool)), ui->nodesSpreadViewWidget->columnEditionWidget(), SLOT(setVisible(bool)));
  connect(ui->editColumnPushButton, SIGNAL(toggled(bool)), ui->edgesSpreadViewWidget->columnEditionWidget(), SLOT(setVisible(bool)));

  return widget;
}

void SpreadView::setData(Graph *graph, DataSet data) {
  if(graph!= _graph) {
    _graph = graph;
    DataSet nodesDataset;

    if(data.exist("nodes")) {
      data.get("nodes",nodesDataset);
    }

    DataSet edgesDataset;

    if(data.exist("edges")) {
      data.get("edges",edgesDataset);
    }

    ui->nodesSpreadViewWidget->setData(graph,nodesDataset,NODE);
    ui->edgesSpreadViewWidget->setData(graph,edgesDataset,EDGE);

  }
  else {
    //Refresh models.
    draw();
  }
}

void SpreadView::getData(Graph **graph, DataSet * d) {
  *graph = _graph;
  DataSet data;
  data.set("nodes",ui->nodesSpreadViewWidget->getData());
  data.set("edges",ui->edgesSpreadViewWidget->getData());
  *d = data;
}

Graph* SpreadView::getGraph() {
  return _graph;
}

void SpreadView::setGraph(Graph *graph) {
  setData(graph,DataSet());
}

void SpreadView::draw() {
  //Refresh models.
  ui->nodesSpreadViewWidget->update();
  ui->edgesSpreadViewWidget->update();
}

void SpreadView::refresh() {
}

void SpreadView::init() {
}


QImage SpreadView::createPicture(int width, int height, bool , int  , int xOffset , int yOffset ) {
  return QPixmap::grabWidget(getCentralWidget(),xOffset,yOffset,width,height).toImage();
}



bool SpreadView::eventFilter(QObject *, QEvent *event) {
  //Override default shortcut
  if(event->type() == QEvent::ShortcutOverride) {
    QKeyEvent* shortcutOverrideEvent  = static_cast<QKeyEvent*>(event);

    //Highlight all the elements
    if(shortcutOverrideEvent->modifiers() == Qt::ControlModifier && shortcutOverrideEvent->key() == Qt::Key_A) {
      currentTable()->graphTableWidget()->selectAll();
      shortcutOverrideEvent->accept();
      return true;
    }
    else if(shortcutOverrideEvent->key() == Qt::Key_Delete) {
      //Suppress selected elements
      Observable::holdObservers();
      currentTable()->deleteHighlightedElements();
      Observable::unholdObservers();
      shortcutOverrideEvent->accept();
      return true;
    }
  }

  return false;

}

SpreadViewTableWidget* SpreadView::currentTable()const {
  return ui->tabWidget->currentWidget() == ui->nodesTab ? ui->nodesSpreadViewWidget: ui->edgesSpreadViewWidget;
}

VIEWPLUGIN(SpreadView, "Table view", "Tulip Team", "07/06/2011", "Spreadsheet view", "2.0")




