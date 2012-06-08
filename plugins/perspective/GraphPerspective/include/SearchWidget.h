#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QtGui/QWidget>

namespace Ui {
class SearchWidget;
}

namespace tlp {
class GraphHierarchiesModel;
class Graph;
}

class SearchWidget : public QWidget {
  Q_OBJECT

  Ui::SearchWidget* _ui;

public:
  explicit SearchWidget(QWidget *parent = 0);
  virtual ~SearchWidget();

signals:
  
public slots:
  void setModel(tlp::GraphHierarchiesModel* model);
  void currentGraphChanged(tlp::Graph* g);
  void setGraph(tlp::Graph* g);

protected slots:
  void graphIndexChanged(int);
};

#endif // SEARCHWIDGET_H
