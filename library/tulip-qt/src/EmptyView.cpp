#include "tulip/EmptyView.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QPushButton>

#include <tulip/ForEach.h>

#include <tulip/GlMainWidget.h>
#include <QtGui/QLabel>

using namespace tlp;
using namespace std;

EmptyView::EmptyView() {
}

EmptyView::~EmptyView() {
}

class MyLabel: public QLabel {
public:
  virtual ~MyLabel() { cout << "delete MyLabel" << endl; }
};

class MyGlMainWidget: public tlp::GlMainWidget {
public:
  MyGlMainWidget(QWidget *parent,AbstractView *view=NULL): GlMainWidget(parent,view) {}
  virtual ~MyGlMainWidget() { cout << "delete MyGlMainWidget" << endl; }
};

void EmptyView::setData(tlp::Graph *graph,tlp::DataSet dataSet) {
  _graph = graph;

  tlp::GlMainWidget *mainWidget = new MyGlMainWidget(0);
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
void EmptyView::draw() {}
void EmptyView::refresh() {}
void EmptyView::init() {}

VIEWPLUGIN(EmptyView, "Empty graphics view for test purposes", "Ludwig Fiolka", "07/01/2011", "Release Candidate", "2.0");
