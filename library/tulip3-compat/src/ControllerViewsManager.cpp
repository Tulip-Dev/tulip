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
#include "tulip3/ControllerViewsManager.h"

#include <QtGui/QWorkspace>
#include <QtGui/QToolBar>

#include <tulip/Graph.h>

#include <tulip/View.h>
#include "tulip3/ControllerViewsTools.h"

using namespace std;

namespace tlp {

ControllerViewsManager::ControllerViewsManager(PerspectiveContext &c) :
              Controller(c), currentView(NULL), currentGraph(NULL) {
}
ControllerViewsManager::~ControllerViewsManager() {
	for(map<QWidget *,View*>::iterator it=viewWidget.begin();it!=viewWidget.end();++it){
		delete (*it).first;
		delete (*it).second;
	}
}
//**********************************************************************
void ControllerViewsManager::attachMainWindow(MainWindowFacade facade) {
	Controller::attachMainWindow(facade);

	connect(mainWindowFacade.getWorkspace(), SIGNAL(windowActivated(QWidget *)), this,
			SLOT(windowActivated(QWidget *)));
}
//**********************************************************************
//**********************************************************************
// Accessors and setters
//**********************************************************************
//**********************************************************************
Graph *ControllerViewsManager::getGraph() {
	return currentGraph;
}
//**********************************************************************
Graph *ControllerViewsManager::getCurrentGraph() {
	return currentGraph;
}
//**********************************************************************
void ControllerViewsManager::setCurrentGraph(Graph *graph) {
	currentGraph = graph;
}
//**********************************************************************
View *ControllerViewsManager::getCurrentView() {
	return currentView;
}
//**********************************************************************
unsigned int ControllerViewsManager::getViewsNumber()const {
	return viewWidget.size();
}
//**********************************************************************
Graph *ControllerViewsManager::getGraphOfView(View *view) {
	if (viewGraph.count(view) != 0)
		return viewGraph[view];

	return NULL;
}
//**********************************************************************
void ControllerViewsManager::setGraphOfView(View *view, Graph *graph) {
	view->setGraph(graph);
	viewGraph[view] = graph;
}
//**********************************************************************
void ControllerViewsManager::setDataOfView(View* view, Graph* graph, const DataSet& dataSet){
	view->setData(graph,dataSet);
	viewGraph[view]= graph;
}

//**********************************************************************
View *ControllerViewsManager::getViewOfWidget(QWidget *widget) {
	if (viewWidget.count(widget) != 0)
		return viewWidget[widget];

	return NULL;
}
//**********************************************************************
void ControllerViewsManager::setViewOfWidget(QWidget *widget, View *view) {
	viewWidget[widget] = view;
}
//**********************************************************************
QWidget *ControllerViewsManager::getWidgetOfView(View *view) {
	for (map<QWidget *, View *>::iterator it = viewWidget.begin(); it != viewWidget.end(); ++it) {
		if ((*it).second == view)
			return (*it).first;
	}
	return NULL;
}
//**********************************************************************
void ControllerViewsManager::getViews(vector<View *> &views){
	for(map<View *,string>::iterator it=viewNames.begin();it!=viewNames.end();++it){
		views.push_back((*it).first);
	}
}
//**********************************************************************
string ControllerViewsManager::getNameOfView(View *view) const{
	map<View *,string>::const_iterator it = viewNames.find(view);
	if(it!=viewNames.end()){
		return it->second;
	}else{
		return "";
	}
}
//**********************************************************************
void ControllerViewsManager::setNameOfView(View *view, const string &name) {
	viewNames[view] = name;
}
//**********************************************************************
QWidget *ControllerViewsManager::getInteractorConfigurationWidgetOfView(View *view){
	if(lastInteractorConfigurationWidgetOnView.count(view)!=0)
		return lastInteractorConfigurationWidgetOnView[view];
	else
		return NULL;
}
//**********************************************************************
//**********************************************************************
// Main functions
//**********************************************************************
//**********************************************************************
void ControllerViewsManager::addView(View *createdView, Graph *graph, DataSet dataSet, bool forceWidgetSize, const QRect &rect, bool maximized, const string &createdViewName, QWidget *createdWidget) {
	connect(createdWidget, SIGNAL(destroyed(QObject *)), this,
			SLOT(widgetWillBeClosed(QObject *)));

	viewGraph[createdView] = graph;
	viewNames[createdView] = createdViewName;
	viewWidget[createdWidget] = createdView;

	mainWindowFacade.getWorkspace()->addWindow(createdWidget);

	string windowTitle = createdViewName + " : " + graph->getAttribute<string> ("name");
	createdWidget->setWindowTitle(QString::fromUtf8(windowTitle.c_str()));

	if (forceWidgetSize) {
		((QWidget*) (createdWidget->parent()))->setGeometry(rect);
	} else {
		QRect newRect = rect;
		if (createdWidget->size().height() < 10 || createdWidget->size().width() < 10) {
			newRect.setSize(QSize(500, 500));
		} else {
			newRect.setSize(createdWidget->size());
		}
		((QWidget*) (createdWidget->parent()))->setGeometry(newRect);
	}



	createdWidget->setMaximumSize(32767, 32767);

	// block signals in order to not execute windowActivated slot before data are set
	mainWindowFacade.getWorkspace()->blockSignals(true);
	if (maximized)
		((QWidget*) (createdWidget->parent()))->showMaximized();
	else
		createdWidget->show();
	mainWindowFacade.getWorkspace()->blockSignals(false);

	createdView->setData(graph,dataSet);

	windowActivated(createdWidget);
}

View* ControllerViewsManager::createView(const string &name, Graph *graph, DataSet dataSet,
		bool forceWidgetSize, const QRect &rect, bool maximized) {
	string createdViewName;
	View *createdView;
	QWidget *createdWidget;
	ControllerViewsTools::createView(name, graph, dataSet, mainWindowFacade.getWorkspace(),
			&createdViewName, &createdView, &createdWidget);

	addView(createdView, graph, dataSet, forceWidgetSize, rect, maximized, createdViewName, createdWidget);

	return createdView;
}
//**********************************************************************
void ControllerViewsManager::installInteractors(View *view) {
	//remove connection of old actions
	QList<QAction *> oldActions = mainWindowFacade.getInteractorsToolBar()->actions();
	for (QList<QAction *>::iterator it = oldActions.begin(); it != oldActions.end(); ++it) {
		disconnect((*it), SIGNAL(triggered()), this, SLOT(changeInteractor()));
	}

	QAction *lastAction = NULL;
	map<View*, QAction *>::iterator it = lastInteractorOnView.find(view);
	if (it != lastInteractorOnView.end())
		lastAction = (*it).second;

	ControllerViewsTools::installInteractors(view, mainWindowFacade.getInteractorsToolBar());

	if (lastAction && mainWindowFacade.getInteractorsToolBar()->actions().contains(lastAction)) {
		changeInteractor(lastAction);
	} else {
		if (mainWindowFacade.getInteractorsToolBar()->actions().size() != 0)
			changeInteractor(mainWindowFacade.getInteractorsToolBar()->actions().front());
	}

	QList<QAction *> newActions = mainWindowFacade.getInteractorsToolBar()->actions();
	for (QList<QAction *>::iterator it = newActions.begin(); it != newActions.end(); ++it) {
		connect((*it), SIGNAL(triggered()), this, SLOT(changeInteractor()));
	}
}
//**********************************************************************
void ControllerViewsManager::updateViewsOfGraph(Graph *graph) {
	for (map<View *, Graph*>::iterator it = viewGraph.begin(); it != viewGraph.end(); ++it) {
		if ((*it).second == graph) {
			(*it).first->setGraph(graph);
		}
	}
}
//**********************************************************************
void ControllerViewsManager::updateViewsOfSubGraphs(Graph *graph) {
	for (map<View *, Graph*>::iterator it = viewGraph.begin(); it != viewGraph.end(); ++it) {
		assert(graph);
		Graph *subGraph = (*it).second;
		if (graph->isDescendantGraph(subGraph)) {
			(*it).first->setGraph(subGraph);
		}
	}
}
//**********************************************************************
void ControllerViewsManager::changeGraphOfViews(Graph *oldGraph, Graph *newGraph) {
	string name = newGraph->getAttribute<string>("name");
	for (map<View *, Graph*>::iterator it = viewGraph.begin(); it != viewGraph.end(); ++it) {
		if ((*it).second == oldGraph) {
			View* view = (*it).first;
			view->setGraph(newGraph);
			// update view title
			QWidget *widget = getWidgetOfView(view);
			string windowTitle = viewNames[view] + " : " + name;
			widget->setWindowTitle(QString::fromUtf8(windowTitle.c_str()));
			(*it).second = newGraph;
		}
	}
}
//**********************************************************************
void ControllerViewsManager::drawViews(bool init) {
	QList<QWidget *> widgetList = mainWindowFacade.getWorkspace()->windowList();
	for (QList<QWidget *>::iterator it = widgetList.begin(); it != widgetList.end(); ++it) {
		if (!init)
			viewWidget[*it]->draw();
		else
			viewWidget[*it]->init();
	}
}
//==================================================
void ControllerViewsManager::saveViewsGraphsHierarchies() {
	viewsGraphsHierarchy.clear();
	for (map<View *, Graph*>::iterator itView = viewGraph.begin(); itView != viewGraph.end(); ++itView) {
		viewsGraphsHierarchy[(*itView).first] = list<unsigned int> ();
		Graph *father = (*itView).second;
		while (father != father->getSuperGraph()) {
			viewsGraphsHierarchy[(*itView).first].push_back(father->getId());
			father = father->getSuperGraph();
		}
		viewsGraphsHierarchy[(*itView).first].push_back(father->getId());
	}
}
//**********************************************************************
void ControllerViewsManager::checkViewsGraphsHierarchy() {
	for (map<View *, Graph*>::iterator itView = viewGraph.begin(); itView != viewGraph.end(); ++itView) {
		Graph *newGraph = NULL;
		for (list<unsigned int>::iterator it = viewsGraphsHierarchy[(*itView).first].begin(); it
		!= viewsGraphsHierarchy[(*itView).first].end(); ++it) {
			newGraph = currentGraph->getRoot()->getDescendantGraph(*it);
			if (!newGraph && (currentGraph->getRoot()->getId() == (*it)))
				newGraph = currentGraph->getRoot();
			if (newGraph)
				break;
		}

		if (newGraph != (*itView).second) {
			(*itView).first->setGraph(newGraph);
			viewGraph[(*itView).first] = newGraph;
		}
	}
}

//**********************************************************************
//**********************************************************************
// Slots
//**********************************************************************
//**********************************************************************
void ControllerViewsManager::createView(QAction *action) {
	createView(action->text().toStdString(), currentGraph, DataSet(), false, QRect(0, 0, 0, 0),
			false);
}
//**********************************************************************
bool ControllerViewsManager::windowActivated(QWidget *widget) {
	//check if that widget (view) is closed
	QWidgetList widgets = mainWindowFacade.getWorkspace()->windowList();
	std::map<QWidget *, View*>::iterator it = viewWidget.find(widget);
	if (it == viewWidget.end())
		return false;

	View *view = getViewOfWidget(widget);

	if(currentView==view)
		return false;

	currentView = view;
	currentGraph = currentView->getGraph();

	installInteractors(view);

	return true;
}
//**********************************************************************
bool ControllerViewsManager::changeInteractor() {
	//Get QAction who emit the signal (sett QObject protected functions)
	QAction *action = (QAction*) sender();
	return changeInteractor(action);
}
//**********************************************************************
bool ControllerViewsManager::changeInteractor(QAction* action) {
	QWidget *configurationWidget;
	return changeInteractor(action, &configurationWidget);
}
//**********************************************************************
bool ControllerViewsManager::changeInteractor(QAction* action, QWidget **configurationWidget) {
	if (!currentView)
		return false;

	// Check if we have a previously intalled interactor
	//   If not, we don't have to draw the view
	bool needRefresh=true;
	if(lastInteractorOnView.count(currentView)==0)
		needRefresh=false;

	lastInteractorOnView[currentView] = action;

	ControllerViewsTools::changeInteractor(currentView, mainWindowFacade.getInteractorsToolBar(),
			action, configurationWidget);

	lastInteractorConfigurationWidgetOnView[currentView] = *configurationWidget;

	if(needRefresh)
		currentView->refresh();

	return true;
}
//**********************************************************************
void ControllerViewsManager::changeWindowTitle(Graph *graph){
	for(std::map<View *,Graph* >::iterator it=viewGraph.begin();it!=viewGraph.end();++it){
		if((*it).second==graph){
			string windowTitle = viewNames[(*it).first] + " : " + graph->getAttribute<string> ("name");

			QWidget *widget = getWidgetOfView((*it).first);
			widget->setWindowTitle(QString::fromUtf8(windowTitle.c_str()));
		}
	}
}
//**********************************************************************
bool ControllerViewsManager::changeGraph(Graph *graph) {
	if (currentGraph == graph)
		return false;
	if (!currentView)
		return false;

	currentGraph = graph;
	viewGraph[currentView] = graph;

	currentView->setGraph(graph);

	QWidget *widget = getWidgetOfView(currentView);

	string windowTitle = viewNames[currentView] + " : " + graph->getAttribute<string> ("name");
	widget->setWindowTitle(QString::fromUtf8(windowTitle.c_str()));

	return true;
}
//**********************************************************************
void ControllerViewsManager::widgetWillBeClosed(QObject *object) {
	QWidget *widget = (QWidget*) object;
	View *view = viewWidget[widget];
	//	widgetWillBeClosed(widget, view);
	delete viewWidget[widget];
	viewWidget.erase(widget);
	viewNames.erase(view);
	lastInteractorOnView.erase(view);
	lastInteractorConfigurationWidgetOnView.erase(view);
	viewGraph.erase(view);
	if (viewWidget.size() == 0) {
		mainWindowFacade.getInteractorsToolBar()->clear();
		currentView = NULL;
		emit willBeClosed();
	}
}
//**********************************************************************

void ControllerViewsManager::closeAllViews() {
	if(mainWindowFacade.getWorkspace())
		mainWindowFacade.getWorkspace()->closeAllWindows();
}

//**********************************************************************

void ControllerViewsManager::closeView(View *view) {
	//Need to activate windows before closing it.(see QWorkspace documentation).
	mainWindowFacade.getWorkspace()->setActiveWindow(getWidgetOfView(view));
	mainWindowFacade.getWorkspace()->closeActiveWindow();
}

//**********************************************************************
void ControllerViewsManager::closeViewsRelatedToGraph(Graph* graph) {
	vector<View*> views = getViewsOfGraph(graph);
	for (vector<View*>::iterator it = views.begin(); it != views.end(); ++it) {
		closeView(*it);
	}
}

//**********************************************************************

vector<View*> ControllerViewsManager::getViewsOfGraph(Graph *graph) {
	vector<View*> views;
	for (map<View*, Graph*>::iterator it = viewGraph.begin(); it != viewGraph.end(); ++it) {
		if (it->second == graph)
			views.push_back(it->first);
	}
	return views;
}
}

