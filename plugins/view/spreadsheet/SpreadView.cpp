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
#include <QtGui/QPixmap>
using namespace std;

namespace tlp {

SpreadView::SpreadView() :
        AbstractView(),ui(new Ui::SpreadViewWidget),_graph(NULL){
}
SpreadView::~SpreadView() {  
    delete ui;
}

QWidget *SpreadView::construct(QWidget *parent) {
	QWidget *widget = AbstractView::construct(parent);	
        QWidget* mainWidget = new QWidget(widget);
        ui->setupUi(mainWidget);
        setCentralWidget(mainWidget);
        installEventFilter(mainWidget);
        installEventFilter(ui->nodesTableView);
	return widget;
}

void SpreadView::setData(Graph *graph, DataSet) {
    _graph = graph;
    ui->nodesTableView->setGraph(graph,NODE);
    ui->edgesTableView->setGraph(graph,EDGE);
}

void SpreadView::getData(Graph **graph, DataSet *) {
    *graph = _graph;
}

Graph* SpreadView::getGraph() {	
    return _graph;
}

void SpreadView::setGraph(Graph *graph) {	
    setData(graph,DataSet());
}

void SpreadView::draw() {	
}

void SpreadView::refresh() {	
}

void SpreadView::init() {
}


QImage SpreadView::createPicture(int width, int height, bool , int  , int xOffset , int yOffset ){
    return QPixmap::grabWidget(getCentralWidget(),xOffset,yOffset,width,height).toImage();
}

VIEWPLUGIN(SpreadView, "Table view", "Tulip Team", "16/04/2008", "Spreadsheet view", "2.0")

}


