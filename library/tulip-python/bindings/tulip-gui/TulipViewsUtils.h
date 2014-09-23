/*
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

/**
 *
 * This file is part of Tulip (www.tulip-software.org)
 *
 * Authors: David Auber and the Tulip development Team
 * from LaBRI, University of Bordeaux
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

#ifndef VIEW_MAIN_WINDOW_H
#define VIEW_MAIN_WINDOW_H

#include <QMainWindow>

#include <tulip/Graph.h>
#include <tulip/Observable.h>
#include <tulip/View.h>
#include <tulip/GraphHierarchiesModel.h>
#include <tulip/Workspace.h>
#include <tulip/WorkspacePanel.h>

typedef std::set< tlp::Observable * >::iterator ObserverIterator;

class TulipViewsManager;

class ViewMainWindow : public QMainWindow {

  Q_OBJECT

public :

  ViewMainWindow();

};

class TulipViewsManager : public QObject, public tlp::Observable {

  Q_OBJECT

public :

  TulipViewsManager();

  std::vector<std::string> getTulipViews();

  std::vector<tlp::View *> getOpenedViews();

  std::vector<tlp::View *> getOpenedViewsWithName(const std::string &viewName);

  tlp::View *addView(const std::string &viewName, tlp::Graph *graph, const tlp::DataSet &dataSet = tlp::DataSet(), bool show=true);

  void closeView(tlp::View *view);

  std::vector<tlp::View*> getViewsOfGraph(tlp::Graph *graph);

  void closeAllViews();

  void closeViewsRelatedToGraph(tlp::Graph* graph);

  void setViewVisible(tlp::View *view, const bool visible);

  bool areViewsVisible();

  void resizeView(tlp::View *view, int width, int height);

  void setViewPos(tlp::View *view, int x, int y);

  tlp::GraphHierarchiesModel *graphModel() const {
    return model;
  }

  tlp::Workspace *tlpWorkspace();

  void treatEvent(const tlp::Event& ev);

public slots:

  void viewDestroyed(QObject *view);

private:

  std::vector<tlp::View *> openedViews;
  std::map<tlp::View *, tlp::WorkspacePanel *> viewToPanel;
  std::map<tlp::View *, ViewMainWindow *> viewToWindow;
  tlp::GraphHierarchiesModel *model;

};

#endif
