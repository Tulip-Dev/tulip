#include "tulip/EmptyView.h"

EmptyView::EmptyView() {
}

EmptyView::~EmptyView() {
}

void EmptyView::setData(tlp::Graph *graph,tlp::DataSet dataSet) {}
void EmptyView::getData(tlp::Graph **graph,tlp::DataSet *dataSet) {}
tlp::Graph *EmptyView::getGraph() {}
void EmptyView::setGraph(tlp::Graph *graph) {}
void EmptyView::draw() {}
void EmptyView::refresh() {}
void EmptyView::init() {}

VIEWPLUGIN(EmptyView, "Empty graphics view for test purposes", "Ludwig Fiolka", "07/01/2011", "Release Candidate", "2.0");
