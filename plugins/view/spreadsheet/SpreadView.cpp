#include "SpreadView.h"

#include <iostream>

#include <tulip/Graph.h>

using namespace std;

namespace tlp {

  VIEWPLUGIN(SpreadViewCreator, "SpreadSheetView", "Tulip Team", "16/04/2008", "Spreadsheet view", "1.0", 1);

  View* SpreadViewCreator::create(const string &pluginName,QWidget *parent) {
    return new SpreadView(pluginName,parent);
  }

  SpreadView::SpreadView(const string &pluginName,QWidget *parent) :
    AbstractView(pluginName,parent){

    mainWidget=new SpreadWidget(this);
    
    setCentralWidget(mainWidget);
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
  
  void SpreadView::reinitAndDraw() {
    draw();
  }

}
