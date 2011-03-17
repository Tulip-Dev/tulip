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
  virtual ~MyGlMainWidget() { cout << __PRETTY_FUNCTION__ << endl; }
};

#include <QtGui/QLabel>
void EmptyView::setData(tlp::Graph *graph,tlp::DataSet dataSet) {
  _graph = graph;

//  QLabel *lbl = new QLabel();
//  lbl->setPixmap(QPixmap("g:/Pictures/amazon.jpg"));
//  setCentralWidget(lbl);

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
  _graph = graph;
}
void EmptyView::draw() { cout << __PRETTY_FUNCTION__ << endl; ((GlMainWidget *)getCentralWidget())->draw(); }
void EmptyView::refresh() {cout << __PRETTY_FUNCTION__ << endl;  ((GlMainWidget *)getCentralWidget())->redraw(); }
void EmptyView::init() {}

VIEWPLUGIN(EmptyView, "Empty graphics view for test purposes", "Ludwig Fiolka", "07/01/2011", "Release Candidate", "2.0");
//INTERACTORPLUGINVIEWEXTENSION(i1, "i1" ,"InteractorNavigation", "Empty graphics view for test purposes", "Ludwig Fiolka", "07/01/2011", "Stable", "1.0");
//INTERACTORPLUGINVIEWEXTENSION(i2, "i2" ,"InteractorSelection", "Empty graphics view for test purposes", "Ludwig Fiolka", "07/01/2011", "Stable", "1.0");
//INTERACTORPLUGINVIEWEXTENSION(i3, "i3" ,"InteractorAddNode", "Empty graphics view for test purposes", "Ludwig Fiolka", "07/01/2011", "Stable", "1.0");
