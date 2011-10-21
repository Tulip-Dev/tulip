#ifndef WORKSPACEPANEL_H
#define WORKSPACEPANEL_H

#include <QtGui/QWidget>
#include <tulip/tulipconf.h>
#include <tulip/DataSet.h>

namespace Ui {
class WorkspacePanel;
}

namespace tlp {

class Graph;
class View;

class TLP_QT_SCOPE WorkspacePanel : public QWidget {
  Q_OBJECT
  Q_PROPERTY(tlp::Graph* graph READ graph WRITE setGraph)

  Ui::WorkspacePanel* _ui;
  tlp::Graph* _graph;
  tlp::View* _view;

public:
  explicit WorkspacePanel(tlp::Graph* graph, const QString& viewName="", const tlp::DataSet& state=tlp::DataSet(), QWidget* parent=0);
  virtual ~WorkspacePanel();

  tlp::View* view() const;
  tlp::Graph* graph() const;

public slots:
  void setGraph(tlp::Graph* graph);
  void setView(const QString& viewName, const tlp::DataSet& state=tlp::DataSet());

protected slots:
  void internalSetView(const QString&,const tlp::DataSet&);
};

}

#endif // WORKSPACEPANEL_H
