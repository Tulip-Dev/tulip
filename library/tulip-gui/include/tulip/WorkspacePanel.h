/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux 1 and Inria Bordeaux - Sud Ouest
 *
 * Tulip is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Tulip is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 */
#ifndef WORKSPACEPANEL_H
#define WORKSPACEPANEL_H

#include <QtGui/QWidget>
#include <tulip/tulipconf.h>
#include <tulip/DataSet.h>
#include <tulip/SimplePluginProgress.h>

namespace Ui {
class WorkspacePanel;
}

namespace tlp {

class Graph;
class View;
class Interactor;

class TLP_QT_SCOPE WorkspacePanel : public QWidget, public tlp::SimplePluginProgress {
  Q_OBJECT
  Q_PROPERTY(tlp::Graph* graph READ graph WRITE setGraph)

  Q_PROPERTY(bool progressMode READ isProgressMode WRITE toggleProgressMode)
  bool _progressMode;

  Ui::WorkspacePanel* _ui;
  tlp::Graph* _graph;
  tlp::View* _view;

public:
  explicit WorkspacePanel(tlp::Graph* graph, const QString& viewName="", const tlp::DataSet& state=tlp::DataSet(), QWidget* parent=0);
  virtual ~WorkspacePanel();

  tlp::View* view() const;
  tlp::Graph* graph() const;
  bool isProgressMode() const;

public slots:
  void setGraph(tlp::Graph* graph);
  void setView(const QString& viewName, const tlp::DataSet& state=tlp::DataSet());
  void toggleProgressMode(bool p);

protected slots:
  void interactorActionTriggered();
  void internalSetView(const QString&,const tlp::DataSet&);
  void internalSetCurrentInteractor(tlp::Interactor*);
  void viewSceneRectChanged(const QRectF&);

protected:
  void progress_handler(int step, int max_step);
};

}

#endif // WORKSPACEPANEL_H
