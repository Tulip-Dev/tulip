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

#include <sipAPItulipgui.h>

#include <tulip/Interactor.h>
#include <tulip/InteractorManager.h>
#include <tulip/MainController.h>
#include <tulip/TemplateFactory.h>

#include <QtGui/QWorkspace>
#include <QtGui/QApplication>

#include "TulipViewsUtils.h"

using namespace std;
using namespace tlp;

ViewMainWindow::ViewMainWindow(const string &viewName) : QMainWindow(), graph(0) {
  setAttribute(Qt::WA_DeleteOnClose, true);
  setWindowIcon(QIcon(":/logo32x32.ico"));
  viewProxy = new BaseGraphicsViewComponent(viewName);
  QWidget *widget=viewProxy->construct(NULL);
  hierarchyWidget = new SGHierarchyWidget();
  connect(hierarchyWidget, SIGNAL(graphChanged(Graph *)), viewProxy, SLOT(setGraph(Graph*)));
  viewProxy->getOptionsTabWiget()->addTab(hierarchyWidget, "Graph Hierarchy");
  setCentralWidget(widget);
  resize(800, 600);

  // Get interactors for this view and  add them to view
  std::list<std::string> interactorsNamesAndPriorityMap(InteractorManager::getInst().getSortedCompatibleInteractors(viewName));

  list<Interactor *> interactorsList;

  for(std::list<std::string>::iterator it=interactorsNamesAndPriorityMap.begin(); it!=interactorsNamesAndPriorityMap.end(); ++it) {
    interactorsList.push_back(InteractorManager::getInst().getInteractor((*it)));
  }

  viewProxy->setInteractors(interactorsList);

  list<QAction *> interactorsActionList;

  for(list<Interactor *>::iterator it=interactorsList.begin(); it!=interactorsList.end(); ++it)
    interactorsActionList.push_back((*it)->getAction());

  if (!interactorsActionList.empty()) {

    toolBar = new QToolBar(this);

    for(list<QAction *>::iterator it=interactorsActionList.begin(); it!=interactorsActionList.end(); ++it) {
      toolBar->addAction(*it);
      connect(*it, SIGNAL(triggered()), this, SLOT(changeInteractor()));
    }

    addToolBar(toolBar);
    QAction *firstInteractor = *(interactorsActionList.begin());
    firstInteractor->activate(QAction::Trigger);
  }
}

ViewMainWindow::~ViewMainWindow() {
  clearObservers();
  delete viewProxy;
}

void ViewMainWindow::setData(Graph *graph, const DataSet &dataSet) {
  clearObservers();
  viewProxy->setData(graph,dataSet);
  hierarchyWidget->setGraph(graph);
  this->graph = graph;
  initObservers();
  viewProxy->init();
  setWindowTitle(("Tulip : " + viewProxy->getRealViewName() + " : " + graph->getName()).c_str());
}

void ViewMainWindow::changeInteractor() {
  QAction *action = (QAction*) sender();
  QList<QAction*> actions=toolBar->actions();

  for(QList<QAction*>::iterator it=actions.begin(); it!=actions.end(); ++it) {
    (*it)->setChecked(false);
  }

  action->setCheckable(true);
  action->setChecked(true);

  InteractorAction *interactorAction=(InteractorAction *)action;
  viewProxy->setActiveInteractor(interactorAction->getInteractor());
}

void ViewMainWindow::initObservers() {
  if (graph) {
    graph->addObserver(this);
    Iterator<PropertyInterface*> *it = graph->getObjectProperties();

    while (it->hasNext()) {
      PropertyInterface* tmp = it->next();
      tmp->addObserver(this);
    }

    delete it;
  }
}

void ViewMainWindow::clearObservers() {
  if (graph) {
    graph->removeObserver(this);
    Iterator<PropertyInterface*> *it =graph->getObjectProperties();

    while (it->hasNext()) {
      (it->next())->removeObserver(this);
    }

    delete it;
  }
}

void ViewMainWindow::update(ObserverIterator begin, ObserverIterator end) {
  viewProxy->draw();
  QApplication::processEvents();
}

std::vector<std::string> TulipViewsManager::getTulipViews() {
  std::vector<std::string> ret;
  tlp::TemplateFactory<tlp::ViewFactory, tlp::View, tlp::ViewContext>::ObjectCreator::const_iterator it;

  for (it=tlp::ViewFactory::factory->objMap.begin(); it != tlp::ViewFactory::factory->objMap.end(); ++it) {
    if (it->first != "Python Script view") {
      ret.push_back(it->first);
    }
  }

  return ret;
}

std::vector<tlp::View *> TulipViewsManager::getOpenedViews() {
  tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());

  if (mainController) {
    std::vector<tlp::View *> views;
    std::vector<tlp::View *> viewsTmp;
    mainController->getViews(viewsTmp);

    for (size_t i = 0 ; i < viewsTmp.size() ; ++i) {
      if (mainController->getNameOfView(viewsTmp[i]) != "Python Script view") {
        views.push_back(viewsTmp[i]);
      }
    }

    return views;
  }
  else {
    return openedViews;
  }
}

std::string TulipViewsManager::getNameOfView(tlp::View *view) {
  tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());
  std::string name;

  if (mainController) {
    name = mainController->getNameOfView(view);
  }
  else {
    name = viewToName[view];
  }

  return name;
}

tlp::View *TulipViewsManager::addView(const std::string &viewName, tlp::Graph *graph, tlp::DataSet dataSet) {
  tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());
  tlp::View *newView = NULL;

  if (mainController) {
    newView = mainController->createView(viewName, graph, dataSet);

  }
  else {
    ViewMainWindow *vmw = new ViewMainWindow(viewName);
    openedViews.push_back(vmw->getView());
    viewToWindow[vmw->getView()] = vmw;
    viewToName[vmw->getView()] = viewName;
    vmw->show();
    vmw->raise();
    vmw->setData(graph, dataSet);
    QApplication::processEvents();
    newView = vmw->getView();
    newView->init();
    newView->draw();
    QApplication::processEvents();
  }

  connect(newView, SIGNAL(destroyed(QObject *)), this, SLOT(viewDestroyed(QObject*)));
  return newView;
}

void TulipViewsManager::closeView(tlp::View *view) {
  tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());

  if (mainController) {
    mainController->closeView(view);
  }
  else {
    delete viewToWindow[view];
  }
}

void TulipViewsManager::tileViews() {
  tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());

  if (mainController) {
    mainController->getMainWindowFacade()->getWorkspace()->tile();
  }
}

std::vector<tlp::View*> TulipViewsManager::getViewsOfGraph(tlp::Graph *graph) {
  std::vector<tlp::View*> ret;
  tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());

  if (mainController) {
    std::vector<tlp::View*> tmp = mainController->getViewsOfGraph(graph);

    for (size_t i = 0 ; i < tmp.size() ; ++i) {
      if (mainController->getNameOfView(tmp[i]) != "Python Script view") {
        ret.push_back(tmp[i]);
      }
    }
  }
  else {
    for (size_t i = 0 ; i < openedViews.size() ; ++i) {
      if (openedViews[i]->getGraph() == graph) {
        ret.push_back(openedViews[i]);
      }
    }
  }

  return ret;
}

void TulipViewsManager::closeAllViews() {
  std::vector<tlp::View*> views;
  tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());

  if (mainController) {
    mainController->getViews(views);

    for (size_t i = 0 ; i < views.size() ; ++i) {
      if (mainController->getNameOfView(views[i]) != "Python Script view") {
        mainController->closeView(views[i]);
      }
    }
  }
  else {
    std::vector<tlp::View*> openedViewsCp(openedViews);

    for (size_t i = 0 ; i < openedViewsCp.size() ; ++i) {
      delete viewToWindow[openedViewsCp[i]];
    }
  }
}

void TulipViewsManager::closeViewsRelatedToGraph(tlp::Graph* graph) {
  std::vector<tlp::View*> views;
  tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());

  if (mainController) {
    views = mainController->getViewsOfGraph(graph);

    for (size_t i = 0 ; i < views.size() ; ++i) {
      if (mainController->getNameOfView(views[i]) != "Python Script view") {
        if (views[i]->getGraph() == graph) {
          mainController->closeView(views[i]);
        }
      }
    }
  }
  else {
    std::vector<tlp::View*> openedViewsCp(openedViews);

    for (size_t i = 0 ; i < openedViewsCp.size() ; ++i) {
      if (openedViewsCp[i]->getGraph() == graph) {
        delete viewToWindow[openedViewsCp[i]];
      }
    }
  }
}

void TulipViewsManager::viewDestroyed(QObject *obj) {
  tlp::View *view = static_cast<tlp::View*>(obj);
  releaseSIPWrapper(view, sipType_tlp_View);
  tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());

  if (!mainController) {
    viewToWindow.erase(view);
    viewToName.erase(view);
    openedViews.erase(std::remove(openedViews.begin(), openedViews.end(), view), openedViews.end());
  }
}

void TulipViewsManager::setViewVisible(tlp::View *view, const bool visible) {
    tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());
    if (!mainController) {
        viewToWindow[view]->setVisible(visible);
        QApplication::processEvents();
    }
}

bool TulipViewsManager::areViewsVisible()  {
    bool ret = false;
    for (size_t i = 0 ; i < openedViews.size() ; ++i) {
        ret = ret || viewToWindow[openedViews[i]]->isVisible();
    }
    return ret;
}

void TulipViewsManager::resizeView(tlp::View *view, int width, int height) {
    tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());
    if (!mainController) {
        viewToWindow[view]->resize(width, height);
        QApplication::processEvents();
    }
}

void TulipViewsManager::setViewPos(tlp::View *view, int x, int y) {
    tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());
    if (!mainController) {
        viewToWindow[view]->move(x, y);
        QApplication::processEvents();
    }
}

void TulipViewsManager::setViewOptionsWidgetsVisible(View *view, const bool visible){
    tlp::MainController *mainController = dynamic_cast<tlp::MainController *>(tlp::Controller::getCurrentController());
    if (!mainController) {
        viewToWindow[view]->getViewProxy()->setOptionsTabWidgetVisible(visible);
        QApplication::processEvents();
    }
}
