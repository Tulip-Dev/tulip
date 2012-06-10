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

#ifndef VIEW_MAIN_WINDOW_H
#define VIEW_MAIN_WINDOW_H

#include <QtGui/QMainWindow>
#include <QtGui/QToolBar>
#include <tulip/Graph.h>
#include <tulip/Observable.h>
#include <tulip/BaseGraphicsViewComponent.h>
#include <tulip/SGHierarchyWidget.h>

typedef std::set< tlp::Observable * >::iterator ObserverIterator;

class ViewMainWindow : public QMainWindow, public tlp::Observable {

  Q_OBJECT

public :

  ViewMainWindow(const std::string &viewName);
  ~ViewMainWindow();

  void setData(tlp::Graph *graph, const tlp::DataSet &data);

  tlp::BaseGraphicsViewComponent *getViewProxy() const {
      return viewProxy;
  }

  tlp::View *getView() const {
    return viewProxy->getRealView();
  }

public slots:

  void observableDestroyed(tlp::Observable *) {}
  void changeInteractor();
  void update(ObserverIterator begin, ObserverIterator end);

private :

  void initObservers();
  void clearObservers();

  tlp::Graph *graph;
  tlp::BaseGraphicsViewComponent *viewProxy;
  tlp::SGHierarchyWidget *hierarchyWidget;
  QToolBar *toolBar;

};

class TulipViewsManager : public QObject {

  Q_OBJECT

public :

  std::vector<std::string> getTulipViews();

  std::vector<tlp::View *> getOpenedViews();

  std::string getNameOfView(tlp::View *view);

  tlp::View *addView(const std::string &viewName, tlp::Graph *graph, tlp::DataSet dataSet = tlp::DataSet());

  void closeView(tlp::View *view);

  void tileViews();

  std::vector<tlp::View*> getViewsOfGraph(tlp::Graph *graph);

  void closeAllViews();

  void closeViewsRelatedToGraph(tlp::Graph* graph);

  void setViewVisible(tlp::View *view, const bool visible);

  bool areViewsVisible();

  void resizeView(tlp::View *view, int width, int height);

  void setViewPos(tlp::View *view, int x, int y);

  void setViewOptionsWidgetsVisible(tlp::View *view, const bool visible);

public slots:

  void viewDestroyed(QObject *view);

private:

  std::vector<tlp::View *> openedViews;
  std::map<tlp::View *, ViewMainWindow *> viewToWindow;
  std::map<tlp::View *, std::string> viewToName;

};

#endif
