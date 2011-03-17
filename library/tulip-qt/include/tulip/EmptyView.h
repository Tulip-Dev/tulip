#ifndef EMPTYVIEW_H
#define EMPTYVIEW_H

#include "AbstractGraphicsView.h"

#include <tulip/GlMainWidget.h>

class EmptyView: public tlp::AbstractGraphicsView {
public:
  EmptyView();
  virtual ~EmptyView();

  virtual void setData(tlp::Graph *graph,tlp::DataSet dataSet);
  virtual void getData(tlp::Graph **graph,tlp::DataSet *dataSet);
  virtual tlp::Graph *getGraph();
  virtual void setGraph(tlp::Graph *graph);
  virtual void draw();
  virtual void refresh();
  virtual void init();

  tlp::Graph *_graph;
  tlp::GlMainWidget *mainWidget;
};

#endif // EMPTYVIEW_H
