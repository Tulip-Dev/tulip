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

#include <sipAPItulipgui.h>

#include <QApplication>

#include <tulip/PluginLister.h>
#include <tulip/WorkspacePanel.h>


#include "TulipViewsUtils.h"

using namespace std;
using namespace tlp;

ViewMainWindow::ViewMainWindow() : QMainWindow() {
  setAttribute(Qt::WA_DeleteOnClose, true);
  setWindowIcon(QIcon(":/logo32x32.ico"));
  resize(800, 600);
}


std::vector<std::string> TulipViewsManager::getTulipViews() {
  std::vector<std::string> ret;
  std::list<std::string> views = PluginLister::instance()->availablePlugins<View>();

  for (std::list<std::string>::iterator it=views.begin(); it != views.end(); ++it) {
    if (*it != "Python Script view") {
      ret.push_back(*it);
    }
  }

  return ret;
}

tlp::Workspace *TulipViewsManager::tlpWorkspace() {
  tlp::Perspective *perspective = tlp::Perspective::instance();

  if (perspective) {
    return perspective->mainWindow()->findChild<tlp::Workspace*>();
  }

  return NULL;
}

TulipViewsManager::TulipViewsManager() {
  model = new GraphHierarchiesModel(this);

}

std::vector<tlp::View *> TulipViewsManager::getOpenedViews() {
  tlp::Workspace *workspace = tlpWorkspace();

  if (workspace) {
    QList<tlp::View*> views = workspace->panels();
    return std::vector<tlp::View *>(views.begin(), views.end());
  }
  else {
    return openedViews;
  }
}

std::vector<tlp::View *> TulipViewsManager::getOpenedViewsWithName(const std::string &viewName) {
  std::vector<tlp::View *> views = getOpenedViews();
  std::vector<tlp::View*> ret;

  for (size_t i = 0 ; i < views.size() ; ++i) {
    if (views[i]->name() == viewName) {
      ret.push_back(views[i]);
    }
  }

  return ret;
}

tlp::View *TulipViewsManager::addView(const std::string &viewName, tlp::Graph *graph, const DataSet &dataSet, bool show) {
  tlp::Workspace *workspace = tlpWorkspace();
  tlp::View *view = NULL;

  if (workspace) {
    workspace->graphModel()->addGraph(graph);
    view = PluginLister::instance()->getPluginObject<View>(viewName,NULL);
    view->setupUi();
    view->setGraph(graph);
    view->setState(dataSet);
    workspace->addPanel(view);
  }
  else {

    graph->addListener(this);

    model->addGraph(graph);

    view = PluginLister::instance()->getPluginObject<View>(viewName,NULL);
    view->setupUi();
    view->setGraph(graph);
    view->setState(dataSet);

    WorkspacePanel *panel = new WorkspacePanel(view);
    panel->setGraphsModel(model);
    panel->viewGraphSet(graph);

    connect(view,SIGNAL(drawNeeded()),view,SLOT(draw()));

    openedViews.push_back(view);
    viewToPanel[view] = panel;

    setViewVisible(view, show);
    view->draw();
    QApplication::processEvents();



  }

  connect(view, SIGNAL(destroyed(QObject *)), this, SLOT(viewDestroyed(QObject*)));

  return view;
}

void TulipViewsManager::closeView(tlp::View *view) {
  tlp::Workspace *workspace = tlpWorkspace();

  if (workspace) {
    workspace->delView(view);
  }
  else {
    if (viewToWindow.find(view) != viewToWindow.end()) {
      delete viewToWindow[view];
      viewToWindow.erase(view);
      viewToPanel.erase(view);
    }

    if (viewToPanel.find(view) != viewToPanel.end()) {
      delete viewToPanel[view];
      viewToPanel.erase(view);
    }
  }
}

std::vector<tlp::View*> TulipViewsManager::getViewsOfGraph(tlp::Graph *graph) {
  tlp::Workspace *workspace = tlpWorkspace();
  std::vector<tlp::View*> ret;

  if (workspace) {
    QList<tlp::View*> views = workspace->panels();

    for (int i = 0 ; i < views.count() ; ++i) {
      if (views.at(i)->graph() == graph) {
        ret.push_back(views.at(i));
      }
    }
  }
  else {
    for (size_t i = 0 ; i < openedViews.size() ; ++i) {
      if (openedViews[i]->graph() == graph) {
        ret.push_back(openedViews[i]);
      }
    }
  }

  return ret;
}

void TulipViewsManager::closeAllViews() {
  tlp::Workspace *workspace = tlpWorkspace();

  if (workspace) {
    QList<tlp::View*> views = workspace->panels();

    for (int i = 0 ; i < views.size() ; ++i) {
      if (views.at(i)->name() != "Python Script view") {
        workspace->delView(views.at(i));
      }
    }
  }
  else {
    std::vector<tlp::View*> openedViewsCp(openedViews);

    for (size_t i = 0 ; i < openedViewsCp.size() ; ++i) {
      closeView(openedViewsCp[i]);
    }

  }

}

void TulipViewsManager::closeViewsRelatedToGraph(tlp::Graph* graph) {
  tlp::Workspace *workspace = tlpWorkspace();

  if (workspace) {
    QList<tlp::View*> views = workspace->panels();

    for (int i = 0 ; i < views.count() ; ++i) {
      if (views.at(i)->graph() == graph) {
        workspace->delView(views.at(i));
      }
    }
  }
  else {
    std::vector<tlp::View*> openedViewsCp(openedViews);

    for (size_t i = 0 ; i < openedViewsCp.size() ; ++i) {
      if (openedViewsCp[i]->graph() == graph) {
        closeView(openedViewsCp[i]);
      }
    }
  }
}

void TulipViewsManager::viewDestroyed(QObject *obj) {
  tlp::Workspace *workspace = tlpWorkspace();
  tlp::View *view = static_cast<tlp::View*>(obj);
  releaseSIPWrapper(view, sipFindType("tlp::View"));

  if (!workspace) {
    viewToWindow.erase(view);
    viewToPanel.erase(view);
    openedViews.erase(std::remove(openedViews.begin(), openedViews.end(), view), openedViews.end());
  }
}

void TulipViewsManager::setViewVisible(tlp::View *view, const bool visible) {
  tlp::Workspace *workspace = tlpWorkspace();

  if (!workspace) {
    if (visible) {
      viewToWindow[view] = new ViewMainWindow();
      viewToWindow[view]->setWindowTitle(("Tulip : " + view->name() + " : " + view->graph()->getName()).c_str());
      viewToWindow[view]->setCentralWidget(viewToPanel[view]);
      viewToWindow[view]->setVisible(true);
    }
    else {
      viewToPanel[view]->setParent(NULL);

      if (viewToWindow.find(view) != viewToWindow.end()) {
        delete viewToWindow[view];
        viewToWindow.erase(view);
      }
    }

    if (visible) {
      viewToWindow[view]->raise();
    }

    QApplication::processEvents();
  }
}

bool TulipViewsManager::areViewsVisible()  {
  bool ret = false;

  tlp::Workspace *workspace = tlpWorkspace();

  if (!workspace) {
    for (size_t i = 0 ; i < openedViews.size() ; ++i) {
      ret = ret || (viewToWindow.find(openedViews[i]) != viewToWindow.end() && viewToWindow[openedViews[i]]->isVisible());
    }
  }

  return ret;
}

void TulipViewsManager::resizeView(tlp::View *view, int width, int height) {
  tlp::Workspace *workspace = tlpWorkspace();

  if (!workspace) {
    viewToWindow[view]->resize(width, height);
    QApplication::processEvents();
  }
}

void TulipViewsManager::setViewPos(tlp::View *view, int x, int y) {
  tlp::Workspace *workspace = tlpWorkspace();

  if (!workspace) {
    viewToWindow[view]->move(x, y);
    QApplication::processEvents();
  }
}

void TulipViewsManager::treatEvent(const tlp::Event& ev) {
  if (ev.type() == Event::TLP_DELETE) {
    Graph *g = static_cast<Graph*>(ev.sender());
    closeViewsRelatedToGraph(g);
  }
}
