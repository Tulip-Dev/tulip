#include "SpreadView.h"

#include <iostream>

#include <tulip/Graph.h>

using namespace std;

namespace tlp {

  SpreadView::SpreadView() :AbstractView() {
  }

  QWidget *SpreadView::construct(QWidget *parent) {
  	QWidget *widget=AbstractView::construct(parent);
    mainWidget=new SpreadWidget(widget);
    setCentralWidget(mainWidget);
    return widget;
  }

  void SpreadView::setData(Graph *graph,DataSet dataSet) {
    mainWidget->setData(graph,dataSet);
  }

  void SpreadView::getData(Graph **graph,DataSet *dataSet) {
    dataSet->set<DataSet>("data",DataSet());
    *graph=mainWidget->getGraph();
  }

  Graph* SpreadView::getGraph() {
    return mainWidget->getGraph();
  }

  void SpreadView::setGraph(Graph *graph) {
    mainWidget->setData(graph,DataSet());
  }

  void SpreadView::draw() {
    mainWidget->redrawView();
  }

  void SpreadView::refresh() {
    draw();
  }

  void SpreadView::init() {
    draw();
  }

  VIEWPLUGIN(SpreadView, "SpreadSheetView", "Tulip Team", "16/04/2008", "Spreadsheet view", "1.0");

}
