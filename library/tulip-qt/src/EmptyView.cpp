#include "tulip/EmptyView.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>

#include <tulip/ForEach.h>

#include <tulip/GlMainWidget.h>
#include <QtGui/QLabel>

#include <tulip/Interactor.h>

using namespace tlp;
using namespace std;

EmptyView::EmptyView() {
}

EmptyView::~EmptyView() {
}

class MyGlMainWidget: public tlp::GlMainWidget {
public:
  MyGlMainWidget(QWidget *parent,View *view=NULL): GlMainWidget(parent,view) {}
  virtual ~MyGlMainWidget() {}
};

void EmptyView::setData(tlp::Graph *graph,tlp::DataSet dataSet) {
  _graph = graph;
  mainWidget = new MyGlMainWidget(0,this);
  mainWidget->setData(graph, dataSet);
  setCentralWidget(mainWidget);
}
void EmptyView::getData(tlp::Graph **graph,tlp::DataSet *dataSet) {
  *graph = _graph;
  dataSet = new DataSet();
}
tlp::Graph *EmptyView::getGraph() {
  return _graph;
}
void EmptyView::setGraph(tlp::Graph *graph) {
  mainWidget->setGraph(graph);
  _graph = graph;
}
void EmptyView::draw() { mainWidget->draw(); }
void EmptyView::refresh() { draw(); }
void EmptyView::init() {}

VIEWPLUGIN(EmptyView, "GraphicsView", "Ludwig Fiolka", "07/01/2011", "Release Candidate", "2.0");
INTERACTORPLUGINVIEWEXTENSION(i1, "i1" ,"InteractorNavigation", "GraphicsView", "Ludwig Fiolka", "07/01/2011", "Stable", "1.0");
INTERACTORPLUGINVIEWEXTENSION(i2, "i2" ,"InteractorSelection", "GraphicsView", "Ludwig Fiolka", "07/01/2011", "Stable", "1.0");
INTERACTORPLUGINVIEWEXTENSION(i3, "i3" ,"InteractorAddNode", "GraphicsView", "Ludwig Fiolka", "07/01/2011", "Stable", "1.0");
INTERACTORPLUGINVIEWEXTENSION(i4, "i4" ,"InteractorGetInformation", "GraphicsView", "Ludwig Fiolka", "07/01/2011", "Stable", "1.0");
INTERACTORPLUGINVIEWEXTENSION(i5, "i5" ,"InteractorRectangleZoom", "GraphicsView", "Ludwig Fiolka", "07/01/2011", "Stable", "1.0");
INTERACTORPLUGINVIEWEXTENSION(i6, "i6" ,"InteractorEditEdgeBends", "GraphicsView", "Ludwig Fiolka", "07/01/2011", "Stable", "1.0");
INTERACTORPLUGINVIEWEXTENSION(i7, "i7" ,"PathFinder", "GraphicsView", "Ludwig Fiolka", "07/01/2011", "Stable", "1.0");
