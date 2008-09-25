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
    View(pluginName,parent){

    mainWidget=new SpreadWidget(this);
    
    setCentralWidget(mainWidget);
  }

  void SpreadView::setData(Graph *graph,DataSet dataSet) {
    mainWidget->setData(graph,dataSet);
  }
  
  DataSet SpreadView::getData() {
    return DataSet();
  }
  
  Graph* SpreadView::getGraph() {
    return mainWidget->getGraph();
  }

  void SpreadView::redrawView() {
    mainWidget->redrawView();
  }

  void SpreadView::addNode(Graph *graph,const node n) {
    mainWidget->addNode(graph,n);
  }
  void SpreadView::addEdge(Graph *graph,const edge e) {
    mainWidget->addEdge(graph,e);
  }
  void SpreadView::delNode(Graph *graph,const node n) {
    mainWidget->delNode(graph,n);
  }
  void SpreadView::delEdge(Graph *graph,const edge e) {
    mainWidget->delEdge(graph,e);
  }

}
